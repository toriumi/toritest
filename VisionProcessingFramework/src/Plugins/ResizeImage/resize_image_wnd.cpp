/**
 * @file      resize_image_wnd.cpp
 * @brief     Window of ResizeImage plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./resize_image_wnd.h"
#include <vector>
#include "./resize_image.h"

BEGIN_EVENT_TABLE(ResizeImageWnd, wxFrame)
EVT_CLOSE(ResizeImageWnd::OnClose)
EVT_BUTTON(kButtonApplyId, ResizeImageWnd::OnApply)
EVT_RADIOBOX(kRboxResizeId, ResizeImageWnd::OnSelectResizeValue)
EVT_RADIOBOX(kRboxHowToResizeId, ResizeImageWnd::OnSelectResizeMode)
EVT_COMMAND_SCROLL(kSliderResizeId, ResizeImageWnd::OnSliderResize)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
ResizeImageWnd::ResizeImageWnd(ResizeImage *parent)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  // Initialize
  parent_ = parent;

  // ------------------------How to Resize--------------------------
  wxString how_to_resize_tbl[2];
  how_to_resize_tbl[0] = wxT("Specific    ");
  how_to_resize_tbl[1] = wxT("Percent");

  how_to_resize_ = kHowToResizeDefault;
  wx_radio_select_how_to_resize_ = new wxRadioBox(
      this, kRboxHowToResizeId, wxT(kRadioHowToResizeText), wxPoint(20, 10),
      wxSize(200, 50), 2, how_to_resize_tbl, 2, wxRA_SPECIFY_COLS,
      wxDefaultValidator, wxT(kRadioHowToResizeText));
  wx_radio_select_how_to_resize_->SetSelection(how_to_resize_);

  // ------------------------Resize Type----------------------------
  // Create Resize Type TBL
  wxString resize_tbl[4];
  resize_tbl[kResizeDouble] = wxT("x 2");
  resize_tbl[kResizeDefault] = wxT("default");
  resize_tbl[kResizeHalf] = wxT("x 1/2");
  resize_tbl[kResizeQuarter] = wxT("x 1/4");

  resize_type_ = kResizeDefault;
  temp_resize_type_ = resize_type_;
  wx_radio_select_resize_ = new wxRadioBox(
      this, kRboxResizeId, wxT(kRadioResizeText), wxPoint(20, 60),
      wxSize(100, 190), 4, resize_tbl, 4, wxRA_SPECIFY_ROWS, wxDefaultValidator,
      wxT(kRadioResizeText));
  wx_radio_select_resize_->SetSelection(resize_type_);

  // Create Apply button
  wx_button_setting_apply_ =
      new wxButton(this, kButtonApplyId, wxT(kButtonApplyName),
                   wxPoint(55, 200), wxSize(60, 30));

  // ------------------------resize slider--------------------------
  wx_static_box_slider_resize_ =
      new wxStaticBox(this, -1, wxT(""), wxPoint(120, 60), wxSize(100, 190));

  slider_resize_value_ = kSliderDefault;
  wx_slider_resize_ = new wxSlider(this, kSliderResizeId, slider_resize_value_,
                                   10, 200, wxPoint(140, 80), wxSize(-1, 160),
                                   wxSL_VERTICAL | wxSL_INVERSE);
  wx_slider_resize_->SetLineSize(5);
  wx_slider_resize_->SetPageSize(5);
  wx_slider_resize_->SetThumbLength(5);

  wxString label;
  label = wxString::Format(wxT("%d%%"), slider_resize_value_);
  wx_static_text_slider_resize_ = new wxStaticText(
      this, kSliderResizeTextId, label, wxPoint(170, 80), wxSize(50, 30));

  LoadSettingsFromFile(wxT(ResizeConfigFile));

  if (how_to_resize_ == 0) {
    wx_radio_select_resize_->Enable();
    wx_button_setting_apply_->Enable();
    wx_static_box_slider_resize_->Disable();
    wx_slider_resize_->Disable();
    wx_static_text_slider_resize_->Disable();
    parent_->set_resize_type(resize_type_);
  } else {
    wx_radio_select_resize_->Disable();
    wx_button_setting_apply_->Disable();
    wx_static_box_slider_resize_->Enable();
    wx_slider_resize_->Enable();
    wx_static_text_slider_resize_->Enable();
    UpdateSliderResizeValue();
  }
}

/**
 * @brief
 * Destructor for this window.
 */
ResizeImageWnd::~ResizeImageWnd() {}

/**
 * @brief
 * The handler function for button(id = kButtonApplyId).
 */
void ResizeImageWnd::OnApply(wxCommandEvent &event) {
  DEBUG_PRINT("ResizeImageWnd::OnApply \n");
  resize_type_ = temp_resize_type_;
  parent_->set_resize_type(resize_type_);

  WriteSettingsToFile(wxT(ResizeConfigFile));
}

/**
 * @brief
 * The handler function for radio box(id = kRboxResizeId).
 */
void ResizeImageWnd::OnSelectResizeValue(wxCommandEvent &event) {
  DEBUG_PRINT("ResizeImageWnd::OnSelectResizeValue \n");
  temp_resize_type_ = (ResizeImageType)wx_radio_select_resize_->GetSelection();
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void ResizeImageWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * The handler function for radio box(id = kRboxHowToResizeId).
 */
void ResizeImageWnd::OnSelectResizeMode(wxCommandEvent &event) {
  DEBUG_PRINT("ResizeImageWnd::OnSelectResizeMode \n");
  how_to_resize_ = wx_radio_select_how_to_resize_->GetSelection();

  if (how_to_resize_ == 0) {
    wx_radio_select_resize_->Enable();
    wx_button_setting_apply_->Enable();
    wx_static_box_slider_resize_->Disable();
    wx_slider_resize_->Disable();
    wx_static_text_slider_resize_->Disable();
  } else {
    wx_radio_select_resize_->Disable();
    wx_button_setting_apply_->Disable();
    wx_static_box_slider_resize_->Enable();
    wx_slider_resize_->Enable();
    wx_static_text_slider_resize_->Enable();
    UpdateSliderResizeValue();
  }
}

/**
 * @brief
 * The handler function for slider(id = kSliderResizeId).
 */
void ResizeImageWnd::OnSliderResize(wxScrollEvent &event) {
  DEBUG_PRINT("ResizeImageWnd::OnSliderResize\n");
  UpdateSliderResizeValue();
}

/**
 * @brief
 * Apply resize slider value.
 */
void ResizeImageWnd::UpdateSliderResizeValue(void) {
  wxString label;

  // rounds the value to multiple of 5
  int value = wx_slider_resize_->GetValue();
  int temp = value % 10;
  value -= temp;
  if (temp <= 2) {
    temp = 0;
  } else if (temp <= 7) {
    temp = 5;
  } else {
    temp = 10;
  }
  value += temp;

  wx_slider_resize_->SetValue(value);
  slider_resize_value_ = value;

  label = wxString::Format(wxT("%d%%"), slider_resize_value_);
  wx_static_text_slider_resize_->SetLabel(label);
  parent_->set_resize_value(static_cast<double>(slider_resize_value_) / 100.0);

  WriteSettingsToFile(wxT(ResizeConfigFile));
}

/**
 * @brief
 * Set the list of parameter setting string for the Resize plugin.
 * @param params [in] settings string.
 */
void ResizeImageWnd::SetPluginSettings(std::vector<wxString> params) {
  wxString line_str;
  long temp_value; /* NOLINT */

  line_str = params[0];
  line_str.ToLong(&temp_value);
  how_to_resize_ = static_cast<int>(temp_value);
  wx_radio_select_how_to_resize_->SetSelection(how_to_resize_);

  line_str = params[1];
  line_str.ToLong(&temp_value);
  temp_resize_type_ = (ResizeImageType)temp_value;
  resize_type_ = temp_resize_type_;
  wx_radio_select_resize_->SetSelection(resize_type_);

  line_str = params[2];
  line_str.ToLong(&temp_value);
  slider_resize_value_ = static_cast<int>(temp_value);
  wx_slider_resize_->SetValue(slider_resize_value_);

  if (how_to_resize_ == 0) {
    wx_radio_select_resize_->Enable();
    wx_button_setting_apply_->Enable();
    wx_static_box_slider_resize_->Disable();
    wx_slider_resize_->Disable();
    wx_static_text_slider_resize_->Disable();
    parent_->set_resize_type(resize_type_);
  } else {
    wx_radio_select_resize_->Disable();
    wx_button_setting_apply_->Disable();
    wx_static_box_slider_resize_->Enable();
    wx_slider_resize_->Enable();
    wx_static_text_slider_resize_->Enable();
    UpdateSliderResizeValue();
  }

  WriteSettingsToFile(wxT(ResizeConfigFile));
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool ResizeImageWnd::LoadSettingsFromFile(wxString file_path) {
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
    how_to_resize_ = static_cast<int>(temp_value);
    wx_radio_select_how_to_resize_->SetSelection(how_to_resize_);

    line_str = text_file.GetNextLine();
    line_str.ToLong(&temp_value);
    temp_resize_type_ = (ResizeImageType)temp_value;
    resize_type_ = temp_resize_type_;
    wx_radio_select_resize_->SetSelection(resize_type_);

    line_str = text_file.GetNextLine();
    line_str.ToLong(&temp_value);
    slider_resize_value_ = static_cast<int>(temp_value);
    wx_slider_resize_->SetValue(slider_resize_value_);
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
bool ResizeImageWnd::WriteSettingsToFile(wxString file_path) {
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

  line_str = wxString::Format(wxT("%d"), how_to_resize_);
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  line_str = wxString::Format(wxT("%d"), resize_type_);
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  line_str = wxString::Format(wxT("%d"), slider_resize_value_);
  parent_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (parent_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
