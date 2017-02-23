/**
 * @file      bin.h
 * @brief     Plugin to load the RAW file.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _BIN_H_
#define _BIN_H_

#include <vector>
#include "./bin_define.h"
#include "./bin_wnd.h"
#include "./plugin_base.h"

class BinWnd;

/**
 * @class Bin
 * @brief Plugin to load the RAW file.
 */
class Bin : public PluginBase {
 private:
  /*! Common parameter */
  CommonParam* common_;
  /*! Setting window */
  BinWnd* bin_wnd_;
  /*! First pixel. */
  int first_pixel_;
  /*! Optical black value */
  int optical_black_;

 public:
  /**
   * @brief
   * Constructor.
   */
  Bin(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Bin(void);

  /**
   * @brief
   * Initialize routine of the Bin plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Bin plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Bin plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Bin plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Bin plugin.
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
   * Set the list of parameter setting string for the Bin plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _BIN_H_*/
