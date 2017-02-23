/**
 * @file      bin_wnd.h
 * @brief     Window of Bin plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _BIN_WND_H_
#define _BIN_WND_H_

#include <vector>
#include "./bin.h"
#include "./bin_define.h"
#include "./include.h"

class Bin;

typedef enum {
  kNoneError = 0,
  kCouldNotOpenFileError,
  kHeaderSizeError,
  kInvalidBitCountError,
  kAllocError,
  kImageSizeError,
} FileReadError;

/**
 * @class BinWnd
 * @brief Setting window of Bin plugin.
 */
class BinWnd : public wxFrame {
 private:
  /*! Pointer to the Bin class */
  Bin* parent_; /* Not Own */
  /*! Raw Bayer image */
  cv::Mat* raw_bayer_image_;
  /*! Temporally Bayer image */
  cv::Mat* temp_raw_bayer_image_;
  /*! First pixel. */
  int first_pixel_;
  /*! Optical black value */
  int bit_count_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit BinWnd(Bin* parent);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~BinWnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for radio box(id = RBOX_BIT_COUNT_ID).
   * Select bit count.
   */
  virtual void OnSelectBitCount(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for radio box(id = RBOX_FIRST_PIXEL_ID).
   * Select bit count.
   */
  virtual void OnSelectFirstPixel(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = BTN_SELECT_RAW_FILE_WND_ID).
   * Open dialog to select RAW file.
   */
  virtual void OnOpenRawFile(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for button(id = BTN_APPLY_ID).
   * Apply selected Raw image.
   */
  virtual void OnUpdate(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * Get RAW image burrer.
   * @param image_buffer [out] RAW image buffer.
   * @return true : success to get a RAW image buffer.
   *         false : fail to get a RAW image buffer.
   */
  bool GetImageBuffer(cv::Mat* image_buffer);

  /**
   * @brief
   * Update the setting window UI to active or inactive 
   * by image processing state.
   * @param state [in] ImageProcessingState
   */
  void UpdateUIForImageProcessingState(ImageProcessingState state);

  /**
   * @brief
   * Get RAW image size.
   * @return a RAW image size.
   */
  CvSize GetImageSize(void);

  /**
   * @brief
   * Set the list of parameter setting string for the Bin plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxRadioBox* wx_radio_box_bit_count_;
  wxRadioBox* wx_radio_box_first_pixel_;
  wxButton* wx_button_open_raw_file_;
  wxStaticText* wx_static_text_file_path_;
  wxTextCtrl* wx_text_ctrl_file_path_;
  wxButton* wx_button_setting_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

  /**
   * @brief
   * Read RAW file and generate image buffer.
   * @param image_file_path [in] RAW image file path
   */
  FileReadError LoadImageData(char* image_file_path);

  /**
   * @brief
   * Load the parameters from the file.
   * @param file_path [in] file path.
   * @return If true, reading the file success
   */
  virtual bool LoadSettingsFromFile(wxString file_path);

  /**
   * @brief
   * Write the parameters to the file.
   * @param file_path [in] file path.
   * @return If true, writing the file success
   */
  virtual bool WriteSettingsToFile(wxString file_path);

  /**
   * @brief
   * Open Raw file.
   * @param wxPathName [in] Raw file path.
   * @param disp_error_dialog [in] 
   *     true  : if file can not open, show error dialog
   *     false : if file can not open, do not show any dialog.
   */
  bool OpenRawFile(wxString wxPathName, bool disp_error_dialog);
};

#endif /* _BIN_WND_H_*/
