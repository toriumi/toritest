/**
 * @file      edge_enhancement_wnd.h
 * @brief     Window of EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _EDGE_ENHANCEMENT_WND_H_
#define _EDGE_ENHANCEMENT_WND_H_

#include <vector>
#include "./edge_enhancement.h"
#include "./edge_enhancement_define.h"
#include "./edge_enhancement_param.h"
#include "./include.h"

class EdgeEnhancement;

/**
 * @class EdgeEnhancementWnd
 * @brief Setting window of EdgeEnhancement plugin.
 */
class EdgeEnhancementWnd : public wxFrame {
 private:
  EdgeEnhancement* parent_;
  EdgeEnhancementParam* param_;

 public:
  /**
   * @brief
   * Constructor for this window.
   * @param parent [in] Pointer to the EdgeEnhancement class
   * @param param [in] Pointer to the EdgeEnhancementParam class
   */
  explicit EdgeEnhancementWnd(EdgeEnhancement* parent,
                              EdgeEnhancementParam* param);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~EdgeEnhancementWnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for BTN_APPLY_ID.
   */
  virtual void OnUpdate(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * Set the list of parameter setting string for the EdgeEnhancement plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

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

 protected:
  /*! UI*/
  wxStaticText* wx_static_coeff_text_;
  wxTextCtrl* wx_textctrl_coeff_text_;
  wxButton* wx_button_coeff_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();
};

#endif /* _EDGE_ENHANCEMENT_WND_H_*/
