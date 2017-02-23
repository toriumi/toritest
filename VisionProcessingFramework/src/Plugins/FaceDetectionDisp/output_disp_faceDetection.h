/**
 * @file      output_disp_faceDetection.h
 * @brief     Display plugin by using Opencv.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _OUTPUT_DISP_FACEDETECTION_H_
#define _OUTPUT_DISP_FACEDETECTION_H_

#include "./common_param.h"
#include "./output_disp_faceDetection_define.h"
#include "./output_disp_faceDetection_wnd.h"
#include "./plugin_base.h"

class OutputDispFaceDetectionWnd;

enum UtilConvertMode {
  UTIL_CONVERT_10U_TO_8U = 0,
  UTIL_CONVERT_10U_TO_16U,
};

/**
 * @class OutputDispFaceDetection
 * @brief Display plugin by using Opencv.
 */
class OutputDispFaceDetection : public PluginBase {
 private:
  /*! Common parameter */
  CommonParam* common_;

  /*! OpenCV displaying window.*/
  OutputDispFaceDetectionWnd* wnd_;

  /*! Current image size.  This parameter is used for the purpose of
      confirming the screen size in one push function. */
  CvSize current_image_size;

 public:
  /**
   * @brief
   * Constructor.
   */
  OutputDispFaceDetection(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~OutputDispFaceDetection(void);

  /**
   * @brief
   * Initialize routine of the OutputDispFaceDetection plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization.
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the OutputDispFaceDetection plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the OutputDispFaceDetection plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the OutputDispFaceDetection plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing.
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Set onepush rectangle for common param.
   * @param start [in] start coordinate.
   * @param end [in] end coordinate.
   */
  virtual void SetOnepushRectangle(cv::Point start, cv::Point end);

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
#endif /* _OUTPUT_DISP_FACEDETECTION_H_*/
