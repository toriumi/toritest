/**
 * @file      direct_access_wnd.h
 * @brief     Header for DirectAccessWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _DIRECT_ACCESS_WND_H_
#define _DIRECT_ACCESS_WND_H_

#include "./include.h"

//class DirectAccess

/**
 * @class DirectAccessWnd
 * @brief Direct access window class
 */
class DirectAccessWnd : public wxFrame {
 private:

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  DirectAccessWnd(MainWnd *main_wnd);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~DirectAccessWnd(void);

  /**
   * @brief
   * The handler function for button(id = kButtonReadId).
   */
  virtual void OnRead(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonWriteId).
   */
  virtual void OnWrite(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

  /* Main window object*/
  MainWnd *parent_;

 protected:
  /*! UI*/
  //wxStaticBox *static_box_white_balance_with_digital_gain_;
  wxStaticText *static_text_slave_address_;
  wxStaticText *static_text_address_;
  wxStaticText *static_text_data_;
  wxTextCtrl *text_ctrl_slave_address_;
  wxTextCtrl *text_ctrl_address_;
  wxTextCtrl *text_ctrl_data_;
  wxButton *button_read_;
  wxButton *button_write_;

 private:
  /* Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _DIRECT_ACCESS_WND_H_*/
