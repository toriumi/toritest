/**
 * @file      avi.h
 * @brief     Plugin header to load the AVI file.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _AVI_H_
#define _AVI_H_

#include "./avi_define.h"
#include "./avi_wnd.h"
#include "./plugin_base.h"

class AviWnd;

/**
 * @class Avi
 * @brief Plugin to load the AVI file.
 */
class Avi : public PluginBase {
 private:
  /*! Parameter setting window.*/
  AviWnd* avi_wnd_;

  /*! Common parameter.*/
  CommonParam* common_;

  /*! First pixel.*/
  int first_pixel_;

  /*! Optical black.*/
  int optical_black_;

 public:
  /**
   * @brief
   * Constructor.
   */
  Avi(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Avi(void);

  /**
   * @brief
   * Initialize routine of the Avi plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Avi plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Avi plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Demosaic plugin.
   * @param src_image [in] src image data.
   * @param dst_image [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Avi plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Close setting window of plugin.
   * @return success or false.
   */
  virtual bool CloseSettingWindow(void);

  /**
   * @brief
   * Set the image processing state.
   * @param state [in] ImageProcessingState
   */
  virtual void set_image_processing_state(ImageProcessingState state);

  /**
   * @brief
   * Set first pixel.
   * @param first_pixel [in] first pixel.
   */
  virtual void set_first_pixel(int first_pixel);

  /**
   * @brief
   * Set optical black.
   * @param optical_black [in] optical black.
   */
  virtual void set_optical_black(int optical_black);

  /**
   * @brief
   * Set the list of parameter setting string for the Avi plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _AVI_H_*/
