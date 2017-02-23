/**
 * @file      edge_enhancement_param.cpp
 * @brief     Parameter of EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./edge_enhancement_param.h"

/**
 * @brief
 * Constructor.
 */
EdgeEnhancementParam::EdgeEnhancementParam() { coeff_ = COEFF_DEFAULT; }

/**
 * @brief
 * Destructor.
 */
EdgeEnhancementParam::~EdgeEnhancementParam() {}

/**
 * @brief
 * Set coeff value.
 * @param value [in] coeff value
 * @return coeff value.
 */
float EdgeEnhancementParam::SetCoeff(float value) {
  /* round off over max and min*/
  if (value < COEFF_MIN) {
    value = COEFF_MIN;
  } else if (value > COEFF_MAX) {
    value = COEFF_MAX;
  }

  coeff_ = value;

  return coeff_;
}

/**
 * @brief
 * Get coeff value.
 * @return coeff value.
 */
float EdgeEnhancementParam::GetCoeff(void) { return coeff_; }
