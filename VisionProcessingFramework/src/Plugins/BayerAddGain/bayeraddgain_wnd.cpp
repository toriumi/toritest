/**
 * @file      bayeraddgain_wnd.cpp
 * @brief     Window of BayerAddGain plugin
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <algorithm>
#include <vector>

#include "./bayeraddgain_wnd.h"
#include "./bayeraddgain.h"

BEGIN_EVENT_TABLE(BayerAddGainWnd, wxFrame)
EVT_CLOSE(BayerAddGainWnd::OnClose)
EVT_BUTTON(kButtonApplyId, BayerAddGainWnd::OnApply)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
BayerAddGainWnd::BayerAddGainWnd(BayerAddGain *bayer_add_gain)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  wxString gain_default_value = wxT("1.00");
  double value = kBayerAddGainDefaultValue;
  int ob_clamp_value = 16;
  wxString ob_clamp = wxT("");

  parent_ = bayer_add_gain;
  if (LoadSettingsFromFile(wxT(kBayerAddGainConfigFile))) {
    gain_default_value =
        gain_default_value.Format(wxT("%f"), parent_->bayer_add_gain_value());
  } else {
    parent_->set_bayer_add_gain_value(static_cast<float>(value));
  }

  ob_clamp << ob_clamp_value;

  static_box_bayer_add_gain_ = new wxStaticBox(
      this, kStaticBoxBayerAddGainId, wxT(kStaticBoxBayerAddGainName),
      wxPoint(kStaticBoxBayerAddGainPointX, kStaticBoxBayerAddGainPointY),
      wxSize(kStaticBoxBayerAddGainSizeX, kStaticBoxBayerAddGainSizeY));

  static_text_pixel_value_ = new wxStaticText(
      this, kStaticTextBayerAddGainId, wxT(kStatictextPixelValueName),
      wxPoint(kStaticTextPixelValuePointX, kStaticTextPixelValuePointY),
      wxSize(kStaticTextPixelValueSizeX, kStaticTextPixelValueSizeY));

  static_text_optical_black1_ = new wxStaticText(
      this, kStaticTextOpticalBlack1Id, ob_clamp,
      wxPoint(kStaticTextOpticalBlack1PointX, kStaticTextOpticalBlack1PointY),
      wxSize(kStaticTextOpticalBlack1SizeX, kStaticTextOpticalBlack1SizeY));

  static_text_asta_ =
      new wxStaticText(this, kStaticTextAstaId, wxT(kStatictextAstaName),
                       wxPoint(kStaticTextAstaPointX, kStaticTextAstaPointY),
                       wxSize(kStaticTextAstaSizeX, kStaticTextAstaSizeY));

  text_ctrl_bayer_add_gain_value_ = new wxTextCtrl(
      this, kTextCtrlBayerAddGainId, gain_default_value,
      wxPoint(kTextCtrlBayerAddGainPointX, kTextCtrlBayerAddGainPointY),
      wxSize(kTextCtrlBayerAddGainSizeX, kTextCtrlBayerAddGainSizeY));

  static_text_plus_ =
      new wxStaticText(this, kStaticTextPlusId, wxT(kStatictextPlusName),
                       wxPoint(kStaticTextPlusPointX, kStaticTextPlusPointY),
                       wxSize(kStaticTextPlusSizeX, kStaticTextPlusSizeY));

  static_text_optical_black2_ = new wxStaticText(
      this, kStaticTextOpticalBlack2Id, ob_clamp,
      wxPoint(kStaticTextOpticalBlack2PointX, kStaticTextOpticalBlack2PointY),
      wxSize(kStaticTextOpticalBlack2SizeX, kStaticTextOpticalBlack2SizeY));

  button_apply_ = new wxButton(this, kButtonApplyId, wxT(kButtonApplyName),
                               wxPoint(kButtonApplyPointX, kButtonApplyPointY),
                               wxSize(kButtonApplySizeX, kButtonApplySizeY));
}

/**
 * @brief
 * Destructor for this window.
 */
BayerAddGainWnd::~BayerAddGainWnd() {}

/**
 * @brief
 * The handler function for EVT_BUTTON.
 */
void BayerAddGainWnd::OnApply(wxCommandEvent &event) {
  double value;

  text_ctrl_bayer_add_gain_value_->GetValue().ToDouble(&value);

  parent_->set_bayer_add_gain_value(static_cast<float>(value));

  WriteSettingsToFile(wxT(kBayerAddGainConfigFile));
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void BayerAddGainWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Set optical black value to static text.
 * @param optical black value
 */
void BayerAddGainWnd::OnOpticalBlack(int optical_black_value) {
  wxString ob_clamp = wxT("");
  ob_clamp << optical_black_value;

  static_text_optical_black1_->SetLabel(ob_clamp);
  static_text_optical_black2_->SetLabel(ob_clamp);
}

/**
 * @brief
 * Set the list of parameter setting string for the BayerAddGain plugin.
 * @param params [in] settings string.
 */
void BayerAddGainWnd::SetPluginSettings(std::vector<wxString> params) {
  double temp_value;
  if (params.size() > 0) {
    wxString parameter_value = params[0];
    parameter_value.ToDouble(&temp_value);
    parent_->set_bayer_add_gain_value(static_cast<float>(temp_value));
    text_ctrl_bayer_add_gain_value_->SetValue(parameter_value);
    WriteSettingsToFile(wxT(kBayerAddGainConfigFile));
  }
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool BayerAddGainWnd::LoadSettingsFromFile(wxString file_path) {
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
    line_str = text_file.GetFirstLine();
    line_str.ToDouble(&temp_value);
    parent_->set_bayer_add_gain_value(static_cast<float>(temp_value));
  }
  text_file.Close();
  return true;
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @return If true, writing the file success
 */
bool BayerAddGainWnd::WriteSettingsToFile(wxString file_path) {
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
  parent_->ClearPluginSettings();

  line_str = wxString::Format(wxT("%f"), parent_->bayer_add_gain_value());
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (parent_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
