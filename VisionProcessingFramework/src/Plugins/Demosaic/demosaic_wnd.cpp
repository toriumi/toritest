/**
 * @file      demosaic_wnd.cpp
 * @brief     Window of Demosaic plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <algorithm>
#include <vector>
#include "./demosaic_wnd.h"

BEGIN_EVENT_TABLE(DemosaicWnd, wxFrame)
EVT_CLOSE(DemosaicWnd::OnClose)
EVT_BUTTON(kButtonApplyId, DemosaicWnd::OnApply)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
DemosaicWnd::DemosaicWnd(Demosaic *demosaic)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  demosaic_ = demosaic;
  wxString color_choice[4];
  color_choice[0] = wxT("RGB(8bit)");
  color_choice[1] = wxT("BGR(8bit)");
  color_choice[2] = wxT("RGB(10bit)");
  color_choice[3] = wxT("BGR(10bit)");

  radio_box_color_ = new wxRadioBox(
      this, wxID_ANY, wxT(kRadioBoxName),
      wxPoint(kRadioBoxPointX, kRadioBoxPointY), wxDefaultSize, 4, color_choice,
      0, wxRA_SPECIFY_ROWS, wxDefaultValidator, wxT(kRadioBoxName));

  button_apply_ = new wxButton(this, kButtonApplyId, wxT(kButtonApllyName),
                               wxPoint(kButtonApllyPointX, kButtonApllyPointY),
                               wxSize(kButtonApllySizeX, kButtonApllySizeY));

  // default setting
  radio_box_color_->SetSelection(kDemosaicBgr888);
  demosaic_->ChangeOutputPortSpec(kDemosaicBgr888);

  LoadSettingsFromFile(wxT(DemosaicConfigFile));
}

/**
 * @brief
 * Destructor for this window.
 */
DemosaicWnd::~DemosaicWnd() {}

/**
 * @brief
 * The handler function for EVT_APPLY.
 */
void DemosaicWnd::OnApply(wxCommandEvent &event) {
  int select_color = radio_box_color_->GetSelection();

  if (set_color_type(select_color) == false) {
    wxMessageDialog dialog(NULL, wxT("Setting could not be changed."),
                           wxT("Error"), wxOK, wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
  }
  WriteSettingsToFile(wxT(DemosaicConfigFile));
}

/**
 * @brief
 * Set the color that you want to convert.
 * @param color_type [in] kind of color
 */
bool DemosaicWnd::set_color_type(int color_type) {
  bool is_change = false;
  is_change = demosaic_->ChangeOutputPortSpec(color_type);

  if (is_change == true) {
    DEBUG_PRINT("Demosaic::set_color_type success change type = %d\n",
                color_type);
    color_type_ = color_type;
    return true;
  } else {
    DEBUG_PRINT("Demosaic::set_color_type fail change type = %d\n", color_type);
    return false;
  }
}

/**
 * @brief
 * Update the setting window UI to active or inactive
 * by image processing state.
 * @param state [in] ImageProcessingState
 */
void DemosaicWnd::UpdateUIForImageProcessingState(ImageProcessingState state) {
  switch (state) {
    case kRun:
      radio_box_color_->Enable(false);
      button_apply_->Enable(false);
      break;
    case kStop:
      radio_box_color_->Enable(true);
      button_apply_->Enable(true);
      break;
    case kPause:
      radio_box_color_->Enable(false);
      button_apply_->Enable(false);
      break;
  }
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void DemosaicWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Set the list of parameter setting string for the Demosaic plugin.
 * @param params [in] settings string.
 */
void DemosaicWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  long temp_value; /* NOLINT */

  line_str = params[0];
  line_str.ToLong(&temp_value);
  color_type_ = static_cast<int>(temp_value);
  radio_box_color_->SetSelection(color_type_);
  demosaic_->ChangeOutputPortSpec(color_type_);
  WriteSettingsToFile(wxT(DemosaicConfigFile));
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool DemosaicWnd::LoadSettingsFromFile(wxString file_path) {
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
    color_type_ = static_cast<int>(temp_value);
    radio_box_color_->SetSelection(color_type_);
    demosaic_->ChangeOutputPortSpec(color_type_);
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
bool DemosaicWnd::WriteSettingsToFile(wxString file_path) {
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
  demosaic_->ClearPluginSettings();

  line_str = wxString::Format(wxT("%d"), color_type_);
  demosaic_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (demosaic_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
