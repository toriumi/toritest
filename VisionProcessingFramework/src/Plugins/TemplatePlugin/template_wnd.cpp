/**
 * @file      template_wnd.cpp
 * @brief     Window of Template plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./template.h"
#include "./template_wnd.h"

BEGIN_EVENT_TABLE(TemplateWnd, wxFrame)
EVT_CLOSE(TemplateWnd::OnClose)
EVT_BUTTON(kApplyButtonId, TemplateWnd::OnApply)
END_EVENT_TABLE();

/********************************************************/
/*  I2C definition                                      */
/********************************************************/
#define IMX219_I2C_ADDR 0x10
#define IU233_I2C_ADDR 0x10
#define IMX408_I2C_ADDR 0x1A
#define IMX290_I2C_ADDR 0x1A
extern "C" {
int __CCIRegReadBySlaveAddress(int CCISalveAddress, int RegAddress, int *data);
int __CCIRegWriteBySlaveAddress(int CCISalveAddress, int RegAddress, int data);
}

/**
 * @fn
 * Constructor for this window.
 */
TemplateWnd::TemplateWnd(Template *parent)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
  wxTextFile text_file;

  parent_ = parent;

  static_box_template_ = new wxStaticBox(this, -1, wxT(kPluginName),
                                         wxPoint(10, 10), wxSize(620, 440));

  static_text_sensor_ = new wxStaticText(this, -1, wxT(kStringSensor),
                                         wxPoint(30, 40), wxSize(60, 25));

  wxString sensor_choice[kSensorCountNum];
  sensor_choice[0] = wxT("IMX219");
  sensor_choice[1] = wxT("IU233");
  sensor_choice[2] = wxT("IMX408");
  sensor_choice[3] = wxT("IMX290");

  combo_box_sensor_ =
      new wxComboBox(this, kComboBoxSensorId, wxEmptyString, wxPoint(100, 40),
                     wxSize(130, 30), kSensorCountNum, sensor_choice,
                     wxCB_DROPDOWN, wxDefaultValidator, wxT(""));
  combo_box_sensor_->SetSelection(0);

  button_apply_ = new wxButton(this, kApplyButtonId, wxT(kStringApply),
                               wxPoint(50, 70), wxSize(200, 30));

  static_text_result_ = new wxStaticText(this, -1, wxT(kStringResult),
                                         wxPoint(30, 100), wxSize(60, 25));

  text_ctrl_result_value_ =
      new wxTextCtrl(this, -1, wxT(""), wxPoint(50, 130), wxSize(550, 300),
                     wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
}

/**
 * @fn
 * Destructor for this window.
 */
TemplateWnd::~TemplateWnd() {}

/**
 * @brief
 * The handler function for button(id = kApplyButtonId).
 */
void TemplateWnd::OnApply(wxCommandEvent &event) {
  DEBUG_PRINT("TemplateWnd::OnApply \n");

  /********************************************************/
  /*  Read/Write sensor by using I2C                      */
  /********************************************************/
  int RegData;
  int slave_address;
  wxString wx_string;
  int i;

  // Get sensor type
  switch (combo_box_sensor_->GetSelection()) {
    case 0:
      slave_address = IMX219_I2C_ADDR;
      break;
    case 1:
      slave_address = IU233_I2C_ADDR;
      break;
    case 2:
      slave_address = IMX408_I2C_ADDR;
      break;
    case 3:
      slave_address = IMX290_I2C_ADDR;
      break;
    default:
      return;
  }

  text_ctrl_result_value_->Clear();

  for (i = 0; i < 10; i++) {
    if (__CCIRegReadBySlaveAddress(slave_address, i * 4, &RegData) > 0) {
      printf(
          "[Read]CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: "
          "0x%.2x\n",
          slave_address, i * 4, RegData);

      wx_string = wxString::Format(wxT("[Read]CCI Slave Address: 0x%x, Reg "
                                       "Address: 0x%.4x, Reg Data: 0x%.2x\n"),
                                   slave_address, i * 4, RegData);
      text_ctrl_result_value_->AppendText(wx_string);

    } else {
      printf("Error.\n");
      text_ctrl_result_value_->AppendText(wxT("Error\n"));
      break;
    }
  }

  RegData = 0x0;
  if (__CCIRegWriteBySlaveAddress(slave_address, 0x0, RegData) > 0) {
    printf(
        "[Write]CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: "
        "0x%.2x\n",
        slave_address, 0x0, RegData);

    wx_string = wxString::Format(wxT("[Write]CCI Slave Address: 0x%x, Reg "
                                     "Address: 0x%.4x, Reg Data: 0x%.2x\n"),
                                 slave_address, 0x0, RegData);
    text_ctrl_result_value_->AppendText(wx_string);
  }

  RegData = 0x02;
  if (__CCIRegWriteBySlaveAddress(slave_address, 0x0, RegData) > 0) {
    printf(
        "[Write]CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: "
        "0x%.2x\n",
        slave_address, 0x0, RegData);

    wx_string = wxString::Format(wxT("[Write]CCI Slave Address: 0x%x, Reg "
                                     "Address: 0x%.4x, Reg Data: 0x%.2x\n"),
                                 slave_address, 0x0, RegData);
    text_ctrl_result_value_->AppendText(wx_string);
  }

  for (i = 0; i < 10; i++) {
    if (__CCIRegReadBySlaveAddress(slave_address, i * 4, &RegData) > 0) {
      printf(
          "[Read]CCI Slave Address: 0x%x, Reg Address: 0x%.4x, Reg Data: "
          "0x%.2x\n",
          slave_address, i * 4, RegData);

      wx_string = wxString::Format(wxT("[Read]CCI Slave Address: 0x%x, Reg "
                                       "Address: 0x%.4x, Reg Data: 0x%.2x\n"),
                                   slave_address, i * 4, RegData);
      text_ctrl_result_value_->AppendText(wx_string);

    } else {
      printf("Error.\n");
      text_ctrl_result_value_->AppendText(wxT("Error\n"));
      break;
    }
  }
}

/**
 * @fn
 * The handler function for EVT_CLOSE.
 */
void TemplateWnd::OnClose(wxCloseEvent &event) { Show(false); }
