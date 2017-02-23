/**
 * @file      sensor_focus.cpp
 * @brief     SensorFocus plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./sensor_focus.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
SensorFocus::SensorFocus() : PluginBase() {
  DEBUG_PRINT("SensorFocus::SensorFocus()\n");

  // Initialize
  sensor_focus_wnd_ = new SensorFocusWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("FocusOIS");

  // Create input port.
  AddInputPortCandidateSpec(kGRAY8);
  AddInputPortCandidateSpec(kGRAY16);

  // Use only src buffer
  set_is_use_dest_buffer(false);
}

/**
 * @brief
 * Destructor.
 */
SensorFocus::~SensorFocus() { delete sensor_focus_wnd_; }

/**
 * @brief
 * Initialize routine of the Color plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool SensorFocus::InitProcess(CommonParam* common) {
  DEBUG_PRINT("SensorFocus::InitProcess \n");
  return true;
}

/**
 * @brief
 * Finalize routine of the Color plugin.
 * This function is empty implementation.
 */
void SensorFocus::EndProcess() {
  DEBUG_PRINT("SensorFocus::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Color plugin.
 * This function is empty implementation.
 */
void SensorFocus::DoPostProcess(void) {
  DEBUG_PRINT("SensorFocus::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Main routine of the Color plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool SensorFocus::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("SensorFocus::DoProcess \n");
  return true;
}

/**
 * @brief
 * Open setting window of the EdgeEnhancement plugin.
 * @param is_running [in] ImageProcessingState
 */
void SensorFocus::OpenSettingWindow(ImageProcessingState state) {
  if (sensor_focus_wnd_ == NULL) {
    DEBUG_PRINT("sensor_focus_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  sensor_focus_wnd_->SetTitle(window_title);
  sensor_focus_wnd_->InitDialog();
  sensor_focus_wnd_->Show(true);
  sensor_focus_wnd_->Raise();
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create SensorFocus plugins\n");
  SensorFocus* plugin = new SensorFocus();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
