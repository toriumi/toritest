/**
 * @file      demosaic_wnd.h
 * @brief     Window of Demosaic plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _DEMOSAIC_WND_H_
#define _DEMOSAIC_WND_H_

#include <vector>
#include "./demosaic.h"
#include "./demosaic_define.h"
#include "./include.h"

class Demosaic;

/**
 * @class DemosaicWnd
 * @brief Setting window of Demosaic plugin.
 */
class DemosaicWnd : public wxFrame {
 private:
  /*! Pointer to the Demosaic class */
  Demosaic *demosaic_;
  /*! Represents the kind of color that you want to convert.*/
  int color_type_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit DemosaicWnd(Demosaic *demosaic_);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~DemosaicWnd(void);

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
   * Update the setting window UI to active or inactive
   * by image processing state.
   * @param state [in] ImageProcessingState
   */
  virtual void UpdateUIForImageProcessingState(ImageProcessingState state);

  /**
   * @brief
   * Set the color that you want to convert.
   * @param color_type [in] kind of color
   */
  bool set_color_type(int color_type);

  /**
   * @brief
   * Get the color that you want to convert.
   * @return color_type
   */
  int color_type(void) { return color_type_; }

  /**
   * @brief
   * Set the list of parameter setting string for the Demosaic plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxStaticBox *static_box_;
  wxRadioButton *radio_button_rgb_;
  wxRadioButton *radio_button_bgr_;
  wxButton *button_apply_;
  wxRadioBox *radio_box_color_;

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

#endif /* _DEMOSAIC_WND_H_*/
