/**
 * @file      resize_image.h
 * @brief     Plugin to resize image.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _RESIZE_IMAGE_H_
#define _RESIZE_IMAGE_H_

#include <vector>
#include "./plugin_base.h"
#include "./resize_image_define.h"
#include "./resize_image_wnd.h"

/**
 * @class ResizeImage
 * @brief Plugin to resize image.
 */
class ResizeImage : public PluginBase {
 private:
  /*! Setting window */
  ResizeImageWnd* resize_image_wnd_;
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;
  /*! Resize type.use when "specific" has been selected. */
  ResizeImageType resize_type_;
  /*! Resize scale.use when "specific" has been selected. */
  double resize_scale_;
  /*! Resize value.use when "percent" has been selected. */
  double resize_value_;

 public:
  /**
   * @brief
   * Constructor.
   */
  ResizeImage(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~ResizeImage(void);

  /**
   * @brief
   * Initialize routine of the Resize plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Resize plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Resize plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Resize plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Resize plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set a resize type as a ResizeImageType.
   * This plugin will change the image size according to this type.
   * or set_resize_value();
   * @param resize_type [in] resize type.
   */
  virtual void set_resize_type(ResizeImageType resize_type);

  /**
   * @brief
   * Set a resize value as a percent.
   * This plugin will change the image size according to this value.
   * or set_resize_type();
   * @param resize_value [in] resize value.
   */
  virtual void set_resize_value(double resize_value);

  /**
   * @brief
   * Set the list of parameter setting string for the Resize plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _RESIZE_IMAGE_H_ */
