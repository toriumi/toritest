/**
 * @file      colormatrix_wnd.cpp
 * @brief     Window of ColorMatrix plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <vector>
#include <algorithm>
#include "./colormatrix_wnd.h"
#include "./colormatrix.h"

BEGIN_EVENT_TABLE(ColorMatrixWnd, wxFrame)
EVT_CLOSE(ColorMatrixWnd::OnClose)
EVT_BUTTON(kButtonFileDialogId, ColorMatrixWnd::OnOpenFileDialog)
EVT_BUTTON(kButtonDefaultId, ColorMatrixWnd::OnDefault)
EVT_BUTTON(kButtonApplyId, ColorMatrixWnd::OnApply)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
ColorMatrixWnd::ColorMatrixWnd(ColorMatrix *parent)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  // Initialize
  color_matrix_[0][0] = 1;
  color_matrix_[0][1] = 0;
  color_matrix_[0][2] = 0;
  color_matrix_[1][0] = 0;
  color_matrix_[1][1] = 1;
  color_matrix_[1][2] = 0;
  color_matrix_[2][0] = 0;
  color_matrix_[2][1] = 0;
  color_matrix_[2][2] = 1;

  temp_color_matrix_[0][0] = 1;
  temp_color_matrix_[0][1] = 0;
  temp_color_matrix_[0][2] = 0;
  temp_color_matrix_[1][0] = 0;
  temp_color_matrix_[1][1] = 1;
  temp_color_matrix_[1][2] = 0;
  temp_color_matrix_[2][0] = 0;
  temp_color_matrix_[2][1] = 0;
  temp_color_matrix_[2][2] = 1;

  parent_ = parent;
  special_flg = false;

  if (LoadSettingsFromFile(wxT(ColorMatrixConfigFile), temp_color_matrix_) ==
      false) {
    WriteSettingsToFile(wxT(ColorMatrixConfigFile), temp_color_matrix_);
  }
  memcpy(color_matrix_, temp_color_matrix_, sizeof(float) * 3 * 3);
  parent_->set_color_matrix(color_matrix_);

  static_text_r1_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextR1Name),
                       wxPoint(kStaticTextR1PointX, kStaticTextR1PointY),
                       wxSize(kStaticTextR1SizeX, kStaticTextR1SizeY));
  text_ctrl_color_matrix_1st_line_1_ = new wxTextCtrl(
      this, kTextCtrlMatrix1st1Id,
      wxString::Format(wxT("%.3f"), color_matrix_[0][0]),
      wxPoint(kTextCtrlMatrix1st1PointX, kTextCtrlMatrix1st1PointY),
      wxSize(kTextCtrlMatrix1st1SizeX, kTextCtrlMatrix1st1SizeY));
  text_ctrl_color_matrix_1st_line_2_ = new wxTextCtrl(
      this, kTextCtrlMatrix1st2Id,
      wxString::Format(wxT("%.3f"), color_matrix_[0][1]),
      wxPoint(kTextCtrlMatrix1st2PointX, kTextCtrlMatrix1st2PointY),
      wxSize(kTextCtrlMatrix1st2SizeX, kTextCtrlMatrix1st2SizeY));
  text_ctrl_color_matrix_1st_line_3_ = new wxTextCtrl(
      this, kTextCtrlMatrix1st3Id,
      wxString::Format(wxT("%.3f"), color_matrix_[0][2]),
      wxPoint(kTextCtrlMatrix1st3PointX, kTextCtrlMatrix1st3PointY),
      wxSize(kTextCtrlMatrix1st3SizeX, kTextCtrlMatrix1st3SizeY));
  static_text_r2_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextR2Name),
                       wxPoint(kStaticTextR2PointX, kStaticTextR2PointY),
                       wxSize(kStaticTextR2SizeX, kStaticTextR2SizeY));

  static_text_g1_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextG1Name),
                       wxPoint(kStaticTextG1PointX, kStaticTextG1PointY),
                       wxSize(kStaticTextG1SizeX, kStaticTextG1SizeY));
  text_ctrl_color_matrix_2nd_line_1_ = new wxTextCtrl(
      this, kTextCtrlMatrix2nd1Id,
      wxString::Format(wxT("%.3f"), color_matrix_[1][0]),
      wxPoint(kTextCtrlMatrix2nd1PointX, kTextCtrlMatrix2nd1PointY),
      wxSize(kTextCtrlMatrix2nd1SizeX, kTextCtrlMatrix2nd1SizeY));
  text_ctrl_color_matrix_2nd_line_2_ = new wxTextCtrl(
      this, kTextCtrlMatrix2nd2Id,
      wxString::Format(wxT("%.3f"), color_matrix_[1][1]),
      wxPoint(kTextCtrlMatrix2nd2PointX, kTextCtrlMatrix2nd2PointY),
      wxSize(kTextCtrlMatrix2nd2SizeX, kTextCtrlMatrix2nd2SizeY));
  text_ctrl_color_matrix_2nd_line_3_ = new wxTextCtrl(
      this, kTextCtrlMatrix2nd3Id,
      wxString::Format(wxT("%.3f"), color_matrix_[1][2]),
      wxPoint(kTextCtrlMatrix2nd3PointX, kTextCtrlMatrix2nd3PointY),
      wxSize(kTextCtrlMatrix2nd3SizeX, kTextCtrlMatrix2nd3SizeY));
  static_text_g2_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextG2Name),
                       wxPoint(kStaticTextG2PointX, kStaticTextG2PointY),
                       wxSize(kStaticTextG2SizeX, kStaticTextG2SizeY));

  static_text_b1_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextB1Name),
                       wxPoint(kStaticTextB1PointX, kStaticTextB1PointY),
                       wxSize(kStaticTextB1SizeX, kStaticTextB1SizeY));
  text_ctrl_color_matrix_3rd_line_1_ = new wxTextCtrl(
      this, kTextCtrlMatrix3rd1Id,
      wxString::Format(wxT("%.3f"), color_matrix_[2][0]),
      wxPoint(kTextCtrlMatrix3rd1PointX, kTextCtrlMatrix3rd1PointY),
      wxSize(kTextCtrlMatrix3rd1SizeX, kTextCtrlMatrix3rd1SizeY));
  text_ctrl_color_matrix_3rd_line_2_ = new wxTextCtrl(
      this, kTextCtrlMatrix3rd2Id,
      wxString::Format(wxT("%.3f"), color_matrix_[2][1]),
      wxPoint(kTextCtrlMatrix3rd2PointX, kTextCtrlMatrix3rd2PointY),
      wxSize(kTextCtrlMatrix3rd2SizeX, kTextCtrlMatrix3rd2SizeY));
  text_ctrl_color_matrix_3rd_line_3_ = new wxTextCtrl(
      this, kTextCtrlMatrix3rd3Id,
      wxString::Format(wxT("%.3f"), color_matrix_[2][2]),
      wxPoint(kTextCtrlMatrix3rd3PointX, kTextCtrlMatrix3rd3PointY),
      wxSize(kTextCtrlMatrix3rd3SizeX, kTextCtrlMatrix3rd3SizeY));
  static_text_b2_ =
      new wxStaticText(this, kNoneId, wxT(kStaticTextB2Name),
                       wxPoint(kStaticTextB2PointX, kStaticTextB2PointY),
                       wxSize(kStaticTextB2SizeX, kStaticTextB2SizeY));
  button_file_dialog_ =
      new wxButton(this, kButtonFileDialogId, wxT(kButtonFileDialogName),
                   wxPoint(kButtonFileDialogPointX, kButtonFileDialogPointY),
                   wxSize(kButtonFileDialogSizeX, kButtonFileDialogSizeY));
  button_default_ =
      new wxButton(this, kButtonDefaultId, wxT(kButtonDefaultName),
                   wxPoint(kButtonDefaultPointX, kButtonDefaultPointY),
                   wxSize(kButtonDefaultSizeX, kButtonDefaultSizeY));
  button_apply_ = new wxButton(this, kButtonApplyId, wxT(kButtonApplyName),
                               wxPoint(kButtonApplyPointX, kButtonApplyPointY),
                               wxSize(kButtonApplySizeX, kButtonApplySizeY));
}

/**
 * @brief
 * Destructor for this window.
 */
ColorMatrixWnd::~ColorMatrixWnd() {}

/**
 * @brief
 * The handler function for button(id = kButtonApplyId).
 */
void ColorMatrixWnd::OnApply(wxCommandEvent &event) {
  DEBUG_PRINT("ColorMatrixWnd::OnApply \n");
  ApplyCurrentValue();
}

/**
 * @brief
 * The handler function for button(id = kButtonDefaultId).
 */
void ColorMatrixWnd::OnDefault(wxCommandEvent &event) {
  special_flg = false;
  /* set default param */
  text_ctrl_color_matrix_1st_line_1_->SetValue(wxT("1.000"));
  text_ctrl_color_matrix_1st_line_2_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_1st_line_3_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_2nd_line_1_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_2nd_line_2_->SetValue(wxT("1.000"));
  text_ctrl_color_matrix_2nd_line_3_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_3rd_line_1_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_3rd_line_2_->SetValue(wxT("0.000"));
  text_ctrl_color_matrix_3rd_line_3_->SetValue(wxT("1.000"));

  /* enable text ctrl */
  text_ctrl_color_matrix_1st_line_1_->SetEditable(true);
  text_ctrl_color_matrix_1st_line_2_->SetEditable(true);
  text_ctrl_color_matrix_1st_line_3_->SetEditable(true);
  text_ctrl_color_matrix_2nd_line_1_->SetEditable(true);
  text_ctrl_color_matrix_2nd_line_2_->SetEditable(true);
  text_ctrl_color_matrix_2nd_line_3_->SetEditable(true);
  text_ctrl_color_matrix_3rd_line_1_->SetEditable(true);
  text_ctrl_color_matrix_3rd_line_2_->SetEditable(true);
  text_ctrl_color_matrix_3rd_line_3_->SetEditable(true);
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void ColorMatrixWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * The handler function for button(id = kButtonFileDialogId).
 * Open dialog to select Color matrix file.
 */
void ColorMatrixWnd::OnOpenFileDialog(wxCommandEvent &event) {
  DEBUG_PRINT("ColorMatrixWnd::OnOpenFileDialog\n");

  wxFileDialog *OpenDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
      wxT("*.csv"), wxFD_OPEN, wxDefaultPosition);

  /* Creates a "open file" dialog*/
  if (OpenDialog->ShowModal() == wxID_OK) {
    wxString wxPathName;
    bool ret = false;

    /* file open*/
    wxPathName = OpenDialog->GetPath();
    ret = LoadSettingsFromFile(wxPathName, temp_color_matrix_);
    if (ret == false) {
      DEBUG_PRINT("Could not open file =%s \n",
                  (const char *)wxPathName.mb_str());
      return;
    }

    if (LoadHideFlag(wxT(ColorMatrixConfigFile)) == false) {
      // Normal setting
      special_flg = false;
      text_ctrl_color_matrix_1st_line_1_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[0][0]));
      text_ctrl_color_matrix_1st_line_2_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[0][1]));
      text_ctrl_color_matrix_1st_line_3_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[0][2]));
      text_ctrl_color_matrix_2nd_line_1_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[1][0]));
      text_ctrl_color_matrix_2nd_line_2_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[1][1]));
      text_ctrl_color_matrix_2nd_line_3_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[1][2]));
      text_ctrl_color_matrix_3rd_line_1_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[2][0]));
      text_ctrl_color_matrix_3rd_line_2_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[2][1]));
      text_ctrl_color_matrix_3rd_line_3_->SetValue(
          wxString::Format(wxT("%.3f"), temp_color_matrix_[2][2]));

      /* enable text ctrl */
      text_ctrl_color_matrix_1st_line_1_->SetEditable(true);
      text_ctrl_color_matrix_1st_line_2_->SetEditable(true);
      text_ctrl_color_matrix_1st_line_3_->SetEditable(true);
      text_ctrl_color_matrix_2nd_line_1_->SetEditable(true);
      text_ctrl_color_matrix_2nd_line_2_->SetEditable(true);
      text_ctrl_color_matrix_2nd_line_3_->SetEditable(true);
      text_ctrl_color_matrix_3rd_line_1_->SetEditable(true);
      text_ctrl_color_matrix_3rd_line_2_->SetEditable(true);
      text_ctrl_color_matrix_3rd_line_3_->SetEditable(true);

    } else {
      // Special setting
      special_flg = true;
      /* hidden param */
      text_ctrl_color_matrix_1st_line_1_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_1st_line_2_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_1st_line_3_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_2nd_line_1_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_2nd_line_2_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_2nd_line_3_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_3rd_line_1_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_3rd_line_2_->SetValue(wxT(kSpecialValName));
      text_ctrl_color_matrix_3rd_line_3_->SetValue(wxT(kSpecialValName));

      /* disable text ctrl */
      text_ctrl_color_matrix_1st_line_1_->SetEditable(false);
      text_ctrl_color_matrix_1st_line_2_->SetEditable(false);
      text_ctrl_color_matrix_1st_line_3_->SetEditable(false);
      text_ctrl_color_matrix_2nd_line_1_->SetEditable(false);
      text_ctrl_color_matrix_2nd_line_2_->SetEditable(false);
      text_ctrl_color_matrix_2nd_line_3_->SetEditable(false);
      text_ctrl_color_matrix_3rd_line_1_->SetEditable(false);
      text_ctrl_color_matrix_3rd_line_2_->SetEditable(false);
      text_ctrl_color_matrix_3rd_line_3_->SetEditable(false);
    }
  }
}

/**
 * @brief
 * Load hide flag from color matrix file.
 * @param file_path [in] file path.
 */
bool ColorMatrixWnd::LoadHideFlag(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = CheckOpenFile(file_path, kCheckOpenFile);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Open(file_path);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Eof();
  if (ret == true) {
    DEBUG_PRINT("Blank init file\n");
    text_file.Close();
    return false;
  } else {
    int i = 0;
    for (line_str = text_file.GetFirstLine(); !text_file.Eof();
         line_str = text_file.GetNextLine(), i++) {
      if (i == 3) {
        if (line_str.Find(wxT("1")) != wxNOT_FOUND) {
          text_file.Close();
          return true;
        }
      }
    }
  }
  text_file.Close();
  return false;
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool ColorMatrixWnd::LoadSettingsFromFile(wxString file_path,
                                             float color_matrix[3][3]) {
  float temp_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  wxTextFile text_file;
  wxStringTokenizer tokenizer;
  wxString token_str, line_str;
  int tokenCnt;
  bool ret = false;
  double value;

  ret = CheckOpenFile(file_path, kCheckOpenFile);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Open(file_path);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Eof();
  if (ret == true) {
    DEBUG_PRINT("Blank init file\n");
    text_file.Close();
    return false;
  } else {
    for (int i = 0; i < 3; i++) {
      if (i == 0) {
        line_str = text_file.GetFirstLine();
      } else {
        line_str = text_file.GetNextLine();
      }
      tokenizer.SetString(line_str, wxT(","), wxTOKEN_DEFAULT);

      // Check Token Cnt
      tokenCnt = tokenizer.CountTokens();
      if (tokenCnt != 3) {
        DEBUG_PRINT(
            "Invalid token count [file = specialMatrix.csv, line = %d, token "
            "count = %d]\n",
            text_file.GetCurrentLine(), tokenCnt);
        text_file.Close();
        return false;
      }
      for (int j = 0; j < 3; j++) {
        token_str = tokenizer.GetNextToken();
        token_str.ToDouble(&value);
        temp_matrix[i][j] = static_cast<float>(value);
      }
    }
  }

  memcpy(color_matrix, temp_matrix, sizeof(float) * 3 * 3);

  text_file.Close();
  return true;
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @param color_matrix [in] 3*3 vector strings.
 * @return If true, writing the file success
 */
bool ColorMatrixWnd::WriteSettingsToFile(wxString file_path,
                                            float color_matrix[3][3]) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_string;
  wxString hide_param = wxT("0");

  ret = CheckOpenFile(file_path, kCreateOpenFile);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
  } else {
    if (LoadHideFlag(file_path) == true) {
      hide_param = wxT("1");
    }
  }
  ret = text_file.Open(file_path);
  if (ret == false) {
    ret = text_file.Create(file_path);
    if (ret == false) {
      DEBUG_PRINT("Fail to create file = %s \n",
                  (const char *)file_path.mb_str());
      return false;
    }
  }

  text_file.Clear();
  parent_->ClearPluginSettings();

  line_string = wxString::Format(wxT("%.3f,%.3f,%.3f"), color_matrix[0][0],
                                     color_matrix[0][1], color_matrix[0][2]);
  parent_->AddLinePluginSettings(line_string);
  text_file.AddLine(line_string);

  line_string = wxString::Format(wxT("%.3f,%.3f,%.3f"), color_matrix[1][0],
                                     color_matrix[1][1], color_matrix[1][2]);
  parent_->AddLinePluginSettings(line_string);
  text_file.AddLine(line_string);

  line_string = wxString::Format(wxT("%.3f,%.3f,%.3f"), color_matrix[2][0],
                                     color_matrix[2][1], color_matrix[2][2]);
  parent_->AddLinePluginSettings(line_string);
  text_file.AddLine(line_string);

  line_string = wxString::Format(wxT(kHideFlagName)) + hide_param;
  text_file.AddLine(line_string);

  if (parent_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}

/**
 * @brief
 * Check open file.
 * @param file_path [in] file path.
 */
bool ColorMatrixWnd::CheckOpenFile(wxString file_path, int open_file_type) {
  char check_file_path[256];
  FILE *check_file;

  memset(check_file_path, 0, sizeof(check_file_path));
  strncpy(check_file_path,
          (const char*)file_path.mb_str(),
          static_cast<int>(file_path.length()));

  if (open_file_type == kCheckOpenFile) {
    check_file = fopen(check_file_path, "r");
    if (check_file == NULL) {
      DEBUG_PRINT("Could not open check file =%s \n",
                                            (const char *)file_path.mb_str());
      return false;
    }
    fclose(check_file);
  } else if (open_file_type == kCreateOpenFile) {
    check_file = fopen(check_file_path, "w+");
    if (check_file == NULL) {
      DEBUG_PRINT("Could not open check file =%s \n",
                                            (const char *)file_path.mb_str());
      return false;
    }
    fclose(check_file);
    chmod(check_file_path, 0777);
  } else {
    DEBUG_PRINT("Failed to open file type =%d \n", open_file_type);
    return false;
  }
  return true;
}

/**
 * @brief
 * Apply current value.
 */
void ColorMatrixWnd::ApplyCurrentValue(void) {
  double value;
  if (special_flg == false) {
    // Rounding %.3f format
    text_ctrl_color_matrix_1st_line_1_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_1st_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_1st_line_2_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_1st_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_1st_line_3_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_1st_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_2nd_line_1_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_2nd_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_2nd_line_2_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_2nd_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_2nd_line_3_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_2nd_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_3rd_line_1_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_3rd_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_3rd_line_2_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_3rd_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), value));
    text_ctrl_color_matrix_3rd_line_3_->GetValue().ToDouble(&value);
    text_ctrl_color_matrix_3rd_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), value));

    // Get matrix values
    text_ctrl_color_matrix_1st_line_1_->GetValue().ToDouble(&value);
    temp_color_matrix_[0][0] = static_cast<float>(value);
    text_ctrl_color_matrix_1st_line_2_->GetValue().ToDouble(&value);
    temp_color_matrix_[0][1] = static_cast<float>(value);
    text_ctrl_color_matrix_1st_line_3_->GetValue().ToDouble(&value);
    temp_color_matrix_[0][2] = static_cast<float>(value);
    text_ctrl_color_matrix_2nd_line_1_->GetValue().ToDouble(&value);
    temp_color_matrix_[1][0] = static_cast<float>(value);
    text_ctrl_color_matrix_2nd_line_2_->GetValue().ToDouble(&value);
    temp_color_matrix_[1][1] = static_cast<float>(value);
    text_ctrl_color_matrix_2nd_line_3_->GetValue().ToDouble(&value);
    temp_color_matrix_[1][2] = static_cast<float>(value);
    text_ctrl_color_matrix_3rd_line_1_->GetValue().ToDouble(&value);
    temp_color_matrix_[2][0] = static_cast<float>(value);
    text_ctrl_color_matrix_3rd_line_2_->GetValue().ToDouble(&value);
    temp_color_matrix_[2][1] = static_cast<float>(value);
    text_ctrl_color_matrix_3rd_line_3_->GetValue().ToDouble(&value);
    temp_color_matrix_[2][2] = static_cast<float>(value);

    WriteSettingsToFile(wxT(ColorMatrixConfigFile), temp_color_matrix_);
  }

  memcpy(color_matrix_, temp_color_matrix_, sizeof(float) * 3 * 3);
  parent_->set_color_matrix(color_matrix_);
}

/**
 * @brief
 * Set plugin settings from flow load function.
 * @param params [in] settings string.
 */
void ColorMatrixWnd::SetPluginSettings(std::vector<wxString> params) {
  DEBUG_PRINT("ColorMatrixWnd::SetPluginSettings\n");
  bool ret = false;

  ret = LoadSettingsFromStrings(params, temp_color_matrix_);
  if (ret == false) {
    DEBUG_PRINT("Could not read settings\n");
    return;
  }

  if (LoadHideFlag(wxT(ColorMatrixConfigFile)) == false) {
    // Normal setting
    special_flg = false;
    text_ctrl_color_matrix_1st_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[0][0]));
    text_ctrl_color_matrix_1st_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[0][1]));
    text_ctrl_color_matrix_1st_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[0][2]));
    text_ctrl_color_matrix_2nd_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[1][0]));
    text_ctrl_color_matrix_2nd_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[1][1]));
    text_ctrl_color_matrix_2nd_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[1][2]));
    text_ctrl_color_matrix_3rd_line_1_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[2][0]));
    text_ctrl_color_matrix_3rd_line_2_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[2][1]));
    text_ctrl_color_matrix_3rd_line_3_->SetValue(
        wxString::Format(wxT("%.3f"), temp_color_matrix_[2][2]));

    /* enable text ctrl */
    text_ctrl_color_matrix_1st_line_1_->SetEditable(true);
    text_ctrl_color_matrix_1st_line_2_->SetEditable(true);
    text_ctrl_color_matrix_1st_line_3_->SetEditable(true);
    text_ctrl_color_matrix_2nd_line_1_->SetEditable(true);
    text_ctrl_color_matrix_2nd_line_2_->SetEditable(true);
    text_ctrl_color_matrix_2nd_line_3_->SetEditable(true);
    text_ctrl_color_matrix_3rd_line_1_->SetEditable(true);
    text_ctrl_color_matrix_3rd_line_2_->SetEditable(true);
    text_ctrl_color_matrix_3rd_line_3_->SetEditable(true);

  } else {
    // Special setting
    special_flg = true;
    /* hidden param */
    text_ctrl_color_matrix_1st_line_1_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_1st_line_2_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_1st_line_3_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_2nd_line_1_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_2nd_line_2_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_2nd_line_3_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_3rd_line_1_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_3rd_line_2_->SetValue(wxT(kSpecialValName));
    text_ctrl_color_matrix_3rd_line_3_->SetValue(wxT(kSpecialValName));

    /* disable text ctrl */
    text_ctrl_color_matrix_1st_line_1_->SetEditable(false);
    text_ctrl_color_matrix_1st_line_2_->SetEditable(false);
    text_ctrl_color_matrix_1st_line_3_->SetEditable(false);
    text_ctrl_color_matrix_2nd_line_1_->SetEditable(false);
    text_ctrl_color_matrix_2nd_line_2_->SetEditable(false);
    text_ctrl_color_matrix_2nd_line_3_->SetEditable(false);
    text_ctrl_color_matrix_3rd_line_1_->SetEditable(false);
    text_ctrl_color_matrix_3rd_line_2_->SetEditable(false);
    text_ctrl_color_matrix_3rd_line_3_->SetEditable(false);
  }

  ApplyCurrentValue();
}

/**
 * @brief
 * Load color matrix from 3*3 vector strings.
 * @param params [in] 3*3 vector strings.
 * @param color_matrix [out] color_matrix.
 */
bool ColorMatrixWnd::LoadSettingsFromStrings(std::vector<wxString> params,
                                             float color_matrix[3][3]) {
  float temp_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  wxStringTokenizer tokenizer;
  wxString token_str, line_str;
  int tokenCnt;
  double value;

  for (int i = 0; i < 3; i++) {
    line_str = params[i];
    tokenizer.SetString(line_str, wxT(","), wxTOKEN_DEFAULT);

    // Check Token Cnt
    tokenCnt = tokenizer.CountTokens();
    if (tokenCnt != 3) {
      DEBUG_PRINT("Invalid token count \n");
      return false;
    }
    for (int j = 0; j < 3; j++) {
      token_str = tokenizer.GetNextToken();
      token_str.ToDouble(&value);
      temp_matrix[i][j] = static_cast<float>(value);
    }
  }

  memcpy(color_matrix, temp_matrix, sizeof(float) * 3 * 3);

  return true;
}
