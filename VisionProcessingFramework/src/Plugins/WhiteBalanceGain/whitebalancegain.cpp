/**
 * @file      whitebalancegain.cpp
 * @brief     WhiteBalanceGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./whitebalancegain.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
WhiteBalanceGain::WhiteBalanceGain() : PluginBase() {
  DEBUG_PRINT("WhiteBalanceGain::WhiteBalanceGain()\n");

  // Initialize
  white_balance_gain_wnd_ = new WhiteBalanceGainWnd(this);

  // Initialize base class(plugin_base.h)
  set_plugin_name("WhiteBalanceGain");

  // Create input port.
  int in_1 = AddInputPortCandidateSpec(kGRAY8);
  int in_2 = AddInputPortCandidateSpec(kGRAY16);
  // Create output port.
  int out_1 = AddOutputPortCandidateSpec(kGRAY8);
  int out_2 = AddOutputPortCandidateSpec(kGRAY16);
  // Create port relation.
  bool is_connect_relation_1 = AddPortRelation(in_1, out_1);
  bool is_connect_relation_2 = AddPortRelation(in_2, out_2);

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
WhiteBalanceGain::~WhiteBalanceGain() { delete white_balance_gain_wnd_; }

/**
 * @brief
 * Initialize routine of the Whitebalancegain plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool WhiteBalanceGain::InitProcess(CommonParam* common) {
  // bool WhiteBalanceGain::InitProcess( CommonFunction* common_func) {
  DEBUG_PRINT("WhiteBalanceGain::InitProcess \n");
  if (is_success_initialized_ == true) {
    common_ = common;
    one_push_ = false;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief
 * Finalize routine of the Whitebalancegain plugin.
 * This function is empty implementation.
 */
void WhiteBalanceGain::EndProcess() {
  DEBUG_PRINT("WhiteBalanceGain::EndProcess) \n");
  /* Do nothing. */
}

/**
 * @brief
 * Post-processing routine of the Whitebalancegain plugin.
 * This function is empty implementation.
 */
void WhiteBalanceGain::DoPostProcess(void) {
  DEBUG_PRINT("WhiteBalanceGain::DoPostProcess) \n");
}

/**
 * @brief
 * Main routine of the Whitebalancegain plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool WhiteBalanceGain::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  DEBUG_PRINT("WhiteBalanceGain::DoProcess \n");
  int ob_clamp = common_->optical_black();
  int first_pixel = common_->first_pixel();
  int first_pixel_height;
  int first_pixel_width;
  int byte, byte_max;
  int start_x, start_y, end_x, end_y;
  double sum_R = 0;
  double sum_GR = 0;
  double sum_GB = 0;
  double sum_B = 0;
  double ave_R = 0;
  double ave_B = 0;
  double ave_G = 0;
  int count_R = 0;
  int count_GR = 0;
  int count_GB = 0;
  int count_B = 0;

  if (first_pixel == 0) {
    first_pixel_height = 0;
    first_pixel_width = 0;
  } else if (first_pixel == 1) {
    first_pixel_height = 0;
    first_pixel_width = 1;
  } else if (first_pixel == 2) {
    first_pixel_height = 1;
    first_pixel_width = 0;
  } else if (first_pixel == 3) {
    first_pixel_height = 1;
    first_pixel_width = 1;
  } else {
    DEBUG_PRINT("Failed to first pixel. \n");
    return false;
  }

  if (src_image->depth() == CV_8U) {
    byte = 1;
    byte_max = 0xFF;
  } else if (src_image->depth() == CV_16U) {
    byte = 2;
    byte_max = 0x03FF;
  }

  // one push.
  if (one_push_ == true) {
    CvRect one_push_rect = common_->GetOnepushRectangle();
    start_x = one_push_rect.x;
    start_y = one_push_rect.y;
    end_x = one_push_rect.width;
    end_y = one_push_rect.height;

    one_push_ = false;

    // Skip for out of range.
    if ((start_x < 0) || (start_y < 0) || (end_x < 0) || (end_y < 0) ||
        (start_x >= src_image->cols) || (start_y >= src_image->rows) ||
        (end_x >= src_image->cols) || (end_y >= src_image->rows)) {
      DEBUG_PRINT("Failed to range. \n");
      return false;
    }

    // 1x1pixel -> 2x2pixel
    if ((start_x == end_x) && (start_y == end_y)) {
      end_x = end_x + 1;
      end_y = end_y + 1;
    } else if ((start_x == end_x) && (start_y != end_y)) {
      end_x = end_x + 1;
    } else if ((start_x != end_x) && (start_y == end_y)) {
      end_y = end_y + 1;
    }

    double pixel_value = 0;
    for (int height = start_y; height <= end_y; height++) {
      for (int width = start_x; width <= end_x; width++) {
        // get the pixel value information
        if (byte == 1) {
          pixel_value = static_cast<double>(
              src_image->data[width + height * src_image->step]);
        } else if (byte == 2) {
          pixel_value = static_cast<double>(
              (src_image->data[width * 2 + height * src_image->step] & 0x00FF) |
              (src_image->data[width * 2 + 1 + height * src_image->step] << 8));
        }
        // get the filter color.
        int filt = (((first_pixel + width) % 2) +
                    ((first_pixel / 2 + height) % 2) * 2) %
                   4;

        // Integrate the pixel value of each color.
        switch (filt) {
          case 0:
            sum_R += pixel_value - ((pixel_value<=ob_clamp)?pixel_value:ob_clamp);
            count_R++;
            break;
          case 1:
            sum_GR += pixel_value - ((pixel_value<=ob_clamp)?pixel_value:ob_clamp);
            count_GR++;
            break;
          case 2:
            sum_GB += pixel_value - ((pixel_value<=ob_clamp)?pixel_value:ob_clamp);
            count_GB++;
            break;
          case 3:
            sum_B += pixel_value - ((pixel_value<=ob_clamp)?pixel_value:ob_clamp);
            count_B++;
            break;
        }
      }
    }

    // Calculate average.
    if (count_R > 0 && count_B > 0 && count_GR > 0 && count_GB > 0) {
      // Detection area over 2x2 pixcel
      ave_R = sum_R / count_R;
      ave_B = sum_B / count_B;
      ave_G = ((sum_GR / count_GR) + (sum_GB / count_GB)) / 2.0;
    }

    // Calculate correction value based on green.
    if (ave_R > 0 && ave_B > 0) {
      white_balanace_gain_red_value_ = ave_G / ave_R;
      white_balanace_gain_green_value_ = kWhiteBalanceGainDefaultValue;
      white_balanace_gain_blue_value_ = ave_G / ave_B;
      white_balance_gain_wnd_->SetTextCtrlValue(WhiteBalanceGainRedValue(),
                                                WhiteBalanceGainBlueValue());
    }
  }
  float red_value = white_balanace_gain_red_value_;
  if (red_value < 0x00) {
    DEBUG_PRINT("Failed to white balance gain red value\n");
    red_value = static_cast<float>(kWhiteBalanceGainDefaultValue);
  }
  float blue_value = white_balanace_gain_blue_value_;
  if (blue_value < 0x00) {
    DEBUG_PRINT("Failed to white balance gain blue value\n");
    blue_value = static_cast<float>(kWhiteBalanceGainDefaultValue);
  }

  // White balance gain.
  if (src_image->depth() == CV_8U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < src_image->cols; j++) {
        char* bayer_image_calc;
        bayer_image_calc = reinterpret_cast<char*>(
            &src_image->data[i * src_image->step + j * byte]);
        if (i % 2 == first_pixel_height) {
          if (j % 2 == first_pixel_width) {  // Red
            if (((signed int)((*bayer_image_calc) - ob_clamp) * red_value) <=
                0x00) {
              *bayer_image_calc = 0x00;
              continue;
            } else if (((signed int)((*bayer_image_calc) - ob_clamp) * red_value) >= byte_max) {
              *bayer_image_calc = byte_max;
              continue;
            }
            *bayer_image_calc =
                ((*bayer_image_calc) - ob_clamp) * red_value;
          } else {  // Green
            //*bayer_image_calc =
            //    ((*bayer_image_calc) - ob_clamp);
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp));
          }
        } else {
          if (j % 2 == first_pixel_width) {  // Green
            //*bayer_image_calc =
            //    ((*bayer_image_calc) - ob_clamp);
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp));
          } else {  // Blue
            if (((signed int)((*bayer_image_calc) - ob_clamp) * blue_value) <=
                0x00) {
              *bayer_image_calc = 0x00;
              continue;
            } else if (((signed int)((*bayer_image_calc) - ob_clamp) * blue_value) >= byte_max) {
              *bayer_image_calc = byte_max;
              continue;
            }
            //*bayer_image_calc =
            //    ((*bayer_image_calc) - ob_clamp) * blue_value;
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp)) * blue_value;

          }
        }
      }
    }
  } else if (src_image->depth() == CV_16U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < src_image->rows; i++) {
      for (int j = 0; j < src_image->cols; j++) {
        INT16* bayer_image_calc;
        bayer_image_calc = reinterpret_cast<INT16*>(
            &src_image->data[i * src_image->step + j * byte]);
        if (i % 2 == first_pixel_height) {
          if (j % 2 == first_pixel_width) {  // Red
            if (((signed int)((*bayer_image_calc) - ob_clamp) * red_value) <=
                0x00) {
              *bayer_image_calc = 0x00;
              continue;
            } else if (((signed int)((*bayer_image_calc) - ob_clamp) * red_value) >= byte_max) {
              *bayer_image_calc = byte_max;
              continue;
            }
            *bayer_image_calc =
                ((*bayer_image_calc) - ob_clamp) * red_value;
          } else {  // Green
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp));
          }
        } else {
          if (j % 2 == first_pixel_width) {  // Green
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp));
          } else {  // Blue
            if (((signed int)((*bayer_image_calc) - ob_clamp) * blue_value) <=
                0x00) {
              *bayer_image_calc = 0x00;
              continue;
            } else if (((signed int)((*bayer_image_calc) - ob_clamp) * blue_value) >= byte_max) {
              *bayer_image_calc = byte_max;
              continue;
            }
            *bayer_image_calc =
                ((*bayer_image_calc) - ((*bayer_image_calc<=ob_clamp)?(*bayer_image_calc):ob_clamp)) * blue_value;
          }
        }
      }
    }
  }

  return true;
}


/**
 * @brief
 * Open setting window of the Whitebalancegain plugin.
 * @param is_running [in] ImageProcessingState
 */
void WhiteBalanceGain::OpenSettingWindow(ImageProcessingState state) {
  if (white_balance_gain_wnd_ == NULL) {
    DEBUG_PRINT("white_balance_gain_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  white_balance_gain_wnd_->SetTitle(window_title);
  white_balance_gain_wnd_->InitDialog();
  white_balance_gain_wnd_->Show(true);
  white_balance_gain_wnd_->Raise();
}

/**
 * @brief
 * Set the list of parameter setting string for the Whitebalancegain plugin.
 * @param params [in] settings string.
 */
void WhiteBalanceGain::SetPluginSettings(std::vector<wxString> params) {
  white_balance_gain_wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create WhiteBalanceGain plugins\n");
  WhiteBalanceGain* plugin = new WhiteBalanceGain();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
