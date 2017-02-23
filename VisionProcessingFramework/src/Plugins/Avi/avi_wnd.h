/**
 * @file      avi_wnd.h
 * @brief     Window of Avi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _AVI_WND_H_
#define _AVI_WND_H_

#include <opencv2/highgui/highgui.hpp>
#include "./avi.h"
#include "./avi_define.h"
#include "./include.h"

class Avi;

typedef enum {
  kNoneError = 0,
  kCouldNotOpenFileError,
  kHeaderSizeError,
  kInvalidBitCountError,
  kAllocError,
  kImageSizeError,
} FileReadError;

/**
 * @class AviWnd
 * @brief Setting window of Avi plugin.
 */
class AviWnd : public wxFrame {
 private:
  /*! Avi image*/
  cv::Mat* avi_image_;

  /*! The last frame of the image*/
  cv::Mat* last_avi_image_;

  /*! The last frame of the Avi for reading image*/
  cv::VideoCapture *capture_image_;

  /*! The last frame of the temporary Avi for reading image*/
  cv::VideoCapture *temp_capture_image_;

  /*! Avi class object*/
  Avi* parent_;

  /*! Fps*/
  double fps_;

  /*! Wait for Fps adjustment*/
  int fps_wait_;

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit AviWnd(Avi* parent);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~AviWnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for kBtnSelectAviFileWndId.
   * Open dialog to select AVI file.
   */
  virtual void OnOpenAviFile(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for kBtnApplyId.
   * Apply selected Avi image.
   */
  virtual void OnUpdate(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * initialize avi image.
   * @param none.
   * @return true : success to initialize avi image.
   *         false : fail to initialize avi image.
   */
  bool InitializeAviImage();

  /**
   * @brief
   * Get AVI image burrer.
   * @param image_buffer [out] AVI image buffer.
   * @return true : success to get image buffer.
   *         false : fail to get image buffer.
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
   * Get AVI image size.
   * @return AVI image size.
   */
  CvSize GetImageSize(void);

  /**
   * @brief
   * Set the list of parameter setting string for the Avi plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxRadioBox* wx_radio_box_bit_count_;
  wxRadioBox* wx_radio_box_first_pixel_;
  wxButton* wx_button_open_avi_file_;
  wxStaticText* wx_static_text_file_path_;
  wxTextCtrl* wx_text_ctrl_file_path_;
  wxButton* wx_button_setting_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

  /**
   * @brief
   * Read AVI file and generate image buffer.
   * @param image_file_path [in] AVI image file path
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
   *     true  : if file can not open, display error dialog
   *     false : if file can not open, not display any dialog.
   */
  bool OpenRawFile(wxString wxPathName, bool disp_error_dialog);
};

#endif /* _AVI_WND_H_*/
