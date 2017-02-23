/**
 * @file      gamma_correct_wnd.cpp
 * @brief     Window of GammaCorrect plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <algorithm>
#include <vector>
#include <string>
#include "./gamma_correct_wnd.h"

BEGIN_EVENT_TABLE(GammaCorrectWnd, wxFrame)
EVT_CLOSE(GammaCorrectWnd::OnClose)
EVT_BUTTON(kButtonApplyId, GammaCorrectWnd::OnApply)
EVT_RADIOBUTTON(kRadioButtonFuncId, GammaCorrectWnd::OnRadioButtonFunction)
EVT_RADIOBUTTON(kRadioButtonTableId, GammaCorrectWnd::OnRadioButtonTable)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
GammaCorrectWnd::GammaCorrectWnd(GammaCorrect *gamma_correct)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  gamma_correct_ = gamma_correct;

  // default setting
  //gamma_value_ = 1.0f;
  gamma_value_ = 2.0f;
  selected_mode_ = kFunctionMode;
  gamma_table_path_ = wxT("");

  gamma_correct_->set_is_use_gamma_table(false);

  static_box_ = new wxStaticBox(this, kNoneId, wxT(kStaticBoxName),
                                wxPoint(kStaticBoxPointX, kStaticBoxPointY),
                                wxSize(kStaticBoxSizeX, kStaticBoxSizeY));
  radio_button_function_ =
      new wxRadioButton(this, kRadioButtonFuncId, wxT(kRadioButtonFuncName),
                        wxPoint(kRadioButtonFuncPointX, kRadioButtonFuncPointY),
                        wxSize(kRadioButtonFuncSizeX, kRadioButtonFuncSizeY));
  text_ctrl_function_ = new wxTextCtrl(
      this, kTextCtrlFuncId, wxString::Format(wxT("%.3f"), gamma_value_),
      wxPoint(kTextCtrlGammaFuncPointX, kTextCtrlGammaFuncPointY),
      wxSize(kTextCtrlGammaFuncSizeX, kTextCtrlGammaFuncSizeY));
  radio_button_table_ = new wxRadioButton(
      this, kRadioButtonTableId, wxT(kRadioButtonTableName),
      wxPoint(kRadioButtonTablePointX, kRadioButtonTablePointY),
      wxSize(kRadioButtonTableSizeX, kRadioButtonTableSizeY));
  button_apply_ = new wxButton(this, kButtonApplyId, wxT(kButtonApllyName),
                               wxPoint(kButtonApllyPointX, kButtonApllyPointY),
                               wxSize(kButtonApllySizeX, kButtonApllySizeY));

  if (selected_mode_ == kFunctionMode) {
    radio_button_function_->SetValue(true);
    radio_button_table_->SetValue(false);
  } else {
    radio_button_function_->SetValue(false);
    radio_button_table_->SetValue(true);
  }

  LoadSettingsFromFile(wxT(GammaCorrectConfigFile));
  CreateGammaTable(gamma_value_);
}

/**
 * @brief
 * Destructor for this window.
 */
GammaCorrectWnd::~GammaCorrectWnd() {}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool GammaCorrectWnd::LoadSettingsFromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;
  double temp_double_value;
  long temp_long_value; /* NOLINT */

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
    // gamma_value
    line_str = text_file.GetFirstLine();
    line_str.ToDouble(&temp_double_value);
    gamma_value_ = static_cast<float>(temp_double_value);
    text_ctrl_function_->SetValue(wxString::Format(wxT("%.3f"), gamma_value_));

    // select mode
    line_str = text_file.GetNextLine();
    line_str.ToLong(&temp_long_value);
    if (static_cast<int>(temp_long_value) == kFunctionMode) {
      selected_mode_ = kFunctionMode;
      radio_button_function_->SetValue(true);
      radio_button_table_->SetValue(false);
    } else {
      selected_mode_ = kTableMode;
      radio_button_function_->SetValue(false);
      radio_button_table_->SetValue(true);
    }

    // gamma table file path
    gamma_table_path_ = text_file.GetNextLine();
  }
  text_file.Close();
  return true;
}

/**
 * @brief
 * The handler function for EVT_RADIO_BUTTON_FUNC.
 */
void GammaCorrectWnd::OnRadioButtonFunction(wxCommandEvent &event) {
  selected_mode_ = kFunctionMode;
  gamma_correct_->set_is_use_gamma_table(false);
}

/**
 * @brief
 * The handler function for EVT_RADIO_BUTTON_TABLE.
 */
void GammaCorrectWnd::OnRadioButtonTable(wxCommandEvent &event) {
  wxFileDialog *OpenDialog =
      new wxFileDialog(this, _(kFileDialogName), wxEmptyString, wxEmptyString,
                       wxT(kFileDialogType), wxFD_OPEN, wxDefaultPosition);

  // Creates a "open file" dialog with 4 file types
  if (OpenDialog->ShowModal() == wxID_OK) {
    // if the user click "Open" instead of "Cancel"
    gamma_correct_->set_is_use_gamma_table(true);
    gamma_table_path_ = OpenDialog->GetPath();
    selected_mode_ = kTableMode;
  } else {  // cancel
    gamma_correct_->set_create_gamma_table(false);
    gamma_correct_->set_is_use_gamma_table(false);
    radio_button_function_->SetValue(true);
    radio_button_table_->SetValue(false);
    selected_mode_ = kFunctionMode;
  }

  // Clean up after ourselves
  OpenDialog->Destroy();
}

/**
 * @brief
 * The handler function for EVT_APPLY.
 */
void GammaCorrectWnd::OnApply(wxCommandEvent &event) {
  /* Function */
  if (gamma_correct_->is_use_gamma_table() == false) {
    DEBUG_PRINT("GammaCorrectWnd::OnApply function mode \n");
    double gamma_func_value;
    text_ctrl_function_->GetValue().ToDouble(&gamma_func_value);
    gamma_value_ = static_cast<float>(gamma_func_value);

    /* Check and create gamma table */
    if (gamma_value_ < MIN_GAMMA_FUNC_VALUE ||
        MAX_GAMMA_FUNC_VALUE < gamma_value_) {
      gamma_value_ = 1.0f;
      text_ctrl_function_->SetValue(
          wxString::Format(wxT("%.3f"), gamma_value_));
      wxMessageDialog dialog(NULL, wxT("value range : 0.0 <= value <=50.0"),
                             wxT("Error"), wxOK, wxPoint(100, 100));
      if (dialog.ShowModal() == wxID_OK) {
      }
      return;
    } else {
      CreateGammaTable(gamma_value_);
    }
  } else { /* Table */
    DEBUG_PRINT("GammaCorrectWnd::OnApply table mode \n");
    wxTextFile gamma_table_file;
    // If error detect, then change table mode to function mode.
    if (!IsExistTableFile()) {
      wxMessageDialog dialog(NULL, wxT("Not exist gamma table file."));
      if (dialog.ShowModal() == wxID_OK) {
      }
      selected_mode_ = kFunctionMode;
      radio_button_function_->SetValue(true);
      radio_button_table_->SetValue(false);
      return;
    }

    std::string err_msg;
    int error_code = CheckGammaTableFile(&gamma_table_file);
    if (error_code != kReadableTableFile) {
      switch (error_code) {
        case kOpenError:
          err_msg = "Could not open a table file";
          break;
        case kEofError:
          err_msg = "End of file a table file";
          break;
        case kInvalidLineError:
          err_msg = "Invalid line count";
          break;
        case kInvalidTokenCountError:
          err_msg = "Detect invalid token count";
          break;
      }
      wxString wx_err_msg(err_msg.c_str(), wxConvUTF8);
      wxMessageDialog dialog(NULL, wx_err_msg, wxT("Error"), wxOK,
                             wxPoint(100, 100));
      if (dialog.ShowModal() == wxID_OK) {
      }
      selected_mode_ = kFunctionMode;
      gamma_table_path_ = wxT("");
      radio_button_function_->SetValue(true);
      radio_button_table_->SetValue(false);
      return;
    }
  }

  WriteSettingsToFile(wxT(GammaCorrectConfigFile));
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @return If true, writing the file success
 */
bool GammaCorrectWnd::WriteSettingsToFile(wxString file_path) {
  DEBUG_PRINT(
      "GammaCorrectWnd::GammaCorrectWnd::WriteGammaSettingstoFile start\n");
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    ret = text_file.Open(file_path);
    if (ret == false) {
      ret = text_file.Create(file_path);
      if (ret == false) {
        DEBUG_PRINT("Fail to create file = %s \n",
                    (const char *)file_path.mb_str());
        return false;
      }
    }
  } else {
    ret = text_file.Create(file_path);
    if (ret == false) {
      DEBUG_PRINT("Fail to create file = %s \n",
                  (const char *)file_path.mb_str());
      return false;
    }
  }

  text_file.Clear();
  gamma_correct_->ClearPluginSettings();

  line_str = wxString::Format(wxT("%.3f"), gamma_value_);
  gamma_correct_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  line_str = wxString::Format(wxT("%d"), selected_mode_);
  gamma_correct_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  line_str = gamma_table_path_;
  gamma_correct_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (gamma_correct_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}

/**
 * @brief
 * Set the list of parameter setting string for the GammaCorrect plugin.
 * @param params [in] settings string.
 */
void GammaCorrectWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  double temp_double_value;
  long temp_long_value; /* NOLINT */

  // gamma value
  line_str = params[0];
  line_str.ToDouble(&temp_double_value);
    gamma_value_ = static_cast<float>(temp_double_value);
  text_ctrl_function_->SetValue(wxString::Format(wxT("%.3f"), gamma_value_));

  // select mode
  line_str = params[1];
  line_str.ToLong(&temp_long_value);
  if (static_cast<int>(temp_long_value) == kFunctionMode) {
    selected_mode_ = kFunctionMode;
    radio_button_function_->SetValue(true);
    radio_button_table_->SetValue(false);
  } else {
    selected_mode_ = kTableMode;
    radio_button_function_->SetValue(false);
    radio_button_table_->SetValue(true);
  }

  // gamma table file path
  if (params.size() > 2) {
    gamma_table_path_ = params[2];
  }

  WriteSettingsToFile(wxT(GammaCorrectConfigFile));
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void GammaCorrectWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Create the gamma lut table(8/10bit)
 * @param value [in] gamma value.
 */
void GammaCorrectWnd::CreateGammaTable(float value) {
  // Create gammma table of 8bit and 10bit from the input value.
  for (int i = 0; i < k8BitTableRow; i++) {
    gamma_correct_->gamma_lut8[i] = static_cast<int>(
        pow(static_cast<UINT16>(i) / 255.0, 1.0 / value) * 255.0);
  }
  for (int i = 0; i < k10BitTableRow; i++) {
    gamma_correct_->gamma_lut10[i] = static_cast<int>(
        pow(static_cast<UINT16>(i) / 1023.0, 1.0 / value) * 1023.0);
  }
}

/**
 * @brief
 * Check that the gamma table file can be read.
 * @param gamma_table_file [in] text file read from a gamma table file.
 * @return GammaTableError
 */
GammaTableError GammaCorrectWnd::CheckGammaTableFile(
    wxTextFile *gamma_table_file) {
  // check available file open
  bool return_value = gamma_table_file->Open(gamma_table_path_);
  if (return_value == false) {
    DEBUG_PRINT("Not open gamma_table_file.\n");
    return kOpenError;
  }

  // check End of file
  bool bEof = gamma_table_file->Eof();
  if (bEof == true) {
    gamma_table_file->Close();
    DEBUG_PRINT("gamma_table_file.Eof\n");
    return kEofError;
  }

  // check invalid line count
  if (gamma_table_file->GetLineCount() != k8BitTableFileRow &&
      gamma_table_file->GetLineCount() != k10BitTableFileRow) {
    char imgFilePath[128];
    memset(imgFilePath, 0, sizeof(imgFilePath));
    strncpy(imgFilePath, (const char *)gamma_table_path_.mb_str(),
            static_cast<int>(gamma_table_path_.length()));
    DEBUG_PRINT("Invalid line count [file = %s, line count = %d]\n",
                imgFilePath, gamma_table_file->GetLineCount());
    gamma_table_file->Close();
    return kInvalidLineError;
  }

  bool is_10bit_table_file = false;
  if (gamma_table_file->GetLineCount() == k10BitTableFileRow) {
    is_10bit_table_file = true;
    gamma_correct_->set_is_use_10bit_lut(true);
  } else {
    gamma_correct_->set_is_use_10bit_lut(false);
  }

  // check invalide token count
  wxStringTokenizer wxTokenizer;
  wxString wxTokenStr, wxLineStr;
  int tokenCnt;
  int line_cnt = 0;
  long gamma_table_value; /* NOLINT */

  for (wxLineStr = gamma_table_file->GetFirstLine(); !gamma_table_file->Eof();
       wxLineStr = gamma_table_file->GetNextLine(), line_cnt++) {
    // read from the third line(= skip line count 0 and 1)
    if (line_cnt > 1) {
      wxTokenizer.SetString(wxLineStr, wxT(","), wxTOKEN_DEFAULT);
      tokenCnt = wxTokenizer.CountTokens();

      if (tokenCnt != 2) {
        DEBUG_PRINT(
            "Invalid token count [file = specialMatrix.csv, line = %d, token "
            "count = %d]\n",
            gamma_table_file->GetCurrentLine(), tokenCnt);
        gamma_table_file->Close();
        DEBUG_PRINT("Invalid token count line = %d\n",
                    gamma_table_file->GetCurrentLine());
        gamma_correct_->reset_table();
        return kInvalidTokenCountError;
      }
      wxTokenizer.GetNextToken();               // bit
      wxTokenStr = wxTokenizer.GetNextToken();  // val
      wxTokenStr.ToLong(&gamma_table_value);
      if (is_10bit_table_file == false) {  // 8bit table
        gamma_correct_->temp_gamma_lut8[line_cnt - 2] =
            static_cast<UINT16>(gamma_table_value);
        DEBUG_PRINT("lut8 line = %d, value = %d\n", line_cnt,
                    gamma_correct_->temp_gamma_lut8[line_cnt - 2]);
      } else {  // 10bit table
        gamma_correct_->temp_gamma_lut10[line_cnt - 2] =
            static_cast<UINT16>(gamma_table_value);
        DEBUG_PRINT("lut10 line = %d, value = %d\n", line_cnt,
                    gamma_correct_->temp_gamma_lut10[line_cnt - 2]);
      }
    }
  }
  gamma_table_file->Close();
  gamma_correct_->set_create_gamma_table(true);
  gamma_correct_->set_is_use_gamma_table(true);
  return kReadableTableFile;
}

/**
 * @brief
 * Get the selected mode.
 * @return select mode(func or table)
 */
int GammaCorrectWnd::SelectMode() { return selected_mode_; }

/**
 * @brief
 * Whether the table file exists.
 * @return If true, table file exists.
 */
bool GammaCorrectWnd::IsExistTableFile() {
  bool ret = wxFile::Exists(gamma_table_path_);
  if (ret == true) {
    return true;
  } else {
    DEBUG_PRINT("File does not exist =%s\n",
                (const char *)gamma_table_path_.mb_str());
    return false;
  }
}
