/**
 * @file      demosaic.h
 * @brief     Demosaic plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _DEMOSAIC_H_
#define _DEMOSAIC_H_

#include <vector>
#include "./demosaic_define.h"
#include "./demosaic_wnd.h"
#include "./plugin_base.h"

class DemosaicWnd;

/**
 * @class Demosaic
 * @brief This plugin convert the RGB image from Bayer image.
 */
class Demosaic : public PluginBase {
 private:
  /*! Common parameter */
  CommonParam* common_param_;
  /*! Setting window */
  DemosaicWnd* demosaic_wnd_;
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;

 public:
  /**
   * @brief
   * Constructor.
   */
  Demosaic(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Demosaic(void);

  /**
   * @brief
   * Initialize routine of the Demosaic plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization.
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Demosaic plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Demosaic plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Demosaic plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing.
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Demosaic plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set the image processing state.
   * @param state [in] ImageProcessingState
   */
  virtual void set_image_processing_state(ImageProcessingState state);

  /**
   * @brief
   * Set the list of parameter setting string for the Demosaic plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _DEMOSAIC_H_*/
