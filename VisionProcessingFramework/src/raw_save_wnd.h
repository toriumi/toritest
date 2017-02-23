/**
 * @file      raw_save_wnd.h
 * @brief     Header for RawSaveWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _RAW_SAVE_WND_H_
#define _RAW_SAVE_WND_H_

#include "./include.h"

class MainWnd;

/**
 * @class RawSaveWnd
 * @brief This class has a raw image file save.
 */
class RawSaveWnd : public wxFrame {
 public:
  /**
   * @brief
   * Constructor.
   * @param main_wnd [in] Main window class object.
   */
  explicit RawSaveWnd(MainWnd *main_wnd);

  /**
   * @brief
   * Destructor.
   */
  virtual ~RawSaveWnd(void);

  /**
   * @brief
   * RawSave window end processing
   * @param event [in] Event parameters.
   */
  virtual void OnClose(wxCloseEvent &event);/* NOLINT */

  /**
   * @brief
   * File path dialog window open processing
   * @param event [in] Event parameters.
   */
  virtual void OnOpenFileDialog(wxCommandEvent &event);/* NOLINT */

  /**
   * @brief
   * Save as raw image processing
   * @param message [in] Error message.
   */
  void OnSave(wxString file_name);

  /**
   * @brief
   * Error message box
   * @param message [in] Error message.
   */
  void ErrorMessageBox(std::string message);

  /* Open file dialog button object*/
  wxComboBox *combo_box_output_;

  /* Open file dialog button object*/
  wxButton *button_file_dialog_;

  /* Main window object*/
  MainWnd *parent_;

  /*! Pause flag*/
  bool raw_save_pause_flag_;

 protected:
  /* Sensor static text object*/
  wxStaticText *static_text_output_;

  /* File static text object*/
  wxStaticText *static_text_file_;

  /* File text ctrl object*/
  wxTextCtrl *text_ctrl_file_;

  /* Save image button object*/
  wxButton *button_save_;

 private:
  /* Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _RAW_SAVE_WND_H_*/
