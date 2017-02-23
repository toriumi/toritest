/**
 * @file      colormatrix.cpp
 * @brief     ColorMatrix plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./colormatrix.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
ColorMatrix::ColorMatrix() : PluginBase() {
  DEBUG_PRINT("ColorMatrix::ColorMatrix()\n");

  // Initialize
  color_matrix_wnd_ = new ColorMatrixWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("ColorMatrix");

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
    DEBUG_PRINT("Color Matrix port relation fail\n");
    is_success_initialized_ = false;
  } else {
    is_success_initialized_ = true;
  }

  // Use only src buffer
  set_is_use_dest_buffer(false);
}

/**
 * @brief
 * Destructor.
 */
ColorMatrix::~ColorMatrix() { delete color_matrix_wnd_; }

/**
 * @brief
 * Initialize routine of the Color plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool ColorMatrix::InitProcess(CommonParam *common) {
  DEBUG_PRINT("ColorMatrix::InitProcess \n");
  if (is_success_initialized_ == false) {
    return false;
  } else {
    return true;
  }
}

/**
 * @brief
 * Finalize routine of the Color plugin.
 * This function is empty implementation.
 */
void ColorMatrix::EndProcess() {
  DEBUG_PRINT("ColorMatrix::EndProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Post-processing routine of the Color plugin.
 * This function is empty implementation.
 */
void ColorMatrix::DoPostProcess(void) {
  DEBUG_PRINT("ColorMatrix::DoPostProcess) \n");
  /* Do nothing*/
}

/**
 * @brief
 * Main routine of the Color plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool ColorMatrix::DoProcess(cv::Mat *src_image, cv::Mat *dst_image) {
  DEBUG_PRINT("ColorMatrix::DoProcess \n");
  int max;
  int zero;
  int bit_num;
  int mat_step = src_image->step;

  if (src_image->depth() == CV_8U) {
    max = 0xFF;
    zero = 0x00;
    bit_num = 1;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < mat_step; j = j + (3 * bit_num)) {
        /* get RGB data */
        unsigned char *img_calc_B =
            (unsigned char *)&src_image->data[i * mat_step + j];
        unsigned char *img_calc_G =
            (unsigned char *)&src_image->data[i * mat_step + j + (1 * bit_num)];
        unsigned char *img_calc_R =
            (unsigned char *)&src_image->data[i * mat_step + j + (2 * bit_num)];

        float value_R, value_G, value_B;
        value_R = (*img_calc_R) * color_matrix_[0][0] +
                  (*img_calc_G) * color_matrix_[0][1] +
                  (*img_calc_B) * color_matrix_[0][2];
        value_G = (*img_calc_R) * color_matrix_[1][0] +
                  (*img_calc_G) * color_matrix_[1][1] +
                  (*img_calc_B) * color_matrix_[1][2];
        value_B = (*img_calc_R) * color_matrix_[2][0] +
                  (*img_calc_G) * color_matrix_[2][1] +
                  (*img_calc_B) * color_matrix_[2][2];

        /* Red */
        if (value_R < static_cast<float>(zero)) {
          *img_calc_R = zero;
        } else if (value_R > static_cast<float>(max)) {
          *img_calc_R = max;
        } else {
          *img_calc_R = (unsigned char)value_R;
        }

        /* Green */
        if (value_G < static_cast<float>(zero)) {
          *img_calc_G = zero;
        } else if (value_G > static_cast<float>(max)) {
          *img_calc_G = max;
        } else {
          *img_calc_G = (unsigned char)value_G;
        }

        /* Blue */
        if (value_B < static_cast<float>(zero)) {
          *img_calc_B = zero;
        } else if (value_B > static_cast<float>(max)) {
          *img_calc_B = max;
        } else {
          *img_calc_B = (unsigned char)value_B;
        }
      }
    }
  } else if (src_image->depth() == CV_16U) {
    max = 0x3FF;
    zero = 0x0000;
    bit_num = 2;

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < mat_step; j = j + (3 * bit_num)) {
        /* get RGB data */
        INT16 *img_calc_B =
            reinterpret_cast<INT16 *>(&src_image->data[i * mat_step + j]);
        INT16 *img_calc_G = reinterpret_cast<INT16 *>(
            &src_image->data[i * mat_step + j + (1 * bit_num)]);
        INT16 *img_calc_R = reinterpret_cast<INT16 *>(
            &src_image->data[i * mat_step + j + (2 * bit_num)]);

        float value_R, value_G, value_B;
        value_R = (*img_calc_R) * color_matrix_[0][0] +
                  (*img_calc_G) * color_matrix_[0][1] +
                  (*img_calc_B) * color_matrix_[0][2];
        value_G = (*img_calc_R) * color_matrix_[1][0] +
                  (*img_calc_G) * color_matrix_[1][1] +
                  (*img_calc_B) * color_matrix_[1][2];
        value_B = (*img_calc_R) * color_matrix_[2][0] +
                  (*img_calc_G) * color_matrix_[2][1] +
                  (*img_calc_B) * color_matrix_[2][2];

        /* Red */
        if (value_R < static_cast<float>(zero)) {
          *img_calc_R = zero;
        } else if (value_R > static_cast<float>(max)) {
          *img_calc_R = max;
        } else {
          *img_calc_R = static_cast<INT16>(value_R);
        }

        /* Green */
        if (value_G < static_cast<float>(zero)) {
          *img_calc_G = zero;
        } else if (value_G > static_cast<float>(max)) {
          *img_calc_G = max;
        } else {
          *img_calc_G = static_cast<INT16>(value_G);
        }

        /* Blue */
        if (value_B < static_cast<float>(zero)) {
          *img_calc_B = zero;
        } else if (value_B > static_cast<float>(max)) {
          *img_calc_B = max;
        } else {
          *img_calc_B = static_cast<INT16>(value_B);
        }
      }
    }
  }
  return true;
}

/**
 * @brief
 * Open setting window of the Color plugin.
 * @param is_running [in] ImageProcessingState
 */
void ColorMatrix::OpenSettingWindow(ImageProcessingState state) {
  if (color_matrix_wnd_ == NULL) {
    DEBUG_PRINT("colormatrix_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  color_matrix_wnd_->SetTitle(window_title);
  color_matrix_wnd_->InitDialog();
  color_matrix_wnd_->Show(true);
  color_matrix_wnd_->Raise();
}

/**
 * @brief
 * Set color matrix.
 * @param color_martix [in] color matrix.
 */
void ColorMatrix::set_color_matrix(float color_martix[3][3]) {
  memcpy(color_matrix_, color_martix, sizeof(float) * 3 * 3);
}

/**
 * @brief
 * Set plugin settings from flow load function.
 * @param params [in] settings string.
 */
void ColorMatrix::SetPluginSettings(std::vector<wxString> params) {
  color_matrix_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase *Create(void) {
  DEBUG_PRINT("Create ColorMatrix plugins\n");
  ColorMatrix *plugin = new ColorMatrix();
  return plugin;
}

extern "C" void Destroy(PluginBase *plugin) { delete plugin; }
