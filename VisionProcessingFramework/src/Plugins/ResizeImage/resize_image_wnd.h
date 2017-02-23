/**
 * @file      resize_image_wnd.h
 * @brief     Window of ResizeImage plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _RESIZE_IMAGE_WND_H_
#define _RESIZE_IMAGE_WND_H_

#include <vector>
#include "./include.h"
#include "./resize_image_define.h"

class ResizeImage;

/**
 * @class ResizeImageWnd
 * @brief Setting window of ResizeImage plugin.
 */
class ResizeImageWnd : public wxFrame {
 private:
  /*! Pointer to the ResizeImage class */
  ResizeImage* parent_;
  /*! It manages the resize type of specific.
      (enable after Apply button clicked) */
  ResizeImageType resize_type_;
  /*! It manages the resize type of specific.(temporally) */
  ResizeImageType temp_resize_type_;
  /*! It manages the selected value of percent. */
  int slider_resize_value_;
  /*! It manages the selected value of how to resize. */
  int how_to_resize_;

 protected:
  wxRadioBox* wx_radio_select_how_to_resize_;
  wxRadioBox* wx_radio_select_resize_;
  wxButton* wx_button_setting_apply_;
  wxStaticBox* wx_static_box_slider_resize_;
  wxSlider* wx_slider_resize_;
  wxStaticText* wx_static_text_slider_resize_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit ResizeImageWnd(ResizeImage* parent);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~ResizeImageWnd(void);

  /**
   * @brief
   * The handler function for button(id = kButtonApplyId).
   */
  virtual void OnApply(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for radio box(id = kRboxResizeId).
   */
  virtual void OnSelectResizeValue(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for radio box(id = kRboxHowToResizeId).
   */
  void OnSelectResizeMode(wxCommandEvent &event); /* NOLINT */

  /**
   * @brief
   * The handler function for slider(id = kSliderResizeId).
   */
  void OnSliderResize(wxScrollEvent& event); /* NOLINT */

  /**
   * @brief
   * Apply resize slider value.
   */
  void UpdateSliderResizeValue(void);

  /**
   * @brief
   * Set the list of parameter setting string for the Resize plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

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

#endif /* _RESIZE_IMAGE_WND_H_*/
