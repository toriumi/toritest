/**
 * @file      main_wnd.cpp
 * @brief     Source for MainWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./main_wnd.h"
#include <queue>
#include <string>
#include <vector>
#include "./logger.h"
#include "./main_wnd_define.h"
#include "./thread_running_cycle_manager.h"

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(STREAMING_ERROR, wxNewEventType())
END_DECLARE_EVENT_TYPES()

DEFINE_LOCAL_EVENT_TYPE(STREAMING_ERROR)

BEGIN_EVENT_TABLE(MainWnd, wxFrame)
EVT_CLOSE(MainWnd::OnClose)
EVT_BUTTON(kBitmapButtonStartId, MainWnd::OnStartMonitor)
EVT_BUTTON(kBitmapButtonStopId, MainWnd::OnStopMonitor)
EVT_BUTTON(kBitmapButtonPauseId, MainWnd::OnPauseMonitor)
EVT_BUTTON(kBitmapButtonPauseId, MainWnd::OnPauseMonitor)
EVT_MENU(kMenuRawOpenId, MainWnd::OnMenuRawOpen)
EVT_MENU(kMenuAviOpenId, MainWnd::OnMenuAviOpen)
EVT_MENU(kMenuRawSaveId, MainWnd::OnMenuRawSave)
EVT_MENU(kMenuSaveAsImageId, MainWnd::OnMenuSaveAsImage)
EVT_MENU(kMenuSensorConfigId, MainWnd::OnMenuSensorConfig)
EVT_MENU(kMenuPluginManagerId, MainWnd::OnMenuPluginManager)
EVT_MENU(kMenuDirectAccessId, MainWnd::OnMenuDirectAccess)
EVT_MENU(kMenuDemoOisId, MainWnd::OnMenuDemoOis)
EVT_MENU(kMenuVersionId, MainWnd::OnMenuVersion)
EVT_IDLE(MainWnd::OnIdle)
EVT_COMMAND(wxID_ANY, STREAMING_ERROR, MainWnd::OnStreamingError)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor.
 */
MainWnd::MainWnd()
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  wxBitmap bitmap; /* Bitmap file.*/

  wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
  /* Creating a Log static text object.*/
  static_text_log_ =
      new wxStaticText(this, kStaticTextLogId, wxT(kStaticTextLogName),
                       wxPoint(kStaticTextLogPointX, kStaticTextLogPointY),
                       wxSize(kStaticTextLogSizeW, kStaticTextLogSizeH));

  /* Creating a Log text ctrl object.*/
  text_ctrl_log_ =
      new wxTextCtrl(this, kTextCtrlLogId, wxT(kTextCtrlLogName),
                     wxPoint(kTextCtrlLogPointX, kTextCtrlLogPointY),
                     wxSize(kTextCtrlLogSizeW, kTextCtrlLogSizeH),
                     wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);

  /* Creating a Log object.*/
  log_text_ctrl_log_ = new wxLogTextCtrl(text_ctrl_log_);

  /* To attach the output target to wxLog.*/
  Logger::Initialize(log_text_ctrl_log_);

  LOG_STATUS("Vision Processing Framework startup!");

  /* PluginManager class object creation.*/
  plugin_manager_ = new PluginManager;

  common_param_ = new CommonParam;

  thread_running_cycle_manager_ = new ThreadRunningCycleManager;

  image_proc_thread_ = NULL;
  image_proc_state_ = kStop;

  /* Get version info */
  int major_ver = plugin_manager_->MajorVersion();
  int target_if_ver = plugin_manager_->TargetPluginIFVersion();
  int minimum_if_ver = plugin_manager_->MinimumRequiredPluginIFVersion();

  /* UI creation of the main window.*/
  /* Creating a menu file object.*/
  menu_file_ = new wxMenu();
  menu_file_->Append(kMenuRawOpenId, wxT(kMenuRawOpen));
  menu_file_->Append(kMenuAviOpenId, wxT(kMenuAviOpen));
  menu_file_->Append(kMenuRawSaveId, wxT(kMenuRawSave));
  menu_file_->Append(kMenuSaveAsImageId, wxT(kMenuSaveAsImage));

  /* Creating a menu sensor config object.*/
  menu_sensor_config_ = new wxMenu();
  menu_sensor_config_->Append(kMenuSensorConfigId, wxT(kMenuSensorConfig));

  /* Creating a menu tool object.*/
  menu_tool_ = new wxMenu();
  menu_tool_->Append(kMenuDirectAccessId, wxT(kMenuDirectAccess));
  menu_tool_->Append(kMenuDemoOisId, wxT(kMenuDemoOis));

  /* Creating a menu plugin manager object.*/
  menu_plugin_manager_ = new wxMenu();
  menu_plugin_manager_->Append(kMenuPluginManagerId, wxT(kMenuPluginManager));

  /* Creating a menu help object.*/
  menu_help_ = new wxMenu();
  menu_help_->Append(kMenuVersionId, wxT(kMenuVersion));

  /* Creating a main window menubar object.*/
  menu_bar_main_wnd_ = new wxMenuBar();
  menu_bar_main_wnd_->Append(menu_file_, wxT(kMenuBarFile));
  menu_bar_main_wnd_->Append(menu_sensor_config_, wxT(kMenuBarSensorConfig));
  menu_bar_main_wnd_->Append(menu_plugin_manager_, wxT(kMenuBarPluginManager));
  menu_bar_main_wnd_->Append(menu_tool_, wxT(kMenuBarTool));
  menu_bar_main_wnd_->Append(menu_help_, wxT(kMenuBarHelp));
  SetMenuBar(menu_bar_main_wnd_);

  /* Creating a Start button object.*/
  bitmap.LoadFile(wxT(kBitmapButtonStartPath), wxBITMAP_TYPE_BMP);
  bitmap_button_start_ = new wxBitmapButton(
      this, kBitmapButtonStartId, bitmap,
      wxPoint(kBitmapButtonStartPointX, kBitmapButtonStartPointY),
      wxSize(kBitmapButtonStartSizeW, kBitmapButtonStartSizeH));

  /* Creating a Stop button object.*/
  bitmap.LoadFile(wxT(kBitmapButtonStopPath), wxBITMAP_TYPE_BMP);
  bitmap_button_stop_ = new wxBitmapButton(
      this, kBitmapButtonStopId, bitmap,
      wxPoint(kBitmapButtonStopPointX, kBitmapButtonStopPointY),
      wxSize(kBitmapButtonStopSizeW, kBitmapButtonStopSizeH));

  /* Creating a Pause button object.*/
  bitmap.LoadFile(wxT(kBitmapButtonPausePath), wxBITMAP_TYPE_BMP);
  bitmap_button_pause_ = new wxBitmapButton(
      this, kBitmapButtonPauseId, bitmap,
      wxPoint(kBitmapButtonPausePointX, kBitmapButtonPausePointY),
      wxSize(kBitmapButtonPauseSizeW, kBitmapButtonPauseSizeH));

  /* disable stop button.*/
  bitmap_button_stop_->Enable(false);

  /* disable pause button.*/
  bitmap_button_pause_->Enable(false);

  button_sizer->Add(bitmap_button_start_, 0, wxALL, 5);
  button_sizer->Add(bitmap_button_stop_, 0, wxALL, 5);
  button_sizer->Add(bitmap_button_pause_, 0, wxALL, 5);
  top_sizer->Add(button_sizer, 0, wxALIGN_LEFT);
  top_sizer->Add(static_text_log_, 0, wxALL, 5);
  top_sizer->Add(text_ctrl_log_, 1, wxEXPAND | wxALL, 5);
  SetSizerAndFit(top_sizer);

  /* Plugin list reading process.*/
  plugin_manager_->LoadPlugins(kPluginPath);

  /* Set root plugin according to the following priority. */
  /* 1.Bin plugin */
  /* 2.Sensor plugin */
  /* 3.Avi plugin */
  /* If these plugins do not exist, not set root plugin. */
  std::vector<std::string> input_plugin_names;
  std::vector<std::string>::iterator itr;
  std::string bin_plugin_name = "Bin";
  std::string sensor_plugin_name = "Sensor";
  std::string avi_plugin_name = "Avi";
  PluginBase *input_plugin = NULL;
  bool is_success = false;
  input_plugin_names = plugin_manager_->GetPluginNames(kInputPlugin);
  for (itr = input_plugin_names.begin(); itr != input_plugin_names.end();
       itr++) {
    std::string name = *itr;
    std::string::size_type index = name.find(sensor_plugin_name);
    if (index != std::string::npos) {
      input_plugin = plugin_manager_->GetPlugin(name);
    } else {
      index = name.find(bin_plugin_name);
      if (index != std::string::npos) {
        input_plugin = plugin_manager_->GetPlugin(name);
      } else {
        index = name.find(avi_plugin_name);
        if (index != std::string::npos) {
          input_plugin = plugin_manager_->GetPlugin(name);
        }
      }
    }
    if (input_plugin != NULL) {
      is_success =
          plugin_manager_->set_root_plugin(input_plugin->plugin_name());
      if (is_success) {
        DEBUG_PRINT("MainWnd::MainWnd update root plugin success.\n");
        break;
      } else {
        DEBUG_PRINT("MainWnd::MainWnd update root plugin fail.\n");
      }
    }
  }
  /* PluginManager window class creation.*/
  plugin_manager_wnd_ =
      new PluginManagerWnd(plugin_manager_, thread_running_cycle_manager_);

  /* save as image window class creation.*/
  save_as_image_wnd_ = new SaveAsImageWnd(this);

  /* save as image window disable save button.*/
  save_as_image_wnd_->button_file_dialog_->Enable(false);

  /* raw save window class creation.*/
  raw_save_wnd_ = new RawSaveWnd(this);

  /* raw save window disable save button.*/
  raw_save_wnd_->button_file_dialog_->Enable(false);

  /* DirectAccess window class creation.*/
  direct_access_wnd_ = new DirectAccessWnd(this);

  /* DemoOis window class creation.*/
  demo_ois_wnd_ = new DemoOisWnd(this);

  /* Version Information window class creation.*/
  version_info_wnd_ =
      new VersionInfoWnd(major_ver, target_if_ver, minimum_if_ver);

  image_proc_state_ = kStop;
}

/**
 * @brief
 * Destructor.
 */
MainWnd::~MainWnd() {
  DEBUG_PRINT("MainWnd::~MainWnd()\n");
  /* PluginManager class object discarded.*/
  //if (plugin_manager_ != NULL) {
  //  delete plugin_manager_;
  //}
  /* PluginManager window class object discarded.*/
  delete plugin_manager_wnd_;
  /* DirectAccess window class object discarded.*/
  delete direct_access_wnd_;
  /* DemoOis window class object discarded.*/
  delete demo_ois_wnd_;
  /* save as image window class discarded.*/
  delete save_as_image_wnd_;
  /* raw save window class discarded.*/
  delete raw_save_wnd_;
  /* Version Information window class discarded.*/
  delete version_info_wnd_;
  /* CommonParam class object discarded.*/
  delete common_param_;
  /* Thread Running cycle Manager class object discarded.*/
  if (thread_running_cycle_manager_ != NULL) {
    delete thread_running_cycle_manager_;
  }
  /* ImageProcessingThread class object discarded.*/
  if (image_proc_thread_ != NULL) {
    delete image_proc_thread_;
  }
  /* PluginManager class object discarded.*/
  if (plugin_manager_ != NULL) {
    delete plugin_manager_;
  }
}

void MainWnd::OnClose(wxCloseEvent &event) {
  DEBUG_PRINT("MainWnd::OnClose\n");
  wxExit();
}

/**
 * @brief
 * Start image processing thread.
 */
void MainWnd::StartProcessingThread() {
  DEBUG_PRINT("MainWnd::StartProcessingThread\n");
  LOG_STATUS("Start Monitoring");

  PluginBase *root_plugin = plugin_manager_->root_plugin();

  if (root_plugin == NULL) {
    wxMessageDialog dialog(NULL, wxT("Root plugin is not selected "),
                           wxT("Error"), wxOK, wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
    LOG_WARNING("Failed to start monitoring");
    return;
  } else {
    DEBUG_PRINT("MainWnd::OnStartMonitor: root_plugin name = %s\n",
                root_plugin->plugin_name().c_str());
  }

  // Check runnable setting
  if (plugin_manager_->CheckExecuteSetting(plugin_manager_->root_plugin()) ==
      false) {
    wxMessageDialog dialog(NULL, wxT("Not correct plugin connect.\n"
                           "For more information in the log."),
                           wxT("Error"), wxOK, wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
    return;
  }

  if (image_proc_thread_ == NULL) {
    /* Create image processing thread and Run it */
    image_proc_thread_ = new ImageProcessingThread(
        root_plugin, this, common_param_, thread_running_cycle_manager_, NULL);
    if (image_proc_thread_ == NULL) {
      LOG_ERROR("Failed to allocate image processing thread")
      return;
    }
    if (image_proc_thread_->Create() != wxTHREAD_NO_ERROR) {
      LOG_ERROR("Failed to create image processing thread")
      return;
    } else {
      DEBUG_PRINT("MainWnd::OnStartMonitor: success to create thread\n");
    }
    image_proc_thread_->Run();
  } else {
    /* Resume */
    image_proc_thread_->Resume();
  }

  /* Update Menu Icon */
  bitmap_button_start_->Enable(false);
  bitmap_button_stop_->Enable(true);
  bitmap_button_pause_->Enable(true);
  /* save as image window enable save button.*/
  save_as_image_wnd_->button_file_dialog_->Enable(true);
  /* raw save window enable save button.*/
  raw_save_wnd_->button_file_dialog_->Enable(true);

  /* Update State */
  image_proc_state_ = kRun;
  plugin_manager_->NotifyState(image_proc_state_);
  plugin_manager_wnd_->set_image_proc_state(image_proc_state_);
}

void MainWnd::OnStartMonitor(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnStartMonitor\n");
  /* Start image processing thread */
  StartProcessingThread();

  Iconize(true);
  Iconize(false);
}

void MainWnd::OnStopMonitor(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnStopMonitor\n");
  LOG_STATUS("Stop Monitoring");

  bool wait = true;
  if (image_proc_state_ == kPause) {
    wait = false;
  }
  /* Stop image processing thread */
  image_proc_thread_->Stop(wait);
  image_proc_thread_->Delete();

  if (image_proc_thread_->IsRunning()) {
    image_proc_thread_->Wait();
  }

  delete image_proc_thread_;
  image_proc_thread_ = NULL;

  /* Update Menu Icon */
  bitmap_button_start_->Enable(true);
  bitmap_button_stop_->Enable(false);
  bitmap_button_pause_->Enable(false);
  /* save as image window disable save button.*/
  save_as_image_wnd_->button_file_dialog_->Enable(false);
  /* raw save window disable save button.*/
  raw_save_wnd_->button_file_dialog_->Enable(false);

  /* Update State */
  image_proc_state_ = kStop;
  plugin_manager_->NotifyState(image_proc_state_);
  plugin_manager_wnd_->set_image_proc_state(image_proc_state_);
}

/**
 * @brief
 * Pause image processing thread.
 */
void MainWnd::PauseProcessingThread() {
  DEBUG_PRINT("MainWnd::PauseProcessingThread\n");
  /* Pause image processing thread */
  image_proc_thread_->DoSaveImage();
  while (true) {
    if (image_proc_thread_->IsCompSaveImage() == true) {
      break;
    }
  }
  image_proc_thread_->Pause();
  while (true) {
    if (!image_proc_thread_->IsRunning()) {
      break;
    }
  }
  /* Update Menu Icon */
  bitmap_button_start_->Enable(true);
  bitmap_button_stop_->Enable(true);
  bitmap_button_pause_->Enable(false);
  /* save as image window enable save button.*/
  save_as_image_wnd_->button_file_dialog_->Enable(true);
  /* raw save window enable save button.*/
  raw_save_wnd_->button_file_dialog_->Enable(true);
  /* Update State */
  image_proc_state_ = kPause;
  plugin_manager_->NotifyState(image_proc_state_);
  plugin_manager_wnd_->set_image_proc_state(image_proc_state_);
  LOG_STATUS("Pause Monitoring");
}

void MainWnd::OnPauseMonitor(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnPauseMonitor\n");
  /* Pause image processing thread */
  PauseProcessingThread();
}

void MainWnd::OnMenuRawOpen(wxCommandEvent &event) {
  //  std::string root_plugin_name =
  //  plugin_manager_->root_plugin()->plugin_name();

  std::vector<std::string> input_plugin_names;
  std::vector<std::string>::iterator itr;
  std::string bin_plugin_name = "Bin";
  PluginBase *input_plugin = NULL;
  input_plugin_names = plugin_manager_->GetPluginNames(kInputPlugin);
  for (itr = input_plugin_names.begin(); itr != input_plugin_names.end();
       itr++) {
    std::string name = *itr;
    std::string::size_type index = name.find(bin_plugin_name);
    if (index != std::string::npos) {
      input_plugin = plugin_manager_->GetPlugin(name);
      break;
    }
  }
  // Get Bin plugin and open setting window.
  //  PluginBase *bin_plugin = plugin_manager_->GetPlugin(bin_plugin_name);
  if (input_plugin != NULL) {
    input_plugin->OpenSettingWindow(image_proc_state_);

    std::string root_plugin_name;
    PluginBase *root_plugin = plugin_manager_->root_plugin();
    if (root_plugin != NULL) {
      root_plugin_name = root_plugin->plugin_name();
    }
    if (root_plugin_name != input_plugin->plugin_name()) {
      bool is_success =
          plugin_manager_->set_root_plugin(input_plugin->plugin_name());
      if (is_success) {
        DEBUG_PRINT("OnMenuRawOpen Success to change root plugin.\n");
        plugin_manager_wnd_->ResetPluginList();
      } else {
        DEBUG_PRINT("OnMenuRawOpen Failt to change root plugin.\n");
      }
    }
  } else {
    wxMessageDialog dialog(NULL, wxT("Bin is not exist."), wxT("Error"), wxOK,
                           wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
    return;
  }
}

void MainWnd::OnMenuAviOpen(wxCommandEvent &event) {
  //  std::string root_plugin_name =
  //  plugin_manager_->root_plugin()->plugin_name();

  std::vector<std::string> input_plugin_names;
  std::vector<std::string>::iterator itr;
  std::string avi_plugin_name = "Avi";
  PluginBase *input_plugin = NULL;
  input_plugin_names = plugin_manager_->GetPluginNames(kInputPlugin);
  for (itr = input_plugin_names.begin(); itr != input_plugin_names.end();
       itr++) {
    std::string name = *itr;
    std::string::size_type index = name.find(avi_plugin_name);
    if (index != std::string::npos) {
      input_plugin = plugin_manager_->GetPlugin(name);
      break;
    }
  }
  // Get Avi plugin and open setting window.
  //  PluginBase *avi_plugin = plugin_manager_->GetPlugin(avi_plugin_name);
  if (input_plugin != NULL) {
    input_plugin->OpenSettingWindow(image_proc_state_);

    std::string root_plugin_name;
    PluginBase *root_plugin = plugin_manager_->root_plugin();
    if (root_plugin != NULL) {
      root_plugin_name = root_plugin->plugin_name();
    }
    if (root_plugin_name != input_plugin->plugin_name()) {
      bool is_success =
          plugin_manager_->set_root_plugin(input_plugin->plugin_name());
      if (is_success) {
        DEBUG_PRINT("OnMenuAviOpen Success to change root plugin.\n");
        plugin_manager_wnd_->ResetPluginList();
      } else {
        DEBUG_PRINT("OnMenuAviOpen Failt to change root plugin.\n");
      }
    }
  } else {
    wxMessageDialog dialog(NULL, wxT("Avi is not exist."), wxT("Error"), wxOK,
                           wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
    return;
  }
}

void MainWnd::OnMenuRawSave(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuRawSave\n");
  std::vector<std::string> input_plugin_names;
  std::vector<std::string>::iterator itr;
  std::string sensor_plugin_name = "Sensor";
  PluginBase *input_plugin = NULL;
  raw_save_wnd_->combo_box_output_->Clear();
  raw_save_wnd_->raw_save_pause_flag_ = false;

  input_plugin_names = plugin_manager_->GetPluginNames(kInputPlugin);
  for (itr = input_plugin_names.begin(); itr != input_plugin_names.end();
       itr++) {
    std::string name = *itr;

    PluginBase *temp_base = plugin_manager_->GetPlugin(name);
    if (temp_base->next_plugins().size() > 0) {
      std::string::size_type index = name.find(sensor_plugin_name);
      if (index != std::string::npos) {
        wxString plugin_name(name.c_str(), wxConvUTF8);
        raw_save_wnd_->combo_box_output_->Append(plugin_name);
      }
    }
  }
  raw_save_wnd_->combo_box_output_->SetSelection(0);
  /* Open raw save window.*/
  raw_save_wnd_->Show(true);
  raw_save_wnd_->Raise();

  /* Pause image processing thread */
  if (image_proc_state_ == kRun) {
    raw_save_wnd_->raw_save_pause_flag_ = true;
    PauseProcessingThread();
  }
}

/**
 * @brief
 * Get the data that plug-in processing.
 * Can be specified the root plugin on Flow.
 * @param plugin [in] root pluin name
 * @return the data that plugin processing
 */
cv::Mat *MainWnd::RawSaveData(std::string plugin_name) {
  DEBUG_PRINT("MainWnd::RawSaveData\n");
  /* Get save raw image*/
  std::string first_plugin_name = "";

  /* Pause image processing thread */
  if (image_proc_state_ == kStop) {
    return NULL;
  }
  if (image_proc_state_ == kRun) {
    raw_save_wnd_->raw_save_pause_flag_ = true;
    PauseProcessingThread();
  }

  first_plugin_name = image_proc_thread_->first_plugin_name();
  if (plugin_name == first_plugin_name) {
    return image_proc_thread_->first_save_image();
  }
  std::vector<ImageProcessingThread *> *threads =
      new std::vector<ImageProcessingThread *>;
  image_proc_thread_->PushImageProcThread(threads);
  std::vector<ImageProcessingThread *>::iterator itr;
  for (itr = threads->begin(); itr != threads->end(); itr++) {
    ImageProcessingThread *tmp_thread = *itr;
    if (tmp_thread != NULL) {
      first_plugin_name = "";
      first_plugin_name = tmp_thread->first_plugin_name();
      if (plugin_name == first_plugin_name) {
        threads->clear();
        delete threads;
        return tmp_thread->first_save_image();
      }
    }
  }
  threads->clear();
  delete threads;
  return NULL;
}

/**
 * @brief
 * Get the data that plug-in processing.
 * Can be specified the end plugin on Flow.
 * @param plugin [in] plugin name that you want to save
 * @return the data that plugin processing
 */
void MainWnd::OnMenuSaveAsImage(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuSaveAsImage\n");
  /* Open save as image window.*/
  std::vector<std::string> output_plugin_names;
  std::vector<std::string>::iterator itr;
  PluginBase *input_plugin = NULL;
  save_as_image_wnd_->combo_box_output_->Clear();
  save_as_image_wnd_->save_as_image_pause_flag_ = false;

  output_plugin_names = plugin_manager_->GetPluginNames(kOutputPlugin);
  for (itr = output_plugin_names.begin(); itr != output_plugin_names.end();
       itr++) {
    std::string name = *itr;
    if (plugin_manager_wnd_->FindPluginNameFromPluginList(name) == true) {
      wxString plugin_name(name.c_str(), wxConvUTF8);
      save_as_image_wnd_->combo_box_output_->Append(plugin_name);
    }
  }
  save_as_image_wnd_->combo_box_output_->SetSelection(0);
  /* Open image save window.*/
  save_as_image_wnd_->Show(true);
  save_as_image_wnd_->Raise();

  /* Pause image processing thread */
  if (image_proc_state_ == kRun) {
    save_as_image_wnd_->save_as_image_pause_flag_ = true;
    PauseProcessingThread();
  }
}

/**
 * @brief
 * XXXX.
 */
cv::Mat *MainWnd::SaveAsImage(std::string plugin_name) {
  DEBUG_PRINT("MainWnd::SaveAsImage\n");
  /* Get save image*/
  std::string last_plugin_name = "";

  /* Pause image processing thread */
  if (image_proc_state_ == kStop) {
    return NULL;
  }
  if (image_proc_state_ == kRun) {
    save_as_image_wnd_->save_as_image_pause_flag_ = true;
    PauseProcessingThread();
  }

  last_plugin_name = image_proc_thread_->last_plugin_name();
  if (plugin_name == last_plugin_name) {
    return image_proc_thread_->last_save_image();
  }
  std::vector<ImageProcessingThread *> *threads =
      new std::vector<ImageProcessingThread *>;
  image_proc_thread_->PushImageProcThread(threads);
  std::vector<ImageProcessingThread *>::iterator itr;
  for (itr = threads->begin(); itr != threads->end(); itr++) {
    ImageProcessingThread *tmp_thread = *itr;
    if (tmp_thread != NULL) {
      last_plugin_name = "";
      last_plugin_name = tmp_thread->last_plugin_name();
      if (plugin_name == last_plugin_name) {
        threads->clear();
        delete threads;
        return tmp_thread->last_save_image();
      }
    }
  }
  threads->clear();
  delete threads;
  return NULL;
}

void MainWnd::OnMenuSensorConfig(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuSensorConfig\n");

  //  std::string root_plugin_name =
  //  plugin_manager_->root_plugin()->plugin_name();
  std::vector<std::string> input_plugin_names;
  std::vector<std::string>::iterator itr;
  std::string sensor_plugin_name = "Sensor";
  PluginBase *input_plugin = NULL;
  input_plugin_names = plugin_manager_->GetPluginNames(kInputPlugin);
  for (itr = input_plugin_names.begin(); itr != input_plugin_names.end();
       itr++) {
    std::string name = *itr;
    std::string::size_type index = name.find(sensor_plugin_name);
    if (index != std::string::npos) {
      input_plugin = plugin_manager_->GetPlugin(name);
      break;
    }
  }
  // Get Sensor plugin and open setting window.
  if (input_plugin != NULL) {
    input_plugin->OpenSettingWindow(image_proc_state_);

    std::string root_plugin_name;
    PluginBase *root_plugin = plugin_manager_->root_plugin();
    if (root_plugin != NULL) {
      root_plugin_name = root_plugin->plugin_name();
    }
    if (root_plugin_name != input_plugin->plugin_name()) {
      bool is_success =
          plugin_manager_->set_root_plugin(input_plugin->plugin_name());
      if (is_success) {
        DEBUG_PRINT("OnMenuSensorConfig Success to change root plugin.\n");
        plugin_manager_wnd_->ResetPluginList();
      } else {
        DEBUG_PRINT("OnMenuSensorConfig Failt to change root plugin.\n");
      }
    }
  } else {
    wxMessageDialog dialog(NULL, wxT("Sensor is not exist."), wxT("Error"),
                           wxOK, wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
    return;
  }
}

void MainWnd::OnMenuPluginManager(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuPluginManager\n");

  /* Open plugin manager window.*/
  plugin_manager_wnd_->set_image_proc_state(image_proc_state_);
  plugin_manager_wnd_->Show(true);
}

void MainWnd::OnMenuDirectAccess(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuDirectAccess\n");

  /* Open direct access window.*/
  direct_access_wnd_->Show(true);
}

void MainWnd::OnMenuDemoOis(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuDemoOis\n");

  /* Open demo ois window.*/
  /*! Pointer to the CommonParam class */
  demo_ois_wnd_->common_param_ = common_param_;
  /*! Pointer to the PluginManager class */
  demo_ois_wnd_->plugin_manager_ = plugin_manager_;
  /*! Pointer to the PluginManagerWnd class */
  demo_ois_wnd_->plugin_manager_wnd_ = plugin_manager_wnd_;
  /*! ImageProcessingState object */
  demo_ois_wnd_->image_proc_state_ = image_proc_state_;
  /*! Pointer to the ThreadRunningCycleManager class */
  demo_ois_wnd_->thread_running_cycle_manager_ = thread_running_cycle_manager_;

  demo_ois_wnd_->Show(true);
}

void MainWnd::OnMenuVersion(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnMenuVersion\n");
  /* Open version information window.*/
  //version_info_wnd_->ShowModal();
  version_info_wnd_->Show(true);
}

void MainWnd::OnIdle(wxIdleEvent &event) {
  enum LogLevel level;
  wxString message;
  if (Logger::GetLogMessage(&level, &message)) {
    switch (level) {
      case kLogFatalError:
        wxLogFatalError(message.c_str());
        break;
      case kLogError:
        wxLogError(message.c_str());
        break;
      case kLogWarning:
        wxLogWarning(message.c_str());
        break;
      case kLogMessage:
        wxLogMessage(message.c_str());
        break;
      case kLogVerbose:
        wxLogVerbose(message.c_str());
        break;
      case kLogStatus:
        wxLogStatus(message.c_str());
        break;
      //  case kLogSysError:
      //    wxLogSysError(message.c_str());
      //    break;
      case kLogDebug:
        wxLogDebug(message.c_str());
        break;
      //  case kLogTrace:
      //    wxLogTrace(message.c_str());
      //    break;
      default:
        break;
    }
  }
}

void MainWnd::OnStreamingError(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnStreamingError\n");
  OnStopMonitor(event);
}

/**
 * @brief
 * Post streaming error event.
 */
void MainWnd::PostStreamingError() {
  wxCommandEvent event(STREAMING_ERROR);

  event.SetString(wxT("Dummy"));
  wxPostEvent(this, event);
}

/**
 * @brief
 * Get the ImageProcessingState.
 * @return image processing state.
 */
ImageProcessingState MainWnd::image_proc_state(void) {
  return image_proc_state_;
}
