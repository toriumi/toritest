/**
 * @file      gamma_correct_wnd.h
 * @brief     Window of GammaCorrect plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _GAMMA_CORRECT_WND_H_
#define _GAMMA_CORRECT_WND_H_

#include <vector>
#include "./gamma_correct.h"
#include "./gamma_correct_define.h"
#include "./include.h"

typedef enum {
  kFunctionMode = 0,
  kTableMode,
} GammmaCorrectMode;

typedef enum {
  // NG
  kOpenError = 0,
  kEofError,
  kInvalidLineError,
  kInvalidTokenCountError,
  // OK
  kReadableTableFile,
} GammaTableError;

class GammaCorrect;

/**
 * @class GammaCorrectWnd
 * @brief Setting window of GammaCorrect plugin.
 */
class GammaCorrectWnd : public wxFrame {
 private:
  /*! Pointer to the GammaCorrect class */
  GammaCorrect *gamma_correct_;
  /*! Coefficient, which is set by the Function mode*/
  float gamma_value_;
  /*! The path of the gamma table file */
  wxString gamma_table_path_;
  /*! Mode that has been set(function or table) */
  int selected_mode_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit GammaCorrectWnd(GammaCorrect *gamma_correct);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~GammaCorrectWnd(void);

  /**
   * @brief
   * The handler function for EVT_RADIO_BUTTON_FUNC.
   */
  virtual void OnRadioButtonFunction(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_RADIO_BUTTON_TABLE.
   */
  virtual void OnRadioButtonTable(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_BUTTON.
   */
  virtual void OnApply(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

  /**
   * @brief
   * Get the selected mode.
   * @return select mode(func or table)
   */
  int SelectMode(void);

  /**
   * @brief
   * Whether the table file exists.
   * @return If true, table file exists.
   */
  bool IsExistTableFile(void);

  /**
   * @brief
   * Set the list of parameter setting string for the GammaCorrect plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxStaticBox *static_box_;
  wxRadioButton *radio_button_function_;
  wxTextCtrl *text_ctrl_function_;
  wxRadioButton *radio_button_table_;
  wxButton *button_apply_;

 private:
  /**
   * @brief
   * Load the parameters from the file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  bool LoadSettingsFromFile(wxString file_path);

  /**
   * @brief
   * Write the parameters to the file.
   * @param file_path [in] file path.
   * @return If true, writing the file success
   */
  bool WriteSettingsToFile(wxString file_path);

  /**
   * @brief
   * Create the gamma lut table(8/10bit)
   * @param value [in] gamma value.
   */
  void CreateGammaTable(float value);

  /**
   * @brief
   * Check that the gamma table file can be read.
   * @param gamma_table_file [in] text file read from a gamma table file.
   * @return GammaTableError
   */
  GammaTableError CheckGammaTableFile(wxTextFile *gamma_table_file);

  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _GAMMA_CORRECT_WND_H_*/
