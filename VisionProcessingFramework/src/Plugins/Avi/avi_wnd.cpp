/**
 * @file      avi_wnd.cpp
 * @brief     Window of Avi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <algorithm>
#include <string>
#include <vector>
#include "./avi_wnd.h"

BEGIN_EVENT_TABLE(AviWnd, wxFrame)
EVT_CLOSE(AviWnd::OnClose)
EVT_BUTTON(kBtnSelectAviFileWndId, AviWnd::OnOpenAviFile)
EVT_BUTTON(kBtnApplyId, AviWnd::OnUpdate)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
AviWnd::AviWnd(Avi *parent)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {

  // Create avi file select button
  wx_button_open_avi_file_ = new wxButton(
     this, kBtnSelectAviFileWndId, wxT(kBtnSelectAviFileWndText),
     wxPoint(kBtnSelectAviFileWndPointX, kBtnSelectAviFileWndPointY),
     wxSize(kBtnSelectAviFileWndSizeW, kBtnSelectAviFileWndSizeH));

  // Create static text(file path)
  wx_static_text_file_path_ =
      new wxStaticText(this, kStaticTextAviFilePathId, wxT("File:"),
                       wxPoint(kStaticTextAviFilePathPointX,
                               kStaticTextAviFilePathPointY),
                       wxSize(kStaticTextAviFilePathSizeW,
                              kStaticTextAviFilePathSizeH));
  // Create static text(file path)
  wx_text_ctrl_file_path_ = new wxTextCtrl(
      this, kTextAviFilePathId, wxT(""),
      wxPoint(kTextAviFilePathPointX, kTextAviFilePathPointY),
      wxSize(kTextAviFilePathSizeW, kTextAviFilePathSizeH),
      wxTE_READONLY);
  // Create apply button
  wx_button_setting_apply_ =
      new wxButton(this, kBtnApplyId, wxT(kBtnApplyText),
                   wxPoint(kBtnApplyPointX, kBtnApplyPointY),
                   wxSize(kBtnApplySizeW, kBtnApplySizeH));

  parent_ = parent;
  avi_image_ = NULL;
  last_avi_image_ = NULL;
  capture_image_ = NULL;
  temp_capture_image_ = NULL;
  fps_ = kDefaultFps;

  LoadSettingsFromFile(wxT(kAviConfigFile));
}

/**
 * @brief
 * Destructor for this window.
 */
AviWnd::~AviWnd() {
  if (avi_image_ != NULL) {
    delete avi_image_;
    avi_image_ = NULL;
  }
  if (last_avi_image_ != NULL) {
    delete last_avi_image_;
    last_avi_image_ = NULL;
  }
  if (temp_capture_image_ != NULL) {
    delete temp_capture_image_;
    temp_capture_image_ = NULL;
  }
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void AviWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * The handler function for BTN_SELECT_AVI_FILE_WND_ID.
 * Open dialog to select AVI file.
 */
void AviWnd::OnOpenAviFile(wxCommandEvent &event) {
  wxFileDialog *OpenDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
      wxT("*.avi"), wxFD_OPEN, wxDefaultPosition);

  /* Creates a "open file" dialog*/
  if (OpenDialog->ShowModal() == wxID_OK) {
    OpenRawFile(OpenDialog->GetPath(), true);
  }
}

/**
 * @brief
 * The handler function for kBtnApplyId.
 */
void AviWnd::OnUpdate(wxCommandEvent &event) {
  if (temp_capture_image_) {
      parent_->set_optical_black(16);
      parent_->set_active_output_port_spec_index(0);
    int image_width =
           static_cast<int>(temp_capture_image_->get(CV_CAP_PROP_FRAME_WIDTH));
    int image_height =
          static_cast<int>(temp_capture_image_->get(CV_CAP_PROP_FRAME_HEIGHT));
    CvSize size = cvSize(image_width, image_height);
    parent_->set_output_image_size(size);
    capture_image_ = temp_capture_image_;
  }
  this->Show(false);
  WriteSettingsToFile(wxT(kAviConfigFile));
}

/**
 * @brief
 * Read AVI file and generate image buffer.
 * @param image_file_path [in] AVI image file path
 */
FileReadError AviWnd::LoadImageData(char *image_file_path) {
  int image_width = 0;
  int image_height = 0;

  DEBUG_PRINT("Avi::OpenFile str=%s\n", image_file_path);
  /* If Image data is already allocated, release Image data.*/
  if (avi_image_ != NULL) {
    delete avi_image_;
    avi_image_ = NULL;
  }
  if (temp_capture_image_ != NULL) {
    delete temp_capture_image_;
    temp_capture_image_ = NULL;
    capture_image_ = NULL;
  }

  /* Open avi file*/
  temp_capture_image_ = new cv::VideoCapture(image_file_path);

  /* Check avi file*/
  if (!temp_capture_image_->isOpened()) {
    DEBUG_PRINT("Avi::OpenFile could not open file\n");
    return kCouldNotOpenFileError;
  }

  fps_ = temp_capture_image_->get(CV_CAP_PROP_FPS);
  if (fps_ == 0) {
    DEBUG_PRINT("AviWnd::InitializeAviImage is not read yet. get framerate\n");
    fps_ = kDefaultFps;
  }
  fps_wait_ = static_cast<int>((1 / fps_) * 1000000);

  /* Create Avi Bayer Image*/
  DEBUG_PRINT("Create Avi Bayer Img\n");

  image_width =
          static_cast<int>(temp_capture_image_->get(CV_CAP_PROP_FRAME_WIDTH));
  image_height =
          static_cast<int>(temp_capture_image_->get(CV_CAP_PROP_FRAME_HEIGHT));

  avi_image_ = new cv::Mat(cvSize(image_width, image_height), CV_8UC3);

  return kNoneError;
}

/**
 * @brief
 * Initialize AVI image .
 * @param none.
 * @return true : success to initialize avi image.
 *         false : fail to initialize avi image.
 */
bool AviWnd::InitializeAviImage() {
  if (capture_image_ == NULL) {
    DEBUG_PRINT("AviWnd::InitializeAviImage is not read yet. exit function\n");
    return false;
  }
  if (capture_image_->set(CV_CAP_PROP_POS_FRAMES, 0) == false) {
    DEBUG_PRINT("AviWnd::InitializeAviImage is not read yet. set frame\n");
    return false;
  }
  int frame_position =
                 static_cast<int>(capture_image_->get(CV_CAP_PROP_POS_FRAMES));
  if (frame_position != 0) {
    DEBUG_PRINT("AviWnd::InitializeAviImage is not read yet. get frame\n");
    return false;
  }
  return true;
}

/**
 * @brief
 * Get AVI image burrer.
 * @param image_buffer [out] AVI image buffer.
 * @return true : success to get image buffer.
 *         false : fail to get image buffer.
 */
bool AviWnd::GetImageBuffer(cv::Mat *image_buffer) {
  if (capture_image_ == NULL) {
    DEBUG_PRINT("CInput::getReadBuffer is not read yet. exit function\n");
    return false;
  }
  usleep(fps_wait_);

  *capture_image_ >> *avi_image_;

  /* Last frame check.*/
  if (avi_image_->empty()) {
    wxString wx_log_info_msg(wxT("[Avi]Play completion - Blank frame grabbed"));
    wxLogInfo(wx_log_info_msg.c_str());
    DEBUG_PRINT("Play completion - Blank frame grabbed \n");

    return false;
  } else {
    if(last_avi_image_ == NULL) {
      last_avi_image_ = new cv::Mat(image_buffer->size(), image_buffer->type());
     }
    *last_avi_image_ = avi_image_->clone();

    *image_buffer = avi_image_->clone();
  }
  return true;
}

/**
 * @brief
 * Get AVI image size.
 * @return AVI image size.
 */
CvSize AviWnd::GetImageSize(void) {
  if (capture_image_ == NULL) {
    DEBUG_PRINT("CInput::getReadBuffer is not read yet. exit function\n");
    return cvSize(-1, -1);
  }
  int image_width =
               static_cast<int>(capture_image_->get(CV_CAP_PROP_FRAME_WIDTH));
  int image_height =
               static_cast<int>(capture_image_->get(CV_CAP_PROP_FRAME_HEIGHT));
  return cvSize(image_width, image_height);
}

/**
 * @brief
 *
 * @param state [out] Image processing state
 */
void AviWnd::UpdateUIForImageProcessingState(ImageProcessingState state) {
  DEBUG_PRINT("AviWnd::UpdateUIForImageProcessingState state = %d\n", state);
  switch (state) {
    case kRun:
      wx_button_open_avi_file_->Enable(false);
      wx_button_setting_apply_->Enable(false);
      break;
    case kStop:
      wx_button_open_avi_file_->Enable(true);
      wx_button_setting_apply_->Enable(true);
      break;
    case kPause:
      wx_button_open_avi_file_->Enable(false);
      wx_button_setting_apply_->Enable(false);
      break;
  }
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool AviWnd::LoadSettingsFromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    ret = text_file.Open(file_path);
    if (ret == false) {
      DEBUG_PRINT("Could not open file =%s\n",
                  (const char *)file_path.mb_str());
      return false;
    }
  } else {
    DEBUG_PRINT("File does not exist =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Eof();
  if (ret == true) {
    DEBUG_PRINT("Blank init file\n");
    text_file.Close();
    return false;
  } else {
    line_str = text_file.GetFirstLine();
    wx_text_ctrl_file_path_->SetValue(line_str);
  }

  text_file.Close();

  ret = OpenRawFile(wx_text_ctrl_file_path_->GetValue(), false);
  if (ret == true) {
    wxCommandEvent event =
                  wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, kBtnApplyId);
    ProcessEvent(event);
  }

  return true;
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @return If true, writing the file success
 */
bool AviWnd::WriteSettingsToFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    printf("AviWnd file exist \n");
    ret = text_file.Open(file_path);
    if (ret == false) {
      ret = text_file.Create(file_path);
      if (ret == false) {
        printf("Fail to create file = %s \n",
                    (const char *)file_path.mb_str());
        return false;
      }
    }
  } else {
    printf("AviWnd file not exist \n");
    ret = text_file.Create(file_path);
    if (ret == false) {
      printf("Fail to create file = %s \n",
                  (const char *)file_path.mb_str());
      return false;
    }
  }

  text_file.Clear();
  parent_->ClearPluginSettings();
  line_str = wx_text_ctrl_file_path_->GetValue();
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (parent_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}


/**
 * @brief
 * Set the list of parameter setting string for the Avi plugin.
 * @param params [in] settings string.
 */
void AviWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  bool ret = false;

  line_str = params[0];
  wx_text_ctrl_file_path_->SetValue(line_str);

  ret = OpenRawFile(wx_text_ctrl_file_path_->GetValue(), false);
  if (ret == true) {
    wxCommandEvent event =
                  wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, kBtnApplyId);
    ProcessEvent(event);
  }
}

/**
 * @brief
 * Open Raw file.
 * @param wxPathName [in] Raw file path.
 */
bool AviWnd::OpenRawFile(wxString wxPathName, bool disp_error_dialog) {
  FileReadError file_read_error;
  char cFilePathName[kMaxFilePath];
  memset(cFilePathName, 0, sizeof(cFilePathName));

  /* open raw file*/
  strncpy(cFilePathName, (const char *)wxPathName.mb_str(),
          static_cast<int>(wxPathName.length()));
  file_read_error = LoadImageData(cFilePathName);

  std::string err_msg;
  switch (file_read_error) {
    case kNoneError:
      break;
    case kCouldNotOpenFileError:
      err_msg = "Could not open file";
      break;
    case kHeaderSizeError:
      err_msg = "Incorrect header size";
      break;
    case kInvalidBitCountError:
      err_msg = "Invalid bit count setting";
      break;
    case kAllocError:
      err_msg = "Could not be allocated";
      break;
    case kImageSizeError:
      err_msg = "Invalid image size";
      break;
  }

  /* error process*/
  if (file_read_error != kNoneError) {
    wxString wx_err_msg(err_msg.c_str(), wxConvUTF8);

    wxString wx_log_error_msg(wxT("[AVI]") + wx_err_msg
                              + wxT(" : ") + wxPathName);
    wxLogError(wx_log_error_msg.c_str());

    if (disp_error_dialog == true) {
      wxMessageDialog dialog(NULL, wx_err_msg, wxT("Error"), wxOK,
                             wxPoint(100, 100));
      if (dialog.ShowModal() == wxID_OK) {
      }
    }
    wx_text_ctrl_file_path_->SetValue(wxT(""));
    return false;
  } else {
    /* normal process*/
    wx_text_ctrl_file_path_->SetValue(wxPathName);
  }
  return true;
}
