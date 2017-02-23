/**
 * @file      sensor_settings_wnd.h
 * @brief     Window of Sensor settings plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_SETTINGS_WND_H_
#define _SENSOR_SETTINGS_WND_H_

#include "./include.h"
#include "./sensor_config.h"
#include "./sensor_define.h"

class Sensor;

/**
 * @class SensorSettingsWnd
 * @brief Window of Sensor settings plugin.
 */
class SensorSettingsWnd : public wxFrame {
 private:
  /*! Sensor config class object.*/
  SensorConfig *sensor_config_;
  /*! Sensor class object.*/
  Sensor *sensor_;

  /*! Analog gain decibel for IMX219.*/
  wxString Imx219_analog_gain_[kImx219AnalogGainMax + 1];

  /*! Difital gain decibel for IMX219.*/
  wxString Imx219_digital_gain_[kImx219DitalGainMax + 1];

  /*! Analog gain decibel for IU233.*/
  wxString Iu233_analog_gain_[kIu233AnalogGainMax + 1];
	
  /*! Digital gain decibel for IU233.*/
  wxString Iu233_digital_gain_[kIu233DitalGainMax + 1];

  /*! Analog gain decibel for IMX378.*/
  wxString Imx378_analog_gain_[kImx378AnalogGainMax + 1];

  /*! Difital gain decibel for IMX378.*/
  wxString Imx378_digital_gain_[kImx378DitalGainMax + 1];

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit SensorSettingsWnd(Sensor *sensor);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~SensorSettingsWnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event);                   /* NOLINT */

  /**
   * @brief
   * To select the sensor settings file path from the file dialog.
   */
  virtual void OnOpenFileDialog(wxCommandEvent &event);        /* NOLINT */

  /**
   * @brief
   * To set the first pixel in the apply button is pressed.
   * @param first_pixel [in] first pixel.
   */
  virtual void OnApplyFirstPixel(wxCommandEvent &event);       /* NOLINT */

  /**
   * @brief
   * To set the image orientation in the apply button is pressed.
   */
  virtual void OnApplyImageOrientation(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * To set the analog gain in the apply button is pressed.
   */
  virtual void OnSliderAnalogGain(wxScrollEvent &event);       /* NOLINT */

  /**
   * @brief
   * To set the digital gain in the apply button is pressed.
   */
  virtual void OnSliderDigitalGain(wxScrollEvent &event);      /* NOLINT */

  /**
   * @brief
   * To set the exposure time in the apply button is pressed.
   */
  virtual void OnSliderExposureTime(wxScrollEvent &event);     /* NOLINT */

  /**
   * @brief
   * To set the analog gain in the apply button is pressed.
   */
  virtual void OnApplyAnalogGain(wxCommandEvent &event);       /* NOLINT */

  /**
   * @brief
   * To set the digital gain in the apply button is pressed.
   */
  virtual void OnApplyDigitalGain(wxCommandEvent &event);      /* NOLINT */

  /**
   * @brief
   * To set the exposure time in the apply button is pressed.
   */
  virtual void OnApplyExposureTime(wxCommandEvent &event);     /* NOLINT */

  /**
   * @brief
   * Read the decibel conversion result file(IMX219,IU233).
   */
  virtual void ReadConvetToDBFile();

  /**
   * @brief
   * Convert from the register value of the analog gain in decibels.
   * @param register_value [in] register value
   * @return decibels value
   */
  virtual wxString ConvertToAnalogGainDB(int register_value);

  /**
   * @brief
   * Convert from the register value of the digital gain in decibels.
   * @param register_value [in] register value
   * @return decibels value
   */
  virtual wxString ConvertToDigitalGainDB(int register_value);

  /**
   * @brief
   * Convert from the register value of the exposure time in decibels.
   * @param register_value [in] register value
   * @return decibels value
   */
  virtual wxString ConvertToExposureTimeMS(int register_value);

  /**
   * @brief
   * Load the parameters from the sensor config ini file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  virtual bool CheckOpenFile(wxString file_path, int open_file_type);

  /**
   * @brief
   * Load the parameters from the sensor config ini file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  virtual bool LoadSensorParamfromFile(wxString file_path);

  /**
   * @brief
   * Write sensor param from file.
   * @param file_path [in] file path.
   */
  virtual bool WriteSensorParamtoFile(wxString file_path);

  /**
   * @brief
   * To set a common the first pixel.
   * @param first_pixel [in] first pixel.
   */
  virtual void SetFirstPixel();

  /**
   * @brief
   * To set the image orientation.
   */
  virtual void SetImageOrientation();

  /**
   * @brief
   * To set the analog gain.
   */
  virtual void SetAnalogGain();

  /**
   * @brief
   * To set the digital gain.
   */
  virtual void SetDigitalGain();

  /**
   * @brief
   * To set the exposure time.
   */
  virtual void SetExposureTime();

  /**
   * @brief
   * To reflect the information read from the sensor ini file.
   * @param params [in] sensor config file path.
   */
  void SetSensorSettings(std::vector<wxString> params);

 protected:
  /* Sensor settings static text object*/
  wxStaticText *static_text_sensor_settings_;

  /* Sensor config text ctrl object*/
  wxTextCtrl *text_ctrl_sensor_config_;

  /* Open file dialog button object*/
  wxButton *button_file_dialog_;

  /* First pixel radio box object.*/
  wxRadioBox *radio_box_first_pixel;

  /* First pixel apply button object.*/
  wxButton *button_first_pixel_apply_;

  /* Image orientation static box object.*/
  wxStaticBox *static_box_image_orientation_;

  /* Image orientation combo box object.*/
  wxComboBox *combo_box_image_orientation_;

  /* Image orientation apply button object.*/
  wxButton *button_image_orientation_apply_;

  /* Analogl gain static box object.*/
  wxStaticBox *static_box_analog_gain_;

  /* Analog gain slider object.*/
  wxSlider *slider_analog_gain_;

  /* Analog gain static text object.*/
  //wxStaticText *static_text_analog_gain_;

  /* Analog gain stext ctrl object*/
  wxTextCtrl *text_ctrl_analog_gain_;

  /* Analog gain static text object.*/
  wxStaticText *static_text_analog_gain_DB_;

  /* Analog gain apply button object.*/
  wxButton *button_image_analog_gain_apply_;

  /* Digital gain static box object.*/
  wxStaticBox *static_box_digital_gain_;

  /* Digital gain slider object.*/
  wxSlider *slider_digital_gain_;

  /* Digital gain static text object*/
  //wxStaticText *static_text_digital_gain_;

  /* Digital gain stext ctrl object*/
  wxTextCtrl *text_ctrl_digital_gain_;

  /* Digital gain static text object*/
  wxStaticText *static_text_digital_gain_DB_;

  /* Digital gain apply button object.*/
  wxButton *button_image_digital_gain_apply_;

  /* Exposure time static box object.*/
  wxStaticBox *static_box_exposure_time_;

  /* Exposure time slider object.*/
  wxSlider *slider_exposure_time_;

  /* Exposure time static text object*/
  //wxStaticText *static_text_exposure_time_;

  /* Exposure time stext ctrl object*/
  wxTextCtrl *text_ctrl_exposure_time_;

  /* Exposure time static text object*/
  wxStaticText *static_text_exposure_time_MS_;

  /* Exposure time apply button object.*/
  wxButton *button_image_exposure_time_apply_;

  /* Sensor settings open file dialog object*/
  wxFileDialog *file_dialog_sensor_settings_open_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

  /**
   * @brief
   * To reflect the information read from the sensor ini file.
   * @param params [in] sensor config file path.
   */
  bool ReadSensorSettingsFile(wxString sensor_settings_file_path);
};

#endif /* _SENSOR_SETTINGS_WND_H_*/
