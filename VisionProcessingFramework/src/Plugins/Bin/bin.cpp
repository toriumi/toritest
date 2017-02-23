/**
 * @file      bin.cpp
 * @brief     Input plugin for RAW file.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./bin.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
Bin::Bin() : PluginBase() {
  DEBUG_PRINT("Bin::Bin()\n");

  /* Initialize */
  bin_wnd_ = new BinWnd(this);
  common_ = NULL;
  first_pixel_ = 0;

  /* PluginName Setting */
  set_plugin_name("Bin");

  // Create output port.
  AddOutputPortCandidateSpec(kGRAY8);
  AddOutputPortCandidateSpec(kGRAY16);
}

/**
 * @brief
 * Destructor.
 */
Bin::~Bin() { delete bin_wnd_; }

/**
 * @brief
 * Initialize routine of the Bin plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool Bin::InitProcess(CommonParam* common) {
  DEBUG_PRINT("Bin::InitProcess \n");

  common_ = common;
  common_->set_first_pixel(first_pixel_);
  DEBUG_PRINT("first_pixel_ init:%d \n", first_pixel_);
  common_->set_optical_black(optical_black_);
  DEBUG_PRINT("optical_black_ init:%d \n", optical_black_);

  return true;
}

/**
 * @brief
 * Finalize routine of the Bin plugin.
 * This function is empty implementation.
 */
void Bin::EndProcess() {
  DEBUG_PRINT("Bin::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Bin plugin.
 * This function is empty implementation.
 */
void Bin::DoPostProcess(void) { DEBUG_PRINT("Bin::DoPostProcess) \n"); }

/**
 * @brief
 * Main routine of the Bin plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool Bin::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("Bin::DoProcess \n");
  return bin_wnd_->GetImageBuffer(dst_image);
}

/**
 * @brief
 * Open setting window of the Bin plugin.
 * @param is_running [in] ImageProcessingState
 */
void Bin::OpenSettingWindow(ImageProcessingState state) {
  if (bin_wnd_ == NULL) {
    DEBUG_PRINT("bin_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  bin_wnd_->SetTitle(window_title);
  bin_wnd_->UpdateUIForImageProcessingState(state);
  bin_wnd_->InitDialog();
  bin_wnd_->Show(true);
  bin_wnd_->Raise();
}

/**
 * @brief
 * Set the image processing state.
 * @param state [in] ImageProcessingState
 */
void Bin::set_image_processing_state(ImageProcessingState state) {
  DEBUG_PRINT("Bin::set_image_processing_state() state = %d\n", state);
  bin_wnd_->UpdateUIForImageProcessingState(state);
}

/**
 * @brief
 * Set first pixel.
 * @param first_pixel [in] first pixel.
 */
void Bin::set_first_pixel(int first_pixel) {
  first_pixel_ = first_pixel;
}

/**
 * @brief
 * Set optical black.
 * @param optical_black [in] optical black.
 */
void Bin::set_optical_black(int optical_black) {
  optical_black_ = optical_black;
}

/**
 * @brief
 * Set the list of parameter setting string for the Bin plugin.
 * @param params [in] settings string.
 */
void Bin::SetPluginSettings(std::vector<wxString> params) {
  bin_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create bin plugins\n");
  Bin* plugin = new Bin();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
