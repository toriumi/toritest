/**
 * @file      whitebalancegain.h
 * @brief     WhiteBalanceGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _WHITE_BALANCE_GAIN_H_
#define _WHITE_BALANCE_GAIN_H_

#include <vector>
#include "./plugin_base.h"
#include "./whitebalancegain_define.h"
#include "./whitebalancegain_wnd.h"

/**
 * @class WhiteBalanceGain
 * @brief WhiteBalanceGain plugin.
 */
class WhiteBalanceGain : public PluginBase {
 private:
  /*! Common parameter */
  CommonParam* common_;
  /*! Setting window */
  WhiteBalanceGainWnd* white_balance_gain_wnd_;
  /*! Red value which is using in the Whitebalancegain plugin */
  float white_balanace_gain_red_value_;
  /*! Green value which is using in the Whitebalancegain plugin */
  float white_balanace_gain_green_value_;
  /*! Blue value which is using in the Whitebalancegain plugin */
  float white_balanace_gain_blue_value_;
  /*! Whether use one push */
  bool one_push_;
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;

 public:
  /**
   * @brief
   * Constructor.
   */
  WhiteBalanceGain(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~WhiteBalanceGain(void);

  /**
   * @brief
   * Initialize routine of the Whitebalancegain plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Whitebalancegain plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Whitebalancegain plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Whitebalancegain plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Whitebalancegain plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Function to use one push.
   * When this function is called in the screen display,
   * the One push function will work.
   */
  virtual void SetOnePush() { one_push_ = true; }

  /**
   * @brief
   * Return the red value which is using in the Whitebalancegain plugin.
   * @return red value.
   */
  virtual float WhiteBalanceGainRedValue() {
    return white_balanace_gain_red_value_;
  }

  /**
   * @brief
   * Set the red value which is using in the Whitebalancegain plugin.
   * @param value [in] red value.
   */
  virtual void SetWhiteBalanceGainRedValue(float value) {
    white_balanace_gain_red_value_ = value;
  }

  /**
   * @brief
   * Return the blue value which is using in the Whitebalancegain plugin.
   * @return blue value.
   */
  virtual float WhiteBalanceGainBlueValue() {
    return white_balanace_gain_blue_value_;
  }

  /**
   * @brief
   * Set the blue value which is using in the Whitebalancegain plugin.
   * @param value [in] blue value.
   */
  virtual void SetWhiteBalanceGainBlueValue(float value) {
    white_balanace_gain_blue_value_ = value;
  }

  /**
   * @brief
   * Set the list of parameter setting string for the Whitebalancegain plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _WHITE_BALANCE_GAIN_H_*/
