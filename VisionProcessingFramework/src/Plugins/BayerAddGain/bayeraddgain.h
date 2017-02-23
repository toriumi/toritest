/**
 * @file      bayeraddgain.h
 * @brief     BayerAddGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _BAYER_ADD_GAIN_H_
#define _BAYER_ADD_GAIN_H_

#include <vector>
#include "./bayeraddgain_define.h"
#include "./bayeraddgain_wnd.h"
#include "./plugin_base.h"

// class BayerAddGainWnd;

/**
 * @class BayerAddGain
 * @brief BayerAddGain plugin.
 */
class BayerAddGain : public PluginBase {
 private:
  /*! Parameter setting window.*/
  BayerAddGainWnd* bayer_add_gain_wnd_;
  /*! Coefficient of bayer add gain */
  float bayer_add_gain_value_;
  /*! Common parameter */
  CommonParam* common_;

 public:
  /**
   * @brief
   * Constructor.
   */
  BayerAddGain(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~BayerAddGain(void);

  /**
   * @brief
   * Initialize routine of the BayerAddGain plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization.
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the BayerAddGain plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the BayerAddGain plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the BayerAddGain plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing.
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the BayerAddGain plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set the list of parameter setting string for the BayerAddGain plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);

  /**
   * @brief
   * Get gain value.
   * @return gain value.
   */
  virtual float bayer_add_gain_value() { return bayer_add_gain_value_; }

  /**
   * @brief
   * Set gain value.
   * @param value [in] gain value
   */
  virtual void set_bayer_add_gain_value(float value) {
    bayer_add_gain_value_ = value;
  }
};
#endif /* _BAYER_ADD_GAIN_H_*/
