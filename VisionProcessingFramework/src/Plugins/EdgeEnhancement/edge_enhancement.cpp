/**
 * @file      edge_enhancement.cpp
 * @brief     EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <vector>
#include "./edge_enhancement.h"

/**
 * @brief
 * Constructor for this plugin.
 */
EdgeEnhancement::EdgeEnhancement() : PluginBase() {
  DEBUG_PRINT("EdgeEnhancement::EdgeEnhancement()\n");

  // Initialize
  param_ = new EdgeEnhancementParam();
  wnd_ = new EdgeEnhancementWnd(this, param_);
  is_success_initialized_ = false;

  // Initialize base class(plugin_base.h)
  set_plugin_name("EdgeEnhancement");

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
    DEBUG_PRINT("Gamma Correct port relation fail\n");
    is_success_initialized_ = false;
  } else {
    is_success_initialized_ = true;
  }
}

/**
 * @brief
 * Destructor for this plugin.
 */
EdgeEnhancement::~EdgeEnhancement() {
  delete param_;
  delete wnd_;
}

/**
 * @brief
 * Initialize routine of the EdgeEnhancement plugin.
 * This function is empty implementation.
 * @param common [in] commom parameters.
 */
bool EdgeEnhancement::InitProcess(CommonParam* common) {
  DEBUG_PRINT("EdgeEnhancement::InitProcess\n");
  if (is_success_initialized_ == true) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief
 * Post-processing routine of the EdgeEnhancement plugin.
 * This function is empty implementation.
 */
void EdgeEnhancement::DoPostProcess(void) {
  DEBUG_PRINT("EdgeEnhancement::DoPostProcess(void) \n");
}

/**
 * @brief
 * Main routine of the EdgeEnhancement plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 */
bool EdgeEnhancement::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  if (src_image == NULL || dst_image == NULL) {
    DEBUG_PRINT("[EdgeEnhancement]src_image == NULL || dst_image == NULL\n");
    return false;
  }

  *dst_image = src_image->clone();

  float coeff = param_->GetCoeff();

  /* edge filster*/
  float FilterData[] = {-coeff / 9.0f, -coeff / 9.0f,          -coeff / 9.0f,
                        -coeff / 9.0f, 1 + (8 * coeff) / 9.0f, -coeff / 9.0f,
                        -coeff / 9.0f, -coeff / 9.0f,          -coeff / 9.0f};

  cv::Mat kernel = cv::Mat(3, 3, CV_32F, FilterData);
  cv::filter2D(*src_image, *dst_image, -1, kernel, cv::Point(-1, -1), 0, -0);

  if (dst_image->depth() == CV_16U) {
    int mat_step = dst_image->step;
    int byte_per_pixel = 2;
    for (int i = 0; i < dst_image->rows; i++) {
      for (int j = 0; j < mat_step; j = j + (3 * byte_per_pixel)) {
        int num = (i * mat_step) + j;

        UINT16 dst_r = static_cast<UINT16>(dst_image->data[num]);
        dst_r = dst_r | (static_cast<UINT16>(dst_image->data[num + 1] << 8));
        UINT16 dst_g = static_cast<UINT16>(dst_image->data[num + 2]);
        dst_g = dst_g | (static_cast<UINT16>(dst_image->data[num + 3] << 8));
        UINT16 dst_b = static_cast<UINT16>(dst_image->data[num + 4]);
        dst_b = dst_b | (static_cast<UINT16>(dst_image->data[num + 5] << 8));

        if (dst_r > 1023) {
          dst_r = 1023;
        }
        if (dst_g > 1023) {
          dst_g = 1023;
        }
        if (dst_b > 1023) {
          dst_b = 1023;
        }

        dst_image->data[num] = dst_r & 0x00FF;
        dst_image->data[num + 1] = dst_r >> 8;
        dst_image->data[num + 2] = dst_g & 0x00FF;
        dst_image->data[num + 3] = dst_g >> 8;
        dst_image->data[num + 4] = dst_b & 0x00FF;
        dst_image->data[num + 5] = dst_b >> 8;
      }
    }
  }

  return true;
}

/**
 * @brief
 * Finalize routine of the EdgeEnhancement plugin.
 * This function is empty implementation.
 */
void EdgeEnhancement::EndProcess() { ; }

/**
 * @brief
 * Open setting window of the EdgeEnhancement plugin.
 * @param is_running [in] ImageProcessingState
 */
void EdgeEnhancement::OpenSettingWindow(ImageProcessingState state) {
  if (wnd_ == NULL) {
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  wnd_->SetTitle(window_title);
  wnd_->InitDialog();
  wnd_->Show(true);
  wnd_->Raise();
}

/**
 * @brief
 * Set the list of parameter setting string for the EdgeEnhancement plugin.
 * @param params [in] settings string.
 */
void EdgeEnhancement::SetPluginSettings(std::vector<wxString> params) {
  wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  EdgeEnhancement* plugin = new EdgeEnhancement();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
