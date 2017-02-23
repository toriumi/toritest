/**
 * @file      gamma_correct.cpp
 * @brief     GammaCorrect plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./gamma_correct.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
GammaCorrect::GammaCorrect() : PluginBase() {
  DEBUG_PRINT("GammaCorrect::GammaCorrect()\n");

  reset_table();
  gamma_correct_wnd_ = new GammaCorrectWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("GammaCorrect");

  is_success_initialized_ = false;
  is_use_10bit_lut_ = false;
  is_use_gamma_table_ = false;
  create_gamma_table_ = false;

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

  set_is_use_dest_buffer(false);
}

/**
 * @brief
 * Destructor.
 */
GammaCorrect::~GammaCorrect() { delete gamma_correct_wnd_; }

/**
 * @brief
 * Initialize routine of the GammaCorrect plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool GammaCorrect::InitProcess(CommonParam* common) {
  DEBUG_PRINT("GammaCorrect::InitProcess \n");
  if (is_success_initialized_ == false) {
    return false;
  }

  if (gamma_correct_wnd_->SelectMode() == kTableMode) {
    if (gamma_correct_wnd_->IsExistTableFile()) {
      return true;
    } else {
      DEBUG_PRINT("GammaCorrect::InitProcess false\n");
      PLUGIN_LOG_ERROR("Not exist table file");
      return false;
    }
  } else {  // Function mode
    return true;
  }
}

/**
 * @brief
 * Finalize routine of the GammaCorrect plugin.
 * This function is empty implementation.
 */
void GammaCorrect::EndProcess() { DEBUG_PRINT("GammaCorrect::EndProcess) \n"); }

/**
 * @brief
 * Post-processing routine of the GammaCorrect plugin.
 * This function is empty implementation.
 */
void GammaCorrect::DoPostProcess(void) {
  DEBUG_PRINT("GammaCorrect::DoPostProcess) \n");
}

/**
 * @brief
 * Main routine of the GammaCorrect plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool GammaCorrect::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("GammaCorrect::DoProcess src_image depth = %d\n",
              src_image->depth());
  // Create  Formal gamma LUT first time only or read new LUT file.
  if (is_use_gamma_table_ == true && create_gamma_table_ == true) {
    if (is_use_10bit_lut_ == false && src_image->depth() == CV_8U) {
      // Use 8bit LUT and treat 8bit image data.
      memcpy(gamma_lut8, temp_gamma_lut8, sizeof(uchar) * k8BitTableFileRow);
    } else if (is_use_10bit_lut_ == true && src_image->depth() == CV_16U) {
      // Use 10bit LUT and treat 16bit image data.
      // Four times temp gamma lut values.
      for (int i = 0; i < k10BitTableFileRow; i++) {
        gamma_lut10[i] = temp_gamma_lut10[i] * 4;
      }
    } else if (is_use_10bit_lut_ == false && src_image->depth() == CV_16U) {
      // Use 8bit LUT and treat 16bit image data.
      // Expand the LUT from 8bit table to 10bit table.
      for (int i = 0; i < k8BitTableFileRow; i++) {
        for (int j = 0; j <= 3; j++) {
          gamma_lut10[(i * 4) + j] = temp_gamma_lut8[i] * 4;
        }
      }
    } else if (is_use_10bit_lut_ == true && src_image->depth() == CV_8U) {
      // Use 10bit LUT and treat 8bit image data.
      // Reduct the LUT from 10bit table to 8bit table.
      int j = 0;
      for (int i = 0; i < k10BitTableFileRow; i++) {
        if (i % 4 == 0) {
          gamma_lut8[j] = temp_gamma_lut10[i];
          j++;
        }
      }
    }
    create_gamma_table_ = false;
  }

  int byte_per_pixel;
  int mat_step = src_image->step;

  if (src_image->depth() == CV_8U) {
    byte_per_pixel = 1;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < mat_step; j = j + (3 * byte_per_pixel)) {
        src_image->data[(i * mat_step) + (j + 0 * byte_per_pixel)] =
            (unsigned char)gamma_lut8
                [src_image->data[(i * mat_step) + (j + 0 * byte_per_pixel)]];
        src_image->data[(i * mat_step) + (j + 1 * byte_per_pixel)] =
            (unsigned char)gamma_lut8
                [src_image->data[(i * mat_step) + (j + 1 * byte_per_pixel)]];
        src_image->data[(i * mat_step) + (j + 2 * byte_per_pixel)] =
            (unsigned char)gamma_lut8
                [src_image->data[(i * mat_step) + (j + 2 * byte_per_pixel)]];
      }
    }
  } else if (src_image->depth() == CV_16U) {
    byte_per_pixel = 2;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < mat_step; j = j + (3 * byte_per_pixel)) {
        int num = (i * mat_step) + j;

        UINT16 src_r = static_cast<UINT16>(src_image->data[num]);
        src_r = src_r | (static_cast<UINT16>(src_image->data[num + 1] << 8));
        UINT16 src_g = static_cast<UINT16>(src_image->data[num + 2]);
        src_g = src_g | (static_cast<UINT16>(src_image->data[num + 3] << 8));
        UINT16 src_b = static_cast<UINT16>(src_image->data[num + 4]);
        src_b = src_b | (static_cast<UINT16>(src_image->data[num + 5] << 8));

        UINT16 lut_r = gamma_lut10[src_r];
        UINT16 lut_g = gamma_lut10[src_g];
        UINT16 lut_b = gamma_lut10[src_b];

        src_image->data[num] = lut_r & 0x00FF;
        src_image->data[num + 1] = lut_r >> 8;
        src_image->data[num + 2] = lut_g & 0x00FF;
        src_image->data[num + 3] = lut_g >> 8;
        src_image->data[num + 4] = lut_b & 0x00FF;
        src_image->data[num + 5] = lut_b >> 8;
      }
    }
  }
  return true;
}

/**
 * @brief
 * Open setting window of the GammaCorrect plugin.
 * @param is_running [in] ImageProcessingState
 */
void GammaCorrect::OpenSettingWindow(ImageProcessingState state) {
  if (gamma_correct_wnd_ == NULL) {
    DEBUG_PRINT("gammacorrect_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  gamma_correct_wnd_->SetTitle(window_title);
  gamma_correct_wnd_->InitDialog();
  gamma_correct_wnd_->Show(true);
  gamma_correct_wnd_->Raise();
}

/**
 * @brief
 * Reset the gamma lut table
 */
void GammaCorrect::reset_table() {
  int table_size_8bit = sizeof(gamma_lut8) / sizeof(gamma_lut8[0]);
  int table_size_10bit = sizeof(gamma_lut10) / sizeof(gamma_lut10[0]);

  for (int i = 0; i < table_size_8bit; i++) {
    gamma_lut8[i] = i;
    temp_gamma_lut8[i] = i;
  }
  for (int i = 0; i < table_size_10bit; i++) {
    gamma_lut10[i] = i;
    temp_gamma_lut10[i] = i;
  }
}

/**
 * @brief
 * Set the list of parameter setting string for the GammaCorrect plugin.
 * @param params [in] settings string.
 */
void GammaCorrect::SetPluginSettings(std::vector<wxString> params) {
  gamma_correct_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create GammaCorrect plugins\n");
  GammaCorrect* plugin = new GammaCorrect();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
