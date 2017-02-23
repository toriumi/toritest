/**
 * @file      direct_access_wnd.cpp
 * @brief     Source for DirectAccessWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <wx/font.h>
#include <string>
#include <vector>
#include "./main_wnd.h"
#include "./direct_access_wnd.h"
#include "./direct_access_wnd_define.h"

BEGIN_EVENT_TABLE(DirectAccessWnd, wxFrame)
EVT_CLOSE(DirectAccessWnd::OnClose)
EVT_BUTTON(kButtonReadId, DirectAccessWnd::OnRead)
EVT_BUTTON(kButtonWriteId, DirectAccessWnd::OnWrite)
END_EVENT_TABLE()

extern "C" {
int __CCIRegReadBySlaveAddress(int CCISalveAddress, int RegAddress, int *data);
int __CCIRegWriteBySlaveAddress(int CCISalveAddress, int RegAddress, int data);
}

/**
 * @brief
 * Constructor for this window.
 */
DirectAccessWnd::DirectAccessWnd(MainWnd *main_wnd)
    : wxFrame(NULL, kWndId, wxT(kWndSettingsTitle),
              wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH)) {
	  parent_ = main_wnd;
	  static_text_slave_address_ = new wxStaticText(this, kTextSalveAddressId, wxT(kStaticTextSalveAddressName),
	                                         wxPoint(kStaticTextSalveAddressPointX, kStaticTextSalveAddressPointY), wxSize(kStaticTextSalveAddressSizeW, kStaticTextSalveAddressSizeH));
	  static_text_address_       = new wxStaticText(this, kTextRegisterAddressId, wxT(kStaticTextRegisterAddressName),
	                                         wxPoint(kStaticTextRegisterAddressPointX, kStaticTextRegisterAddressPointY), wxSize(kStaticTextRegisterAddressSizeW, kStaticTextRegisterAddressSizeH));
	  static_text_data_          = new wxStaticText(this, kTextDataId, wxT(kStaticTextDataName),
	                                         wxPoint(kStaticTextDataPointX, kStaticTextDataPointY), wxSize(kStaticTextDataSizeW, kStaticTextDataSizeH));

	  text_ctrl_slave_address_ =
	      new wxTextCtrl(this, -1, wxT("0x20"), wxPoint(kTextSalveAddressPointX, kTextSalveAddressPointY), wxSize(kTextSalveAddressSizeW, kTextSalveAddressSizeH),
	                     wxTE_PROCESS_TAB | wxTE_LEFT);
	  text_ctrl_address_ =
	      new wxTextCtrl(this, -1, wxT("0x0000"), wxPoint(kTextRegisterAddressPointX, kTextRegisterAddressPointY), wxSize(kTextRegisterAddressSizeW, kTextRegisterAddressSizeH),
	                     wxTE_PROCESS_TAB | wxTE_LEFT);
	  text_ctrl_data_ =
	      new wxTextCtrl(this, -1, wxT("0x00"), wxPoint(kTextDataPointX, kTextDataPointY), wxSize(kTextDataSizeW, kTextDataSizeH),
	                     wxTE_PROCESS_TAB | wxTE_LEFT);

	  button_read_ = new wxButton(this, kButtonReadId, wxT(kButtonReadName),
	                               wxPoint(kButtonReadPointX, kButtonReadPointY), wxSize(kButtonReadSizeW, kButtonReadSizeH));

	  button_write_ = new wxButton(this, kButtonWriteId, wxT(kButtonWriteName),
	                               wxPoint(kButtonWritePointX, kButtonWritePointY), wxSize(kButtonWriteSizeW, kButtonWriteSizeH));

	  //button_set_ = new wxButton(this, kSetButtonId, wxT("Set"),
	  //                             wxPoint(10+25+25+64+25+64, 10+25+25+25+25), wxSize(64, 25));

}

/**
 * @brief
 * Destructor for this window.
 */
DirectAccessWnd::~DirectAccessWnd() {}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void DirectAccessWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Register Read
 */
void DirectAccessWnd::OnRead(wxCommandEvent& event) {
  DEBUG_PRINT("DirectAccessWnd::OnRead\n");
  int regdata = 0x00;
  int slave_address = 0;
  int address = 0x0000;

  sscanf(text_ctrl_slave_address_->GetLineText(0).ToAscii(), "%x", &slave_address);
  if (slave_address <= 0xff) {
    slave_address >>= 1;
  }
  else {
    DEBUG_PRINT("read: invalid slave address\n");
    return;
  }

  sscanf(text_ctrl_address_->GetLineText(0).ToAscii(), "%x", &address);
  sscanf(text_ctrl_data_->GetLineText(0).ToAscii(), "%x", &regdata);
  if (regdata > 0xff || regdata < 0) {
    DEBUG_PRINT("read: invalid data\n");
    return;
  }

  if (address > 0xffff || address < 0) {
    DEBUG_PRINT("read: invalid address\n");
    return;
  }

  DEBUG_PRINT("read: slave=%02x, address=%04x\n", slave_address, address);
  if (__CCIRegReadBySlaveAddress(slave_address, address, &regdata) > 0) {
    DEBUG_PRINT("read: slave=%02x, address=%04x, RegData = %02x\n", slave_address, address, regdata);
    text_ctrl_data_->SetValue(wxString::Format(wxT("0x%02x"), regdata));
  }
  else {
    DEBUG_PRINT("read: error!\n");
  }
}

/**
 * @brief
 * Register Write
 */
void DirectAccessWnd::OnWrite(wxCommandEvent& event) {
  DEBUG_PRINT("DirectAccessWnd::OnWrite\n");
  int regdata = 0x00;
  int slave_address = 0;
  int address = 0x0000;

  sscanf(text_ctrl_slave_address_->GetLineText(0).ToAscii(), "%x", &slave_address);
  if (slave_address <= 0xff) {
    slave_address >>= 1;
  }
  else {
    DEBUG_PRINT("write: invalid slave address\n");
    return;
  }

  sscanf(text_ctrl_address_->GetLineText(0).ToAscii(), "%x", &address);
  sscanf(text_ctrl_data_->GetLineText(0).ToAscii(), "%x", &regdata);
  if (regdata > 0xff || regdata < 0) {
    DEBUG_PRINT("write: invalid data\n");
    return;
  }

  if (address > 0xffff || address < 0) {
    DEBUG_PRINT("write: invalid address\n");
    return;
  }

  if (__CCIRegWriteBySlaveAddress(slave_address, address, regdata) > 0) {
    DEBUG_PRINT("write: slave=%02x, address=%04x, RegData = %02x\n", slave_address, address, regdata);
    if (__CCIRegReadBySlaveAddress(slave_address, address, &regdata) > 0) {
      DEBUG_PRINT("read: slave=%02x, address=%04x, RegData = %02x\n", slave_address, address, regdata);
      text_ctrl_data_->SetValue(wxString::Format(wxT("0x%02x"), regdata));
    }
  }
  else {
    DEBUG_PRINT("write: error!\n");
  }
}
