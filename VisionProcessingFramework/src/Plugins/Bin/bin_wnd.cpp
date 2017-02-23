/**
 * @file      bin_wnd.cpp
 * @brief     Window of Bin plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./bin_wnd.h"
#include <algorithm>
#include <string>
#include <vector>

BEGIN_EVENT_TABLE(BinWnd, wxFrame)
EVT_CLOSE(BinWnd::OnClose)
EVT_RADIOBOX(RBOX_BIT_COUNT_ID, BinWnd::OnSelectBitCount)
EVT_RADIOBOX(RBOX_FIRST_PIXEL_ID, BinWnd::OnSelectFirstPixel)
EVT_BUTTON(BTN_SELECT_RAW_FILE_WND_ID, BinWnd::OnOpenRawFile)
EVT_BUTTON(BTN_APPLY_ID, BinWnd::OnUpdate)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
BinWnd::BinWnd(Bin *parent)
    : wxFrame(NULL, WND_ID, wxT(WND_TITLE), wxPoint(WND_POINT_X, WND_POINT_Y),
              //              wxSize(WND_SIZE_W, WND_SIZE_H)) {
              wxSize(220, 300)) {
  wxString bit_count_choice[2];
  wxString first_pixel_choice[4];

  // Create bit count radio box
  bit_count_choice[0] = wxT("8bit");
  bit_count_choice[1] = wxT("10bit");
  wx_radio_box_bit_count_ = new wxRadioBox(
      this, RBOX_BIT_COUNT_ID, wxT("Bit count"),
      wxPoint(RBOX_BIT_COUNT_POINT_X, RBOX_BIT_COUNT_POINT_Y),
      wxSize(RBOX_BIT_COUNT_SIZE_W, RBOX_BIT_COUNT_SIZE_H), 2, bit_count_choice,
      2, wxRA_SPECIFY_COLS, wxDefaultValidator, wxT(ST_BIT_COUNT_TEXT));
  wx_radio_box_bit_count_->SetSelection(0);
  bit_count_ = SSP_FRAME_BAYER8;

  // Create first pixel radio box
  first_pixel_choice[0] = wxT("R");
  first_pixel_choice[1] = wxT("GB");
  first_pixel_choice[2] = wxT("GR");
  first_pixel_choice[3] = wxT("B");
  wx_radio_box_first_pixel_ = new wxRadioBox(
      this, RBOX_FIRST_PIXEL_ID, wxT("First pixel"),
      wxPoint(RBOX_FIRST_PIXEL_POINT_X, RBOX_FIRST_PIXEL_POINT_Y),
      wxSize(RBOX_FIRST_PIXEL_SIZE_W, RBOX_FIRST_PIXEL_SIZE_H), 4,
      first_pixel_choice, 4, wxRA_SPECIFY_COLS, wxDefaultValidator,
      wxT("First pixel"));
  wx_radio_box_first_pixel_->SetSelection(0);
  first_pixel_ = 0;

  // Todo: Get initialvalue from ini file.

  // Create raw file select button
  wx_button_open_raw_file_ = new wxButton(
      this, BTN_SELECT_RAW_FILE_WND_ID, wxT(BTN_SELECT_RAW_FILE_WND_TEXT),
      wxPoint(BTN_SELECT_RAW_FILE_WND_POINT_X, BTN_SELECT_RAW_FILE_WND_POINT_Y),
      wxSize(BTN_SELECT_RAW_FILE_WND_SIZE_W, BTN_SELECT_RAW_FILE_WND_SIZE_H));

  // Create static text(file path)
  wx_static_text_file_path_ =
      new wxStaticText(this, STATIC_TEXT_RAW_FILE_PATH_ID, wxT("File:"),
                       wxPoint(STATIC_TEXT_RAW_FILE_PATH_POINT_X,
                               STATIC_TEXT_RAW_FILE_PATH_POINT_Y),
                       wxSize(STATIC_TEXT_RAW_FILE_PATH_SIZE_W,
                              STATIC_TEXT_RAW_FILE_PATH_SIZE_H));
  // Create static text(file path)
  wx_text_ctrl_file_path_ = new wxTextCtrl(
      this, TEXT_RAW_FILE_PATH_ID, wxT(""),
      wxPoint(TEXT_RAW_FILE_PATH_POINT_X, TEXT_RAW_FILE_PATH_POINT_Y),
      wxSize(TEXT_RAW_FILE_PATH_SIZE_W, TEXT_RAW_FILE_PATH_SIZE_H),
      wxTE_READONLY);
  // Create apply button
  wx_button_setting_apply_ =
      new wxButton(this, BTN_APPLY_ID, wxT(BTN_APPLY_TEXT),
                   wxPoint(BTN_APPLY_POINT_X, BTN_APPLY_POINT_Y),
                   wxSize(BTN_APPLY_SIZE_W, BTN_APPLY_SIZE_H));

  // Initialize
  parent_ = parent;
  temp_raw_bayer_image_ = NULL;
  raw_bayer_image_ = NULL;

  LoadSettingsFromFile(wxT(BinConfigFile));
}

/**
 * @brief
 * Destructor for this window.
 */
BinWnd::~BinWnd() {
  if (temp_raw_bayer_image_ != NULL) {
    delete temp_raw_bayer_image_;
    temp_raw_bayer_image_ = NULL;
  }

  if (raw_bayer_image_ != NULL) {
    delete raw_bayer_image_;
    raw_bayer_image_ = NULL;
  }
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void BinWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * The handler function for radio box(id = RBOX_BIT_COUNT_ID).
 * Select bit count.
 */
void BinWnd::OnSelectBitCount(wxCommandEvent &event) {
  // Nothing to do.
}

/**
 * @brief
 * The handler function for RBOX_FIRST_PIXEL_ID.
 * Select bit count.
 */
void BinWnd::OnSelectFirstPixel(wxCommandEvent &event) {
  first_pixel_ = wx_radio_box_first_pixel_->GetSelection();
}

/**
 * @brief
 * The handler function for radio box(id = RBOX_FIRST_PIXEL_ID).
 * Select bit count.
 */
void BinWnd::OnOpenRawFile(wxCommandEvent &event) {
  wxFileDialog *OpenDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
      wxT("*.raw"), wxFD_OPEN, wxDefaultPosition);

  /* Creates a "open file" dialog*/
  if (OpenDialog->ShowModal() == wxID_OK) {
    OpenRawFile(OpenDialog->GetPath(), true);
  }
}

/**
 * @brief
 * The handler function for button(id = BTN_APPLY_ID).
 * Apply selected Raw image.
 */
void BinWnd::OnUpdate(wxCommandEvent &event) {
  int current_bit_count;
  bool ret = true;

  if (!temp_raw_bayer_image_) {
    return;
  }

  if (wx_radio_box_bit_count_->GetSelection() == 0) {
    current_bit_count = SSP_FRAME_BAYER8;
  } else {
    current_bit_count = SSP_FRAME_BAYER16;
  }

  if (temp_raw_bayer_image_ && bit_count_ != current_bit_count) {
    ret = OpenRawFile(wx_text_ctrl_file_path_->GetValue(), true);
  }

  if (ret == true) {
    if (bit_count_ == SSP_FRAME_BAYER8) {
      parent_->set_optical_black(16);
      parent_->set_active_output_port_spec_index(0);
    } else if (bit_count_ == SSP_FRAME_BAYER16) {
      parent_->set_optical_black(64);
      parent_->set_active_output_port_spec_index(1);
    }
    CvSize size = cvSize(temp_raw_bayer_image_->size().width,
                         temp_raw_bayer_image_->size().height);
    parent_->set_output_image_size(size);

    if (raw_bayer_image_ != NULL) {
      delete raw_bayer_image_;
    }
    raw_bayer_image_ = new cv::Mat(cvSize(temp_raw_bayer_image_->size().width,
                                          temp_raw_bayer_image_->size().height),
                                   temp_raw_bayer_image_->type());

    *raw_bayer_image_ = temp_raw_bayer_image_->clone();
    parent_->set_first_pixel(first_pixel_);

    this->Show(false);
    WriteSettingsToFile(wxT(BinConfigFile));
  }
}

/**
 * @brief
 * Read RAW file and generate image buffer.
 * @param image_file_path [in] RAW image file path
 */
FileReadError BinWnd::LoadImageData(char *image_file_path) {
  FILE *fp;
  char *image_data_buffer;
  WORD image_header_buffer[BIN_HEADER_WORD_SIZE];
  unsigned int image_width, image_height;
  DWORD image_size;
  int size_of_one_pixel_ = -1;

  DEBUG_PRINT("Bin::OpenFile str=%s\n", image_file_path);

  if (wx_radio_box_bit_count_->GetSelection() == 0) {
    bit_count_ = SSP_FRAME_BAYER8;
  } else {
    bit_count_ = SSP_FRAME_BAYER16;
  }

  /* If Image data is already allocated, release Image data.*/
  if (temp_raw_bayer_image_ != NULL) {
    delete temp_raw_bayer_image_;
    temp_raw_bayer_image_ = NULL;
  }

  /* Open RAW file*/
  fp = fopen((const char *)image_file_path, (const char *)"rb");
  if (fp == NULL) {
    DEBUG_PRINT("Bin::OpenFile could not open file\n");
    return kCouldNotOpenFileError;
  }

  /* Read file header(width : 2byte, height : 2byte)*/
  if (BIN_HEADER_WORD_SIZE !=
      fread(reinterpret_cast<void *>(image_header_buffer), sizeof(WORD),
            BIN_HEADER_WORD_SIZE, fp)) {
    fclose(fp);
    return kHeaderSizeError;
  }
  image_width =
      static_cast<unsigned int>(image_header_buffer[BIN_HEADER_WIDTH_POS]);
  image_height =
      static_cast<unsigned int>(image_header_buffer[BIN_HEADER_HEIGHT_POS]);
  DEBUG_PRINT("Bin::OpenFile width=%d , height = %d\n", image_width,
              image_height);

  /* Read file body*/
  image_size = image_width * image_height;
  if (bit_count_ == SSP_FRAME_BAYER8) {
    size_of_one_pixel_ = sizeof(char);
  } else if (bit_count_ == SSP_FRAME_BAYER16) {
    size_of_one_pixel_ = sizeof(WORD);
  } else {
    DEBUG_PRINT("Invalid bit count setting\n");
    return kInvalidBitCountError;
  }

  image_data_buffer =
      reinterpret_cast<char *>(malloc(size_of_one_pixel_ * image_size));

  if (!image_data_buffer) {
    fclose(fp);
    // return false;
    return kAllocError;
  }

  if (image_size != fread(reinterpret_cast<void *>(image_data_buffer),
                          size_of_one_pixel_, image_size, fp)) {
    free(image_data_buffer);
    fclose(fp);
    return kImageSizeError;
  }
  fclose(fp);

  /* Create Raw Bayer Image*/
  DEBUG_PRINT("Create Raw Bayer Img\n");

  if (bit_count_ == SSP_FRAME_BAYER8) {
    temp_raw_bayer_image_ =
        new cv::Mat(cvSize(image_width, image_height), CV_8UC1);
  } else if (bit_count_ == SSP_FRAME_BAYER16) {
    temp_raw_bayer_image_ =
        new cv::Mat(cvSize(image_width, image_height), CV_16UC1);
  }
  if (!temp_raw_bayer_image_) {
    free(image_data_buffer);
    return kAllocError;
  }

  if (bit_count_ == SSP_FRAME_BAYER8) {
    DEBUG_PRINT("BayerImg->step=%d\n",
                temp_raw_bayer_image_->rows * temp_raw_bayer_image_->cols);
    for (unsigned int height_cnt = 0; height_cnt < image_height; height_cnt++) {
      for (unsigned int width_cnt = 0; width_cnt < image_width; width_cnt++) {
        ((unsigned char *)(temp_raw_bayer_image_->data +
                           temp_raw_bayer_image_->step *
                               height_cnt))[width_cnt] =
            image_data_buffer[height_cnt * temp_raw_bayer_image_->step +
                              width_cnt];
      }
    }
  } else if (bit_count_ == SSP_FRAME_BAYER16) {
    WORD *temp_image_data_buffer = reinterpret_cast<WORD *>(image_data_buffer);
    temp_raw_bayer_image_ =
        new cv::Mat(cvSize(image_width, image_height), CV_16UC1);

    DEBUG_PRINT("BayerImg->step=%d\n",
                temp_raw_bayer_image_->rows * temp_raw_bayer_image_->cols);
    for (unsigned int height_cnt = 0; height_cnt < image_height; height_cnt++) {
      for (unsigned int width_cnt = 0; width_cnt < image_width; width_cnt++) {
        ((unsigned char *)(temp_raw_bayer_image_->data +
                           temp_raw_bayer_image_->step *
                               height_cnt))[width_cnt * 2] =
            (uchar)(
                temp_image_data_buffer[height_cnt * image_width + width_cnt] &
                0x00FF);
        ((unsigned char *)(temp_raw_bayer_image_->data +
                           temp_raw_bayer_image_->step *
                               height_cnt))[width_cnt * 2 + 1] =
            (uchar)(
                temp_image_data_buffer[height_cnt * image_width + width_cnt] >>
                8);
      }
    }
  }
  free(image_data_buffer);
  return kNoneError;
}

/**
 * @brief
 * Get RAW image burrer.
 * @param image_buffer [out] RAW image buffer.
 * @return true : success to get a RAW image buffer.
 *         false : fail to get a RAW image buffer.
 */
bool BinWnd::GetImageBuffer(cv::Mat *image_buffer) {
  if (raw_bayer_image_ == NULL) {
    DEBUG_PRINT("CInput::getReadBuffer is not read yet. exit function\n");
    return false;
  }

  *image_buffer = raw_bayer_image_->clone();

  return true;
}

/**
 * @brief
 * Get RAW image size.
 * @return a RAW image size.
 */
CvSize BinWnd::GetImageSize(void) {
  if (raw_bayer_image_ == NULL) {
    DEBUG_PRINT("CInput::getReadBuffer is not read yet. exit function\n");
    return cvSize(-1, -1);
  }
  return raw_bayer_image_->size();
}

/**
 * @brief
 * Update the setting window UI to active or inactive
 * by image processing state.
 * @param state [in] ImageProcessingState
 */
void BinWnd::UpdateUIForImageProcessingState(ImageProcessingState state) {
  DEBUG_PRINT("BinWnd::UpdateUIForImageProcessingState state = %d\n", state);
  switch (state) {
    case kRun:
      wx_radio_box_first_pixel_->Enable(false);
      wx_radio_box_bit_count_->Enable(false);
      wx_button_open_raw_file_->Enable(false);
      wx_button_setting_apply_->Enable(false);
      break;
    case kStop:
      wx_radio_box_first_pixel_->Enable(true);
      wx_radio_box_bit_count_->Enable(true);
      wx_button_open_raw_file_->Enable(true);
      wx_button_setting_apply_->Enable(true);
      break;
    case kPause:
      wx_radio_box_first_pixel_->Enable(false);
      wx_radio_box_bit_count_->Enable(false);
      wx_button_open_raw_file_->Enable(false);
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
bool BinWnd::LoadSettingsFromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;
  long temp_value; /* NOLINT */

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
    line_str.ToLong(&temp_value);
    bit_count_ = static_cast<int>(temp_value);
    if (bit_count_ == SSP_FRAME_BAYER8) {
      wx_radio_box_bit_count_->SetSelection(0);
    } else {
      wx_radio_box_bit_count_->SetSelection(1);
    }

    line_str = text_file.GetNextLine();
    line_str.ToLong(&temp_value);
    first_pixel_ = static_cast<int>(temp_value);
    wx_radio_box_first_pixel_->SetSelection(first_pixel_);

    line_str = text_file.GetNextLine();
    wx_text_ctrl_file_path_->SetValue(line_str);
  }

  text_file.Close();

  ret = OpenRawFile(wx_text_ctrl_file_path_->GetValue(), false);
  if (ret == true) {
    wxCommandEvent event =
        wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, BTN_APPLY_ID);
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
bool BinWnd::WriteSettingsToFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    printf("BinWnd file exist \n");
    ret = text_file.Open(file_path);
    if (ret == false) {
      ret = text_file.Create(file_path);
      if (ret == false) {
        printf("Fail to create file = %s \n", (const char *)file_path.mb_str());
        return false;
      }
    }
  } else {
    printf("BinWnd file not exist \n");
    ret = text_file.Create(file_path);
    if (ret == false) {
      printf("Fail to create file = %s \n", (const char *)file_path.mb_str());
      return false;
    }
  }

  text_file.Clear();
  parent_->ClearPluginSettings();

  // Bit count -> bit_count_
  line_str = wxString::Format(wxT("%d"), bit_count_);
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  // First pixel -> first_pixel_
  line_str = wxString::Format(wxT("%d"), first_pixel_);
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  // File path ->wx_text_ctrl_file_path_->GetValue()
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
 * Set the list of parameter setting string for the Bin plugin.
 * @param params [in] settings string.
 */
void BinWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  bool ret = false;
  long temp_value; /* NOLINT */

  line_str = params[0];
  line_str.ToLong(&temp_value);
  bit_count_ = static_cast<int>(temp_value);
  if (bit_count_ == SSP_FRAME_BAYER8) {
    wx_radio_box_bit_count_->SetSelection(0);
  } else {
    wx_radio_box_bit_count_->SetSelection(1);
  }

  line_str = params[1];
  line_str.ToLong(&temp_value);
  first_pixel_ = static_cast<int>(temp_value);
  wx_radio_box_first_pixel_->SetSelection(first_pixel_);

  line_str = params[2];
  wx_text_ctrl_file_path_->SetValue(line_str);

  ret = OpenRawFile(wx_text_ctrl_file_path_->GetValue(), false);
  if (ret == true) {
    wxCommandEvent event =
        wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, BTN_APPLY_ID);
    ProcessEvent(event);
  }
}

/**
 * @brief
 * Open Raw file.
 * @param wxPathName [in] Raw file path.
 * @param disp_error_dialog [in]
 *     true  : if file can not open, show error dialog
 *     false : if file can not open, do not show any dialog.
 */
bool BinWnd::OpenRawFile(wxString wxPathName, bool disp_error_dialog) {
  FileReadError file_read_error;
  char cFilePathName[MAX_FILE_PATH];
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

  if (file_read_error != kNoneError) {  // error process
    wxString wx_err_msg(err_msg.c_str(), wxConvUTF8);

    wxString wx_log_error_msg(wxT("[Bin]") + wx_err_msg + wxT(" : ") +
                              wxPathName);
    wxLogError(wx_log_error_msg.c_str());

    if (disp_error_dialog == true) {
      wxMessageDialog dialog(NULL, wx_err_msg, wxT("Error"), wxOK,
                             wxPoint(100, 100));
      if (dialog.ShowModal() == wxID_OK) {
      }
    }
    wx_text_ctrl_file_path_->SetValue(wxT(""));
    return false;
  } else {  // normal process
    wx_text_ctrl_file_path_->SetValue(wxPathName);
  }
  return true;
}
