/**
 * @file      sensor_config.h
 * @brief     Sensor config table header.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_CONFIG_H_
#define _SENSOR_CONFIG_H_

#include "./sensor_define.h"
#include "wx/wx.h"

/**
 * @class SensorConfig
 * @brief Input plugin for sensor config table class.
 */
class SensorConfig {
 private:
  /*! Sensor name.*/
  wxString sensor_name_;

  /*! Image size.*/
  wxString size_;

  /*! Sensor config file path.*/
  wxString sensor_config_file_path_;

 public:
  /**
   * @brief
   * Constructor.
   */
  SensorConfig(void) {}

  /**
   * @brief
   * Destructor.
   */
  virtual ~SensorConfig(void) {}

  /**
   * @brief
   * Get a path of the sensor config file.
   * @param path [out] path of the sensor config file.
   */
  virtual wxString sensor_config_file_path(void) {
    return sensor_config_file_path_;
  }
  /**
   * @brief
   * Set a path of the sensor config file.
   * @param path [in] path of the sensor config file.
   */
  virtual void set_sensor_config_file_path(wxString sensor_config_file_path) {
    sensor_config_file_path_ = sensor_config_file_path;
  }
  /**
   * @brief
   * Get a name of the sensor.
   * @param sensor_name_ [out] name of the sensor.
   */
  virtual wxString sensor_name(void) { return sensor_name_; }

  /**
   * @brief
   * Set a name of the sensor.
   * @param sensor_name [out] name of the sensor.
   */
  virtual void set_sensor_name(wxString sensor_name) {
    sensor_name_ = sensor_name;
  }

  /**
   * @brief
   * Get a image of the image file.
   * @param set_image_size [out] size of the image file.
   */
  virtual wxString image_size(void) { return size_; }

  /**
   * @brief
   * Set a size of the image file.
   * @param set_image_size [in] size of the image file.
   */
  virtual void set_image_size(wxString image_size) { size_ = image_size; }
};
#endif /* _SENSOR_CONFIG_H_*/
