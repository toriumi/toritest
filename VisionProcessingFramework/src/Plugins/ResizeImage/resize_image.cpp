/**
 * @file      resize_image.cpp
 * @brief     Resize Image plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./resize_image.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
ResizeImage::ResizeImage() : PluginBase() {
  DEBUG_PRINT("ResizeImage::ResizeImage()\n");

  // Initialize
  resize_type_ = kResizeDefault;
  resize_scale_ = 1.0;
  resize_image_wnd_ = new ResizeImageWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("ResizeImage");

  // Create input port.
  int input_port_id_1 = AddInputPortCandidateSpec(kBGR888);
  int input_port_id_2 = AddInputPortCandidateSpec(kBGR48);

  // Create output port.
  int output_port_id_1 = AddOutputPortCandidateSpec(kBGR888);
  int output_port_id_2 = AddOutputPortCandidateSpec(kBGR48);

  // Create port relation.
  bool is_connect_relation_1 =
      AddPortRelation(input_port_id_1, output_port_id_1);
  bool is_connect_relation_2 =
      AddPortRelation(input_port_id_2, output_port_id_2);

  // Check port relation.
  if (is_connect_relation_1 == false || is_connect_relation_2 == false) {
    DEBUG_PRINT("Resize Image port relation fail\n");
    is_success_initialized_ = false;
  } else {
    is_success_initialized_ = true;
  }

  // Use only src buffer
  set_is_use_dest_buffer(true);
}

/**
 * @brief
 * Destructor.
 */
ResizeImage::~ResizeImage() { delete resize_image_wnd_; }

/**
 * @brief
 * Initialize routine of the Resize plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool ResizeImage::InitProcess(CommonParam* common) {
  DEBUG_PRINT("ResizeImage::InitProcess \n");
  if (is_success_initialized_ == false) {
    DEBUG_PRINT("ResizeImage : is_success_initialized_ == false\n");
    return false;
  } else {
    CvSize size;
    size = cvSize(1280 * resize_scale_, 720 * resize_scale_);
    set_output_image_size(size);
    return true;
  }
}

/**
 * @brief
 * Finalize routine of the Resize plugin.
 * This function is empty implementation.
 */
void ResizeImage::EndProcess() {
  DEBUG_PRINT("ResizeImage::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Resize plugin.
 * This function is empty implementation.
 */
void ResizeImage::DoPostProcess(void) {
  DEBUG_PRINT("ResizeImage::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Main routine of the Resize plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool ResizeImage::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("ResizeImage::DoProcess \n");

  cv::resize(*src_image, *dst_image, cv::Size(), resize_scale_,
             resize_scale_);

  return true;
}

/**
 * @brief
 * Open setting window of the Resize plugin.
 * @param is_running [in] ImageProcessingState
 */
void ResizeImage::OpenSettingWindow(ImageProcessingState state) {
  if (resize_image_wnd_ == NULL) {
    DEBUG_PRINT("resize_image_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  resize_image_wnd_->SetTitle(window_title);
  resize_image_wnd_->InitDialog();
  resize_image_wnd_->Show(true);
  resize_image_wnd_->Raise();
}

/**
 * @brief
 * Set a resize type as a ResizeImageType.
 * This plugin will change the image size according to this type.
 * or set_resize_value();
 * @param resize_type [in] resize type.
 */
void ResizeImage::set_resize_type(ResizeImageType resize_type) {
  resize_type_ = resize_type;
  DEBUG_PRINT("ResizeImage::set_resize_type = %d \n", resize_type_);

  CvSize size;
  if (resize_type_ == kResizeDouble) {
    DEBUG_PRINT("set ResizeImage : Double size\n");
    resize_scale_ = 2.0;
  } else if (resize_type_ == kResizeDefault) {
    DEBUG_PRINT("set ResizeImage : Original size\n");
    resize_scale_ = 1.0;
  } else if (resize_type_ == kResizeHalf) {
    DEBUG_PRINT("set ResizeImage : Half size\n");
    resize_scale_ = 0.5;
  } else if (resize_type_ == kResizeQuarter) {
    DEBUG_PRINT("set ResizeImage : Quarter size\n");
    resize_scale_ = 0.25;
  } else {
    DEBUG_PRINT("set ResizeImage : Invalid size\n");
    // Same as default
    resize_scale_ = 1.0;
  }
  size = cvSize(1280 * resize_scale_, 720 * resize_scale_);
  set_output_image_size(size);
}

/**
 * @brief
 * Set a resize value as a percent.
 * This plugin will change the image size according to this value.
 * or set_resize_type();
 * @param resize_value [in] resize value.
 */
void ResizeImage::set_resize_value(double resize_value) {
  CvSize size;
  DEBUG_PRINT("ResizeImage::set_resize_value = %lf \n", resize_value);

  resize_scale_ = resize_value;

  size = cvSize(1280 * resize_scale_, 720 * resize_scale_);

  set_output_image_size(size);
}

/**
 * @brief
 * Set plugin settings from flow load function.
 * @param params [in] settings string.
 */
void ResizeImage::SetPluginSettings(std::vector<wxString> params) {
  resize_image_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create ResizeImage plugins\n");
  ResizeImage* plugin = new ResizeImage();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
