/**
 * @file      sensor_wnd.h
 * @brief     Window of Sensor plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_WND_H_
#define _SENSOR_WND_H_

#include "./include.h"
#include "./sensor_config.h"
#include "./sensor_define.h"

class Sensor;

/**
 * @class SensorWnd
 * @brief Window of Sensor plugin.
 */
class SensorWnd : public wxFrame {
 private:
  /*! Sensor config class object.*/
  //SensorConfig *sensor_config_;
	
  /*! Sensor class object.*/
  Sensor *sensor_;

  /*! Sensor config file path.*/
  char sensor_config_file_path_[128];

  /*! Image buffer width.*/
  int image_width_;

  /*! Image buffer height.*/
  int image_height_;

 public:
  /*! Sensor config class object.*/
  SensorConfig *sensor_config_;
  /**
   * @brief
   * Constructor for this window.
   */
  explicit SensorWnd(Sensor *sensor);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~SensorWnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event);                   /* NOLINT */

  /**
   * @brief
   * To select the sensor configuration file path from the file dialog.
   */
  virtual void OnOpenFileDialog(wxCommandEvent &event);        /* NOLINT */

  /**
   * @brief
   * Performing the initialization processing of the SSD with the apply button
   * pressed.
   */
  virtual void OnApply(wxCommandEvent &event);                 /* NOLINT */

  /**
   * @brief
   * Open sensor setting window of the Sensor plugin.
   */
  virtual void OnOpenSensorSettingsWnd(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * Set the image processing state.
   * @param state [in] ImageProcessingState
   */
  void UpdateUIForImageProcessingState(ImageProcessingState state);

  /**
   * @brief
   * To set the binning mode.
   */
  bool SetBinningMode();

  /**
   * @brief
   * Check open ini file.
   * @param file_path [in] file path.
   * @param open_file_type [in] open file type.
   * @return If true, open the file success
   */
  virtual bool CheckOpenFile(wxString file_path, int open_file_type);

  /**
   * Load the parameters from the sensor config ini file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  virtual bool LoadSensorParamfromFile(wxString file_path);

  /**
   * @brief
   * Write the parameters to the sensor config ini file.
   * @param file_path [in] file path.
   * @param sensor_config_file_path [in] sensor config file path.
   * @param bit_count [in] bit count.
   * @return If true, writing the file success
   */
  virtual bool WriteSensorParamtoFile(wxString file_path,
                                      wxString sensor_config_file_path,
                                      int bit_count);

  /**
   * @brief
   * To check the binning mode.
   * @param address [out] Binning mode address
   * @param address_value [out] Binning mode register value
   */
  bool CheckBinningMode(int address, int *address_value);

  /**
   * @brief
   * Do the sensor initial setting of the SSP..
   * @param params [in] sensor config file path.
   */
  void SetSensorConfig(std::vector<wxString>params);

 protected:
  /* Sensor config static text object*/
  wxStaticText *static_text_sensor_config_;

  /* Sensor config text ctrl object*/
  wxTextCtrl *text_ctrl_sensor_config_;

  /* Open file dialog button object*/
  wxButton *button_file_dialog_;

  /* Sensor name static text object*/
  wxStaticText *static_sensor_name_;

  /* Sensor name value static text object*/
  wxStaticText *static_sensor_name_value_;

  /* Image size static text object*/
  wxStaticText *static_text_image_size_;

  /* Image size value static text object*/
  wxStaticText *static_text_image_size_value_;

  /* Binning mode static text object*/
  wxStaticText *static_text_binning_mode_;

  /* Binning mode height value static text object*/
  wxStaticText *static_text_binning_mode_value_height_;

  /* Binning mode width value static text object*/
  wxStaticText *static_text_binning_mode_value_width_;

  /* Bit count static text object*/
  wxStaticText *static_text_bit_count_;

  /* Bit count combo box object*/
  wxComboBox *combo_box_bit_count_;

  /* Sensor settings button object*/
  wxButton *button_sensor_settings_;

  /* Apply button object*/
  wxButton *button_apply_;

  /* Sensor config open file dialog object*/
  wxFileDialog *file_dialog_sensor_config_open_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

  /**
   * @brief
   * Sensor name from the sensor configuration file and extracts the 
   * frame size.
   * @param sensor_config_file_path [in] sensor config file path
   * @return If true, open the file success
   */
  bool ReadSensorConfigFile(wxString sensor_config_file_path);
};

#endif /* _TEMPLATE_WND_H_*/
