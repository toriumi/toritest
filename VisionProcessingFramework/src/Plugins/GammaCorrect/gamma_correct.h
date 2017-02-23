/**
 * @file      gamma_correct.h
 * @brief     GammaCorrect plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _GAMMA_CORRECT_H_
#define _GAMMA_CORRECT_H_

#include <vector>
#include "./gamma_correct_define.h"
#include "./gamma_correct_wnd.h"
#include "./plugin_base.h"

class GammaCorrectWnd;

/**
 * @class GammaCorrect
 * @brief GammaCorrect plugin.
 */
class GammaCorrect : public PluginBase {
 private:
  /*! Parameter setting window.*/
  GammaCorrectWnd* gamma_correct_wnd_;
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;
  /*! Whether to use the table file */
  bool is_use_gamma_table_;
  /*! Whether creation of the gamma table has been completed*/
  bool create_gamma_table_;
  /*! Coefficient, which is set by the Function mode */
  float gamma_correct_value_;
  /* Whether using the 10-bit table */
  bool is_use_10bit_lut_;

 public:
  /* 8bit look up table for gamma correction. */
  unsigned char gamma_lut8[k8BitTableRow];
  /* 10bit look up table for gamma correction. */
  UINT16 gamma_lut10[k10BitTableRow];
  /* 8bit temporary look up table for gamma correction. */
  unsigned char temp_gamma_lut8[k8BitTableRow];
  /* 10bit temporary look up table for gamma correction. */
  UINT16 temp_gamma_lut10[k10BitTableRow];

 public:
  /**
   * @brief
   * Constructor.
   */
  GammaCorrect(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~GammaCorrect(void);

  /**
   * @brief
   * Initialize routine of the GammaCorrect plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the GammaCorrect plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the GammaCorrect plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the GammaCorrect plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the GammaCorrect plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set already creation of the gamma table has been completed.
   * @param flag [in] If true, creating the table has been completed.
   */
  virtual void set_create_gamma_table(bool flag) { create_gamma_table_ = flag; }

  /**
   * @brief
   * Get already creation of the gamma table has been completed.
   * @return If true, creating the table has been completed.
   */
  virtual bool create_gamma_table() { return create_gamma_table_; }

  /**
   * @brief
   * Set whether or not to use the gamma table file.
   * @param flag [in] If true, use the gamma table file.
   */
  virtual void set_is_use_gamma_table(bool is_use) {
    is_use_gamma_table_ = is_use;
  }

  /**
   * @brief
   * Get whether or not to use the gamma table fike
   * @return If true, use the gamma table file
   */
  virtual bool is_use_gamma_table(void) { return is_use_gamma_table_; }

  /**
   * @brief
   * Set whether or not to use 10bit gamma table
   * @param flag [in] If true, use the 10bit lut table.
   */
  virtual void set_is_use_10bit_lut(bool is_use) { is_use_10bit_lut_ = is_use; }

  /**
   * @brief
   * Get whether or not to use the gamma table
   * @return If true, use the 10bit lut table.
   */
  virtual bool is_use_10bit_lut(void) { return is_use_10bit_lut_; }

  /**
   * @brief
   * Reset the gamma lut table.
   */
  virtual void reset_table(void);

  /**
   * @brief
   * Set the list of parameter setting string for the GammaCorrect plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _GAMMA_CORRECT_H_*/
