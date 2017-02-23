/**
 * @file      save_as_image_wnd.cpp
 * @brief     Source for SaveAsImageWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./save_as_image_wnd.h"
#include <string>
#include <vector>
#include "./save_as_image_wnd_define.h"
#include "./main_wnd.h"

BEGIN_EVENT_TABLE(SaveAsImageWnd, wxFrame)
EVT_CLOSE(SaveAsImageWnd::OnClose)
EVT_BUTTON(kButtonFileDialogId, SaveAsImageWnd::OnOpenFileDialog)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor.
 * @param main_wnd [in] Main window class object.
 */
SaveAsImageWnd::SaveAsImageWnd(MainWnd *main_wnd)
    : wxFrame(NULL, kWndId, wxT(kSaveAsImageWndTitle),
              wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  parent_ = main_wnd;
  save_as_image_pause_flag_ = false;
  /* UI creation of the main window.*/
  /* Creating a sensor static text object.*/
  static_text_output_ = new wxStaticText(
      this, kStaticTextOutputId, wxT(kStaticTextOutputName),
      wxPoint(kStaticTextOutputPointX, kStaticTextOutputPointY),
      wxSize(kStaticTextOutputSizeW, kStaticTextOutputSizeH));

  /* Creating a open file dialog button object.*/
  combo_box_output_ =
      new wxComboBox(this, kComboBoxOutputId, wxEmptyString,
                     wxPoint(kComboBoxOutputPointX, kComboBoxOutputPointY),
                     wxSize(kComboBoxOutputSizeW, kComboBoxOutputSizeH), 0, 0,
                     wxCB_DROPDOWN, wxDefaultValidator, wxT(""));

  /* Creating a file dialog button object.*/
  button_file_dialog_ =
      new wxButton(this, kButtonFileDialogId, wxT(kButtonFileDialogName),
                   wxPoint(kButtonFileDialogPointX, kButtonFileDialogPointY),
                   wxSize(kButtonFileDialogSizeW, kButtonFileDialogSizeH));
}

/**
 * @brief
 * Destructor.
 */
SaveAsImageWnd::~SaveAsImageWnd() {}

/**
 * @brief
 * SaveAsImage window end processing
 * @param event [in] Event parameters.
 */
void SaveAsImageWnd::OnClose(wxCloseEvent &event) {
  if (save_as_image_pause_flag_ == true) {
    parent_->StartProcessingThread();
  }
  save_as_image_pause_flag_ = false;
  this->Show(false);
}

/**
 * @brief
 * File path dialog window open processing
 * @param event [in] Event parameters.
 */
void SaveAsImageWnd::OnOpenFileDialog(wxCommandEvent &event) {
  DEBUG_PRINT("SaveAsImageWnd::OnOpenFileDialog\n");
  wxFileDialog *SaveDialog = new wxFileDialog(
   this, _("Select file"), wxEmptyString, wxEmptyString,
  wxT("JPEG Files(*.jpg) | *.jpg | PNG Files(*.png) | *.png | BITMAP Files(*.bmp) | *.bmp"),
   wxFD_SAVE, wxDefaultPosition);

  /* Creates a "Raw save" dialog*/
  if (SaveDialog->ShowModal() == wxID_OK) {
    wxString image_file_path;
    int index = 0;
    /* file open*/
    image_file_path = SaveDialog->GetPath();
    index = SaveDialog->GetFilterIndex();

    if (image_file_path.Find(wxT(".jpg")) == wxNOT_FOUND) {
      if (image_file_path.Find(wxT(".png")) == wxNOT_FOUND) {
        if (image_file_path.Find(wxT(".bmp")) == wxNOT_FOUND) {
          index = SaveDialog->GetFilterIndex();
          if (index == 0) {
            image_file_path += wxT(".jpg");
          } else if (index == 1) {
            image_file_path += wxT(".png");
          } else if (index == 2) {
            image_file_path += wxT(".bmp");
          }
        }
      }
    }
    OnSave(image_file_path);
  }
}

/**
 * @brief
 * Error message box
 * @param message [in] Error message.
 */
void SaveAsImageWnd::ErrorMessageBox(std::string message) {
  wxString wx_err_msg(message.c_str(), wxConvUTF8);
  wxMessageDialog dialog(NULL, wx_err_msg, wxT("Error"), wxOK,
                         wxPoint(100, 100));
  if (dialog.ShowModal() == wxID_OK) {
  }
}

/**
 * @brief
 * Save as image processing
 * @param image_file_path [in] Image file output path.
 */
void SaveAsImageWnd::OnSave(wxString image_file_path) {
  DEBUG_PRINT("SaveAsImageWnd::OnSave\n");
  std::string plugin_name = "";
  std::string file_path = "";
  wxString value = wxT("");
  int image_width = 0;
  int image_height = 0;
  int image_step = 0;

  cv::Mat* temp_image;
  cv::Mat* image;

  /* image pointer check*/
  plugin_name = std::string(combo_box_output_->GetValue().mb_str());
  temp_image = parent_->SaveAsImage(plugin_name);
  if (image == NULL) {
    DEBUG_PRINT("Failed to get image \n");
    ErrorMessageBox("Failed to get image.");
    return;
  }
  /* image size check*/
  image_width = temp_image->size().width;
  image_height = temp_image->size().height;
  image_step = temp_image->step;
  if ((image_width == 0) || (image_height == 0) || (image_step == 0)) {
    DEBUG_PRINT("Image is the invalid size \n");
    ErrorMessageBox("Image is the invalid size.");
    return;
  }
  /* 16bit image convert to 8bit image*/
  if (temp_image->depth() == CV_16U) {
    image = UtilGetCvConvertScale(temp_image, UTIL_CONVERT_10U_TO_8U, 0);
    if (!image) {
      DEBUG_PRINT("It failed to convert 8bit image from 16bit \n");
      ErrorMessageBox("It failed to convert 8bit image from 16bit.");
      return;
    }
  } else {
    image = temp_image;
  }

  /* image path check*/
  value = image_file_path[static_cast<int>(image_file_path.size()) - 4];
  value += image_file_path[static_cast<int>(image_file_path.size()) - 3];
  value += image_file_path[static_cast<int>(image_file_path.size()) - 2];
  value += image_file_path[static_cast<int>(image_file_path.size()) - 1];

  if (value.Find(wxT(".jpg")) == wxNOT_FOUND) {
    if (value.Find(wxT(".png")) == wxNOT_FOUND) {
      if (value.Find(wxT(".bmp")) == wxNOT_FOUND) {
        DEBUG_PRINT("Extension of the file path is invalid \n");
        ErrorMessageBox("Extension of the file path is invalid.");
        return;
      }
    }
  }
  file_path = std::string(image_file_path.mb_str());
  cv::imwrite(file_path, *image);

  if (save_as_image_pause_flag_ == true) {
    parent_->StartProcessingThread();
  }
  save_as_image_pause_flag_ = false;
  this->Show(false);
}

/**
 * @brief
 * utilGetCvConvertScale.
 * @param src_image [in] src cv::Mat
 * @param cvt_mode [in] enum UtilConvertMode
 * @param shift [in] Value added to the scaled source array elements
 * @return dst cv::Mat
 */
cv::Mat* SaveAsImageWnd::UtilGetCvConvertScale(cv::Mat* src_image,
                                               int cvt_mode, double shift) {
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

