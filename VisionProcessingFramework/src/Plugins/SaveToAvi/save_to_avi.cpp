/**
 * @file      save_to_avi.cpp
 * @brief     SaveToAvi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./save_to_avi.h"
#include <vector>

/**
 * @brief
 * Constructor.
 */
SaveToAvi::SaveToAvi() : PluginBase() {
  DEBUG_PRINT("SaveToAvi::SaveToAvi()\n");

  set_plugin_name("SaveToAvi");

  AddInputPortCandidateSpec(kBGR888); /* BGR888 */
  AddInputPortCandidateSpec(kBGR48);  /* BGR48 */

  set_is_use_dest_buffer(false);

  // Initialize
  wnd_ = new SaveToAviWnd(this);
  wxString wx_string(plugin_name().c_str(), wxConvUTF8);
  wnd_->InitDialog();
}

/**
 * @brief
 * Destructor.
 */
SaveToAvi::~SaveToAvi() { delete wnd_; }

/**
 * @brief
 * Initialize routine of the SaveToAvi plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization
 */
bool SaveToAvi::InitProcess(CommonParam* common) {
  DEBUG_PRINT("OutputDispOpencv::InitProcess \n");
  common_ = common;
  wnd_->SetWindowName(plugin_name());
  wnd_->PostCaptureInit();
  wnd_->InitAvi();

  return true;
}

/**
 * @brief
 * Finalize routine of the SaveToAvi plugin.
 * This function is empty implementation.
 */
void SaveToAvi::EndProcess() {
  DEBUG_PRINT("OutputDispOpencv::EndProcess \n");
  wnd_->PostCaptureEnd();
}

/**
 * @brief
 * Post-processing routine of the SaveToAvi plugin.
 * This function is empty implementation.
 */
void SaveToAvi::DoPostProcess(void) {}

/**
 * @brief
 * Main routine of the SaveToAvi plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing
 */
bool SaveToAvi::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  cv::Mat* temp_image;

  if (src_image == NULL) {
    DEBUG_PRINT("[SaveToAvi]src_image == NULL\n");
    return false;
  }

  if (wnd_ == NULL) {
    DEBUG_PRINT("[SaveToAvi]wnd_ == NULL\n");
    return false;
  }

  if (src_image->depth() == CV_16U) {
    temp_image = UtilGetCvConvertScale(src_image, UTIL_CONVERT_10U_TO_8U, 0);
    if (temp_image) {
      if (wnd_->Enqueue(temp_image) == false) {
        DEBUG_PRINT("Enqueue failed");
        delete temp_image;
        return false;
      } else {
        delete temp_image;
      }
    }
  } else {
    if (wnd_->Enqueue(src_image) == false) {
      DEBUG_PRINT("Enqueue failed");
      return false;
    }
  }

  wnd_->PostCapture();
  return true;
}

/**
 * @brief
 * Open setting window of the SaveToAvi plugin.
 * @param is_running [in] ImageProcessingState
 */
void SaveToAvi::OpenSettingWindow(ImageProcessingState state) {
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

/**
 * @brief
 * Close setting window of the SaveToAvi plugin.
 * @return If true, success close window
 */
bool SaveToAvi::CloseSettingWindow() {
  if (wnd_ == NULL) {
    return false;
  }
  wnd_->Show(false);
  return true;
}

/**
 * @brief
 * Convert the bit depth of image.
 * @param src_image [in] Pointer to the src image
 * @param cvt_mode [in] enum UtilConvertMode
 * @param shift [in] Value added to the scaled source array elements
 * @return Pointer to the converted image.
 */
cv::Mat* SaveToAvi::UtilGetCvConvertScale(cv::Mat* src_image, int cvt_mode,
                                          double shift) {
  cv::Mat* dst_image;
  int src_channels = src_image->channels();
  int max_width = src_image->size().width * src_channels;
  double scale;

  if (cvt_mode == UTIL_CONVERT_10U_TO_8U) {
    switch (src_channels) {
      case 1:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height), CV_8UC1);
        break;
      case 3:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height), CV_8UC3);
        break;
      default:
        return NULL;
    }
    scale = 1.0 / 4.0;  // change scale 10bit -> 8bit
  } else if (cvt_mode == UTIL_CONVERT_10U_TO_16U) {
    switch (src_channels) {
      case 1:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height),
            CV_16UC1);
        break;
      case 3:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height),
            CV_16UC3);
        break;
      default:
        return NULL;
    }
    scale = 64.0;  // change scale 10bit -> 16bit
  } else {
    DEBUG_PRINT("Invalid convert mode =%d return NULL", cvt_mode);
    return NULL;
  }

  if (cvt_mode == UTIL_CONVERT_10U_TO_8U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < dst_image->size().height; i++) {
      for (int j = 0; j < max_width; j += src_channels) {
        ((unsigned char*)(dst_image->data + dst_image->step * i))[j] =
            (reinterpret_cast<INT16*>(src_image->data +
                                      src_image->step * i))[j] *
                scale +
            shift;
        if (src_channels == 3) {
          ((unsigned char*)(dst_image->data + dst_image->step * i))[j + 1] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 1] *
                  scale +
              shift;
          ((unsigned char*)(dst_image->data + dst_image->step * i))[j + 2] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 2] *
                  scale +
              shift;
        }
      }
    }
  } else if (cvt_mode == UTIL_CONVERT_10U_TO_16U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < dst_image->size().height; i++) {
      for (int j = 0; j < max_width; j += src_channels) {
        // 16bit code
        (reinterpret_cast<INT16*>(dst_image->data + dst_image->step * i))[j] =
            (reinterpret_cast<INT16*>(src_image->data +
                                      src_image->step * i))[j] *
                scale +
            shift;
        if (src_channels == 3) {
          (reinterpret_cast<INT16*>(dst_image->data +
                                    dst_image->step * i))[j + 1] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 1] *
                  scale +
              shift;
          (reinterpret_cast<INT16*>(dst_image->data +
                                    dst_image->step * i))[j + 2] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 2] *
                  scale +
              shift;
        }
      }
    }
  } else {
    DEBUG_PRINT("Invalid convert mode =%d return NULL", cvt_mode);
    return NULL;
  }
  return dst_image;
}

/**
 * @brief
 * Set the list of parameter setting string for the SaveToAvi plugin.
 * @param params [in] settings string.
 */
void SaveToAvi::SetPluginSettings(std::vector<wxString> params) {
  wnd_->SetPluginSettings(params);
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create Template plugins\n");
  SaveToAvi* plugin = new SaveToAvi();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
