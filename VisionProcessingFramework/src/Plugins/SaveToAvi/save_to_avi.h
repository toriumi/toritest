/**
 * @file      save_to_avi.h
 * @brief     SaveToAvi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SAVE_TO_AVI_H_
#define _SAVE_TO_AVI_H_

#include <vector>
#include "./common_param.h"
#include "./plugin_base.h"
#include "./save_to_avi_define.h"
#include "./save_to_avi_wnd.h"

class SaveToAviWnd;

enum UtilConvertMode {
  UTIL_CONVERT_10U_TO_8U = 0,
  UTIL_CONVERT_10U_TO_16U,
};

/**
 * @class SaveToAvi
 * @brief Load the AVI file in RGB format.
 */
class SaveToAvi : public PluginBase {
 private:
  /*! Parameter setting window.*/
  SaveToAviWnd* wnd_;
  /*! Common parameter */
  CommonParam* common_;

 public:
  /**
   * @brief
   * Constructor.
   */
  SaveToAvi(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~SaveToAvi(void);

  /**
   * @brief
   * Initialize routine of the SaveToAvi plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the SaveToAvi plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the SaveToAvi plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the SaveToAvi plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the SaveToAvi plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Close setting window of the SaveToAvi plugin.
   * @return If true, success close window
   */
  virtual bool CloseSettingWindow(void);

  /**
   * @brief
   * Set the list of parameter setting string for the SaveToAvi plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);

 private:
  /**
   * @brief
   * Convert the bit depth of image.
   * @param src_image [in] Pointer to the src image
   * @param cvt_mode [in] enum UtilConvertMode
   * @param shift [in] Value added to the scaled source array elements
   * @return Pointer to the converted image.
   */
  virtual cv::Mat* UtilGetCvConvertScale(cv::Mat* src_image, int cvt_mode,
                                         double shift);
};
#endif /* _OUTPUT_DISP_OPENCV_H_*/
