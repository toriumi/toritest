/**
 * @file      output_disp_faceDetection_wnd.cpp
 * @brief     Setting window of OutputDispFaceDetection plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./output_disp_faceDetection_wnd.h"
#include <string>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_UPDATE, wxNewEventType())
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_INITIALIZE, wxNewEventType())
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_END, wxNewEventType())
END_DECLARE_EVENT_TYPES()

DEFINE_LOCAL_EVENT_TYPE(CAPTURE_UPDATE)
DEFINE_LOCAL_EVENT_TYPE(CAPTURE_INITIALIZE)
DEFINE_LOCAL_EVENT_TYPE(CAPTURE_END)

BEGIN_EVENT_TABLE(OutputDispFaceDetectionWnd, wxFrame)
EVT_CLOSE(OutputDispFaceDetectionWnd::OnClose)
EVT_COMMAND(wxID_ANY, CAPTURE_UPDATE, OutputDispFaceDetectionWnd::OnCapture)
EVT_COMMAND(wxID_ANY, CAPTURE_INITIALIZE, OutputDispFaceDetectionWnd::OnCaptureInit)
EVT_COMMAND(wxID_ANY, CAPTURE_END, OutputDispFaceDetectionWnd::OnCaptureEnd)
END_EVENT_TABLE()

class OutputDispFaceDetection;

static cv::Mat* draw_rect_image;
static cv::Point start_point;
static cv::Point end_point;
static bool is_drawing_rect;
static OutputDispFaceDetection* owner_plugin_;
static std::string display_window_name = "";

/**
 * @brief
 * Constructor for this window.
 */
OutputDispFaceDetectionWnd::OutputDispFaceDetectionWnd(OutputDispFaceDetection* owner_plugin)
    : wxFrame(NULL, WND_ID, wxT(WND_TITLE), wxPoint(WND_POINT_X, WND_POINT_Y),
              wxSize(0, 0)) {
  owner_plugin_ = owner_plugin;

  frame_count_ = 0;
  now_time_ = 0;
  diff_time_ = 0;
  fps_ = 0;
  frequency_ = (1000 / cv::getTickFrequency());
  start_time_ = cv::getTickCount();
  is_drawing_rect = false;
  draw_rect_image = NULL;

  que_manager_ = new QueManager();
#ifdef _INHIBIT_POST_EVENT_
  is_send_capture_update = false;
#endif /* _INHIBIT_POST_EVENT_ */
}

/**
 * @brief
 * Destructor for this window.
 */
OutputDispFaceDetectionWnd::~OutputDispFaceDetectionWnd() { DeleteQueue(); }

/**
 * @brief
 * Set window name.
 * @param window_name [in] window name.
 */
void OutputDispFaceDetectionWnd::SetWindowName(std::string window_name) {
  display_window_name = window_name;
}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void OutputDispFaceDetectionWnd::OnClose(wxCloseEvent& event) { Show(false); }

/**
 * @brief
 * Enqueue cv::Mat data.
 * @param enq_data [in] cv::Mat data.
 */
bool OutputDispFaceDetectionWnd::Enqueue(data_t enq_data) {
  if (que_manager_ == NULL) {
    DEBUG_PRINT("Enqueue Image data fail : que_manager_ is NULL\n");
  } else {
    return que_manager_->Enqueue(enq_data);
  }
  return false;
}

/**
 * @brief
 * Delete cv::Mat data queue.
 */
void OutputDispFaceDetectionWnd::DeleteQueue() { delete que_manager_; }

/**
 * @brief
 * The handler function for local event(CAPTURE_UPDATE).
 * Display an image by using cv::imshow.
 */
void OutputDispFaceDetectionWnd::OnCapture(wxCommandEvent& event) {
  //  DEBUG_PRINT("OutputDispFaceDetectionWnd::OnCapture\n");
  cv::Mat* que;

  que = que_manager_->Dequeue();
  draw_rect_image = que;
  if ((que->size().width == 0) || (que->size().height == 0)) {
    DEBUG_PRINT("size w:%d h:%d\n", que->size().width, que->size().height);
    return;
  }
  if (que->cols == 0 || que->rows == 0) {
    DEBUG_PRINT("size cols:%d rows:%d\n", que->cols, que->rows);
    return;
  }

  /* fps.*/
  CaptureFps(que);
  DrawOnepushRectangle(que);
  cv::imshow(display_window_name.c_str(), *que);

  cv::waitKey(0);

  frame_count_++;
#ifdef _INHIBIT_POST_EVENT_
  is_send_capture_update = false;
#endif /* _INHIBIT_POST_EVENT_ */
}

/**
 * @brief
 * Post local event(CAPTURE_UPDATE) for refresh screen on own thread.
 */
void OutputDispFaceDetectionWnd::PostCapture(void) {
  DEBUG_PRINT("OutputDispFaceDetectionWnd::PostCapture\n");

#ifdef _INHIBIT_POST_EVENT_
  if (is_send_capture_update == false) {
#endif /* _INHIBIT_POST_EVENT_ */
    wxCommandEvent event(CAPTURE_UPDATE);

    event.SetString(wxT("This is the data"));
    wxPostEvent(this, event);
#ifdef _INHIBIT_POST_EVENT_
    is_send_capture_update = true;
  }
#endif /* _INHIBIT_POST_EVENT_ */
}

/**
 * @brief
 * The handler function for local event(CAPTURE_INITIALIZE).
 * Initialize the screen settings.
 */
void OutputDispFaceDetectionWnd::OnCaptureInit(wxCommandEvent& event) {
  cv::namedWindow(display_window_name.c_str(),
                  CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  cv::setMouseCallback(display_window_name.c_str(), MouseCallback);
  cv::waitKey(2);

  frame_count_ = 0;
  diff_time_ = 0;
  fps_ = 0;
  now_time_ = cv::getTickCount();
  start_time_ = now_time_;
}

/**
 * @brief
 * Post local event(CAPTURE_INITIALIZE) for initialize
 * the screen settings on own thread.
 */
void OutputDispFaceDetectionWnd::PostCaptureInit(void) {
  DEBUG_PRINT("OutputDispFaceDetectionWnd::PostCaptureInit\n");

  wxCommandEvent event(CAPTURE_INITIALIZE);

  event.SetString(wxT("This is the init"));

  wxPostEvent(this, event);
}

/**
 * @brief
 * The handler function for local event(CAPTURE_END).
 * destroy the screen.
 */
void OutputDispFaceDetectionWnd::OnCaptureEnd(wxCommandEvent& event) {
  cv::destroyWindow(display_window_name.c_str());
}

/**
 * @brief
 * Post local event(CAPTURE_END) for destroy the screen on own thread.
 */
void OutputDispFaceDetectionWnd::PostCaptureEnd(void) {
  DEBUG_PRINT("OutputDispFaceDetectionWnd::PostCaptureEnd\n");

  wxCommandEvent event(CAPTURE_END);

  event.SetString(wxT("This is the end"));

  wxPostEvent(this, event);
}

/**
 * @brief
 * Calculate and draw the FPS in the upper left on capture_image.
 * @param capture_image [in] image data.
 */
void OutputDispFaceDetectionWnd::CaptureFps(cv::Mat* picuture_Image) {
  //  DEBUG_PRINT("OutputDispFaceDetectionWnd::OnCaptureFps\n");
  now_time_ = cv::getTickCount();
  diff_time_ = static_cast<int>((now_time_ - start_time_) * frequency_);

  if (diff_time_ >= kDiffTimeCheck) {
    start_time_ = now_time_;
    fps_ = static_cast<float>(frame_count_) / kFrameCountNum;
    frame_count_ = 0;
  }
  std::string fps_display = "FPS:";
  std::ostringstream os;
  os << fps_;
  fps_display += os.str();

  cv::putText(*picuture_Image, fps_display,
              cv::Point(kPutTextPointW, kPutTextPointH),
              cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2, CV_AA);
}

/**
 * @brief
 * Mouse callback function.
 * @param event [in] CV_EVENT.
 * @param x [in] x-coordinate of the window.
 * @param y [in] y-coordinate of the window
 * @param flags [in] CV_EVENT_FLAG.
 * @param param [in] unused.
 */
void OutputDispFaceDetectionWnd::MouseCallback(int event, int x, int y, int flags,
                                        void* param = NULL) {
  switch (event) {
    case CV_EVENT_MOUSEMOVE:
      if ((flags & CV_EVENT_FLAG_LBUTTON) && is_drawing_rect) {
        end_point.x = x;
        end_point.y = y;
      }
      break;
    case CV_EVENT_LBUTTONDOWN:
      is_drawing_rect = true;
      start_point.x = x;
      start_point.y = y;
      end_point.x = x;
      end_point.y = y;
      break;
    case CV_EVENT_LBUTTONUP:
      end_point.x = x;
      end_point.y = y;
      is_drawing_rect = false;
      owner_plugin_->SetOnepushRectangle(start_point, end_point);
      break;
  }
}

/**
 * @brief
 * Draw one push rectangle on img.
 * @param img [in] image data.
 */
void OutputDispFaceDetectionWnd::DrawOnepushRectangle(cv::Mat* img) {
  if (is_drawing_rect == true) {
    if (img != NULL) {
      cv::rectangle(*img, start_point, end_point, cvScalar(0xff, 0x00, 0x00));
      cv::imshow(display_window_name.c_str(), *img);

      cv::waitKey(0);
    }
  }
}
