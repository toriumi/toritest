/**
 * @file      avi.cpp
 * @brief     Plugin to load the AVI file.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./avi.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
Avi::Avi() : PluginBase() {
  DEBUG_PRINT("Avi::Avi()\n");

  /* Initialize */
  avi_wnd_ = new AviWnd(this);
  common_ = NULL;
  first_pixel_ = 0;

  /* PluginName Setting */
  set_plugin_name("Avi");

  /*********************************
    PortSpec Settings
  *********************************/
  /* InputPort  */
  /*   No available InputPort */
  /* OutputPort */
  AddOutputPortCandidateSpec(kBGR888);
  AddOutputPortCandidateSpec(kBGR48);
}

/**
 * @brief
 * Destructor.
 */
Avi::~Avi() { delete avi_wnd_; }

/**
 * @brief
 * Initialize process for plugin.
 * @param common [in] commom parameters.
 */
bool Avi::InitProcess(CommonParam* common) {
  DEBUG_PRINT("Avi::InitProcess \n");
  /* Do nothing.*/

  common_ = common;
  common_->set_first_pixel(first_pixel_);
  DEBUG_PRINT("first_pixel_ init:%d \n", first_pixel_);
  common_->set_optical_black(optical_black_);
  DEBUG_PRINT("optical_black_ init:%d \n", optical_black_);

  if (avi_wnd_->InitializeAviImage() == false) {
    return false;
  }
  return true;
}

/**
 * @brief
 * Finalize routine of the Avi plugin.
 * This function is empty implementation.
 */
void Avi::EndProcess() {
  DEBUG_PRINT("Avi::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Demosaic plugin.
 * This function is empty implementation.
 */
void Avi::DoPostProcess(void) {
  DEBUG_PRINT("Avi::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Main routine of the Avi plugin.
 * @param src_image [in] src image data.
 * @param dst_image [out] dst image data.
 * @return If true, success in the main processing
 */
bool Avi::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("Avi::DoProcess \n");
  return avi_wnd_->GetImageBuffer(dst_image);
}

/**
 * @brief
 * Open setting window of plugin.
 * @param state [in] Image processing state
 */
void Avi::OpenSettingWindow(ImageProcessingState state) {
  if (avi_wnd_ == NULL) {
    DEBUG_PRINT("avi_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  avi_wnd_->SetTitle(window_title);
  avi_wnd_->UpdateUIForImageProcessingState(state);
  avi_wnd_->InitDialog();
  avi_wnd_->Show(true);
  avi_wnd_->Raise();
}

/**
 * @brief
 * Close setting window of plugin.
 * @return true, Close a avi settings window.
 */
bool Avi::CloseSettingWindow() {
  if (avi_wnd_ == NULL) {
    return false;
  }

  avi_wnd_->Show(false);

  return true;
}

/**
 * @brief
 * Set the image processing state.
 * @param state [in] ImageProcessingState
 */
void Avi::set_image_processing_state(ImageProcessingState state) {
  DEBUG_PRINT("Avi::set_image_processing_state() state = %d\n", state);
  avi_wnd_->UpdateUIForImageProcessingState(state);
}

/**
 * @brief
 * Set first pixel.
 * @param first_pixel [in] first pixel.
 */
void Avi::set_first_pixel(int first_pixel) {
  first_pixel_ = first_pixel;
}

/**
 * @brief
 * Set optical black.
 * @param optical_black [in] optical black.
 */
void Avi::set_optical_black(int optical_black) {
  optical_black_ = optical_black;
}

/**
 * @brief
 * Set the list of parameter setting string for the Demosaic plugin.
 * @param params [in] settings string.
 */
void Avi::SetPluginSettings(std::vector<wxString> params) {
  avi_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create avi plugins\n");
  Avi* plugin = new Avi();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
