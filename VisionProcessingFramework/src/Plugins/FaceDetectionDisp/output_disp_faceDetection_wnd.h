/**
 * @file      output_disp_faceDetection_wnd.h
 * @brief     OpenCV displaying window.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#ifndef _OUTPUT_DISP_FACEDETECTION_WND_H_
#define _OUTPUT_DISP_FACEDETECTION_WND_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <string> /* NOLINT */
#include "./include.h"
#include "./output_disp_faceDetection.h"
#include "./output_disp_faceDetection_define.h"
#include "./que_manager.h"

class OutputDispFaceDetection;

/**
 * @class OutputDispfaceDetectionWnd
 * @brief OpenCV displaying window.
 */
class OutputDispFaceDetectionWnd : public wxFrame {
 private:
  /*! Management OpenCV image que */
  QueManager* que_manager_;
  /*! Frame counter of this plugin. use this for FPS calculation. */
  int frame_count_;
  /*! Set a current time by GetTickCount() in  FPS captureing process. */
  int64 now_time_;
  /*! Set a elapsed time in FPS captureing process. */
  int64 diff_time_;
  /*! Set a FPS of this plugin. */
  float fps_;
  /*! Frequency. use this for FPS calculation. */
  double frequency_;
  /*! Set a start time to calculate FPS. */
  int64 start_time_;

#ifdef _INHIBIT_POST_EVENT_
  /*! Flag for suppressing the wxCommandEvent(CAPTURE_UPDATE) */
  bool is_send_capture_update;
#endif /* _INHIBIT_POST_EVENT_ */

 public:
  /**
   * @brief
   * Constructor for this window.
   */
  explicit OutputDispFaceDetectionWnd(OutputDispFaceDetection* owner_plugin);

  /**
   * @brief
   * Destructor for this window.
   */
  virtual ~OutputDispFaceDetectionWnd(void);

  /**
   * @brief
   * Post local event(CAPTURE_UPDATE) for refresh screen on own thread.
   */
  virtual void PostCapture(void);

  /**
   * @brief
   * Post local event(CAPTURE_INITIALIZE) for initialize
   * the screen settings on own thread.
   */
  virtual void PostCaptureInit(void);

  /**
   * @brief
   * Post local event(CAPTURE_END) for destroy the screen on own thread.
   */
  virtual void PostCaptureEnd(void);

  /**
   * @brief
   * Calculate and draw the FPS in the upper left on capture_image.
   * @param capture_image [in] image data.
   */
  virtual void CaptureFps(cv::Mat* capture_image);

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
   * Set window name.
   * @param window_name [in] window name.
   */
  void SetWindowName(std::string window_name);

 private:
  /*! Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE();

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
   * Mouse callback function.
   * @param event [in] CV_EVENT.
   * @param x [in] x-coordinate of the window.
   * @param y [in] y-coordinate of the window
   * @param flags [in] CV_EVENT_FLAG.
   * @param param [in] unused.
   */
  static void MouseCallback(int event, int x, int y, int flags, void* param);

  /**
   * @brief
   * Draw one push rectangle on img.
   * @param img [in] image data.
   */
  static void DrawOnepushRectangle(cv::Mat* img);
};

#endif /* _OUTPUT_DISP_FACEDETECTION_WND_H_*/
