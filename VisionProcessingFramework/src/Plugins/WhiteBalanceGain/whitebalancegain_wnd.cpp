/**
 * @file      whitebalancegain_wnd.cpp
 * @brief     Window of WhiteBalanceGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./whitebalancegain.h"
#include <algorithm>
#include <vector>
#include "./whitebalancegain_wnd.h"

BEGIN_EVENT_TABLE(WhiteBalanceGainWnd, wxFrame)
EVT_CLOSE(WhiteBalanceGainWnd::OnClose)
EVT_BUTTON(kDefaultButtonId, WhiteBalanceGainWnd::OnDefault)
EVT_BUTTON(kOnePushButtonId, WhiteBalanceGainWnd::OnOnePush)
EVT_BUTTON(kApplyButtonId, WhiteBalanceGainWnd::OnApply)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
WhiteBalanceGainWnd::WhiteBalanceGainWnd(WhiteBalanceGain *white_balance_gain)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  wxString text_param[2];
  double red_value = kWhiteBalanceGainDefaultValue;
  double blue_value = kWhiteBalanceGainDefaultValue;
  text_param[0] = wxT("1.00");
  text_param[1] = wxT("1.00");

  white_balance_gain_ = white_balance_gain;

  white_balance_gain_->SetWhiteBalanceGainRedValue(
      static_cast<float>(red_value));
  white_balance_gain_->SetWhiteBalanceGainBlueValue(
      static_cast<float>(blue_value));

  static_box_white_balance_with_digital_gain_ = new wxStaticBox(
      this, -1, wxT(kPluginName), wxPoint(10, 10), wxSize(350, 140));

  static_text_red_ = new wxStaticText(this, -1, wxT(kStringRed),
                                      wxPoint(50, 40), wxSize(50, 25));

  static_text_green_ = new wxStaticText(this, -1, wxT(kStringGreen),
                                        wxPoint(150, 40), wxSize(50, 25));

  static_text_blue_ = new wxStaticText(this, -1, wxT(kStringBlue),
                                       wxPoint(250, 40), wxSize(50, 25));

  text_ctrl_red_value_ =
      new wxTextCtrl(this, -1, text_param[0], wxPoint(50, 70), wxSize(60, 30));

  static_text_green_value_ =
      new wxStaticText(this, -1, wxT("1.00"), wxPoint(150, 70), wxSize(60, 30));

  text_ctrl_blue_value_ =
      new wxTextCtrl(this, -1, text_param[1], wxPoint(250, 70), wxSize(60, 30));

  button_default_ = new wxButton(this, kDefaultButtonId, wxT(kButtonDefault),
                                 wxPoint(250, 110), wxSize(80, 30));

  button_one_push_ = new wxButton(this, kOnePushButtonId, wxT(kStringOnPush),
                                  wxPoint(150, 110), wxSize(80, 30));

  button_apply_ = new wxButton(this, kApplyButtonId, wxT(kStringApply),
                               wxPoint(50, 110), wxSize(80, 30));

  LoadSettingsFromFile(wxT(kWhiteBalanceGainConfigFile));
}

/**
 * @brief
 * Destructor for this window.
 */
WhiteBalanceGainWnd::~WhiteBalanceGainWnd() {}

void WhiteBalanceGainWnd::OnDefault(wxCommandEvent &event) {
  DEBUG_PRINT("WhiteBalanceGainWnd::OnDefault \n");

  text_ctrl_red_value_->SetValue(wxString::Format(
      wxT("%.2f"), static_cast<float>(kWhiteBalanceGainDefaultValue)));
  text_ctrl_blue_value_->SetValue(wxString::Format(
      wxT("%.2f"), static_cast<float>(kWhiteBalanceGainDefaultValue)));
}

/**
 * @brief
 * The handler function for button(id = kOnePushButtonId).
 */
void WhiteBalanceGainWnd::OnOnePush(wxCommandEvent &event) {
  DEBUG_PRINT("WhiteBalanceGainWnd::OnOnePush \n");
  white_balance_gain_->SetOnePush();
}

/**
 * @brief
 * The handler function for button(id = kApplyButtonId).
 */
void WhiteBalanceGainWnd::OnApply(wxCommandEvent &event) {
  DEBUG_PRINT("WhiteBalanceGainWnd::OnApply \n");
  double value;

  text_ctrl_red_value_->GetValue().ToDouble(&value);
  white_balance_gain_->SetWhiteBalanceGainRedValue(static_cast<float>(value));
  text_ctrl_blue_value_->GetValue().ToDouble(&value);
  white_balance_gain_->SetWhiteBalanceGainBlueValue(static_cast<float>(value));

  WriteSettingsToFile(wxT(kWhiteBalanceGainConfigFile));
}

/**
 * @brief
 * Set red value and blue value to text ctrl area.
 */
void WhiteBalanceGainWnd::SetTextCtrlValue(
    float white_balanace_gain_red_value, float white_balanace_gain_blue_value) {
  text_ctrl_red_value_->SetValue(wxString::Format(
      wxT("%.2f"), static_cast<float>(white_balanace_gain_red_value)));
  text_ctrl_blue_value_->SetValue(wxString::Format(
      wxT("%.2f"), static_cast<float>(white_balanace_gain_blue_value)));

  WriteSettingsToFile(wxT(kWhiteBalanceGainConfigFile));
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void WhiteBalanceGainWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Set the list of parameter setting string for the Color plugin.
 * @param params [in] settings string.
 */
void WhiteBalanceGainWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  double temp_value;

  // Red
  line_str = params[0];
  line_str.ToDouble(&temp_value);
  text_ctrl_red_value_->SetValue(
      wxString::Format(wxT("%.2f"), static_cast<float>(temp_value)));

  // Blue
  line_str = params[1];
  line_str.ToDouble(&temp_value);
  text_ctrl_blue_value_->SetValue(
      wxString::Format(wxT("%.2f"), static_cast<float>(temp_value)));

  text_ctrl_red_value_->GetValue().ToDouble(&temp_value);
  white_balance_gain_->SetWhiteBalanceGainRedValue(
      static_cast<float>(temp_value));
  text_ctrl_blue_value_->GetValue().ToDouble(&temp_value);
  white_balance_gain_->SetWhiteBalanceGainBlueValue(
      static_cast<float>(temp_value));

  WriteSettingsToFile(wxT(kWhiteBalanceGainConfigFile));
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool WhiteBalanceGainWnd::LoadSettingsFromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;
  double temp_value;

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
    // Red
    line_str = text_file.GetFirstLine();
    line_str.ToDouble(&temp_value);
    text_ctrl_red_value_->SetValue(
        wxString::Format(wxT("%.2f"), static_cast<float>(temp_value)));

    // Blue
    line_str = text_file.GetNextLine();
    line_str.ToDouble(&temp_value);
    text_ctrl_blue_value_->SetValue(
        wxString::Format(wxT("%.2f"), static_cast<float>(temp_value)));
  }

  text_file.Close();

  text_ctrl_red_value_->GetValue().ToDouble(&temp_value);
  white_balance_gain_->SetWhiteBalanceGainRedValue(
      static_cast<float>(temp_value));
  text_ctrl_blue_value_->GetValue().ToDouble(&temp_value);
  white_balance_gain_->SetWhiteBalanceGainBlueValue(
      static_cast<float>(temp_value));

  return true;
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @param color_matrix [in] 3*3 vector strings.
 * @return If true, writing the file success
 */
bool WhiteBalanceGainWnd::WriteSettingsToFile(wxString file_path) {
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
  white_balance_gain_->ClearPluginSettings();

  // Red
  line_str = text_ctrl_red_value_->GetValue();
  white_balance_gain_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  // Blue
  line_str = text_ctrl_blue_value_->GetValue();
  white_balance_gain_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (white_balance_gain_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
