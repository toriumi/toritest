/**
 * @file      image_processing_thread.cpp
 * @brief     Source for ImageProcessingThread class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./image_processing_thread.h"
#include <map>
#include <vector>
#include "./logger.h"
#include "./main_wnd.h"
#include "./plugin_manager.h"

/**
 * @brief
 * Constructor.
 * @param plugin [in] pointer to the IPlugin class.
 * @param main_wnd [in] pointer to the MainWnd class.
 * @param common_param [in] pointer to the CommonParam class.
 * @param thread_running_cycle_manager [in] pointer to the
 * ThreadRunningCycleManager class.
 * @param wait_sem [in] pointer to a semaphore object for synchronization branch
 * point.
 */
ImageProcessingThread::ImageProcessingThread(
    IPlugin* plugin, MainWnd* main_wnd, CommonParam* common_param,
    ThreadRunningCycleManager* thread_running_cycle_manager,
    wxSemaphore* wait_sem)
    : wxThread(wxTHREAD_JOINABLE) {
  main_wnd_ = main_wnd;
  root_plugin_ = plugin;
  wait_sem_ = wait_sem;
  receive_image_ = NULL;
  thread_running_cycle_manager_ = thread_running_cycle_manager;
  stop_flag_ = false;
  is_running_ = false;
  comp_save_image_ = false;
  do_save_image_flag_ = false;
  first_save_image_ = NULL;
  last_save_image_ = NULL;
  common_param_ = common_param;
}

/**
 * @brief
 * Destructor.
 */
ImageProcessingThread::~ImageProcessingThread() {
  ClearSubThreadMap();
  if (receive_image_ != NULL) {
    delete receive_image_;
    receive_image_ = NULL;
  }
  if (first_save_image_ != NULL) {
    delete first_save_image_;
    first_save_image_ = NULL;
  }
  if (last_save_image_ != NULL) {
    delete last_save_image_;
    last_save_image_ = NULL;
  }
}

/**
 * @brief
 * Clear a table for managing sub-threads.
 */
void ImageProcessingThread::ClearSubThreadMap() {
  std::map<IPlugin*, SubThreadInfo*>::iterator itr;

  for (itr = sub_threads_.begin(); itr != sub_threads_.end(); ++itr) {
    SubThreadInfo* sub_thread_info = itr->second;
    if (sub_thread_info != NULL) {
      wxSemaphore* sem = sub_thread_info->sem;
      if (sem != NULL) {
        delete sem;
      }
      std::vector<ImageProcessingThread*>* threads = sub_thread_info->threads;
      if (threads != NULL) {
        for (int i = 0; i < threads->size(); i++) {
          ImageProcessingThread* thread = (*threads)[i];
          if (thread != NULL) {
            delete thread;
          }
        }
        delete threads;
      }
      delete sub_thread_info;
    }
  }
}

/**
 * @brief
 * Thread entry point.
 * @return thread exit code.
 */
wxThread::ExitCode ImageProcessingThread::Entry() {
  DEBUG_PRINT("[ImageProcessingThread] Start - tid:%d\n", this->GetId());
  is_running_ = true;

  cv::Mat* src_image = NULL;
  cv::Mat* dst_image = NULL;
  cv::Mat* temp_image = NULL;
  timeval start_time, end_time;
  double elapsed_time;
  bool comp_first_image_save = false;

  // Clear sub thread Map
  ClearSubThreadMap();

  //////////////////////////////////////////////////////////////
  // InitProcess
  //////////////////////////////////////////////////////////////
  PluginBase* plugin = reinterpret_cast<PluginBase*>(root_plugin_);
  PluginBase* next_plugin;
  PluginBase* temp_next_plugin;
  bool init_process_success = true;
  DEBUG_PRINT("[ImageProcessingThread] Initialize - tid:%d\n", this->GetId());
  while (1) {
    if (plugin) {
      DEBUG_PRINT(
          "[ImageProcessingThread] Do InitProcess plugin = %s - tid:%d\n",
          plugin->plugin_name().c_str(), this->GetId());
      if (plugin->InitProcess(common_param_) == false) {
        DEBUG_PRINT("[ImageProcessingThread] InitProcess fail plugin = %s\n",
                    plugin->plugin_name().c_str());
        LOG_ERROR("Failed to InitProcess - plugin:%s",
                  wxString::FromUTF8(plugin->plugin_name().c_str()).c_str());
        init_process_success = false;
        main_wnd_->PostStreamingError();
        break;
      }
      if (plugin->next_plugins().size() > 0) {
        temp_next_plugin = NULL;
        for (int i = 0; i < plugin->next_plugins().size(); i++) {
          next_plugin =
              reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
          if (!next_plugin) {
            main_wnd_->PostStreamingError();
            break;
          }
          unsigned int cycle = thread_running_cycle_manager_->GetCycle(
              plugin->plugin_name(), next_plugin->plugin_name());
          if (cycle == 0) {  // main flow
            temp_next_plugin = next_plugin;
          } else {  // sub flow
            SubThreadInfo* sub_thread_info = sub_threads_[plugin];
            wxSemaphore* sem = NULL;
            std::vector<ImageProcessingThread*>* threads = NULL;
            if (sub_thread_info == NULL) {
              sub_thread_info = new SubThreadInfo;
              sub_thread_info->sem = new wxSemaphore();
              sub_thread_info->threads =
                  new std::vector<ImageProcessingThread*>;
            }
            sem = sub_thread_info->sem;
            threads = sub_thread_info->threads;

            ImageProcessingThread* thread =
                new ImageProcessingThread(next_plugin, main_wnd_, common_param_,
                                          thread_running_cycle_manager_, sem);
            if (thread->Create() != wxTHREAD_NO_ERROR) {
              LOG_ERROR("Failed to create image processing sub thread");
              return (wxThread::ExitCode)0;
            }
            threads->push_back(thread);
            sub_threads_[plugin] = sub_thread_info;
            thread->Run();
          }
        }
        plugin = temp_next_plugin;
      } else {
        // End of flow
        break;
      }
    } else {
      // End of flow
      break;
    }
  }

  DEBUG_PRINT("[ImageProcessingThread] Mainloop - tid:%d\n", this->GetId());
  unsigned int frame_counter = 1;
  plugin = reinterpret_cast<PluginBase*>(root_plugin_);
  int plugin_index;
  while (!TestDestroy() && !stop_flag() && init_process_success) {
    DEBUG_PRINT("[ImageProcessingThread] frame_counter:%d, tid:%d\n",
                frame_counter, this->GetId());
    if (wait_sem_ && plugin == root_plugin_) {
      DEBUG_PRINT(
          "[ImageProcessingThread] Before sem wait tid:%d wait_sem_:0x%08x\n",
          this->GetId(), wait_sem_);
      wait_sem_->Wait();
      DEBUG_PRINT(
          "[ImageProcessingThread] After sem wait tid:%d wait_sem_:0x%08x\n",
          this->GetId(), wait_sem_);
      if (stop_flag()) break;
      receive_image_mutex_.Lock();
      if (src_image != NULL) {
        delete src_image;
        src_image = NULL;
      }
      src_image = new cv::Mat(receive_image_->size(), receive_image_->type());
      *src_image = receive_image_->clone();
      receive_image_mutex_.Unlock();
    }

    CvSize size;
    if (plugin) {
      gettimeofday(&start_time, NULL);
      if (plugin->output_port_candidate_specs().size() > 0) {
        size = plugin->output_image_size();
        int type = -1;
        if (size.width == 0 && size.height == 0) {
          DEBUG_PRINT("[ImageProcessingThread] output size is zero tid:%d\n",
                      this->GetId());
          LOG_ERROR("Output image size is zero - plugin:%s",
                    wxString::FromUTF8(plugin->plugin_name().c_str()).c_str());
          main_wnd_->PostStreamingError();
          break;
        }
        PortSpec* port_spec = plugin->output_port_spec();
        if (port_spec == NULL) {
          DEBUG_PRINT("[ImageProcessingThread] port_spec is NULL tid:%d\n",
                      this->GetId());
          LOG_ERROR("PortSpec is NULL - plugin:%s",
                    wxString::FromUTF8(plugin->plugin_name().c_str()).c_str());
          main_wnd_->PostStreamingError();
          break;
        }
        PlaneType plane_type = port_spec->plane_type();
        type = PluginManager::GetDepthAndChannelType(plane_type);

        if (type == -1) {
          DEBUG_PRINT(
              "[ImageProcessingThread] depth or channel not supported tid:%d\n",
              this->GetId());
          LOG_ERROR("Unknown plane type was detected - plugin:%s",
                    wxString::FromUTF8(plugin->plugin_name().c_str()).c_str());
          main_wnd_->PostStreamingError();
          break;
        }
        if (dst_image != NULL && ((dst_image->size().width != size.width) ||
                                  (dst_image->size().height != size.height) ||
                                  (dst_image->type() != type))) {
          DEBUG_PRINT("[ImageProcessingThread] release dst_image tid:%d\n",
                      this->GetId());
          DEBUG_PRINT(
              "[ImageProcessingThread] dst_image - width:%d, height:%d, "
              "type:%d\n",
              dst_image->size().width, dst_image->size().height,
              dst_image->type());
          DEBUG_PRINT("[ImageProcessingThread] width:%d, height:%d, type:%d\n",
                      size.width, size.height, type);
          delete dst_image;
          dst_image = NULL;
        }
        if (plugin->is_use_dest_buffer()) {
          if (dst_image == NULL) {
            DEBUG_PRINT(
                "[ImageProcessingThread] allocate dst image buffer - width:%d, "
                "height:%d, type:%d\n",
                size.width, size.height, type);
            dst_image = new cv::Mat(size, type);
          }
        } else {
          temp_image = dst_image;
          dst_image = src_image;
        }
      }
      //////////////////////////////////////////////////////////////
      // DoProcess
      //////////////////////////////////////////////////////////////
      DEBUG_PRINT("[ImageProcessingThread] DoProcess %s - tid:%d\n",
                  plugin->plugin_name().c_str(), this->GetId());
      if (plugin->DoProcess(src_image, dst_image) == false) {
        DEBUG_PRINT("[ImageProcessingThread] DoProcess fail plugin = %s\n",
                    plugin->plugin_name().c_str());
        LOG_ERROR("Failed to DoProcess - plugin:%s",
                  wxString::FromUTF8(plugin->plugin_name().c_str()).c_str());
        if (plugin->output_port_candidate_specs().size() > 0) {
          if (!plugin->is_use_dest_buffer()) {
            src_image = temp_image;
          }
        }
        main_wnd_->PostStreamingError();
        break;
      }
      plugin_index++;
      // If the first plugin and do_save_image_flag_ is on
      if (plugin_index == 1 && do_save_image_flag_ == true) {
        if (first_save_image_ != NULL) {
          delete first_save_image_;
          first_save_image_ = NULL;
        }
        // Save only Input plugin
        if (plugin->plugin_type() == kInputPlugin) {
          first_save_image_ = new cv::Mat(src_image->size(), src_image->type());
          if (plugin->is_use_dest_buffer()) {
            *first_save_image_ = dst_image->clone();
          } else {
            *first_save_image_ = src_image->clone();
          }
          first_plugin_name_ = plugin->plugin_name();
        }
        comp_first_image_save = true;
        DEBUG_PRINT(
            "[ImageProcessingThread] comp first plugin image save  tid:%d\n",
            this->GetId());
      }
      if (plugin->output_port_candidate_specs().size() > 0) {
        if (!plugin->is_use_dest_buffer()) {
          src_image = temp_image;
        }
      }
      temp_next_plugin = NULL;
      if (plugin->next_plugins().size() > 0) {
        for (int i = 0; i < plugin->next_plugins().size(); i++) {
          next_plugin =
              reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
          if (next_plugin) {
            next_plugin->set_input_image_size(size);
            unsigned int cycle = thread_running_cycle_manager_->GetCycle(
                plugin->plugin_name(), next_plugin->plugin_name());
            if (cycle == 0) {  // main flow
              temp_next_plugin = next_plugin;
            } else {  // sub flow
              if ((frame_counter % cycle) == 0) {
                SubThreadInfo* sub_thread_info = sub_threads_[plugin];
                if (sub_thread_info != NULL) {
                  wxSemaphore* sem = sub_thread_info->sem;
                  std::vector<ImageProcessingThread*>* threads =
                      sub_thread_info->threads;
                  std::vector<ImageProcessingThread*>::iterator itr;
                  for (itr = threads->begin(); itr != threads->end(); itr++) {
                    ImageProcessingThread* thread = *itr;
                    if (!thread->stop_flag()) {
                      thread->set_receive_image(dst_image);
                    }
                  }
                  sem->Post();
                }
              }
            }
          }
        }
        temp_image = dst_image;
        dst_image = src_image;
        src_image = temp_image;
        DEBUG_PRINT(
            "[ImageProcessingThread] image buffer swapping complete tid:%d\n",
            this->GetId());
        gettimeofday(&end_time, NULL);
        // sec to ms
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time +=
            (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // us to ms
        // cout << elapsed_time << " ms.\n";
        plugin->set_proc_time(static_cast<float>(elapsed_time));
      }
      if (temp_next_plugin) {
        plugin = temp_next_plugin;
      } else {
        DEBUG_PRINT("[ImageProcessingThread] DoPostProcess start tid:%d\n",
                    this->GetId());
        gettimeofday(&end_time, NULL);
        // sec to ms
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
        elapsed_time +=
            (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // us to ms
        // cout << elapsed_time << " ms.\n";
        plugin->set_proc_time(static_cast<float>(elapsed_time));

        if (do_save_image_flag_ == true && comp_first_image_save == true) {
          if (last_save_image_ != NULL) {
            delete last_save_image_;
            last_save_image_ = NULL;
          }

          last_save_image_ = new cv::Mat(src_image->size(), src_image->type());
          if (plugin->is_use_dest_buffer()) {
            *last_save_image_ = dst_image->clone();
          } else {
            *last_save_image_ = src_image->clone();
          }
          last_plugin_name_ = plugin->plugin_name();
          comp_save_image_ = true;
          do_save_image_flag_ = false;
          DEBUG_PRINT(
              "[ImageProcessingThread] comp last plugin image save  tid:%d\n",
              this->GetId());
        }

        //////////////////////////////////////////////////////////////
        // DoPostProcess
        //////////////////////////////////////////////////////////////
        plugin = reinterpret_cast<PluginBase*>(root_plugin_);
        while (1) {
          if (plugin) {
            DEBUG_PRINT("[ImageProcessingThread] DoPostProcess %s - tid:%d\n",
                        plugin->plugin_name().c_str(), this->GetId());
            plugin->DoPostProcess();
            if (plugin->next_plugins().size() > 0) {
              temp_next_plugin = NULL;
              for (int i = 0; i < plugin->next_plugins().size(); i++) {
                next_plugin =
                    reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
                if (!next_plugin) {
                  main_wnd_->PostStreamingError();
                  break;
                }
                unsigned int cycle = thread_running_cycle_manager_->GetCycle(
                    plugin->plugin_name(), next_plugin->plugin_name());
                if (cycle == 0) {  // main flow
                  temp_next_plugin = next_plugin;
                }
              }
              plugin = temp_next_plugin;
            } else {
              break;
            }
          } else {
            break;
          }
        }
        // End of Frame
        plugin = reinterpret_cast<PluginBase*>(root_plugin_);
        frame_counter++;
        plugin_index = 0;
      }
    }
  }

  plugin = reinterpret_cast<PluginBase*>(root_plugin_);
  float time = 0.0f;
  float all_time = 0.0f;

  //////////////////////////////////////////////////////////////
  // EndProcess
  //////////////////////////////////////////////////////////////
  while (1) {
    if (plugin) {
      DEBUG_PRINT("[ImageProcessingThread] EndProcess %s - tid:%d\n",
                  plugin->plugin_name().c_str(), this->GetId());
      plugin->EndProcess();
      time = plugin->proc_time();
      all_time += time;
      //printf("plugin name:%s time:%f(ms)\n", plugin->plugin_name().c_str(),
      //       time);
      if (plugin->next_plugins().size() > 0) {
        temp_next_plugin = NULL;
        for (int i = 0; i < plugin->next_plugins().size(); i++) {
          next_plugin =
              reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
          if (!next_plugin) {
            main_wnd_->PostStreamingError();
            break;
          }
          unsigned int cycle = thread_running_cycle_manager_->GetCycle(
              plugin->plugin_name(), next_plugin->plugin_name());
          if (cycle == 0) {  // main flow
            temp_next_plugin = next_plugin;
          }
        }
        plugin = temp_next_plugin;
      } else {
        break;
      }
    } else {
      break;
    }
  }
  if (src_image != NULL) {
    delete src_image;
    src_image = NULL;
  }
  if (dst_image != NULL) {
    delete dst_image;
    dst_image = NULL;
  }
  if (receive_image_ != NULL) {
    delete receive_image_;
    receive_image_ = NULL;
  }
  DEBUG_PRINT("[ImageProcessingThread] end - tid:%d\n", this->GetId());
  printf("all_time:%f(ms)\n", all_time);
  is_running_ = false;
  return (wxThread::ExitCode)0;
}

/**
 * @brief
 * Set a pointer to an image buffer data.
 * The buffer is input data for sub thread.
 * @param receive_image [in] pointer to an image buffer data.
 */
void ImageProcessingThread::set_receive_image(cv::Mat* dst_image) {
  receive_image_mutex_.Lock();
  if (receive_image_ != NULL &&
      ((dst_image->size().width != receive_image_->size().width) ||
       (dst_image->size().height != receive_image_->size().height) ||
       (dst_image->type() != receive_image_->type()))) {
    DEBUG_PRINT("[ImageProcessingThread] set_receive_image resize tid:%d\n",
                this->GetId());
    delete receive_image_;
    receive_image_ = NULL;
  }
  if (receive_image_ == NULL) {
    DEBUG_PRINT("[ImageProcessingThread] create receive_image tid:%d\n",
                this->GetId());
    receive_image_ = new cv::Mat(dst_image->size(), dst_image->type());
  }
  *receive_image_ = dst_image->clone();
  receive_image_mutex_.Unlock();
}

/**
 * @brief
 * Waits for the thread to terminate and returns.
 */
void ImageProcessingThread::WaitStop() {
  DEBUG_PRINT("[ImageProcessingThread] wait start tid:%d\n", this->GetId());
  while (is_running()) {
    usleep(1000);
  }
  DEBUG_PRINT("[ImageProcessingThread] wait end tid:%d\n", this->GetId());
}

/**
 * @brief
 * Stop the thread.
 * @param wait [in] Waits for the thread to terminate and returns, if wait is
 * true.
 */
void ImageProcessingThread::Stop(bool wait) {
  DEBUG_PRINT("[ImageProcessingThread] Stop start tid:%d\n", this->GetId());
  set_stop_flag();
  if (wait) {
    while (is_running()) {
        usleep(1000);
    }
  }
  DEBUG_PRINT("[ImageProcessingThread] Stop end tid:%d\n", this->GetId());
}

/**
 * @brief
 * Delete the thread and sub-threads.
 * @param rc [out] The thread exit code, if rc is not NULL.
 * return thread errors.
 */
wxThreadError ImageProcessingThread::Delete(ExitCode* rc) {
  DEBUG_PRINT("[ImageProcessingThread] Delete start tid:%d\n", this->GetId());

  wxThreadError err = wxThread::Delete(rc);
  std::map<IPlugin*, SubThreadInfo*>::iterator itr_sub_threads;
  for (itr_sub_threads = sub_threads_.begin();
       itr_sub_threads != sub_threads_.end(); ++itr_sub_threads) {
    SubThreadInfo* sub_thread_info = itr_sub_threads->second;
    if (sub_thread_info != NULL) {
      wxSemaphore* sem = sub_thread_info->sem;
      std::vector<ImageProcessingThread*>* threads = sub_thread_info->threads;
      if (threads != NULL) {
        std::vector<ImageProcessingThread*>::iterator itr;
        for (itr = threads->begin(); itr != threads->end(); ++itr) {
          ImageProcessingThread* thread = (*itr);
          if (thread != NULL) {
            thread->Stop(false);
          }
        }
        for (itr = threads->begin(); itr != threads->end(); ++itr) {
          ImageProcessingThread* thread = (*itr);
          if (thread != NULL) {
            sem->Post();
          }
        }
        for (itr = threads->begin(); itr != threads->end(); ++itr) {
          ImageProcessingThread* thread = (*itr);
          if (thread != NULL) {
            thread->Delete();
            if (thread->IsRunning()) {
              thread->Wait();
            }
          }
        }
        for (itr = threads->begin(); itr != threads->end(); ++itr) {
          ImageProcessingThread* thread = (*itr);
          if (thread != NULL) {
            thread->WaitStop();
          }
        }
      }
    }
  }
  DEBUG_PRINT("[ImageProcessingThread] Delete end tid:%d\n", this->GetId());
  return err;
}

/**
 * @brief
 * Set an image buffer saving flag.
 */
void ImageProcessingThread::DoSaveImage(void) {
  DEBUG_PRINT("[ImageProcessingThread] DoSaveImage tid:%d\n", this->GetId());
  do_save_image_flag_ = true;

  std::map<IPlugin*, SubThreadInfo*>::iterator itr;
  for (itr = sub_threads_.begin(); itr != sub_threads_.end(); ++itr) {
    SubThreadInfo* sub_thread_info = itr->second;
    if (sub_thread_info != NULL) {
      std::vector<ImageProcessingThread*>* threads = sub_thread_info->threads;
      if (threads != NULL) {
        for (int i = 0; i < threads->size(); i++) {
          ImageProcessingThread* thread = (*threads)[i];
          if (thread != NULL) {
            DEBUG_PRINT("[ImageProcessingThread] DoSaveImage sub thread\n");
            thread->DoSaveImage();
          }
        }
      }
    }
  }
}

/**
 * @brief
 * Is the image buffer data saving completed?
 * @return true, the image buffer data saving completed.
 */
bool ImageProcessingThread::IsCompSaveImage(void) {
  if (comp_save_image_ == false) {
    return false;
  }
  bool ret_val = true;
  std::map<IPlugin*, SubThreadInfo*>::iterator itr;
  for (itr = sub_threads_.begin(); itr != sub_threads_.end(); ++itr) {
    SubThreadInfo* sub_thread_info = itr->second;
    if (sub_thread_info != NULL) {
      std::vector<ImageProcessingThread*>* threads = sub_thread_info->threads;
      if (threads != NULL) {
        for (int i = 0; i < threads->size(); i++) {
          ImageProcessingThread* thread = (*threads)[i];
          if (thread != NULL) {
            ret_val = thread->IsCompSaveImage();
            if (ret_val == false) {
              break;
            }
          }
        }
      }
    }
  }
  return ret_val;
}

/**
 * @brief
 * Add new sub-threads to the map table.
 * @param threads [in] new sub-threads.
 */
void ImageProcessingThread::PushImageProcThread(
    std::vector<ImageProcessingThread*>* all_threads) {
  printf("ImageProcessingThread::PushImageProcThread start\n");
  std::map<IPlugin*, SubThreadInfo*>::iterator itr;
  for (itr = sub_threads_.begin(); itr != sub_threads_.end(); ++itr) {
    SubThreadInfo* sub_thread_info = itr->second;
    if (sub_thread_info != NULL) {
      std::vector<ImageProcessingThread*>* threads = sub_thread_info->threads;
      if (threads != NULL) {
        for (int i = 0; i < threads->size(); i++) {
          ImageProcessingThread* thread = (*threads)[i];
          if (thread != NULL) {
            all_threads->push_back(thread);
            printf(
                "ImageProcessingThread::PushImageProcThread "
                "threads->push_back\n");
            thread->PushImageProcThread(threads);
          }
        }
      }
    }
  }
}
