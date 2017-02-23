/**
 * @file      template_wnd.h
 * @brief     Window of Template plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _TEMPLATE_WND_H_
#define _TEMPLATE_WND_H_

#include "./include.h"
#include "./template_define.h"

class Template;

/**
 * @class TemplateWnd
 * @brief Setting window of Template plugin.
 * In this window, it implements the sample code
 * to read / write Sensor param by using I2C.
 */
class TemplateWnd : public wxFrame {
 private:
  /*! Pointer to the Template class */
  Template *parent_;

 public:
  /**
   * @fn
   * Constructor for this window.
   */
  explicit TemplateWnd(Template *parent);

  /**
   * @fn
   * Destructor for this window.
   */
  virtual ~TemplateWnd(void);

  /**
   * @brief
   * The handler function for button(id = kApplyButtonId).
   */
  virtual void OnApply(wxCommandEvent &event);   /* NOLINT */

  /**
   * @fn
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

 protected:
  /*! UI*/
  wxStaticBox *static_box_template_;
  wxStaticText *static_text_sensor_;
  wxComboBox *combo_box_sensor_;
  wxStaticText *static_text_result_;
  wxTextCtrl *text_ctrl_result_value_;
  wxButton *button_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _TEMPLATE_WND_H_*/
