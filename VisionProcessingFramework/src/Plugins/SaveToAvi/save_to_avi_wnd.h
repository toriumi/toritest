/**
 * @file      save_to_avi_wnd.h
 * @brief     Setting window of SaveToAvi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#ifndef _SAVE_TO_AVI_WND_H_
#define _SAVE_TO_AVI_WND_H_

#include <opencv/cv.h>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "./include.h"
#include "./que_manager.h"
#include "./save_to_avi.h"
#include "./save_to_avi_define.h"

class SaveToAvi;

/**
 * @class SaveToAviWnd
 * @brief Setting window of SaveToAvi plugin.
 */
class SaveToAviWnd : public wxFrame {
 private:
  /*! Pointer to the QueManager class */
  QueManager* que_manager_;
  /*! whether can be created the AVI file. */
  bool video_writer_flag_;
  /*! Output path of the AVI file */
  wxString video_writer_path_;
  /*! Pointer to the OpenCV VideoWriter class */
  cv::VideoWriter* writer_;
  /*! Video of fps */
  double fps_;
  /*! Width of video */
  int writer_width_;
  /*! Height of video*/
  int writer_height_;
  /*! Pointer to the SaveToAvi class */
  SaveToAvi* save_to_avi_;

 public:
  /**
   * @brief
   * Constructor for this window.
   * @param save_to_avi [in] Pointer to the SaveToAvi class
   */
  explicit SaveToAviWnd(SaveToAvi* save_to_avi);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~SaveToAviWnd(void);

  /**
   * @brief
   * Initialize routine for SaveToAviWnd.
   */
  virtual void InitAvi(void);

  /**
   * @brief
   * Post local event(CAPTURE_UPDATE) for refresh screen on own thread.
   */
  virtual void PostCapture(void);

  /**
   * @brief
   * Post local event(CAPTURE_INITIALIZE) for initialize the screen settings on
   * own thread.
   */
  virtual void PostCaptureInit(void);

  /**
   * @brief
   * Post local event(CAPTURE_END) for destroy the screen on own thread.
   */
  virtual void PostCaptureEnd(void);

  /**
   * @brief
   * The handler function for EVT_CLOSE.
   */
  virtual void OnClose(wxCloseEvent& event); /* NOLINT */

  /**
   * @brief
   * Enqueue cv::Mat data.
   * @param enq_data [in] cv::Mat data.
   */
  bool Enqueue(data_t enq_data);

  /**
   * @brief
   * Set the window name.
   * @param window_name [in] window name.
   */
  void SetWindowName(std::string window_name);

  /**
   * @brief
   * Set the list of parameter setting string for the SaveToAvi plugin.
   * @param params [in] settings string.
   */
  void SetPluginSettings(std::vector<wxString> params);

 protected:
  /*! UI*/
  wxButton* wx_button_open_avi_file_;
  wxStaticText* wx_static_text_file_path_;
  wxTextCtrl* wx_text_ctrl_file_path_;
  wxStaticBox* static_box_fps_;
  wxSlider* slider_fps_;
  wxStaticText* static_text_fps_;
  wxStaticText* static_text_fps_label_;
  wxButton* wx_button_setting_apply_;

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

  /**
   * @brief
   * The handler function for BTN_SELECT_AVI_FILE_WND_ID.
   * Save dialog to select AVI file.
   */
  virtual void OpenAviFile(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for BTN_APPLY_ID.
   * Reflect the settings, and then start to save the data.
   */
  virtual void OnUpdate(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for kSliderFpsId.
   * Set the fps of video with the slider.
   */
  virtual void OnSliderFps(wxScrollEvent& event); /* NOLINT */

  /**
   * @brief
   * Initialization routine for writing data to an AVI file
   * @param width [in] width of the video
   * @param height [in] height of the video
   */
  virtual void InitVideoWriter(int width, int height);

  /**
   * @brief
   * Delete cv::Mat data queue.
   */
  void DeleteQueue();

  /**
   * @brief
   * The handler function for local event(CAPTURE_UPDATE).
   * Display an image by using cv::imshow.
   */
  virtual void OnCapture(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for local event(CAPTURE_INITIALIZE).
   * Initialize the screen settings.
   */
  virtual void OnCaptureInit(wxCommandEvent& event); /* NOLINT */

  /**
   * @brief
   * The handler function for local event(CAPTURE_END).
   * destroy the screen.
   */
  virtual void OnCaptureEnd(wxCommandEvent& event); /* NOLINT */

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

#endif /* _SAVE_TO_AVI_WND_H_*/
