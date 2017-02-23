/**
 * @file      whitebalancegain_wnd.h
 * @brief     Window of WhiteBalanceGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _WHITE_BALANCE_GAIN_WND_H_
#define _WHITE_BALANCE_GAIN_WND_H_

#include <vector>
#include "./include.h"
#include "./whitebalancegain_define.h"

class WhiteBalanceGain;

/**
 * @class WhiteBalanceGainWnd
 * @brief Setting window of WhiteBalanceGain plugin.
 */
class WhiteBalanceGainWnd : public wxFrame {
 private:
  /*! Pointer to the WhiteBalanceGain class */
  WhiteBalanceGain *white_balance_gain_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit WhiteBalanceGainWnd(WhiteBalanceGain *white_balance_gain);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~WhiteBalanceGainWnd(void);

  /**
   * @brief
   * The handler function for button(id = kDefaultButtonId).
   */
  virtual void OnDefault(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kOnePushButtonId).
   */
  virtual void OnOnePush(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = kApplyButtonId).
   */
  virtual void OnApply(wxCommandEvent &event);   /* NOLINT */

  /**
   * @brief
   * Set red value and blue value to text ctrl area.
   */
  virtual void SetTextCtrlValue(float white_balanace_gain_red_value,
                                float white_balanace_gain_blue_value);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent &event); /* NOLINT */

  /**
   * @brief
   * Set the list of parameter setting string for the Color plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxStaticBox *static_box_white_balance_with_digital_gain_;
  wxStaticText *static_text_red_;
  wxStaticText *static_text_green_;
  wxStaticText *static_text_blue_;
  wxStaticText *static_text_green_value_;
  wxTextCtrl *text_ctrl_red_value_;
  wxTextCtrl *text_ctrl_blue_value_;
  wxButton *button_default_;
  wxButton *button_one_push_;
  wxButton *button_apply_;

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
   * @param color_matrix [in] 3*3 vector strings.
   * @return If true, writing the file success
   */
  bool WriteSettingsToFile(wxString file_path);

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _WHITE_BALANCE_GAIN_WND_H_*/
