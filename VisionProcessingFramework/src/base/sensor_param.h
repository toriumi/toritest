/**
* @file      sensor_param.h
* @brief     Header for SensorParam class
* @author    Vision Processing Community
* @copyright 2016 Vision Processing Community. All rights reserved.
*/

#ifndef _SENSOR_PARAM_
#define _SENSOR_PARAM_

#include "./include.h"

/**
 * @class SensorParam
 * @brief This sensor parameters class to be used in the sensor plugin.
 */
class SensorParam {
 private:
  /*! Orientation of the image. */
  int image_orientation_;

  /*! White balance adjustment parameters with digital gain.(red)*/
  float wbwdg_red_;

  /*! White balance adjustment parameters with digital gain.(green)*/
  float wbwdg_green_;

  /*! White balance adjustment parameters with digital gain.(blue)*/
  float wbwdg_blue_;

  /*! Use or Not white balance with digital gain.(red)*/
  bool is_wbwdg_;

 public:
  /**
   * @brief
   * Constructor.
   */
  SensorParam(void);

  /**
   * @brief
   * Destructor.
   */
  ~SensorParam(void);

  /**
   * @brief
   * Set white balance adjustment parameters with digital gain value.
   * @param red [in] White balance adjustment parameters with digital
   *                 gain.(red)
   * @param green [in] White balance adjustment parameters with digital
   *                   gain.(green)
   * @param blue [in] White balance adjustment parameters with digital
   *                  gain.(blue)
   */
  void SetWhiteBalanceWithDigitalGainValue(float red, float green, float blue);

  /**
   * @brief
   * Set whether or not to use the white balance with digital gain
   * function.
   * @param is_wbwdg [in]  true = enable, false = disable
   */
  void set_is_wbwdg(bool is_wbwdg);

  /**
   * @brief
   * Get whether or not to use the white balance with digital gain
   * function.
   * @return true, Implementation of white balance with digital gain
   */
  bool is_wbwdg(void);
};

#endif /* _SENSOR_PARAM_*/
