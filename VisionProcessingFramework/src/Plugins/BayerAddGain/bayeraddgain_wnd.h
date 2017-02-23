/**
 * @file      bayeraddgain_wnd.h
 * @brief     Window of BayerAddGain plugin
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _BAYER_ADD_GAIN_WND_H_
#define _BAYER_ADD_GAIN_WND_H_

#include <vector>

#include "./bayeraddgain_define.h"
#include "./include.h"

class BayerAddGain;

/**
 * @class BayerAddGainWnd
 * @brief Setting window of BayerAddGain plugin.
 */
class BayerAddGainWnd : public wxFrame {
 private:
  /*! Pointer to the BayerAddGain class */
  BayerAddGain *parent_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit BayerAddGainWnd(BayerAddGain *bayer_add_gain);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~BayerAddGainWnd(void);

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
   * Set optical black value to static text.
   * @param optical black value
   */
  virtual void OnOpticalBlack(int optical_black_value); /* NOLINT */

  /**
   * @brief
   * Set the list of parameter setting string for the BayerAddGain plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxStaticBox *static_box_bayer_add_gain_;
  wxStaticText *static_text_pixel_value_;
  wxStaticText *static_text_optical_black1_;
  wxStaticText *static_text_optical_black2_;
  wxStaticText *static_text_asta_;
  wxStaticText *static_text_plus_;
  wxTextCtrl *text_ctrl_bayer_add_gain_value_;
  wxButton *button_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

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
};

#endif /* _BAYER_ADD_GAIN_WND_H_*/
