/**
 * @file      common_param.cpp
 * @brief     Source for CommonParam class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./common_param.h"

/**
 * @brief
 * Constructor.
 */
CommonParam::CommonParam() { sensor_param_ = new SensorParam(); }

/**
 * @brief
 * Destructor.
 */
CommonParam::~CommonParam() {
  if (sensor_param_ != NULL) {
    delete sensor_param_;
  }
}

/**
 * @brief
 * Set Onepush rect point.
 * @param start_x [in] Onepush start point X.
 * @param start_y [in] Onepush start point Y.
 * @param end_x [in] Onepush end point X.
 * @param end_y [in] Onepush end point Y.
 * @return
 */
void CommonParam::SetOnepushRectabgle(int start_x, int start_y, int end_x,
                                      int end_y) {
  onepush_start_x_ = start_x;
  onepush_start_y_ = start_y;
  onepush_end_x_ = end_x;
  onepush_end_y_ = end_y;
}

/**
 * @brief
 * Get Onepush rect point.
 * @return One push rect point.
 */
CvRect CommonParam::GetOnepushRectangle() {
  return cvRect(onepush_start_x_, onepush_start_y_, onepush_end_x_,
                onepush_end_y_);
}
