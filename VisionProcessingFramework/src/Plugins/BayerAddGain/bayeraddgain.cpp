/**
 * @file      bayeraddgain.cpp
 * @brief     BayerAddGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./bayeraddgain.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
BayerAddGain::BayerAddGain() : PluginBase() {
  DEBUG_PRINT("BayerAddGain::BayerAddGain()\n");

  // Initialize
  bayer_add_gain_wnd_ = new BayerAddGainWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("BayerAddGain");

  int in_1 = AddInputPortCandidateSpec(kGRAY8);
  int in_2 = AddInputPortCandidateSpec(kGRAY16);
  int out_1 = AddOutputPortCandidateSpec(kGRAY8);
  int out_2 = AddOutputPortCandidateSpec(kGRAY16);

  // Create port relation.
  AddPortRelation(in_1, out_1);
  AddPortRelation(in_2, out_2);

  set_is_use_dest_buffer(false);
}

/**
 * @brief
 * Destructor.
 */
BayerAddGain::~BayerAddGain() { delete bayer_add_gain_wnd_; }

/**
 * @brief
 * Initialize routine of the BayerAddGain plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization.
 */
bool BayerAddGain::InitProcess(CommonParam* common) {
  DEBUG_PRINT("BayerAddGain::InitProcess \n");
  common_ = common;
  bayer_add_gain_wnd_->OnOpticalBlack(common_->optical_black());
  return true;
}

/**
 * @brief
 * Finalize routine of the BayerAddGain plugin.
 * This function is empty implementation.
 */
void BayerAddGain::EndProcess() {
  DEBUG_PRINT("BayerAddGain::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the BayerAddGain plugin.
 * This function is empty implementation.
 */
void BayerAddGain::DoPostProcess(void) {
  DEBUG_PRINT("BayerAddGain::DoPostProcess) \n");
}

/**
 * @brief
 * Main routine of the BayerAddGain plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing.
 */
bool BayerAddGain::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("BayerAddGain::DoProcess \n");

  int ob_clamp = common_->optical_black();
  int max;
  int bit_num;
  float value = bayer_add_gain_value();

  if (value < 0x00) {
    DEBUG_PRINT("Failed to bayer add gain value\n");
    value = static_cast<float>(kBayerAddGainDefaultValue);
  }

  if (src_image->depth() == 0) {
    max = 0xFF;
    bit_num = 1;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < src_image->cols; j++) {
        char* bayer_img_calc;
        bayer_img_calc = reinterpret_cast<char*>(
            &src_image->data[i * src_image->step + j * bit_num]);

        if ((((*bayer_img_calc) - ob_clamp) * value + ob_clamp) <= 0x00) {
          *bayer_img_calc = 0x00;
        } else if ((((*bayer_img_calc) - ob_clamp) * value + ob_clamp) >= max) {
          *bayer_img_calc = max;
        } else {
          *bayer_img_calc = ((*bayer_img_calc) - ob_clamp) * value + ob_clamp;
        }
      }
    }
  } else if (src_image->depth() == 2) {
    max = 0x3FF;
    bit_num = 2;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < src_image->cols; j++) {
        INT16* bayer_img_calc;
        bayer_img_calc = reinterpret_cast<INT16*>(
            &src_image->data[i * src_image->step + j * bit_num]);

        if ((((*bayer_img_calc) - ob_clamp) * value + ob_clamp) <= 0x00) {
          *bayer_img_calc = 0x00;
        } else if (((*bayer_img_calc) - ob_clamp) * value + ob_clamp >= max) {
          *bayer_img_calc = max;
        } else {
          *bayer_img_calc = ((*bayer_img_calc) - ob_clamp) * value + ob_clamp;
        }
      }
    }
  }
  return true;
}

/**
 * @brief
 * Open setting window of the BayerAddGain plugin.
 * @param is_running [in] ImageProcessingState
 */
void BayerAddGain::OpenSettingWindow(ImageProcessingState state) {
  if (bayer_add_gain_wnd_ == NULL) {
    DEBUG_PRINT("bayeraddgain_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  bayer_add_gain_wnd_->SetTitle(window_title);
  bayer_add_gain_wnd_->InitDialog();
  bayer_add_gain_wnd_->Show(true);
  bayer_add_gain_wnd_->Raise();
}

/**
 * @brief
 * Set the list of parameter setting string for the BayerAddGain plugin.
 * @param params [in] settings string.
 */
void BayerAddGain::SetPluginSettings(std::vector<wxString> params) {
  bayer_add_gain_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create BayerAddGain plugins\n");
  BayerAddGain* plugin = new BayerAddGain();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
