/**
 * @file      edge_enhancement_wnd.cpp
 * @brief     Window of EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <vector>
#include "./edge_enhancement_wnd.h"

BEGIN_EVENT_TABLE(EdgeEnhancementWnd, wxFrame)
EVT_CLOSE(EdgeEnhancementWnd::OnClose)
EVT_BUTTON(BTN_APPLY_ID, EdgeEnhancementWnd::OnUpdate)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 * @param param [in] EdgeEnhancementParam param.
 */
EdgeEnhancementWnd::EdgeEnhancementWnd(EdgeEnhancement *parent,
                                       EdgeEnhancementParam *param)
    : wxFrame(NULL, WND_ID, wxT(WND_TITLE), wxPoint(WND_POINT_X, WND_POINT_Y),
              wxSize(WND_SIZE_W, WND_SIZE_H)) {
  this->parent_ = parent;
  this->param_ = param;

  float coeff = param_->GetCoeff();

  wx_static_coeff_text_ =
      new wxStaticText(this, ST_COEFF_ID, wxT(ST_COEFF_TEXT),
                       wxPoint(ST_COEFF_POINT_X, ST_COEFF_POINT_Y),
                       wxSize(ST_COEFF_SIZE_W, ST_COEFF_SIZE_H));
  wx_textctrl_coeff_text_ =
      new wxTextCtrl(this, TCTRL_COEFF_ID, wxString::Format(wxT("%.3f"), coeff),
                     wxPoint(TCTRL_COEFF_POINT_X, TCTRL_COEFF_POINT_Y),
                     wxSize(TCTRL_COEFF_SIZE_W, TCTRL_COEFF_SIZE_H));
  wx_button_coeff_apply_ =
      new wxButton(this, BTN_APPLY_ID, wxT(BTN_APPLY_TEXT),
                   wxPoint(BTN_APPLY_POINT_X, BTN_APPLY_POINT_Y),
                   wxSize(BTN_APPLY_SIZE_W, BTN_APPLY_SIZE_H));

  LoadSettingsFromFile(wxT(EdgeConfigFile));
}

/**
 * @brief
 * Destructor for this window.
 */
EdgeEnhancementWnd::~EdgeEnhancementWnd() {}

void EdgeEnhancementWnd::OnClose(wxCloseEvent &event) {
  DEBUG_PRINT("EdgeEnhancementWnd::OnClose\n");
  Show(false);
}

/**
 * @brief
 * The handler function for BTN_APPLY_ID.
 */
void EdgeEnhancementWnd::OnUpdate(wxCommandEvent &event) {
  DEBUG_PRINT("EdgeEnhancementWnd::OnUpdate\n");
  double val;
  wx_textctrl_coeff_text_->GetValue().ToDouble(&val);

  if (COEFF_MIN <= val && val <= COEFF_MAX) {
    val = param_->SetCoeff(val);
    wx_textctrl_coeff_text_->SetValue(wxString::Format(wxT("%.3f"), val));
  } else {
    wxMessageDialog dialog(NULL, wxT("value range : 0 <= value <= 100"),
                           wxT("Error"), wxOK, wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
  }

  WriteSettingsToFile(wxT(EdgeConfigFile));
}

/**
 * @brief
 * Set the list of parameter setting string for the EdgeEnhancement plugin.
 * @param params [in] settings string.
 */
void EdgeEnhancementWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  double temp_value;

  line_str = params[0];
  line_str.ToDouble(&temp_value);
  param_->SetCoeff(static_cast<float>(temp_value));
  wx_textctrl_coeff_text_->SetValue(
      wxString::Format(wxT("%.3f"), param_->GetCoeff()));

  WriteSettingsToFile(wxT(EdgeConfigFile));
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool EdgeEnhancementWnd::LoadSettingsFromFile(wxString file_path) {
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
    param_->SetCoeff(static_cast<float>(temp_value));
    wx_textctrl_coeff_text_->SetValue(
        wxString::Format(wxT("%.3f"), param_->GetCoeff()));
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
bool EdgeEnhancementWnd::WriteSettingsToFile(wxString file_path) {
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

  line_str = wxString::Format(wxT("%.3f"), param_->GetCoeff());
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (parent_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
