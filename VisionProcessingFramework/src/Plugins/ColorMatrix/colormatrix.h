/**
 * @file      colormatrix.h
 * @brief     ColorMatrix plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _COLOR_MATRIX_H_
#define _COLOR_MATRIX_H_

#include <vector>
#include "./colormatrix_define.h"
#include "./colormatrix_wnd.h"
#include "./plugin_base.h"

// class ColorMatrixWnd;

/**
 * @class ColorMatrix
 * @brief ColorMatrix plugin.
 */
class ColorMatrix : public PluginBase {
 private:
  /*! Setting window */
  ColorMatrixWnd* color_matrix_wnd_;
  /*! Color matrix table */
  float color_matrix_[3][3];
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;

 public:
  /**
   * @brief
   * Constructor.
   */
  ColorMatrix(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~ColorMatrix(void);

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
   * Open setting window of the Color plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set color matrix.
   * @param color_martix [in] color matrix.
   */
  virtual void set_color_matrix(float color_martix[3][3]);

  /**
   * @brief
   * Set plugin settings from flow load function.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _COLOR_MATRIX_H_*/
