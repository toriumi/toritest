/**
 * @file      Template.cpp
 * @brief     Template plugin of VPF.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <vector>
#include "./template.h"
#include "./template_wnd.h"

/**
 * @fn
 * Constructor.
 */
Template::Template() : PluginBase() {
  DEBUG_PRINT("Template::Template()\n");

  wnd_ = new TemplateWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("Template");

  // Create input port.
  int input_port_id_1 = AddInputPortCandidateSpec(kGRAY8);
  int input_port_id_2 = AddInputPortCandidateSpec(kGRAY16);

  // Create output port.
  int output_port_id_1 = AddOutputPortCandidateSpec(kGRAY8);
  int output_port_id_2 = AddOutputPortCandidateSpec(kGRAY16);

  // Create port relation.
  bool is_connect_relation_1 =
      AddPortRelation(input_port_id_1, output_port_id_1);
  bool is_connect_relation_2 =
      AddPortRelation(input_port_id_2, output_port_id_2);

  // Check port relation.
  if (is_connect_relation_1 == false || is_connect_relation_2 == false) {
    DEBUG_PRINT("Template port relation fail\n");
    is_success_initialized_ = false;
  } else {
    is_success_initialized_ = true;
  }

  // Use only src buffer
  set_is_use_dest_buffer(true);
}

/**
 * @fn
 * Destructor.
 */
Template::~Template() {
  delete wnd_;
}

/**
 * @brief
 * Initialize routine of the Template plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool Template::InitProcess(CommonParam *common) {
  DEBUG_PRINT("Template::InitProcess \n");
  if (is_success_initialized_ == false) {
    return false;
  } else {
    common_ = common;
    return true;
  }
}

/**
 * @brief
 * Finalize routine of the Template plugin.
 * This function is empty implementation.
 */
void Template::EndProcess() {
  DEBUG_PRINT("Template::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @fn
 * Post process for plugin.
 */
void Template::DoPostProcess(void) {
  DEBUG_PRINT("Template::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Template plugin.
 * This function is empty implementation.
 */
bool Template::DoProcess(cv::Mat *src_image, cv::Mat *dst_image) {
  DEBUG_PRINT("Template::DoProcess \n");

  *dst_image = src_image->clone();

  return true;
}


/**
 * @brief
 * Open setting window of the Template plugin.
 * @param is_running [in] ImageProcessingState
 */
void Template::OpenSettingWindow(ImageProcessingState state) {
  if (wnd_ == NULL) {
    DEBUG_PRINT("wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  wnd_->SetTitle(window_title);
  wnd_->InitDialog();
  wnd_->Show(true);
  wnd_->Raise();
}

extern "C" PluginBase *Create(void) {
  DEBUG_PRINT("Create Template plugins\n");
  Template *plugin = new Template();
  return plugin;
}

extern "C" void Destroy(PluginBase *plugin) { delete plugin; }
