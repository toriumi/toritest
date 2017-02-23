/**
 * @file      sensor_settings_wnd.cpp
 * @brief     Window of sensor settings.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./sensor_settings_wnd.h"
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "./sensor.h"

BEGIN_EVENT_TABLE(SensorSettingsWnd, wxFrame)
EVT_CLOSE(SensorSettingsWnd::OnClose)
EVT_BUTTON(kButtonFileDialogId, SensorSettingsWnd::OnOpenFileDialog)
EVT_BUTTON(kButtonApplyFirstPixelId, SensorSettingsWnd::OnApplyFirstPixel)
EVT_BUTTON(kButtonApplyImageOrientationId,
           SensorSettingsWnd::OnApplyImageOrientation)
EVT_COMMAND_SCROLL(kSliderAnalogGainId, SensorSettingsWnd::OnSliderAnalogGain)
EVT_COMMAND_SCROLL(kSliderDigitalGainId,
                   SensorSettingsWnd::OnSliderDigitalGain)
EVT_COMMAND_SCROLL(kSliderExposureTimeId,
                   SensorSettingsWnd::OnSliderExposureTime)
EVT_BUTTON(kButtonApplyAnalogGainId, SensorSettingsWnd::OnApplyAnalogGain)
EVT_BUTTON(kButtonApplyDigitalGainId, SensorSettingsWnd::OnApplyDigitalGain)
EVT_BUTTON(kButtonApplyExposureTimeId, SensorSettingsWnd::OnApplyExposureTime)
END_EVENT_TABLE();

/**
 * @brief
 * Constructor for this window.
 */
SensorSettingsWnd::SensorSettingsWnd(Sensor* sensor)
    : wxFrame(NULL, kWndId, wxT(kWndSettingsTitle),
              wxPoint(kWndPointX, kWndPointY + 450),
              wxSize(kWndSizeW + 120, kWndSizeH + 40)) {
  sensor_ = sensor;

  /* Creating a open sensor settings static text object.*/
  static_text_sensor_settings_ = new wxStaticText(
      this, kStaticTextSensorConfigId, wxT(kStaticTextSensorSettingsName),
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

  /*First Pixel*/
  wxString first_pixel_choice[kFirstPixelNum];
  first_pixel_choice[0] = wxT(kFirstPixelR);
  first_pixel_choice[1] = wxT(kFirstPixelGB);
  first_pixel_choice[2] = wxT(kFirstPixelGR);
  first_pixel_choice[3] = wxT(kFirstPixelB);

  radio_box_first_pixel =
      new wxRadioBox(this, -1, wxT(kFirstPixelName), wxPoint(10, 80),
                  wxSize(360, 70), 4, first_pixel_choice, 4,
                  wxRA_SPECIFY_COLS, wxDefaultValidator, wxT(kFirstPixelName));
  radio_box_first_pixel->SetSelection(0);
  sensor_->SetFirstPixel(0);

  /* Creating a apply button object.*/
  button_first_pixel_apply_ =
      new wxButton(this, kButtonApplyFirstPixelId, wxT(kButtonApplyName),
                   wxPoint(280, 100),
                   wxSize(kButtonApplySizeW, kButtonApplySizeH));

  radio_box_first_pixel->Enable(false);
  button_first_pixel_apply_->Enable(false);

  /*Image orientation*/
  /* Creating a bit count combo box object.*/
  wxString image_orientation_choice[4];
  image_orientation_choice[0] = wxT("Normal");
  image_orientation_choice[1] = wxT("Horizontal mirror");
  image_orientation_choice[2] = wxT("Vertical flip");
  image_orientation_choice[3] = wxT("V flip & H mirror");

  static_box_image_orientation_ = new wxStaticBox(
      this, -1, wxT("Image orientation"), wxPoint(10, 150), wxSize(360, 80));

  combo_box_image_orientation_ =
     new wxComboBox(this, kComboBoxBitCountId, wxEmptyString, wxPoint(20, 180),
                    wxSize(kComboBoxBitCountSizeW, kComboBoxBitCountSizeH), 4,
                    image_orientation_choice, wxCB_DROPDOWN,
                    wxDefaultValidator, wxT("Normal"));

  button_image_orientation_apply_ =
      new wxButton(this, kButtonApplyImageOrientationId, wxT(kButtonApplyName),
                   wxPoint(280, 180),
                   wxSize(kButtonApplySizeW, kButtonApplySizeH));
  static_box_image_orientation_->Enable(false);
  combo_box_image_orientation_->Enable(false);
  button_image_orientation_apply_->Enable(false);

  /*Analog gain*/
  static_box_analog_gain_ = new wxStaticBox(this, -1, wxT("Analog gain"),
                                            wxPoint(10, 250), wxSize(290, 50));

  slider_analog_gain_ =
      new wxSlider(this, kSliderAnalogGainId, 0, 0, 1, wxPoint(10, 270),
                   wxSize(260, -1));

  //static_text_analog_gain_ =
  //   new wxStaticText(this, 30080, wxT(""), wxPoint(290, 270), wxSize(50, 30));
  static_box_analog_gain_->Enable(false);
  slider_analog_gain_->Enable(false);
  //static_text_analog_gain_->SetLabel(wxT(""));
  text_ctrl_analog_gain_ =
      new wxTextCtrl(this, -1, wxT(""), wxPoint(290, 260), wxSize(80, 40));
  static_text_analog_gain_DB_ =
     new wxStaticText(this, -1, wxT("(db)"), wxPoint(370, 270), wxSize(40, 30));
  button_image_analog_gain_apply_ =
      new wxButton(this, kButtonApplyAnalogGainId, wxT(kButtonApplyName),
                   wxPoint(410, 260),
                   wxSize(kButtonApplySizeW, kButtonApplySizeH));
  text_ctrl_analog_gain_->Enable(false);
  static_text_analog_gain_DB_->Enable(false);
  button_image_analog_gain_apply_->Enable(false);

  /*Digital gain*/
  static_box_digital_gain_ = new wxStaticBox(this, -1, wxT("Digital gain"),
                                            wxPoint(10, 300), wxSize(290, 50));
  slider_digital_gain_ =
      new wxSlider(this, kSliderDigitalGainId, 0, 0, 1, wxPoint(10, 320),
                   wxSize(260, -1));

  //static_text_digital_gain_ =
  //   new wxStaticText(this, 30080, wxT(""), wxPoint(290, 320), wxSize(50, 30));
  static_box_digital_gain_->Enable(false);
  slider_digital_gain_->Enable(false);
  //static_text_digital_gain_->SetLabel(wxT(""));
  text_ctrl_digital_gain_ =
      new wxTextCtrl(this, -1, wxT(""), wxPoint(290, 310), wxSize(80, 40));
  static_text_digital_gain_DB_ =
     new wxStaticText(this, -1, wxT("(db)"), wxPoint(370, 320), wxSize(40, 30));
  button_image_digital_gain_apply_ =
      new wxButton(this, kButtonApplyDigitalGainId, wxT(kButtonApplyName),
                   wxPoint(410, 310),
                   wxSize(kButtonApplySizeW, kButtonApplySizeH));
  text_ctrl_digital_gain_->Enable(false);
  static_text_digital_gain_DB_->Enable(false);
  button_image_digital_gain_apply_->Enable(false);

  /*exposure time*/
  static_box_exposure_time_ = new wxStaticBox(
      this, -1, wxT("Exposure time"), wxPoint(10, 350), wxSize(290, 50));

  slider_exposure_time_ =
      new wxSlider(this, kSliderExposureTimeId, 0, 0, 1, wxPoint(10, 370),
                   wxSize(260, -1));

  //static_text_exposure_time_ =
  //   new wxStaticText(this, 30080, wxT(""), wxPoint(290, 370), wxSize(50, 30));
  static_box_exposure_time_->Enable(false);
  slider_exposure_time_->Enable(false);
  //static_text_exposure_time_->SetLabel(wxT(""));
  text_ctrl_exposure_time_ =
      new wxTextCtrl(this, -1, wxT(""), wxPoint(290, 360), wxSize(80, 40));
  static_text_exposure_time_MS_ =
     new wxStaticText(this, -1, wxT("(ms)"), wxPoint(370, 370), wxSize(40, 30));
  button_image_exposure_time_apply_ =
      new wxButton(this, kButtonApplyExposureTimeId, wxT(kButtonApplyName),
                   wxPoint(410, 360),
                   wxSize(kButtonApplySizeW, kButtonApplySizeH));
  text_ctrl_exposure_time_->Enable(false);
  static_text_exposure_time_MS_->Enable(false);
  button_image_exposure_time_apply_->Enable(false);

  /*file dialog*/
  file_dialog_sensor_settings_open_ = new wxFileDialog(
      this, wxT(kFileDialogSensorConfigOpenWndName), wxEmptyString,
      wxEmptyString, wxT(kFileDialogSensorSettings), wxFD_OPEN,
      wxDefaultPosition);

  if (LoadSensorParamfromFile(wxT(kSensorParamFilePath)) == false) {
    WriteSensorParamtoFile(wxT(kSensorParamFilePath));
  }
}

/**
 * @brief
 * Destructor for this window.
 */
SensorSettingsWnd::~SensorSettingsWnd() {
  WriteSensorParamtoFile(wxT(kSensorParamFilePath));
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void SensorSettingsWnd::OnClose(wxCloseEvent& event) {
  WriteSensorParamtoFile(wxT(kSensorParamFilePath));
  Show(false);
}

/**
 * @brief
 * To select the sensor settings file path from the file dialog.
 */
void SensorSettingsWnd::OnOpenFileDialog(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnOpenFileDialog\n");

  if (file_dialog_sensor_settings_open_->ShowModal() == wxID_OK) {
    bool retval = false;

    radio_box_first_pixel->Enable(false);
    button_first_pixel_apply_->Enable(false);
    static_box_image_orientation_->Enable(false);
    combo_box_image_orientation_->Enable(false);
    button_image_orientation_apply_->Enable(false);
    static_box_analog_gain_->Enable(false);
    slider_analog_gain_->Enable(false);
    slider_analog_gain_->SetValue(0);
    //static_text_analog_gain_->SetLabel(wxT(""));
    static_box_digital_gain_->Enable(false);
    slider_digital_gain_->Enable(false);
    slider_digital_gain_->SetValue(0);
    //static_text_digital_gain_->SetLabel(wxT(""));
    static_box_exposure_time_->Enable(false);
    slider_exposure_time_->Enable(false);
    slider_exposure_time_->SetValue(0);
    //static_text_exposure_time_->SetLabel(wxT(""));
    sensor_->analog_gain_param_temp_ = kRegisterNone;
    sensor_->digital_gain_param_temp_ = kRegisterNone;
    sensor_->coarse_integration_time_temp_ = kRegisterNone;
    sensor_->orien_reg_temp_ = kRegisterNone;
    text_ctrl_analog_gain_->SetValue(wxT(""));
    text_ctrl_digital_gain_->SetValue(wxT(""));
    text_ctrl_exposure_time_->SetValue(wxT(""));
    text_ctrl_analog_gain_->Enable(false);
    text_ctrl_digital_gain_->Enable(false);
    text_ctrl_exposure_time_->Enable(false);
    static_text_digital_gain_DB_->Enable(false);
    static_text_analog_gain_DB_->Enable(false);
    static_text_exposure_time_MS_->Enable(false);
    button_image_analog_gain_apply_->Enable(false);
    button_image_digital_gain_apply_->Enable(false);
    button_image_exposure_time_apply_->Enable(false);

    wxString sensor_settings_file_path =
        file_dialog_sensor_settings_open_->GetPath();
    retval = ReadSensorSettingsFile(sensor_settings_file_path);
    if (retval == false) {
      return;
    }
  }
  // Refresh MainWnd class window.
  Refresh();
}

/**
 * @brief
 * Sensor name from the sensor settings file and extracts the frame size.
 * @param sensor_settings_file_path [in] sensor settings file path
 */
bool SensorSettingsWnd::ReadSensorSettingsFile(
                                        wxString sensor_settings_file_path) {
  DEBUG_PRINT("SensorSettingsWnd::ReadSensorSettingsFile\n");
  wxString wxLineStr, wxTokenStr;
  wxTextFile read_csv_file;
  wxStringTokenizer wxTokenizer;
  bool retval = false;
  int token_count = 0;
  char value[128];

  text_ctrl_sensor_config_->SetValue(sensor_settings_file_path);

  retval = read_csv_file.Open(sensor_settings_file_path);
  if (retval == false) {
    return false;
  }
  retval = read_csv_file.Eof();
  if (retval == true) {
    read_csv_file.Close();
    return false;
  }

  for (wxLineStr = read_csv_file.GetFirstLine(); !read_csv_file.Eof();
       wxLineStr = read_csv_file.GetNextLine()) {
    wxTokenizer.SetString(wxLineStr, wxT(","), wxTOKEN_DEFAULT);

    token_count = wxTokenizer.CountTokens();

    if (token_count == 0) {
      read_csv_file.Close();
      return true;
    }
    // csvfile param set
    wxTokenStr = wxTokenizer.GetNextToken();

    if (wxTokenStr == wxT("sensor_type")) {
      wxTokenStr = wxTokenizer.GetNextToken();
      sensor_->sensor_type_ = wxTokenStr;
      radio_box_first_pixel->Enable(true);
      button_first_pixel_apply_->Enable(true);
      if (sensor_->sensor_type_ != sensor_->sensor_config_type_) {
        DEBUG_PRINT("Failed to sensor settings file sensor type\n");
        return false;
      }
      if ((sensor_->sensor_type_ == wxT("IMX219")) ||
          (sensor_->sensor_type_ == wxT("IMX378")) ||
          (sensor_->sensor_type_ == wxT("IU233"))) {
        ReadConvetToDBFile();
      }
    } else if (wxTokenStr == wxT("register name")) {
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
      }
    } else if (wxTokenStr == wxT("analog_gain")) {
      long idef, imin, imax, addr; /* NOLINT */
      memset(value, 0, sizeof(value));
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        if (i == 1) {
          wxTokenStr.ToLong(&idef);
        } else if (i == 2) {
          wxTokenStr.ToLong(&imin);
        } else if (i == 3) {
          wxTokenStr.ToLong(&imax);
        } else if (i == 4) {
          strncpy(value, (const char*)wxTokenStr.mb_str(),
                  static_cast<int>(wxTokenStr.length()));
          addr = strtol(value, NULL, 16);
          sensor_->again_addr_ = static_cast<int>(addr);
        } else {
          strncpy(value, (const char*)wxTokenStr.mb_str(),
                  static_cast<int>(wxTokenStr.length()));
          addr = strtol(value, NULL, 16);
          sensor_->again_addr1_ = static_cast<int>(addr);
        }
      }
      if (slider_analog_gain_) {
        delete slider_analog_gain_;
      }
      slider_analog_gain_ = new wxSlider(
          this, kSliderAnalogGainId, static_cast<int>(idef),
          static_cast<int>(imin),
          static_cast<int>(imax), wxPoint(10, 270), wxSize(260, -1));
      static_box_analog_gain_->Enable(true);
      slider_analog_gain_->Enable(true);
      //static_text_analog_gain_->SetLabel(
      //    ConvertToAnalogGainDB(static_cast<int>(idef)));
      text_ctrl_analog_gain_->SetValue(ConvertToAnalogGainDB(static_cast<int>(idef)));
      text_ctrl_analog_gain_->Enable(true);
      static_text_analog_gain_DB_->Enable(true);
      button_image_analog_gain_apply_->Enable(true);
      sensor_->analog_gain_param_ = static_cast<int>(idef);
      sensor_->analog_gain_param_temp_ = static_cast<int>(idef);
    } else if (wxTokenStr == wxT("digital_gain")) {
      memset(value, 0, sizeof(value));
      long idef, imin, imax, addr; /* NOLINT */
      sensor_->dgain_addr_.clear();
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        if (i == 1) {
          wxTokenStr.ToLong(&idef);
        } else if (i == 2) {
          wxTokenStr.ToLong(&imin);
        } else if (i == 3) {
          wxTokenStr.ToLong(&imax);
        } else {
          if (sensor_->sensor_type_ == wxT("IMX378")) {
        	if (i == 4) {
        	  strncpy(value, (const char*)wxTokenStr.mb_str(),
        	         static_cast<int>(wxTokenStr.length()));
        	  addr = strtol(value, NULL, 16);
        	  sensor_->dgain_addr1_ = static_cast<int>(addr);
        	} else {
        	  strncpy(value, (const char*)wxTokenStr.mb_str(),
        	         static_cast<int>(wxTokenStr.length()));
        	  addr = strtol(value, NULL, 16);
        	  sensor_->dgain_addr2_ = static_cast<int>(addr);
        	}
          } else {
            strncpy(value, (const char*)wxTokenStr.mb_str(),
                    static_cast<int>(wxTokenStr.length()));
            addr = strtol(value, NULL, 16);
            sensor_->dgain_addr_.push_back(static_cast<int>(addr));
          }
        }
      }
      if (slider_digital_gain_) {
        delete slider_digital_gain_;
      }
      slider_digital_gain_ = new wxSlider(
          this, kSliderDigitalGainId, static_cast<int>(idef),
          static_cast<int>(imin),
          static_cast<int>(imax), wxPoint(10, 320), wxSize(260, -1));
      static_box_digital_gain_->Enable(true);
      slider_digital_gain_->Enable(true);
      //static_text_digital_gain_->SetLabel(
      //    ConvertToDigitalGainDB(static_cast<int>(idef)));
      text_ctrl_digital_gain_->SetValue(ConvertToDigitalGainDB(static_cast<int>(idef)));
      text_ctrl_digital_gain_->Enable(true);
      static_text_digital_gain_DB_->Enable(true);
      button_image_digital_gain_apply_->Enable(true);
      sensor_->digital_gain_param_ = static_cast<int>(idef);
      sensor_->digital_gain_param_temp_ = static_cast<int>(idef);
    } else if (wxTokenStr == wxT("exposure_time")) {
      memset(value, 0, sizeof(value));
      long idef, imin, imax, addr; /* NOLINT */
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        if (i == 1) {
          wxTokenStr.ToLong(&idef);
        } else if (i == 2) {
          wxTokenStr.ToLong(&imin);
        } else if (i == 3) {
          wxTokenStr.ToLong(&imax);
        } else if (i == 4) {
          strncpy(value, (const char*)wxTokenStr.mb_str(),
                  static_cast<int>(wxTokenStr.length()));
          addr = strtol(value, NULL, 16);
          sensor_->etime_addr1_ = static_cast<int>(addr);
        } else {
          strncpy(value, (const char*)wxTokenStr.mb_str(),
                  static_cast<int>(wxTokenStr.length()));
          addr = strtol(value, NULL, 16);
          sensor_->etime_addr2_ = static_cast<int>(addr);
        }
      }
      if (slider_exposure_time_) {
        delete slider_exposure_time_;
      }
      slider_exposure_time_ = new wxSlider(
          this, kSliderExposureTimeId, static_cast<int>(idef),
          static_cast<int>(imin),
          static_cast<int>(imax), wxPoint(10, 370), wxSize(260, -1));

      static_box_exposure_time_->Enable(true);
      slider_exposure_time_->Enable(true);
      //static_text_exposure_time_->SetLabel(
      //    ConvertToExposureTimeMS(static_cast<int>(idef)));
      text_ctrl_exposure_time_->SetValue(ConvertToExposureTimeMS(static_cast<int>(idef)));
      text_ctrl_exposure_time_->Enable(true);
      static_text_exposure_time_MS_->Enable(true);
      button_image_exposure_time_apply_->Enable(true);
      sensor_->coarse_integration_time_ = static_cast<int>(idef);
      sensor_->coarse_integration_time_temp_ = static_cast<int>(idef);
    } else if (wxTokenStr == wxT("image_orientation")) {
      memset(value, 0, sizeof(value));
      long addr; /* NOLINT */
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        if (i == 4) {
          strncpy(value, (const char*)wxTokenStr.mb_str(),
                  static_cast<int>(wxTokenStr.length()));
          addr = strtol(value, NULL, 16);
          sensor_->image_addr_ = static_cast<int>(addr);
        }
      }
      static_box_image_orientation_->Enable(true);
      combo_box_image_orientation_->Enable(true);
      button_image_orientation_apply_->Enable(true);
      sensor_->orien_reg_ = 0;
      sensor_->orien_reg_temp_ = 0;
      combo_box_image_orientation_->SetValue(wxT("Normal"));
      combo_box_image_orientation_->SetSelection(0);
    } else if (wxTokenStr == wxT("register to sec")) {
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
      }
    } else if (wxTokenStr == wxT("exposure_time_ms")) {
      long exposure_time_value; /* NOLINT */
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        if (i == 1) {
          wxTokenStr.ToLong(&exposure_time_value);
          sensor_->frame_length_line_ = sensor_->line_length_regvalue_;
          DEBUG_PRINT("frame_length_line_: %d \n", (int)sensor_->line_length_regvalue_);
          //sensor_->frame_length_line_ = static_cast<int>(exposure_time_value);
          //DEBUG_PRINT("frame_length_line_: %d \n", (int)exposure_time_value);
        } else if (i == 2) {
          wxTokenStr.ToLong(&exposure_time_value);
          sensor_->offset_ = static_cast<int>(exposure_time_value);
          DEBUG_PRINT("offset_: %d \n", (int)exposure_time_value);
        }
      }
      //static_text_exposure_time_->SetLabel(
      //    ConvertToExposureTimeMS(sensor_->coarse_integration_time_));
      text_ctrl_exposure_time_->SetValue(ConvertToExposureTimeMS(sensor_->coarse_integration_time_));

    } else if (wxTokenStr == wxT("binning_mode")) {
      memset(value, 0, sizeof(value));
      long binning_mode_value; /* NOLINT */
      sensor_->binning_mode_address_.clear();
      sensor_->binning_mode_value_.clear();
      for (int i = 1; i < token_count; i++) {
        wxTokenStr = wxTokenizer.GetNextToken();
        strncpy(value, (const char*)wxTokenStr.mb_str(),
                                        static_cast<int>(wxTokenStr.length()));
        if (i <= 2) {
          binning_mode_value = strtol(value, NULL, 16);
          sensor_->binning_mode_address_.push_back(
                                        static_cast<int>(binning_mode_value));
        } else {
          wxTokenStr.ToLong(&binning_mode_value);
          sensor_->binning_mode_value_.push_back(
                                        static_cast<int>(binning_mode_value));
        }
      }
    } else {
      DEBUG_PRINT("Failed to wxTokenStr\n");
    }
  }
  sensor_->SetBinningMode();
  WriteSensorParamtoFile(wxT(kSensorParamFilePath));
  read_csv_file.Close();
  return true;
}

/**
 * @brief
 * Read the decibel conversion result file(IMX219,IU233).
 */
void SensorSettingsWnd::ReadConvetToDBFile() {
  DEBUG_PRINT("SensorSettingsWnd::ReadConvetToDBFile\n");
  wxTextFile read_db_file;
  bool retval = false;
  int get_line_count = 0;

  retval = read_db_file.Open(wxT(kImx219AnalogGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read imx219 analog gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Imx219_analog_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Imx219_analog_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }

  retval = read_db_file.Open(wxT(kImx219DigitalGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read imx219 digital gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Imx219_digital_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Imx219_digital_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }

  retval = read_db_file.Open(wxT(kIu233AnalogGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read iu233 analog gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Iu233_analog_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Iu233_analog_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }

  retval = read_db_file.Open(wxT(kIu233DigitalGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read iu233 digital gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Iu233_digital_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Iu233_digital_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }

  retval = read_db_file.Open(wxT(kImx378AnalogGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read imx378 analog gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Imx378_analog_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Imx378_analog_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }

  retval = read_db_file.Open(wxT(kImx378DigitalGainDBFile));
  if (retval == false) {
    DEBUG_PRINT("Failed to read imx378 digital gain db file\n");
  } else {
    get_line_count = read_db_file.GetLineCount();
    Imx378_digital_gain_[0] = read_db_file.GetFirstLine();
    for (int i = 1; i < get_line_count; i++) {
      Imx378_digital_gain_[i] = read_db_file.GetNextLine();
    }
    read_db_file.Close();
  }
  return;
}

/**
 * @brief
 * Convert from the register value of the analog gain in decibels.
 * @param register_value [in] register value
 * @return decibels value
 */
wxString SensorSettingsWnd::ConvertToAnalogGainDB(int register_value) {
  DEBUG_PRINT("SensorSettingsWnd::ConvertToAnalogGainDB\n");
  wxString value = wxT("");
  float analog_gain_db = 0;

  if (sensor_->sensor_type_ == wxT("IMX219")) {
    if (register_value > kImx219AnalogGainMax) {
      DEBUG_PRINT("Failed to convert to analog gain\n");
      return value;
    }
    value = Imx219_analog_gain_[register_value];
  } else if (sensor_->sensor_type_ == wxT("IU233")) {
    if ((register_value) > kIu233AnalogGainMax) {
      DEBUG_PRINT("Failed to convert to analog gain\n");
      return value;
    }
    value = Iu233_analog_gain_[register_value];
  } else if (sensor_->sensor_type_ == wxT("IMX290")) {
    analog_gain_db = static_cast<float>(register_value);
    value = wxString::Format(wxT("%.2f"), (analog_gain_db * 3 / 10));
  } else if (sensor_->sensor_type_ == wxT("IMX408")) {
    analog_gain_db = static_cast<float>(register_value);
    //value = wxString::Format(wxT("%.2f"), ((analog_gain_db / 511) * 24));
    value = wxString::Format(wxT("%.2f"), ((analog_gain_db / 255) * 24));
  } else if (sensor_->sensor_type_ == wxT("IMX378")) {
    if (register_value > kImx378AnalogGainMax) {
      DEBUG_PRINT("Failed to convert to analog gain\n");
      return value;
    }
    value = Imx378_analog_gain_[register_value];
  }
  //value += wxT(kDB);
  return value;
}

/**
 * @brief
 * Convert from the register value of the digital gain in decibels.
 * @param register_value [in] register value
 * @return decibels value
 */
wxString SensorSettingsWnd::ConvertToDigitalGainDB(int register_value) {
  DEBUG_PRINT("SensorSettingsWnd::ConvertToDigitalGainDB\n");
  wxString value;
  float analog_gain_db;

  if (sensor_->sensor_type_ == wxT("IMX219")) {
    if (register_value - 256 > kImx219DitalGainMax) {
      DEBUG_PRINT("Failed to convert to digital gain\n");
      return value;
    }
    value = Imx219_digital_gain_[register_value - 256];
  } else if (sensor_->sensor_type_ == wxT("IU233")) {
    if (register_value - 1 > kIu233DitalGainMax) {
      DEBUG_PRINT("Failed to convert to digital gain\n");
      return value;
    }
    value = Iu233_digital_gain_[register_value - 1];
  } else if (sensor_->sensor_type_ == wxT("IMX408")) {
    analog_gain_db = static_cast<float>(register_value);
    value = wxString::Format(wxT("%.2f"), ((analog_gain_db / 511) * 24));
  } else if (sensor_->sensor_type_ == wxT("IMX378")) {
    if (register_value - 256 > kImx378DitalGainMax) {
      DEBUG_PRINT("Failed to convert to digital gain\n");
      return value;
    }
    value = Imx378_digital_gain_[register_value - 256];
  }
  //value += wxT(kDB);1

  return value;
}

/**
 * @brief
 * Convert from the register value of the exposure time in decibels.
 * @param register_value [in] register value
 * @return decibels value
 */
wxString SensorSettingsWnd::ConvertToExposureTimeMS(int register_value) {
  DEBUG_PRINT("SensorSettingsWnd::ConvertToExposureTimeMS\n");
  wxString value = wxT("");
  float exposure_time_ms = 0;
  //float coarse_ratio = 0;
  //float fps = 0;

  if (sensor_->sensor_type_ == wxT("IMX290")) {
    value << register_value;
    return value;
  }
  DEBUG_PRINT("register_value: %d \n", register_value);
  DEBUG_PRINT("frame_length_line_: %d \n", sensor_->frame_length_line_);

  if ((sensor_->sensor_type_ == wxT("IMX219")) ||
      (sensor_->sensor_type_ == wxT("IU233"))  ||
      (sensor_->sensor_type_ == wxT("IMX378"))  ||
      (sensor_->sensor_type_ == wxT("IMX408"))) {
	if (sensor_->frequency_ > -1E-10 && sensor_->frequency_ < 1E-10) {
      exposure_time_ms = 0;
	} else {
      //if (register_value <= sensor_->frame_length_line_) {
      if (register_value <= sensor_->line_length_regvalue_) {
        exposure_time_ms = static_cast<float>(
            //(1.0 / (60 * sensor_->frame_length_line_)) * register_value);
            //(1.0 / (sensor_->frequency_ * sensor_->frame_length_line_)) * register_value);
            (1.0 / (sensor_->frequency_ * sensor_->line_length_regvalue_)) * register_value);

      //} else if (register_value > sensor_->frame_length_line_) {
      } else if (register_value > sensor_->line_length_regvalue_) {
        exposure_time_ms = static_cast<float>(
            //(1.0 / (60 * (sensor_->frame_length_line_ + sensor_->offset_))) * register_value);
            //(1.0 / (sensor_->frequency_ * (sensor_->frame_length_line_ + sensor_->offset_))) * register_value);
            (1.0 / (sensor_->frequency_ * (sensor_->line_length_regvalue_ + sensor_->offset_))) * register_value);
      }
      //coarse_ratio = static_cast<float>(register_value / sensor_->coarse_regvalue_);
      //fps = static_cast<float>(sensor_->frequency_ / coarse_ratio);
      //exposure_time_ms = (1/fps) * 1000;
	}
  }
  exposure_time_ms = exposure_time_ms * 1000;
  DEBUG_PRINT("exposure_time_ms1: %f \n", exposure_time_ms);
  value = wxString::Format(wxT("%.2f"), exposure_time_ms);
  //value += wxT(kMS);

  return value;
}

/**
 * @brief
 * To set a common the first pixel.
 * @param first_pixel [in] first pixel.
 */
void SensorSettingsWnd::SetFirstPixel() {
  DEBUG_PRINT("SensorSettingsWnd::SetFirstPixel\n");
  int serection_value;
  serection_value = radio_box_first_pixel->GetSelection();
  DEBUG_PRINT("SensorSettingsWnd::OnApplyFirstPixel %d\n", serection_value);
  sensor_->SetFirstPixel(serection_value);
}

/**
 * @brief
 * To set the first pixel in the apply button is pressed.
 * @param first_pixel [in] first pixel.
 */
void SensorSettingsWnd::OnApplyFirstPixel(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnApplyFirstPixel\n");
  SetFirstPixel();
  WriteSensorParamtoFile(wxT(kSensorParamFilePath));
}

/**
 * @brief
 * To set the image orientation.
 */
void SensorSettingsWnd::SetImageOrientation() {
  DEBUG_PRINT("SensorSettingsWnd::SetImageOrientation\n");
  int r_value[4] = {0, 1, 2, 3};
  int gr_value[4] = {1, 0, 3, 2};
  int gb_value[4] = {2, 3, 0, 1};
  int b_value[4] = {3, 2, 1, 0};
  int set_first_pixel;
  int first_pixel;
  int image_value = combo_box_image_orientation_->GetSelection();

  if (sensor_->sensor_type_ == wxT("IMX408")) {
    r_value[0] = 0;
    r_value[1] = 0;
    r_value[2] = 2;
    r_value[3] = 2;
    gr_value[0] = 1;
    gr_value[1] = 1;
    gr_value[2] = 3;
    gr_value[3] = 3;
    gb_value[0] = 2;
    gb_value[1] = 2;
    gb_value[2] = 0;
    gb_value[3] = 0;
    b_value[0] =  3;
    b_value[1] =  3;
    b_value[2] =  1;
    b_value[3] =  1;
  }

  if (radio_box_first_pixel->IsEnabled() == true) {
    first_pixel = sensor_->FirstPixel();
  } else {
    first_pixel = radio_box_first_pixel->GetSelection();
  }

  if (image_value == 0) {
    radio_box_first_pixel->Enable(true);
    button_first_pixel_apply_->Enable(true);
  } else {
    radio_box_first_pixel->Enable(false);
    button_first_pixel_apply_->Enable(false);
  }
  if (first_pixel == 0) {
    set_first_pixel = r_value[image_value];
  } else if (first_pixel == 1) {
    set_first_pixel = gr_value[image_value];
  } else if (first_pixel == 2) {
    set_first_pixel = gb_value[image_value];
  } else if (first_pixel == 3) {
    set_first_pixel = b_value[image_value];
  }
  sensor_->SetFirstPixel(set_first_pixel);
  sensor_->orien_reg_ = image_value;
  sensor_->orien_reg_temp_ = image_value;
}

/**
 * @brief
 * To set the image orientation in the apply button is pressed.
 */
void SensorSettingsWnd::OnApplyImageOrientation(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnApplyImageOrientation\n");
  SetImageOrientation();
  WriteSensorParamtoFile(wxT(kSensorParamFilePath));
}

/**
 * @brief
 * To set the analog gain.
 */
void SensorSettingsWnd::SetAnalogGain() {
  DEBUG_PRINT("SensorSettingsWnd::SetAnalogGain\n");
  int value;
  value = slider_analog_gain_->GetValue();
  //static_text_analog_gain_->SetLabel(ConvertToAnalogGainDB(value));
  text_ctrl_analog_gain_->SetValue(ConvertToAnalogGainDB(value));
  sensor_->analog_gain_param_ = value;
  sensor_->analog_gain_param_temp_ = value;
}

/**
 * @brief
 * To set the analog gain in the apply button is pressed.
 */
void SensorSettingsWnd::OnSliderAnalogGain(wxScrollEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnSliderAnalogGain\n");
  SetAnalogGain();
}

/**
 * @brief
 * To set the digital gain.
 */
void SensorSettingsWnd::SetDigitalGain() {
  DEBUG_PRINT("SensorSettingsWnd::SetDigitalGain\n");
  int value;
  value = slider_digital_gain_->GetValue();
  //static_text_digital_gain_->SetLabel(ConvertToDigitalGainDB(value));
  text_ctrl_digital_gain_->SetValue(ConvertToDigitalGainDB(value));
  sensor_->digital_gain_param_ = value;
  sensor_->digital_gain_param_temp_ = value;
}

/**
 * @brief
 * To set the digital gain in the apply button is pressed.
 */
void SensorSettingsWnd::OnSliderDigitalGain(wxScrollEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnSliderDigitalGain\n");
  SetDigitalGain();
}

/**
 * @brief
 * To set the exposure time.
 */
void SensorSettingsWnd::SetExposureTime() {
  DEBUG_PRINT("SensorSettingsWnd::SetExposureTime\n");
  int value;
  value = slider_exposure_time_->GetValue();
  //static_text_exposure_time_->SetLabel(ConvertToExposureTimeMS(value));
  text_ctrl_exposure_time_->SetValue(ConvertToExposureTimeMS(value));
  sensor_->coarse_integration_time_ = value;
  sensor_->coarse_integration_time_temp_ = value;
}

/**
 * @brief
 * To set the exposure time in the apply button is pressed.
 */
void SensorSettingsWnd::OnSliderExposureTime(wxScrollEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnSliderExposureTime\n");
  SetExposureTime();
}

/**
 * @brief
 * To set the exposure time in the apply button is pressed.
 */
void SensorSettingsWnd::OnApplyAnalogGain(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnApplyAnalogGain\n");
  double value;
  double midval;
  double df = 1E-4;
  int low = 0;
  int high;
  if ((sensor_->sensor_type_ == wxT("IMX219")) ||
      (sensor_->sensor_type_ == wxT("IMX378")) ||
      (sensor_->sensor_type_ == wxT("IU233"))) {
    text_ctrl_analog_gain_->GetValue().ToDouble(&value);
    if (sensor_->sensor_type_ == wxT("IMX219")) {
      high = kImx219AnalogGainMax;
      while (low < high) {
        int mid = low + (high - low) / 2;
        wxString wxmidval = Imx219_analog_gain_[mid];
        wxmidval.ToDouble(&midval);
        if (midval < value - df) {
          low = mid + 1;
        } else if (midval < value + df) {
          low = mid;
          break;
        } else {
          high = mid - 1;
        }
      }
      slider_analog_gain_->SetValue(low);
    } else if (sensor_->sensor_type_ == wxT("IU233")) {
	  high = kIu233AnalogGainMax;
	  while (low < high) {
	    int mid = low + (high - low) / 2;
	    wxString wxmidval = Iu233_analog_gain_[mid];
	    wxmidval.ToDouble(&midval);
	    if (midval < value - df) {
	      low = mid + 1;
	    } else if (midval < value + df) {
	      low = mid;
	      break;
	    } else {
	      high = mid - 1;
	    }
	  }
	  slider_analog_gain_->SetValue(low);
    } else if (sensor_->sensor_type_ == wxT("IMX378")) {
  	  high = kImx378AnalogGainMax;
  	  while (low < high) {
  	    int mid = low + (high - low) / 2;
  	    wxString wxmidval = Imx378_analog_gain_[mid];
  	    wxmidval.ToDouble(&midval);
  	    if (midval < value - df) {
  	      low = mid + 1;
  	    } else if (midval < value + df) {
  	      low = mid;
  	      break;
  	    } else {
  	      high = mid - 1;
  	    }
  	  }
  	  slider_analog_gain_->SetValue(low);
    }
  }
  SetAnalogGain();
}

/**
 * @brief
 * To set the exposure time in the apply button is pressed.
 */
void SensorSettingsWnd::OnApplyDigitalGain(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnApplyDigitalGain\n");
  double value;
  double midval;
  double df = 1E-4;
  int low = 0;
  int high;
  if ((sensor_->sensor_type_ == wxT("IMX219")) ||
      (sensor_->sensor_type_ == wxT("IMX378")) ||
      (sensor_->sensor_type_ == wxT("IU233"))) {
    text_ctrl_digital_gain_->GetValue().ToDouble(&value);
    if (sensor_->sensor_type_ == wxT("IMX219")) {
      high = kImx219DitalGainMax;
      while (low < high) {
        int mid = low + (high - low) / 2;
        wxString wxmidval = Imx219_digital_gain_[mid];
        wxmidval.ToDouble(&midval);
        if (midval < value - df) {
          low = mid + 1;
        } else if (midval < value + df) {
          low = mid;
          break;
        } else {
          high = mid - 1;
        }
      }
      slider_digital_gain_->SetValue(low + 256);
    } else if (sensor_->sensor_type_ == wxT("IU233")) {
	  high = kIu233DitalGainMax;
	  while (low < high) {
	    int mid = low + (high - low) / 2;
	    wxString wxmidval = Iu233_digital_gain_[mid];
	    wxmidval.ToDouble(&midval);
	    if (midval < value - df) {
	      low = mid + 1;
	    } else if (midval < value + df) {
	      low = mid;
	      break;
	    } else {
	      high = mid - 1;
	    }
	  }
	  slider_digital_gain_->SetValue(low + 1);
    } else if (sensor_->sensor_type_ == wxT("IMX378")) {
      high = kImx378DitalGainMax;
      while (low < high) {
        int mid = low + (high - low) / 2;
        wxString wxmidval = Imx378_digital_gain_[mid];
        wxmidval.ToDouble(&midval);
        if (midval < value - df) {
          low = mid + 1;
        } else if (midval < value + df) {
          low = mid;
          break;
        } else {
          high = mid - 1;
        }
      }
       slider_digital_gain_->SetValue(low + 256);
    }
  }
  SetDigitalGain();
}

/**
 * @brief
 * To set the exposure time in the apply button is pressed.
 */
void SensorSettingsWnd::OnApplyExposureTime(wxCommandEvent& event) {
  DEBUG_PRINT("SensorSettingsWnd::OnApplyExposureTime\n");
  double value = 0;
  double set_value = 0;
  text_ctrl_exposure_time_->GetValue().ToDouble(&value);
  if ((sensor_->sensor_type_ == wxT("IMX219")) ||
      (sensor_->sensor_type_ == wxT("IU233"))  ||
      (sensor_->sensor_type_ == wxT("IMX378"))  ||
      (sensor_->sensor_type_ == wxT("IMX408"))) {
    if(value <= ((1.0 / sensor_->frequency_) * 1000)) {
      //set_value = value * 60 * sensor_->frame_length_line_ / 1000;line_length_regvalue_
      //set_value = value * sensor_->frequency_ * sensor_->frame_length_line_ / 1000;
      set_value = value * sensor_->frequency_ * sensor_->line_length_regvalue_ / 1000;
    } else {
	  //set_value = value * 60 * (sensor_->frame_length_line_ + sensor_->offset_) / 1000;
  	  //set_value = value * sensor_->frequency_ * (sensor_->frame_length_line_ + sensor_->offset_) / 1000;
	  set_value = value * sensor_->frequency_ * (sensor_->line_length_regvalue_ + sensor_->offset_) / 1000;
    }
	//set_value =((value / 1000) * sensor_->frequency_) * sensor_->coarse_regvalue_;
    slider_exposure_time_->SetValue(set_value);
  }
  SetExposureTime();
}

/**
 * @brief
 * Load the parameters from the sensor config ini file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool SensorSettingsWnd::LoadSensorParamfromFile(wxString file_path) {
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

  wxString sensor_settings_file_path = text_file[2];
  if (sensor_settings_file_path == wxT("")) {
    text_file.Close();
    return true;
  }

  ret = ReadSensorSettingsFile(sensor_settings_file_path);
  if (ret == false) {
    return false;
  }

  wxString first_pixel = text_file[3];
  if (first_pixel != wxT("")) {
    long value;
    first_pixel.ToLong(&value);
    radio_box_first_pixel->SetSelection(static_cast<int>(value));
    SetFirstPixel();
  }

  wxString image_orientation = text_file[4];
  if (image_orientation != wxT("")) {
    long value;
    image_orientation.ToLong(&value);
    combo_box_image_orientation_->SetSelection(static_cast<int>(value));
    SetImageOrientation();
  }

  wxString analog_gain = text_file[5];
  if (analog_gain != wxT("")) {
    long value;
    analog_gain.ToLong(&value);
    slider_analog_gain_->SetValue(static_cast<int>(value));
    SetAnalogGain();
  }

  wxString digital_gain = text_file[6];
  if (digital_gain != wxT("")) {
    long value;
    digital_gain.ToLong(&value);
    slider_digital_gain_->SetValue(static_cast<int>(value));
    SetDigitalGain();
  }

  wxString exposure_time = text_file[7];
  if (exposure_time != wxT("")) {
    long value;
    exposure_time.ToLong(&value);
    slider_exposure_time_->SetValue(static_cast<int>(value));
    SetExposureTime();
  }
  text_file.Close();
  return true;
}

/**
 * @brief
 * Write the parameters to the sensor config ini file.
 * @param file_path [in] file path.
 * @return If true, writing the file success
 */
bool SensorSettingsWnd::WriteSensorParamtoFile(wxString file_path) {
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
  sensor_->ClearPluginSettings();

  int line_count = text_file.GetLineCount();
  if (10 > line_count) {
    for (int i = 0; i < (10 - line_count); i++) {
      text_file.AddLine(wxString::Format(wxT("")));
    }
  }

  wxString sensor_settings_file_path = text_ctrl_sensor_config_->GetValue();
  text_file[2] = sensor_settings_file_path;

  {
    int value;
    wxString first_pixel;
    value = radio_box_first_pixel->GetSelection();
    first_pixel << value;
    text_file[3] = first_pixel;
  }
  if (combo_box_image_orientation_->IsEnabled() == true) {
    int value;
    wxString image_orientation;

    value = combo_box_image_orientation_->GetSelection();
    image_orientation << value;
    text_file[4] = image_orientation;
  } else {
    text_file[4] = wxT("");
  }
  if (slider_analog_gain_->IsEnabled() == true) {
    int value;
    wxString analog_gain;

    value = slider_analog_gain_->GetValue();
    analog_gain << value;
    text_file[5] = analog_gain;
  } else {
    text_file[5] = wxT("");
  }
  if (slider_digital_gain_->IsEnabled() == true) {
    int value;
    wxString digital_gain;

    value = slider_digital_gain_->GetValue();
    digital_gain << value;
    text_file[6] = digital_gain;
  } else {
    text_file[6] = wxT("");
  }
  if (slider_exposure_time_->IsEnabled() == true) {
    int value;
    wxString exposure_time;

    value = slider_exposure_time_->GetValue();
    exposure_time << value;
    text_file[7] = exposure_time;
  } else {
    text_file[7] = wxT("");
  }

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
bool SensorSettingsWnd::CheckOpenFile(wxString file_path, int open_file_type) {
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
void SensorSettingsWnd::SetSensorSettings(std::vector<wxString> params) {
  DEBUG_PRINT("SensorWnd::SetPluginSettings\n");
  bool ret = false;

  wxString sensor_settings_file_path = params[2];
  if (sensor_settings_file_path == wxT("")) {
    return;
  }

  ret = ReadSensorSettingsFile(sensor_settings_file_path);
  if (ret == false) {
    DEBUG_PRINT("Failed to open sensor settings file.\n");
    return;
  }

  wxString first_pixel = params[3];
  if (first_pixel != wxT("")) {
    long value;
    first_pixel.ToLong(&value);
    radio_box_first_pixel->SetSelection(static_cast<int>(value));
    SetFirstPixel();
  }

  wxString image_orientation = params[4];
  if (image_orientation != wxT("")) {
    long value;
    image_orientation.ToLong(&value);
    combo_box_image_orientation_->SetSelection(static_cast<int>(value));
    SetImageOrientation();
  }

  wxString analog_gain = params[5];
  if (analog_gain != wxT("")) {
    long value;
    analog_gain.ToLong(&value);
    slider_analog_gain_->SetValue(static_cast<int>(value));
    SetAnalogGain();
  }

  wxString digital_gain = params[6];
  if (digital_gain != wxT("")) {
    long value;
    digital_gain.ToLong(&value);
    slider_digital_gain_->SetValue(static_cast<int>(value));
    SetDigitalGain();
  }

  wxString exposure_time = params[7];
  if (exposure_time != wxT("")) {
    long value;
    exposure_time.ToLong(&value);
    slider_exposure_time_->SetValue(static_cast<int>(value));
    SetExposureTime();
  }
}
