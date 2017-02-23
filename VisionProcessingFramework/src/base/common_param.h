/**
 * @file      common_param.h
 * @brief     Header for CommonParam class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#ifndef _COMMON_PARAM_
#define _COMMON_PARAM_

#include "./include.h"
#include "./sensor_param.h"

/**
 * @class CommonParam
 * @brief This class provides accessing method
 *        to the common parameter for all plugins.
 */
class CommonParam {
 private:
  /*! One push start point X coordinate. */
  int onepush_start_x_;

  /*! One push start point Y coordinate. */
  int onepush_start_y_;

  /*! One push end point X coordinate. */
  int onepush_end_x_;

  /*! One push end point Y coordinate. */
  int onepush_end_y_;

  /*! First pixel. */
  int first_pixel_;

  /*! Optical black value */
  int optical_black_;

  /*! Sensor parameter. */
  SensorParam* sensor_param_;

 public:
  /**
   * @brief
   * Constructor.
   */
  CommonParam(void);

  /**
   * @brief
   * Destructor.
   */
  ~CommonParam(void);

  /**
   * @brief
   * Set first pixel.
   * @param first_pixel [in] first pixel.
   */
  void set_first_pixel(int first_pixel) { first_pixel_ = first_pixel; }

  /**
   * @brief
   * Get first pixel.
   * @return first pixel
   */
  int first_pixel(void) { return first_pixel_; }

  /**
   * @brief
   * Set optical black value.
   * @param optical_black [in] optical black value.
   */
  void set_optical_black(int optical_black) { optical_black_ = optical_black; }

  /**
   * @brief
   * Get optical black value.
   * @return optical black value
   */
  int optical_black(void) { return optical_black_; }

  /**
   * @brief
   * Set Onepush rect point.
   * @param start_x [in] Onepush start point X.
   * @param start_y [in] Onepush start point Y.
   * @param end_x [in] Onepush end point X.
   * @param end_y [in] Onepush end point Y.
   * @return
   */
  void SetOnepushRectabgle(int start_x, int start_y, int end_x, int end_y);

  /**
   * @brief
   * Get Onepush rect point.
   * @return One push rect point.
   */
  CvRect GetOnepushRectangle(void);

  /**
   * @brief
   * Get sensor parameter.
   * @return sensor parameter.
   */
  SensorParam* sensor_param(void);
};

#endif /* _COMMON_PARAM_*/
