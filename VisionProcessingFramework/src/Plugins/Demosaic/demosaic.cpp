/**
 * @file      demosaic.cpp
 * @brief     Demosaic plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./demosaic.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
Demosaic::Demosaic() : PluginBase() {
  DEBUG_PRINT("Demosaic::Demosaic()\n");
  set_plugin_name("Demosaic");
  demosaic_wnd_ = new DemosaicWnd(this);

  // Create input port.
  int in_1 = AddInputPortCandidateSpec(kGRAY8);
  int in_2 = AddInputPortCandidateSpec(kGRAY16);

  // Create output port.
  int out_rgb888 = AddOutputPortCandidateSpec(kRGB888);
  int out_bgr888 = AddOutputPortCandidateSpec(kBGR888);
  int out_rgb48 = AddOutputPortCandidateSpec(kRGB48);
  int out_bgr48 = AddOutputPortCandidateSpec(kBGR48);

  // Create port relation.
  bool is_connect_relation_1 = AddPortRelation(in_1, out_rgb888);
  bool is_connect_relation_2 = AddPortRelation(in_2, out_rgb48);
  bool is_connect_relation_3 = AddPortRelation(in_1, out_bgr888);
  bool is_connect_relation_4 = AddPortRelation(in_2, out_bgr48);

  // Check port relation.
  if (is_connect_relation_1 == false || is_connect_relation_2 == false ||
      is_connect_relation_3 == false || is_connect_relation_4 == false) {
    DEBUG_PRINT("Demosaic port relation fail\n");
    is_success_initialized_ = false;
  } else {
    is_success_initialized_ = true;
  }

  demosaic_wnd_->set_color_type(kDemosaicBgr888);
}

/**
 * @brief
 * Destructor.
 */
Demosaic::~Demosaic() { delete demosaic_wnd_; }

/**
 * @brief
 * Initialize routine of the Demosaic plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization.
 */
bool Demosaic::InitProcess(CommonParam* common) {
  DEBUG_PRINT("Demosaic::InitProcess \n");
  if (is_success_initialized_ == false) {
    return false;
  } else {
    common_param_ = common;
    return true;
  }
}

/**
 * @brief
 * Finalize routine of the Demosaic plugin.
 * This function is empty implementation.
 */
void Demosaic::EndProcess() {
  DEBUG_PRINT("Demosaic::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Demosaic plugin.
 * This function is empty implementation.
 */
void Demosaic::DoPostProcess(void) {
  DEBUG_PRINT("Demosaic::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Main routine of the Demosaic plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing.
 */
bool Demosaic::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("Demosaic::DoProcess \n");
  int first_pixel = common_param_->first_pixel();
  int type;

  if (demosaic_wnd_->color_type() == kDemosaicRgb888 ||
      demosaic_wnd_->color_type() == kDemosaicRgb48) {
    if (first_pixel == 0) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerBG2RGB;
    } else if (first_pixel == 1) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerGB2RGB;
    } else if (first_pixel == 2) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerGR2RGB;
    } else if (first_pixel == 3) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerRG2RGB;
    } else {
      DEBUG_PRINT("Demosaic::DoProcess fail first pixel = %d\n", first_pixel);
      return false;
    }
  } else if (demosaic_wnd_->color_type() == kDemosaicBgr888 ||
             demosaic_wnd_->color_type() == kDemosaicBgr48) {
    if (first_pixel == 0) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerBG2BGR;
    } else if (first_pixel == 1) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerGB2BGR;
    } else if (first_pixel == 2) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerGR2BGR;
    } else if (first_pixel == 3) {
      DEBUG_PRINT("Demosaic::DoProcess first pixel = %d\n", first_pixel);
      type = CV_BayerRG2BGR;
    } else {
      DEBUG_PRINT("Demosaic::DoProcess fail first pixel = %d\n", first_pixel);
      return false;
    }
  }
  cv::cvtColor(*src_image, *dst_image, type);
  return true;
}

/**
 * @brief
 * Open setting window of the Demosaic plugin.
 * @param is_running [in] ImageProcessingState
 */
void Demosaic::OpenSettingWindow(ImageProcessingState state) {
  if (demosaic_wnd_ == NULL) {
    DEBUG_PRINT("demosaic_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  demosaic_wnd_->SetTitle(window_title);
  demosaic_wnd_->UpdateUIForImageProcessingState(state);
  demosaic_wnd_->InitDialog();
  demosaic_wnd_->Show(true);
  demosaic_wnd_->Raise();
}

/**
 * @brief
 * Set the image processing state.
 * @param state [in] ImageProcessingState
 */
void Demosaic::set_image_processing_state(ImageProcessingState state) {
  demosaic_wnd_->UpdateUIForImageProcessingState(state);
}

/**
 * @brief
 * Set the list of parameter setting string for the Demosaic plugin.
 * @param params [in] settings string.
 */
void Demosaic::SetPluginSettings(std::vector<wxString> params) {
  demosaic_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create Template plugins\n");
  Demosaic* plugin = new Demosaic();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
