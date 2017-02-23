/**
 * @file      save_as_image_wnd.h
 * @brief     Header for SaveAsImageWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SAVE_AS_IMAGE_WND_H_
#define _SAVE_AS_IMAGE_WND_H_

#include "./include.h"
#include <opencv/highgui.h>

class MainWnd;

enum UtilConvertMode {
  UTIL_CONVERT_10U_TO_8U = 0,
  UTIL_CONVERT_10U_TO_16U,
};

/**
 * @class SaveAsImageWnd
 * @brief This class has a image file save.
 */
class SaveAsImageWnd : public wxFrame {
 public:
  /**
   * @brief
   * Constructor.
   * @param main_wnd [in] Main window class object.
   */
  explicit SaveAsImageWnd(MainWnd * main_wnd);

  /**
   * @brief
   * Destructor.
   */
  virtual ~SaveAsImageWnd(void);

  /**
   * @brief
   * Save as image window end processing
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
   * utilGetCvConvertScale.
   * @param src_image [in] src cv::Mat
   * @param cvt_mode [in] enum UtilConvertMode
   * @param shift [in] Value added to the scaled source array elements
   * @return dst cv::Mat
   */
  virtual cv::Mat* UtilGetCvConvertScale(cv::Mat* src_image, int cvt_mode,
                                         double shift);

  /**
   * @brief
   * Error message box
   * @param message [in] Error message.
   */
  void ErrorMessageBox(std::string message);

  /**
   * @brief
   * Save as image processing
   * @param message [in] Error message.
   */
  void OnSave(wxString file_name);

  /* Open file dialog button object*/
  wxComboBox *combo_box_output_;

  /* Open file dialog button object*/
  wxButton *button_file_dialog_;

  /* Main window object*/
  MainWnd *parent_;

  /*! Pause flag*/
  bool save_as_image_pause_flag_;

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

#endif /* _SAVE_AS_IMAGE_WND_H_*/
