/**
 * @file      main_wnd.h
 * @brief     Header for MainWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _MAIN_WND_H_
#define _MAIN_WND_H_

#include <queue>
#include <sstream>

#include "./common_function.h"
#include "./common_param.h"
#include "./image_processing_thread.h"
#include "./include.h"
#include "./plugin_manager.h"
#include "./plugin_manager_wnd.h"
#include "./save_as_image_wnd.h"
#include "./raw_save_wnd.h"
#include "./thread_running_cycle_manager.h"
#include "./version_info_wnd.h"
#include "./direct_access_wnd.h"
#include "./demo_ois_wnd.h"

/**
 * @class MainWnd
 * @brief Vision Processing Framework main window
 */
class MainWnd : public wxFrame {
 private:
  /*! Pointer to the CommonParam class */
  CommonParam *common_param_;

  /*! Pointer to the PluginManager class */
  PluginManager *plugin_manager_;

  /*! Pointer to the PluginManagerWnd class */
  PluginManagerWnd *plugin_manager_wnd_;

  /*! Pointer to the SaveAsImageWnd class */
  SaveAsImageWnd *save_as_image_wnd_;

  /*! Pointer to the RawSaveWnd class */
  RawSaveWnd *raw_save_wnd_;

  /*! Pointer to the VersionInfoWnd class */
  VersionInfoWnd *version_info_wnd_;

  /*! Pointer to the ImageProcessingThread class */
  ImageProcessingThread *image_proc_thread_;

  /*! ImageProcessingState object */
  //ImageProcessingState image_proc_state_;

  /*! Pointer to the ThreadRunningCycleManager class */
  ThreadRunningCycleManager *thread_running_cycle_manager_;

  /*! Pointer to the DirectAccessWnd class */
  DirectAccessWnd *direct_access_wnd_;

  /*! Pointer to the DemoOisWnd class */
  DemoOisWnd *demo_ois_wnd_;

 public:
  /*! ImageProcessingState object */
  ImageProcessingState image_proc_state_;
  /**
   * @brief
   * Constructor.
   */
  MainWnd(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~MainWnd(void);

  virtual void OnClose(wxCloseEvent &event);               /* NOLINT */
  virtual void OnStartMonitor(wxCommandEvent &event);      /* NOLINT */
  virtual void OnStopMonitor(wxCommandEvent &event);       /* NOLINT */
  virtual void OnPauseMonitor(wxCommandEvent &event);      /* NOLINT */
  virtual void OnMenuRawOpen(wxCommandEvent &event);       /* NOLINT */
  virtual void OnMenuAviOpen(wxCommandEvent &event);       /* NOLINT */
  virtual void OnMenuRawSave(wxCommandEvent &event);       /* NOLINT */
  virtual void OnMenuSaveAsImage(wxCommandEvent &event);   /* NOLINT */
  virtual void OnMenuSensorConfig(wxCommandEvent &event);  /* NOLINT */
  virtual void OnMenuPluginManager(wxCommandEvent &event); /* NOLINT */
  virtual void OnMenuDirectAccess(wxCommandEvent &event);  /* NOLINT */
  virtual void OnMenuVersion(wxCommandEvent &event);       /* NOLINT */
  virtual void OnIdle(wxIdleEvent &event);                 /* NOLINT */
  virtual void OnStreamingError(wxCommandEvent& eventt);   /* NOLINT */
  virtual void OnMenuDemoOis(wxCommandEvent &event);       /* NOLINT */

  /**
   * @brief
   * Get the ImageProcessingState.
   * @return image processing state.
   */
  ImageProcessingState image_proc_state(void);

  /**
   * @brief
   * Start image processing thread.
   */
  void StartProcessingThread(void);

  /**
   * @brief
   * Pause image processing thread.
   */
  void PauseProcessingThread(void);

  /**
   * @brief
   * Post streaming error event.
   */
  void PostStreamingError();

  /**
   * @brief
   * Get the data that plug-in processing.
   * Can be specified the root plugin on Flow.
   * @param plugin [in] root pluin name
   * @return the data that plugin processing
   */
  cv::Mat* RawSaveData(std::string plugin_name);

  /**
   * @brief
   * Get the data that plug-in processing.
   * Can be specified the end plugin on Flow.
   * @param plugin [in] plugin name that you want to save
   * @return the data that plugin processing
   */
  cv::Mat* SaveAsImage(std::string plugin_name);

 protected:
  wxMenuBar *menu_bar_main_wnd_;
  wxMenu *menu_file_;
  wxMenu *menu_sensor_config_;
  wxMenu *menu_plugin_manager_;
  wxMenu *menu_tool_;
  wxMenu *menu_help_;
  wxBitmapButton *bitmap_button_start_;
  wxBitmapButton *bitmap_button_stop_;
  wxBitmapButton *bitmap_button_pause_;
  wxStaticText *static_text_log_;
  wxTextCtrl *text_ctrl_log_;
  wxLogTextCtrl *log_text_ctrl_log_;

 private:
  DECLARE_EVENT_TABLE();
};

#endif /* _MAIN_WND_H_*/
