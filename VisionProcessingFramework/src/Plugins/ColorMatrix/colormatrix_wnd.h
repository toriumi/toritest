/**
 * @file      colormatrix_wnd.h
 * @brief     Window of ColorMatrix plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _COLOR_MATRIX_WND_H_
#define _COLOR_MATRIX_WND_H_

#include <vector>
#include "./colormatrix_define.h"
#include "./include.h"

class ColorMatrix;

/**
 * @class ColorMatrixWnd
 * @brief Setting window of ColorMatrix plugin.
 */
class ColorMatrixWnd : public wxFrame {
 private:
  /*! Pointer to the ColorMatrix class */
  ColorMatrix *parent_;
  /*! For special settings. If true is set, Hide the value of the colormatrix*/
  bool special_flg;
  /*! Color matrix table */
  float color_matrix_[3][3];
  /*! Temporally color matrix table */
  float temp_color_matrix_[3][3];

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit ColorMatrixWnd(ColorMatrix *color_matrix);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~ColorMatrixWnd(void);

  /**
   * @brief
   * The handler function for button(id = kButtonApplyId).
   */
  virtual void OnApply(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonDefaultId).
   */
  virtual void OnDefault(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kButtonFileDialogId).
   * Open dialog to select Color matrix file.
   */
  virtual void OnOpenFileDialog(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * Set the list of parameter setting string for the Color plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

  /**
   * @brief
   * Set the list of parameter setting string for the Color plugin.
   * @param params [in] settings string.
   * @param color_matrix [in] 3*3 vector strings.
   */
  bool LoadSettingsFromStrings(std::vector<wxString> params,
                               float color_matrix[3][3]);

 private:
  /**
   * @brief
   * Load hide flag from color matrix file.
   * @param file_path [in] file path.
   */
  virtual bool LoadHideFlag(wxString file_path);

  /**
   * @brief
   * Check open file.
   * @param file_path [in] file path.
   */
  virtual bool CheckOpenFile(wxString file_path, int open_file_type);

  /**
   * @brief
   * Load the parameters from the file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  virtual bool LoadSettingsFromFile(wxString file_path,
                                    float color_matrix[3][3]);

  /**
   * @brief
   * Write the parameters to the file.
   * @param file_path [in] file path.
   * @param color_matrix [in] 3*3 vector strings.
   * @return If true, writing the file success
   */
  virtual bool WriteSettingsToFile(wxString file_path,
                                      float color_matrix[3][3]);

  /**
   * @brief
   * Apply current value.
   */
  virtual void ApplyCurrentValue(void);

 protected:
  /*! UI*/
  wxStaticText *static_text_r1_;
  wxStaticText *static_text_g1_;
  wxStaticText *static_text_b1_;
  wxStaticText *static_text_r2_;
  wxStaticText *static_text_g2_;
  wxStaticText *static_text_b2_;
  wxTextCtrl *text_ctrl_color_matrix_1st_line_1_;
  wxTextCtrl *text_ctrl_color_matrix_1st_line_2_;
  wxTextCtrl *text_ctrl_color_matrix_1st_line_3_;
  wxTextCtrl *text_ctrl_color_matrix_2nd_line_1_;
  wxTextCtrl *text_ctrl_color_matrix_2nd_line_2_;
  wxTextCtrl *text_ctrl_color_matrix_2nd_line_3_;
  wxTextCtrl *text_ctrl_color_matrix_3rd_line_1_;
  wxTextCtrl *text_ctrl_color_matrix_3rd_line_2_;
  wxTextCtrl *text_ctrl_color_matrix_3rd_line_3_;
  wxButton *button_file_dialog_;
  wxButton *button_default_;
  wxButton *button_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _COLOR_MATRIX_WND_H_*/
