/**
 * @file      raw_save_wnd.cpp
 * @brief     Source for RawSaveWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <vector>
#include "./raw_save_wnd.h"
#include "./raw_save_wnd_define.h"
#include "./main_wnd.h"
#include <string>

BEGIN_EVENT_TABLE(RawSaveWnd, wxFrame)
EVT_CLOSE(RawSaveWnd::OnClose)
EVT_BUTTON(kButtonFileDialogId, RawSaveWnd::OnOpenFileDialog)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor.
 * @param main_wnd [in] Main window class object.
 */
RawSaveWnd::RawSaveWnd(MainWnd *main_wnd)
    : wxFrame(NULL, kWndId, wxT(kRawSaveWndTitle),
      wxPoint(kWndPointX, kWndPointY), wxSize(kWndSizeW, kWndSizeH)) {
  parent_ = main_wnd;
  raw_save_pause_flag_ = false;

  /* UI creation of the main window.*/
  /* Creating a sensor static text object.*/
  static_text_output_ = new wxStaticText(
      this, kStaticTextOutputId, wxT(kStaticTextSensorName),
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
RawSaveWnd::~RawSaveWnd() {}

/**
 * @brief
 * RawSave window end processing
 * @param event [in] Event parameters.
 */
void RawSaveWnd::OnClose(wxCloseEvent &event) {
  if (raw_save_pause_flag_ == true) {
    parent_->StartProcessingThread();
  }
  raw_save_pause_flag_ = false;
  this->Show(false);
}

/**
 * @brief
 * File path dialog window open processing
 * @param event [in] Event parameters.
 */
void RawSaveWnd::OnOpenFileDialog(wxCommandEvent &event) {
  DEBUG_PRINT("RawSaveWnd::OnOpenFileDialog\n");
  wxFileDialog *SaveDialog = new wxFileDialog(
      this, _("Select file"), wxEmptyString, wxEmptyString,
      wxT("Raw Files (*.raw)|*.raw"), wxFD_SAVE, wxDefaultPosition);

  /* Creates a "Raw save" dialog*/
  if (SaveDialog->ShowModal() == wxID_OK) {
    wxString raw_file_path;
    /* file open*/
    raw_file_path = SaveDialog->GetPath();
    if (raw_file_path.Find(wxT(".raw")) == wxNOT_FOUND) {
      raw_file_path += wxT(".raw");
    }
    OnSave(raw_file_path);
  }
}

/**
 * @brief
 * Error message box
 * @param message [in] Error message.
 */
void RawSaveWnd::ErrorMessageBox(std::string message) {
  wxString wx_err_msg(message.c_str(), wxConvUTF8);
  wxMessageDialog dialog(NULL, wx_err_msg, wxT("Error"), wxOK,
                         wxPoint(100, 100));
  if (dialog.ShowModal() == wxID_OK) {
  }
}

/**
 * @brief
 * Save as raw image processing
 * @param raw_file_path [in] Raw image file output path.
 */
void RawSaveWnd::OnSave(wxString raw_file_path) {
  DEBUG_PRINT("RawSaveWnd::OnSave\n");
  FILE *file;
  char file_path[512];
  memset(file_path, 0, sizeof(file_path));
  std::string plugin_name ="";
  wxString value = wxT("");
  int image_width = 0;
  int image_height = 0;
  int image_step = 0;

  /* raw image pointer check*/
  plugin_name = std::string(combo_box_output_->GetValue().mb_str());
  cv::Mat* raw_image = parent_->RawSaveData(plugin_name);
  if (raw_image == NULL) {
    DEBUG_PRINT("Failed to get raw image \n");
    ErrorMessageBox("Failed to get raw image.");
    return;
  }
  /* raw image size check*/
  image_width = raw_image->size().width;
  image_height = raw_image->size().height;
  image_step = raw_image->step;
  if ((image_width == 0) || (image_height == 0) || (image_step == 0)) {
    DEBUG_PRINT("Raw image is the invalid size \n");
    ErrorMessageBox("Raw image is the invalid size.");
    return;
  }

  /* raw image path check*/
  if (raw_file_path == wxT("")) {
    DEBUG_PRINT("File path is empty \n");
    ErrorMessageBox("File path is empty.");
    return;
  }
  value = raw_file_path[static_cast<int>(raw_file_path.length()) - 4];
  value += raw_file_path[static_cast<int>(raw_file_path.length()) - 3];
  value += raw_file_path[static_cast<int>(raw_file_path.length()) - 2];
  value += raw_file_path[static_cast<int>(raw_file_path.length()) - 1];

  if (value.Find(wxT(".raw")) == wxNOT_FOUND) {
    DEBUG_PRINT("Extension of the file path is invalid \n");
    ErrorMessageBox("Extension of the file path is invalid.");
    return;
  }
  strncpy(file_path, (const char *)raw_file_path.mb_str(),
          static_cast<int>(raw_file_path.length()));
  file = fopen(file_path, "wb");
  if (file == NULL) {
    DEBUG_PRINT("Could not open check file \n");
    ErrorMessageBox("Could not open check file.");
    return;
  }
  /* write header*/
  UINT16 header_width = static_cast<UINT16>(image_width);
  UINT16 header_height = static_cast<UINT16>(image_height);

  fwrite(&header_width, sizeof(header_width), 1, file);
  fwrite(&header_height, sizeof(header_height), 1, file);

  /* write raw file*/
  fwrite(raw_image->data, image_step * image_height, 1, file);
  fclose(file);
  if (raw_save_pause_flag_ == true) {
    parent_->StartProcessingThread();
  }
  raw_save_pause_flag_ = false;
  this->Show(false);
}
