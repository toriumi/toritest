/**
 * @file      sensor_focus.h
 * @brief     SensorFocus plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_FOCUS_H_
#define _SENSOR_FOCUS_H_

#include "./plugin_base.h"
#include "./sensor_focus_define.h"
#include "./sensor_focus_wnd.h"

/**
 * @class SensorFocus
 * @brief SensorFocus plugin.
 */
class SensorFocus : public PluginBase {
 private:
  /*! Setting window */
  SensorFocusWnd* sensor_focus_wnd_;

 public:
  /**
   * @brief
   * Constructor.
   */
  SensorFocus(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~SensorFocus(void);

  /**
   * @brief
   * Initialize routine of the Color plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Color plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Color plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Color plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the EdgeEnhancement plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);
};
#endif /* _SENSOR_FOCUS_H_ */
