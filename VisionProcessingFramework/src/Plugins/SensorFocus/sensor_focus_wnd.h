/**
 * @file      sensor_focus_wnd.h
 * @brief     Window of SensorFocus plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_FOCUS_WND_H_
#define _SENSOR_FOCUS_WND_H_

#include "./include.h"
#include "./sensor_focus_define.h"

typedef struct {
  int adr;
  int dat;
} T_I2CRegWriteData;

class SensorFocus;

/**
 * @class SensorFocusWnd
 * @brief Setting window of SensorFocus plugin.
 */
class SensorFocusWnd : public wxFrame {
 private:
  /*! Pointer to the SensorFocus class */
  SensorFocus* parent_;

 protected:
  /*! UI*/
  wxRadioBox* radio_box_focus_mode_;
  wxRadioBox* radio_box_ois_mode_;
  wxButton* button_image_Init_apply_;
  wxButton* button_image_InitDemo_apply_;
  wxButton* button_image_focus_apply_;
  wxButton* button_image_OIS_apply_;
  wxButton* button_image_OIS_DemoStart_;
  wxButton* button_image_OIS_DemoStop_;
  wxStaticText *static_text_focus_code_;
  wxTextCtrl *text_ctrl_focus_code_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit SensorFocusWnd(SensorFocus* parent);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~SensorFocusWnd(void);

  /**
   * @brief
   * The handler function for button(id = kButtonInitId).
   */
  virtual void OnInit(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonInitDemoId).
   */
  virtual void OnInitDemo(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonFocusModeApplyId).
   */
  virtual void OnFocusModeApply(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonOISModeApplyId).
   */
  virtual void OnOISModeApply(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonOISModeDemoStartId).
   */
  virtual void OnOISModeDemoStart(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonOISModeDemoSttopId).
   */
  virtual void OnOISModeDemoStop(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * To select the sensor focus settings file path from the file dialog.
   */
  //virtual void OnOpenFileDialog(wxCommandEvent &event);        /* NOLINT */

  // static const T_I2CRegWriteData i2cdat_[];
  static const T_I2CRegWriteData i2cdat_gyro_[];
  static const T_I2CRegWriteData i2cdat_dp1_[];
  static const T_I2CRegWriteData i2cdat_dp2_[];
  static const T_I2CRegWriteData i2cdat_SineTest_dp1_[];
  static const T_I2CRegWriteData i2cdat_SineTest_dp2_[];
  static const T_I2CRegWriteData i2cdat_calb_[];
  static const T_I2CRegWriteData i2cdat_gpo_[];
  static const T_I2CRegWriteData i2cdat_pwmfrequency_[];

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _SENSOR_FOCUS_WND_H_*/
