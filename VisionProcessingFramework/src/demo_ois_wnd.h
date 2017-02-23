/**
 * @file      demo_ois_wnd.h
 * @brief     Header for DemoOisWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _DEMO_OIS_WND_H_
#define _DEMO_OIS_WND_H_

#include "./include.h"
#include "./plugin_manager_wnd.h"
#include "./plugin_edit_canvas.h"
#define _SENSOR_DEFINE_H_
#include "./Plugins/Sensor/sensor.h"
#define _SENSOR_FOCUS_DEFINE_H_
#include "./Plugins/SensorFocus/sensor_focus_wnd.h"

/**
 * @class DemoOisWnd
 * @brief Demo Ois window class
 */
class DemoOisWnd : public wxFrame {
 private:

 public:
  /* main_wnd.h */
  /*! Pointer to the CommonParam class */
  CommonParam *common_param_;
  /*! Pointer to the PluginManager class */
  PluginManager *plugin_manager_;
  /*! Pointer to the PluginManagerWnd class */
  PluginManagerWnd *plugin_manager_wnd_;
  /*! ImageProcessingState object */
  ImageProcessingState image_proc_state_;
  /*! Pointer to the ThreadRunningCycleManager class */
  ThreadRunningCycleManager *thread_running_cycle_manager_;

  /**
   * @brief
   * Constructor for this window.
   */
  DemoOisWnd(MainWnd *main_wnd);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~DemoOisWnd(void);

  /**
   * @brief
   * The handler function for button(id = kButtonDemoOisLoadId).
   */
  virtual void OnLoad(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonDemoOisStartId).
   */
  virtual void OnStart(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonDemoOisDemoStartId).
   */
  virtual void OnDemoStart(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

  virtual void OnMenuSensorConfig(); /* NOLINT */
  virtual void OnMenuPluginManager(); /* NOLINT */
  virtual void OnOpenSensorSettingsWnd(); /* NOLINT */
  virtual void ApplySensor(); /* NOLINT */
  virtual void ShowFocusOIS(); /* NOLINT */
  virtual void OnStartMonitor(); /* NOLINT */
  virtual void OnInit(); /* NOLINT */
  virtual void OnInitDemo(); /* NOLINT */

  /* Main window object*/
  MainWnd *parent_;

 protected:
  /*! UI*/
  //wxStaticBox *static_box_white_balance_with_digital_gain_;
  wxButton *button_ois_load_apply_;
  wxButton *button_ois_start_apply_;
  wxButton *button_ois_demostart_apply_;

  // static const T_I2CRegWriteData i2cdat_[];
  static const T_I2CRegWriteData i2cdat_gyro_[];
  static const T_I2CRegWriteData i2cdat_dp1_[];
  static const T_I2CRegWriteData i2cdat_dp2_[];
  static const T_I2CRegWriteData i2cdat_calb_[];
  static const T_I2CRegWriteData i2cdat_gpo_[];
  static const T_I2CRegWriteData i2cdat_pwmfrequency_[];
  static const T_I2CRegWriteData i2cdat_SineTest_dp1_[];
  static const T_I2CRegWriteData i2cdat_SineTest_dp2_[];

 private:
  /* Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _DEMO_OIS_WND_H_*/
