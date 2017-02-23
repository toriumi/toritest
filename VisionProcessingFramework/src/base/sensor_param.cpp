/**
 * @file      sensor_param.cpp
 * @brief     Source for SensorParam class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./sensor_param.h"

/**
 * @brief
 * Constructor.
 */
SensorParam::SensorParam() { ; }

/**
 * @brief
 * Destructor.
 */
SensorParam::~SensorParam() { ; }

/**
 * @brief
 * Set white balance adjustment parameters with digital gain value.
 * @param red [in] White balance adjustment parameters with digital gain.(red)
 * @param green [in] White balance adjustment parameters with digital gain.
 *                  (green)
 * @param blue [in] White balance adjustment parameters with digital gain.
 *                  (blue)
 */
void SensorParam::SetWhiteBalanceWithDigitalGainValue(float red, float green,
                                                      float blue) {
  wbwdg_red_ = red;
  wbwdg_green_ = green;
  wbwdg_blue_ = blue;
}

/**
 * @brief
 * Set whether or not to use the white balance with digital gain function.
 * @param is_wbwdg [in]  true = enable, false = disable
 */
void SensorParam::set_is_wbwdg(bool is_wbwdg) { is_wbwdg_ = is_wbwdg; }

/**
 * @brief
 * Get whether or not to use the white balance with digital gain function.
 * @return true, Implementation of white balance with digital gain
 */
bool SensorParam::is_wbwdg(void) { return is_wbwdg_; }
