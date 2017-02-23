/**
 * @file      edge_enhancement_param.h
 * @brief     Parameter of EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _EDGE_ENHANCEMENT_PARAM_H_
#define _EDGE_ENHANCEMENT_PARAM_H_

#include "./edge_enhancement_define.h"
#include "./include.h"

/**
 * @class EdgeEnhancementParam
 * @brief Parameter of EdgeEnhancement plugin.
 */
class EdgeEnhancementParam {
 private:
  /*! coefficient*/
  float coeff_;

 public:
  /**
   * @brief
   * Constructor.
   */
  EdgeEnhancementParam(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~EdgeEnhancementParam(void);

  /**
   * @brief
   * Set coeff value.
   * @param value [in] coeff value
   * @return coeff value.
   */
  virtual float SetCoeff(float value);

  /**
   * @brief
   * Get coeff value.
   * @return coeff value.
   */
  virtual float GetCoeff(void);
};
#endif /* _EDGE_ENHANCEMENT_PARAM_H_*/
