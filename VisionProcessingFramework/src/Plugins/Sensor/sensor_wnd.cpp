/**
 * @file      sensor_wnd.cpp
 * @brief     Window of Sensor config.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./sensor_wnd.h"
#include <vector>
#include <algorithm>
#include "./sensor.h"

BEGIN_EVENT_TABLE(SensorWnd, wxFrame)
EVT_CLOSE(SensorWnd::OnClose)
EVT_BUTTON(kButtonFileDialogId, SensorWnd::OnOpenFileDialog)
EVT_BUTTON(kButtonApplyId, SensorWnd::OnApply)
EVT_BUTTON(kButtonSensorSettingsId, SensorWnd::OnOpenSensorSettingsWnd)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
SensorWnd::SensorWnd(Sensor* sensor)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  sensor_ = sensor;

  wxString bit_count_choice[kBitCountNum];
  bit_count_choice[0] = wxT(kBitCount8);
  bit_count_choice[1] = wxT(kBitCount10);
  bit_count_choice[2] = wxT(kBitCount12);

  /* Creating a open Sensor config static text object.*/
  static_text_sensor_config_ = new wxStaticText(
      this, kStaticTextSensorConfigId, wxT(kStaticTextSensorConfigName),
      wxPoint(kStaticTextSensorConfigPointX, kStaticTextSensorConfigPointY),
      wxSize(kStaticTextSensorConfigSizeW, kStaticTextSensorConfigSizeH));

  /* Creating a Sensor config text ctrl object.*/
  text_ctrl_sensor_config_ = new wxTextCtrl(
      this, kTextCtrlSensorConfigId, wxT(""),
      wxPoint(kTextCtrlSensorConfigPointX, kTextCtrlSensorConfigPointY),
      wxSize(kTextCtrlSensorConfigSizeW, kTextCtrlSensorConfigSizeH));

  /* Creating a Open file dialog button object.*/
  button_file_dialog_ =
      new wxButton(this, kButtonFileDialogId, wxT(kButtonFileDialogName),
                   wxPoint(kButtonFileDialogPointX, kButtonFileDialogPointY),
                   wxSize(kButtonFileDialogSizeW, kButtonFileDialogSizeH));

  /* Creating a Sensor name static text object.*/
  static_sensor_name_ = new wxStaticText(
      this, kStaticTextSensorNameId, wxT(kStaticTextSensorName),
      wxPoint(kStaticTextSensorNamePointX, kStaticTextSensorNamePointY),
      wxSize(kStaticTextSensorNameSizeW, kStaticTextSensorNameSizeH));

  /* Creating a sensor name value static text object.*/
  static_sensor_name_value_ = new wxStaticText(
      this, kStaticTextSensorNameValueId, wxT(kStaticTextSensorNameValue),
      wxPoint(kStaticTextSensorNameValuePointX,
              kStaticTextSensorNameValuePointY),
      wxSize(kStaticTextSensorNameValueSizeW, kStaticTextSensorNameValueSizeH));

  /* Creating a image size static text object.*/
  static_text_image_size_ =
      new wxStaticText(this, kStaticTextImageId, wxT(kStaticTextImageName),
                       wxPoint(kStaticTextImagePointX, kStaticTextImagePointY),
                       wxSize(kStaticTextImageSizeW, kStaticTextImageSizeH));

  /* Creating a image size value static text object.*/
  static_text_image_size_value_ = new wxStaticText(
      this, kStaticTextImageSizeValueId, wxT(kStaticTextImageSizeName),
      wxPoint(kStaticTextImageSizeValuePointX, kStaticTextImageSizeValuePointY),
      wxSize(kStaticTextImageSizeValueSizeW, kStaticTextImageSizeValueSizeH));

  /* Creating a binning mode static text object.*/
  static_text_binning_mode_ =
     new wxStaticText(this, kStaticTextBinningModeId,
         wxT(kStaticTextBinningModeName),
         wxPoint(kStaticTextBinningModePointX, kStaticTextBinningModePointY),
         wxSize(kStaticTextBinningModeSizeW, kStaticTextBinningModeSizeH));

  /* Creating a binning mode value static text object.*/
  static_text_binning_mode_value_width_ = new wxStaticText(
     this, kStaticTextBinningModeWidthValueId,
     wxT(kStaticTextBinningModeWidthValueName),
     wxPoint(kStaticTextBinningModeWidthValuePointX,
             kStaticTextBinningModeWidthValuePointY),
     wxSize(kStaticTextBinningModeWidthValueSizeW,
            kStaticTextBinningModeWidthValueSizeH));

  /* Binning mode height value static text definition*/
  static_text_binning_mode_value_height_ = new wxStaticText(
     this, kStaticTextBinningModeHeightValueId,
     wxT(kStaticTextBinningModeHeightValueName),
     wxPoint(kStaticTextBinningModeHeightValuePointX,
             kStaticTextBinningModeHeightValuePointY),
     wxSize(kStaticTextBinningModeHeightValueSizeW,
            kStaticTextBinningModeHeightValueSizeH));

  /* Creating a bit count static text object.*/
  static_text_bit_count_ = new wxStaticText(
      this, kStaticTextBitCountId, wxT(kStaticTextBitCountName),
      wxPoint(kStaticTextBitCountPointX, kStaticTextBitCountPointY),
      wxSize(kStaticTextBitCountSizeW, kStaticTextBitCountSizeH));

  /* Creating a bit count combo box object.*/
  combo_box_bit_count_ = new wxComboBox(
      this, kComboBoxBitCountId, wxEmptyString,
      wxPoint(kComboBoxBitCountPointX, kComboBoxBitCountPointY),
      wxSize(kComboBoxBitCountSizeW, kComboBoxBitCountSizeH), kBitCountNum,
      bit_count_choice, wxCB_DROPDOWN, wxDefaultValidator, wxT(""));

  combo_box_bit_count_->Enable(false);

  /* Creating a open sensor settings window button object.*/
  button_sensor_settings_ = new wxButton(
      this, kButtonSensorSettingsId, wxT(kButtonSensorSettingsName),
      wxPoint(kButtonSensorSettingsPointX, kButtonSensorSettingsPointY),
      wxSize(kButtonSensorSettingsSizeW, kButtonSensorSettingsSizeH));

  /* Creating a apply button object.*/
  button_apply_ = new wxButton(this, kButtonApplyId, wxT(kButtonApplyName),
                               wxPoint(kButtonApplyPointX, kButtonApplyPointY),
                               wxSize(kButtonApplySizeW, kButtonApplySizeH));

  file_dialog_sensor_config_open_ = new wxFileDialog(
      this, wxT(kFileDialogSensorConfigOpenWndName), wxEmptyString,
      wxEmptyString, wxT(kFileDialogSensorConfig), wxFD_OPEN,
      wxDefaultPosition);

  sensor_config_ = new SensorConfig;
  image_width_ = 0;
  image_height_ = 0;

  if (LoadSensorParamfromFile(wxT(kSensorParamFilePath)) == false) {
    WriteSensorParamtoFile(wxT(kSensorParamFilePath), wxT(""), 8);
  }
}

/**
 * @brief
 * Destructor for this window.
 */
SensorWnd::~SensorWnd() { delete sensor_config_; }

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void SensorWnd::OnClose(wxCloseEvent& event) { Show(false); }

/**
 * @brief
 * To select the sensor configuration file path from the file dialog.
 */
void SensorWnd::OnOpenFileDialog(wxCommandEvent& event) {
  DEBUG_PRINT("SensorWnd::OnOpenFileDialog\n");

  if (file_dialog_sensor_config_open_->ShowModal() == wxID_OK) {
    wxString sensor_config_file_path;
    combo_box_bit_count_->Enable(false);
    combo_box_bit_count_->SetValue(wxT(""));
    sensor_->InitSensorSettingWindow();

    sensor_config_file_path = file_dialog_sensor_config_open_->GetPath();
    bool retval = ReadSensorConfigFile(sensor_config_file_path);
    if (retval == false) {
      return;
    }
  }
  /* Refresh MainWnd class window.*/
  Refresh();
}

/**
 * @brief
 * Sensor name from the sensor configuration file and extracts the frame size.
 * @param sensor_config_file_path [in] sensor config file path
 */
bool SensorWnd::ReadSensorConfigFile(wxString sensor_config_file_path) {
  DEBUG_PRINT("SensorWnd::ReadSensorConfigFile\n");
  wxXmlDocument document;
  wxString contents, value;
  wxString width, height;
  wxStringTokenizer tokenizer;
  wxXmlNode* child;
  long value_temp;

  sensor_->sensor_config_type_ = wxT("");
  static_sensor_name_value_->SetLabel(wxT(""));
  static_text_image_size_value_->SetLabel(wxT(""));
  static_text_binning_mode_value_width_->SetLabel(wxT(""));
  static_text_binning_mode_value_height_->SetLabel(wxT(""));
  image_width_ = 0;
  image_height_ = 0;

  sensor_config_->set_sensor_config_file_path(sensor_config_file_path);
  text_ctrl_sensor_config_->SetValue(sensor_config_file_path);

  if (!document.Load(sensor_config_file_path)) {
    return false;
  }

  if (document.GetRoot()->GetName() != wxT(kProfileClass)) {
    return false;
  }

  child = document.GetRoot()->GetChildren();
  while (child) {
    if (child->GetName() == wxT(kComment)) {
      contents += child->GetNodeContent();

      // Get sensor name.
      tokenizer.SetString(contents, wxT(" "), wxTOKEN_DEFAULT);
      value = tokenizer.GetNextToken();
      sensor_config_->set_sensor_name(value);
      sensor_->sensor_config_type_ = value;
      static_sensor_name_value_->SetLabel(value);
      // Get image size.
      value = tokenizer.GetNextToken();
      sensor_config_->set_image_size(value);
      static_text_image_size_value_->SetLabel(value);
      tokenizer.SetString(value, wxT("x"), wxTOKEN_DEFAULT);

      width = tokenizer.GetNextToken();
      width.ToLong(&value_temp);
      image_width_ = static_cast<int>(value_temp);

      height = tokenizer.GetNextToken();
      height.ToLong(&value_temp);
      image_height_ = static_cast<int>(value_temp);

      combo_box_bit_count_->Enable(true);
      combo_box_bit_count_->SetValue(wxT(kBitCount8));
      break;
    }
    child = child->GetNext();
  }
  WriteSensorParamtoFile(wxT(kSensorParamFilePath),
                         sensor_config_file_path,
                         8);
  return true;
}
/**
 * @brief
 * Performing the initialization processing of the SSD with the apply button
 * pressed.
 */
void SensorWnd::OnApply(wxCommandEvent& event) {
  DEBUG_PRINT("SensorWnd::OnApply\n");
  wxString bit_count;
  int bit_count_type;
  int bit_count_num;
  CvSize size;

  memset(sensor_config_file_path_, 0, sizeof(sensor_config_file_path_));

  if (sensor_config_->sensor_config_file_path().Length() >= 128) {
    return;
  }
  strncpy(sensor_config_file_path_,
          (const char*)sensor_config_->sensor_config_file_path().mb_str(),
          static_cast<int>(sensor_config_->sensor_config_file_path().length()));

  bit_count = combo_box_bit_count_->GetValue();

  if (bit_count != wxT(kBitCount8) && bit_count != wxT(kBitCount10)) {
    return;
  }
  bool is_success_bit_count_changed = true;
  bool is_changed_flg = false;
  // change bit count
  if (bit_count == wxT(kBitCount8)) {
    if (sensor_->ChangeOutputPortSpec(0) == true) {
      DEBUG_PRINT("Success changed output port 8bit\n");
      bit_count_type = 0x02;
      bit_count_num = 8;
      combo_box_bit_count_->SetValue(wxT(kBitCount8));
    } else {
      DEBUG_PRINT("Fail changed output port 8bit\n");
      combo_box_bit_count_->SetValue(wxT(kBitCount10));
      is_success_bit_count_changed = false;
    }
  } else if (bit_count == wxT(kBitCount10)) {
    sensor_->set_active_output_port_spec_index(0);
    if (sensor_->ChangeOutputPortSpec(1) == true) {
      DEBUG_PRINT("Success changed output port 10bit\n");
      bit_count_type = 0x06;
      bit_count_num = 10;
      combo_box_bit_count_->SetValue(wxT(kBitCount10));
      is_changed_flg = true;
    } else {
      DEBUG_PRINT("Fail changed output port 10bit\n");
      combo_box_bit_count_->SetValue(wxT(kBitCount8));
      is_success_bit_count_changed = false;
    }
  }

  if (is_success_bit_count_changed == false) {
    wxMessageDialog dialog(
       NULL, wxT("Could not changed output port.\n1.out of range for active "
                 "port index\n2.could not use output port."),
       wxT("Error"), wxOK, wxPoint(kMessageDialogPointX, kMessageDialogPointY));
    if (dialog.ShowModal() == wxID_OK) {
    }
    return;
  }
  WriteSensorParamtoFile(wxT(kSensorParamFilePath),
                         sensor_config_->sensor_config_file_path(),
                         bit_count_num);

  if ((image_width_ == 0) || (image_height_ == 0)) {
    DEBUG_PRINT("Failed to image size w:%d h:%d\n",
                image_width_, image_height_);
    return;
  }
  size = cvSize(image_width_, image_height_);

  if (sensor_->SensorConfig(bit_count_type, sensor_config_file_path_, size) ==
      false) {
    return;
  }

}

/**
 * @brief
 * Open sensor setting window of the Sensor plugin.
 */
void SensorWnd::OnOpenSensorSettingsWnd(wxCommandEvent& event) {
  DEBUG_PRINT("SensorWnd::OnOpenSensorSettingsWnd\n");
  sensor_->OpenSensorSettingWindow();
}

/**
 * @brief
 * Set the image processing state.
 * @param state [in] ImageProcessingState
 */
void SensorWnd::UpdateUIForImageProcessingState(ImageProcessingState state) {
  DEBUG_PRINT("SensorWnd::UpdateUIForImageProcessingState state = %d\n", state);
  switch (state) {
    case kRun:
      combo_box_bit_count_->Enable(false);
      button_apply_->Enable(false);
      button_file_dialog_->Enable(false);
      break;
    case kStop:
      combo_box_bit_count_->Enable(true);
      button_apply_->Enable(true);
      button_file_dialog_->Enable(true);
      sensor_->analog_gain_param_ = sensor_->analog_gain_param_temp_;
      sensor_->digital_gain_param_ = sensor_->digital_gain_param_temp_;
      sensor_->coarse_integration_time_ =
          sensor_->coarse_integration_time_temp_;
      sensor_->orien_reg_ = sensor_->orien_reg_temp_;
      break;
    case kPause:
      combo_box_bit_count_->Enable(false);
      button_apply_->Enable(false);
      button_file_dialog_->Enable(false);
      break;
  }
}

/**
 * @brief
 * To set the binning mode.
 */
bool SensorWnd::SetBinningMode() {
  DEBUG_PRINT("SensorWnd::SetBinningMode\n");
  std::vector<int>::iterator address_itr;
  std::vector<int>::iterator value_itr;
  wxString binning_mode_height = wxT(kBinningModeUnknown);
  wxString binning_mode_width = wxT(kBinningModeUnknown);
  int value = 0;
  int value_count = 0;
  int temp_address = 0;
  int address_count = 0;

  /* Check binning mode type*/
  for (address_itr = sensor_->binning_mode_address_.begin(); address_itr !=
                         sensor_->binning_mode_address_.end(); address_itr++) {
    value_count = 0;
    if (address_count == 0) {
      temp_address = *address_itr;
      /* binning mode V*/
      if (CheckBinningMode(*address_itr, &value) == false) {
        continue;
      }
      value = value & 0x0F;
      for (value_itr = sensor_->binning_mode_value_.begin(); value_itr !=
                             sensor_->binning_mode_value_.end(); value_itr++) {
        if (value  == *value_itr) {
          switch (value_count) {
            case 0:
              binning_mode_height = wxT(kBinningModeNone);
              break;
            case 1:
              binning_mode_height = wxT(kBinningModeX2);
              break;
            case 2:
              binning_mode_height = wxT(kBinningModeX4);
              break;
            case 3:
              binning_mode_width = wxT(kBinningModeAnalog);
              break;
          }
        }
        value_count++;
      }
    } else if (address_count == 1) {
      /* binning mode H*/
      if (CheckBinningMode(*address_itr, &value) == false) {
        continue;
      }
      if (temp_address == *address_itr) {
        value = (value & 0xF0) >> 4;
      } else {
        value = value & 0x0F;
      }
      for (value_itr = sensor_->binning_mode_value_.begin(); value_itr !=
                             sensor_->binning_mode_value_.end(); value_itr++) {
        if (value  == *value_itr) {
          switch (value_count) {
            case 0:
              binning_mode_width = wxT(kBinningModeNone);
              break;
            case 1:
              binning_mode_width = wxT(kBinningModeX2);
              break;
            case 2:
              binning_mode_width = wxT(kBinningModeX4);
              break;
            case 3:
              binning_mode_width = wxT(kBinningModeAnalog);
              break;
          }
        }
        value_count++;
      }
    }
    address_count++;
  }
  static_text_binning_mode_value_width_->SetLabel(
                                             wxT("H  ") + binning_mode_width);
  static_text_binning_mode_value_height_->SetLabel(
                                             wxT("V  ") + binning_mode_height);
  return true;
}

/**
 * @brief
 * To check the binning mode.
 * @param address [out] Binning mode address
 * @param address_value [out] Binning mode register value
 */
bool SensorWnd::CheckBinningMode(int address, int *address_value) {
  DEBUG_PRINT("SensorWnd::CheckBinningMode\n");
  long value; /* NOLINT */
  wxXmlDocument document;
  wxString contents;
  wxXmlNode* child;
  wxXmlNode* serch_child;
  static_text_binning_mode_value_width_->SetLabel(wxT(""));
  static_text_binning_mode_value_height_->SetLabel(wxT(""));

  if (!document.Load(sensor_config_->sensor_config_file_path())) {
    return false;
  }

  if (document.GetRoot()->GetName() != wxT(kProfileClass)) {
    return false;
  }

  child = document.GetRoot()->GetChildren();
  while (child) {
    if (child->GetName() == wxT("RegisterSettings")) {
      child = child->GetChildren();
      break;
    }
    child = child->GetNext();
  }

  while (child) {
    if (child->GetName() == wxT("RegisterSetting")) {
      serch_child = child->GetChildren();
      if (serch_child->GetName() == wxT(kAddress)) {
        contents = serch_child->GetNodeContent();
        contents.ToLong(&value);
        if (address == value) {
          serch_child = serch_child->GetNext();
          if (serch_child->GetName() == wxT(kAddressValue)) {
            contents = serch_child->GetNodeContent();
            contents.ToLong(&value);
            *address_value = static_cast<int>(value);
            return true;
          }
          return false;
        }
      }
    }
    child = child->GetNext();
  }
  return false;
}

/**
 * @brief
 * Load the parameters from the sensor config ini file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool SensorWnd::LoadSensorParamfromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;

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
  }
  wxString sensor_config_file_path = text_file.GetFirstLine();
  if (ReadSensorConfigFile(sensor_config_file_path) == false) {
    text_file.Close();
    return false;
  }
  combo_box_bit_count_->Enable(true);
  wxString bit_count = text_file.GetNextLine();
  if (bit_count == wxT("8")) {
    combo_box_bit_count_->SetValue(wxT(kBitCount8));
  } else if (bit_count == wxT("10")) {
    combo_box_bit_count_->SetValue(wxT(kBitCount10));
  } else {
    combo_box_bit_count_->SetValue(wxT(kBitCount8));
  }
  text_file.Close();
  return true;
}

/**
 * @brief
 * Write the parameters to the sensor config ini file.
 * @param file_path [in] file path.
 * @param sensor_config_file_path [in] sensor config file path.
 * @param bit_count [in] bit count.
 * @return If true, writing the file success
 */
bool SensorWnd::WriteSensorParamtoFile(wxString file_path,
                                       wxString sensor_config_file_path,
                                       int bit_count) {
  wxTextFile text_file;
  bool ret = false;
  wxString wx_bit_count = wxT("");

  ret = CheckOpenFile(file_path, kCreateOpenFile);
  if (ret == false) {
    DEBUG_PRINT("Could not open file =%s\n", (const char *)file_path.mb_str());
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
  if (sensor_config_file_path == wxT("")) {
    text_file.Clear();
  }
  sensor_->ClearPluginSettings();

  int line_count = text_file.GetLineCount();
  if (10 > line_count) {
    for (int i = 0; i < (10 - line_count); i++) {
      text_file.AddLine(wxString::Format(wxT("")));
    }
  }

  text_file[0] = sensor_config_file_path;
  wx_bit_count << bit_count;
  text_file[1] = wx_bit_count;

  for (int i = 0; i < 8; i++) {
    sensor_->AddLinePluginSettings(text_file[i]);
  }

  text_file.Write();
  text_file.Close();

  return true;
}

/**
 * @brief
 * Check open ini file.
 * @param file_path [in] file path.
 * @param open_file_type [in] open file type.
 * @return If true, open the file success
 */
bool SensorWnd::CheckOpenFile(wxString file_path, int open_file_type) {
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
    check_file = fopen(check_file_path, "r");
    if (check_file == NULL) {
      check_file = fopen(check_file_path, "w+");
      if (check_file == NULL) {
        DEBUG_PRINT("Could not open check file =%s \n",
                                            (const char *)file_path.mb_str());
        return false;
      }
    }
    fclose(check_file);
  } else {
    DEBUG_PRINT("Failed to open file type =%d \n", open_file_type);
    return false;
  }
  return true;
}

/**
 * @brief
 * To reflect the information read from the sensor ini file.
 * @param params [in] sensor config file path.
 */
void SensorWnd::SetSensorConfig(std::vector<wxString> params) {
  DEBUG_PRINT("SensorWnd::SetPluginSettings\n");

  wxString sensor_config_file_path = params[0];
  if (ReadSensorConfigFile(sensor_config_file_path) == false) {
    DEBUG_PRINT("Failed to open sensor config file.\n");
    return;
  }
  combo_box_bit_count_->Enable(true);
  wxString bit_count = params[1];
  if (bit_count == wxT("8")) {
    combo_box_bit_count_->SetValue(wxT(kBitCount8));
  } else if (bit_count == wxT("10")) {
    combo_box_bit_count_->SetValue(wxT(kBitCount10));
  } else {
    combo_box_bit_count_->SetValue(wxT(kBitCount8));
  }
}
