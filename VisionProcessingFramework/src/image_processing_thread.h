/**
 * @file      image_processing_thread.h
 * @brief     Header for ImageProcessingThread class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _IMAGE_PROCESSING_THREAD_H_
#define _IMAGE_PROCESSING_THREAD_H_

#include <map>
#include <vector>
#include <string>
#include "./common_param.h"
#include "./image_processing_thread.h"
#include "./include.h"
#include "./plugin_base.h"
#include "./thread_running_cycle_manager.h"

class MainWnd;

typedef bool ThreadFunc(void);

/**
 * @class ImageProcessingThread
 * @brief This class implements a thread process to execute plugins.
 */
class ImageProcessingThread : public wxThread {
  typedef struct SubThreadInfo {
    wxSemaphore* sem;
    std::vector<ImageProcessingThread*>* threads;
  } SubThreadInfo;

 public:
  /**
   * @brief
   * Constructor.
   * @param plugin [in] pointer to the IPlugin class.
   * @param main_wnd [in] pointer to the MainWnd class.
   * @param common_param [in] pointer to the CommonParam class.
   * @param thread_running_cycle_manager [in] pointer to the
   * ThreadRunningCycleManager class.
   * @param wait_sem [in] pointer to a semaphore object for synchronization
   * branch point.
   */
  ImageProcessingThread(IPlugin* plugin, MainWnd* main_wnd,
                        CommonParam* common_param,
                        ThreadRunningCycleManager* thread_running_cycle_manager,
                        wxSemaphore* wait_sem = NULL);

  /**
   * @brief
   * Destructor.
   */
  virtual ~ImageProcessingThread(void);

  /**
   * @brief
   * Thread entry point.
   * @return thread exit code.
   */
  virtual wxThread::ExitCode Entry(void);

  /**
   * @brief
   * Delete the thread and sub-threads.
   * @param rc [out] The thread exit code, if rc is not NULL.
   * return thread errors.
   */
  wxThreadError Delete(ExitCode* rc = reinterpret_cast<ExitCode*>(NULL));

  /**
   * @brief
   * Stop the thread.
   * @param wait [in] Waits for the thread to terminate and returns, if wait is
   * true.
   */
  void Stop(bool wait = true);

  /**
   * @brief
   * Waits for the thread to terminate and returns.
   */
  void WaitStop();

  /**
   * @brief
   * Set a pointer to an image buffer data. The buffer is input data for sub
   * thread.
   * @param receive_image [in] pointer to an image buffer data.
   */
  void set_receive_image(cv::Mat* receive_image);

  /**
   * @brief
   * Set thread stopping flag.
   */
  void set_stop_flag() { stop_flag_ = true; }

  /**
   * @brief
   * Get thread stopping flag.
   * @return thread stopping flag.
   */
  bool stop_flag() { return stop_flag_; }

  /**
   * @brief
   * Get thread running state.
   * @return true, this thread is currently running.
   */
  bool is_running() { return is_running_; }


  /**
   * @brief
   * Set an image buffer saving flag.
   */
  void DoSaveImage(void);

  /**
   * @brief
   * Is the image buffer data saving completed?
   * @return true, the image buffer data saving completed.
   */
  bool IsCompSaveImage(void);

  /**
   * @brief
   * Get a pointer to an image buffer data outputted by input plugin on the
   * flow.
   * @return Pointer to an image buffer data.
   */
  cv::Mat* first_save_image(void) { return first_save_image_; }

  /**
   * @brief
   * Get a pointer to an image buffer data outputted by last plugin on the flow.
   * @return Pointer to an image buffer data.
   */
  cv::Mat* last_save_image(void) { return last_save_image_; }

  /**
   * @brief
   * Get a plugin name of input plugin on the flow.
   * @return plugin name.
   */
  std::string first_plugin_name(void) { return first_plugin_name_; }

  /**
   * @brief
   * Get a plugin name of last plugin on the flow.
   * @return plugin name.
   */
  std::string last_plugin_name(void) { return last_plugin_name_; }

  /**
   * @brief
   * Get a pointer to the map table for managing sub-thread.
   * @return Pointer to the map table.
   */
  std::map<IPlugin*, SubThreadInfo*> sub_threads(void) { return sub_threads_; }

  /**
   * @brief
   * Add new sub-threads to the map table.
   * @param threads [in] new sub-threads.
   */
  void PushImageProcThread(std::vector<ImageProcessingThread*>* threads);

 private:
  /**
   * @brief
   * Clear a table for managing sub-threads.
   */
  void ClearSubThreadMap();

  /*! Pointer to the root plugin (NOT own it) */
  IPlugin* root_plugin_;

  /*! Map table for managing sub-threads */
  std::map<IPlugin*, SubThreadInfo*> sub_threads_;

  /*! Pointer to an image buffer data */
  cv::Mat* receive_image_;

  /*! Pointer to a semaphore object for synchronization branch point (NOT own
   * it) */
  wxSemaphore* wait_sem_;

  /*! Flags to require stopping the thread. */
  bool stop_flag_;

  /*! Flags to indicate whether the thread is running or not. */
  bool is_running_;

  /*! Pointer to a mutex object for atomic access to an image buffer data */
  wxMutex receive_image_mutex_;

  /*! Pointer to the MainWnd class (NOT own it) */
  MainWnd* main_wnd_;

  /*! Pointer to the ThreadRunningCycleManager class (NOT own it) */
  ThreadRunningCycleManager* thread_running_cycle_manager_;

  /*! Pointer to the CommonParam class (NOT own it) */
  CommonParam* common_param_;

  /*! Flags to indicate whether image buffer data saving completed or not. */
  bool comp_save_image_;

  /*! Flags to indicate whether the image buffer data is saved or not. */
  bool do_save_image_flag_;

  /*! Pointer to an image buffer data which was output by input plugin on the
   * flow. */
  cv::Mat* first_save_image_;

  /*! Pointer to an image buffer data which was output by last plugin on the
   * flow. */
  cv::Mat* last_save_image_;

  /*! The name of the input plugin on the flow. */
  std::string first_plugin_name_;

  /*! The name of the last plugin on the flow. */
  std::string last_plugin_name_;
};

#endif /* _IMAGE_PROCESSING_THREAD_H_*/
