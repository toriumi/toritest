/**
 * @file      save_to_avi_wnd.cpp
 * @brief     Setting window of SaveToAvi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./save_to_avi_wnd.h"
#include <string>
#include <vector>
#include "./../../logger.h"

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_UPDATE, wxNewEventType())
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_INITIALIZE, wxNewEventType())
DECLARE_LOCAL_EVENT_TYPE(CAPTURE_END, wxNewEventType())
END_DECLARE_EVENT_TYPES()
DEFINE_LOCAL_EVENT_TYPE(CAPTURE_UPDATE)
DEFINE_LOCAL_EVENT_TYPE(CAPTURE_INITIALIZE)
DEFINE_LOCAL_EVENT_TYPE(CAPTURE_END)
BEGIN_EVENT_TABLE(SaveToAviWnd, wxFrame)
EVT_CLOSE(SaveToAviWnd::OnClose)
EVT_COMMAND(wxID_ANY, CAPTURE_UPDATE, SaveToAviWnd::OnCapture)
EVT_COMMAND(wxID_ANY, CAPTURE_INITIALIZE, SaveToAviWnd::OnCaptureInit)
EVT_COMMAND(wxID_ANY, CAPTURE_END, SaveToAviWnd::OnCaptureEnd)
EVT_BUTTON(BTN_SELECT_AVI_FILE_WND_ID, SaveToAviWnd::OpenAviFile)
EVT_COMMAND_SCROLL(kSliderFpsId, SaveToAviWnd::OnSliderFps)
EVT_BUTTON(BTN_APPLY_ID, SaveToAviWnd::OnUpdate)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor for this window.
 * @param save_to_avi [in] Pointer to the SaveToAvi class
 */
SaveToAviWnd::SaveToAviWnd(SaveToAvi *save_to_avi)
    : wxFrame(NULL, WND_ID, wxT(WND_TITLE), wxPoint(WND_POINT_X, WND_POINT_Y),
              wxSize(WND_SIZE_W, WND_SIZE_H)) {
  save_to_avi_ = save_to_avi;

  // Create avi file select button
  wx_button_open_avi_file_ = new wxButton(
      this, BTN_SELECT_AVI_FILE_WND_ID, wxT(BTN_SELECT_AVI_FILE_WND_TEXT),
      wxPoint(BTN_SELECT_AVI_FILE_WND_POINT_X, BTN_SELECT_AVI_FILE_WND_POINT_Y),
      wxSize(BTN_SELECT_AVI_FILE_WND_SIZE_W, BTN_SELECT_AVI_FILE_WND_SIZE_H));

  // Create static text(file path)
  wx_static_text_file_path_ =
      new wxStaticText(this, STATIC_TEXT_AVI_FILE_PATH_ID, wxT("File:"),
                       wxPoint(STATIC_TEXT_AVI_FILE_PATH_POINT_X,
                               STATIC_TEXT_AVI_FILE_PATH_POINT_Y),
                       wxSize(STATIC_TEXT_AVI_FILE_PATH_SIZE_W,
                              STATIC_TEXT_AVI_FILE_PATH_SIZE_H));

  // Create static text(file path)
  wx_text_ctrl_file_path_ = new wxTextCtrl(
      this, TEXT_AVI_FILE_PATH_ID, wxT(""),
      wxPoint(TEXT_AVI_FILE_PATH_POINT_X, TEXT_AVI_FILE_PATH_POINT_Y),
      wxSize(TEXT_AVI_FILE_PATH_SIZE_W, TEXT_AVI_FILE_PATH_SIZE_H),
      wxTE_READONLY);

  // Create fps static box
  static_box_fps_ =
      new wxStaticBox(this, -1, wxT("Fps"), wxPoint(10, 10), wxSize(280, 50));

  // Create fps slider
  slider_fps_ = new wxSlider(this, kSliderFpsId, 600, 5, 600, wxPoint(10, 30),
                             wxSize(200, -1));

  // Create static text(fps)
  static_text_fps_ =
      new wxStaticText(this, -1, wxT("60.0"), wxPoint(220, 30), wxSize(50, 30));

  // Create static text(fps)
  static_text_fps_label_ = new wxStaticText(this, -1, wxT("(fps)"),
                                            wxPoint(250, 30), wxSize(50, 30));

  // Create apply button
  wx_button_setting_apply_ =
      new wxButton(this, BTN_APPLY_ID, wxT(BTN_APPLY_TEXT),
                   wxPoint(BTN_APPLY_POINT_X, BTN_APPLY_POINT_Y),
                   wxSize(BTN_APPLY_SIZE_W, BTN_APPLY_SIZE_H));

  LoadSettingsFromFile(wxT(kSaveToAviConfigFile));
  que_manager_ = new QueManager();
  writer_width_ = 0;
  writer_height_ = 0;
  writer_ = NULL;
  fps_ = kVideoWriterDefaultFps;
}

/**
 * @brief
 * Destructor for this window.
 */
SaveToAviWnd::~SaveToAviWnd() {
  DeleteQueue();
  if (writer_ != NULL) {
    delete writer_;
    writer_ = NULL;
  }
}

/**
 * @brief
 * Set the window name.
 * @param window_name [in] window name.
 */
void SaveToAviWnd::SetWindowName(std::string window_name) {}

/**
 * @brief
 * The handler function for EVT_CLOSE.
 */
void SaveToAviWnd::OnClose(wxCloseEvent &event) { Show(false); }

/**
 * @brief
 * Initialize routine for SaveToAviWnd.
 */
void SaveToAviWnd::InitAvi() { video_writer_flag_ = true; }

/**
 * @brief
 * Initialization routine for writing data to an AVI file
 * @param width [in] width of the video
 * @param height [in] height of the video
 */
void SaveToAviWnd::InitVideoWriter(int width, int height) {
  if (video_writer_path_ == wxT("")) {
    LOG_ERROR("[plugin:SaveToAvi] Failed to avi file path\n");
    return;
  }
  char cFilePathName[256];
  memset(cFilePathName, 0, sizeof(cFilePathName));

  /* open avi file*/
  strncpy(cFilePathName, (const char *)video_writer_path_.mb_str(),
          static_cast<int>(video_writer_path_.length()));

  if (writer_ != NULL) {
    delete writer_;
    writer_ = NULL;
  }
  writer_width_ = width;
  writer_height_ = height;
  writer_ = new cv::VideoWriter(cFilePathName, kVideoWriterNoCodec, fps_,
                                cv::Size(width, height), true);
}

/**
 * @brief
 * Enqueue cv::Mat data.
 * @param enq_data [in] cv::Mat data.
 */
bool SaveToAviWnd::Enqueue(data_t enq_data) {
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
void SaveToAviWnd::DeleteQueue() { delete que_manager_; }

/**
 * @brief
 * The handler function for local event(CAPTURE_UPDATE).
 * Display an image by using cv::imshow.
 */
void SaveToAviWnd::OnCapture(wxCommandEvent &event) {
  cv::Mat *que;

  que = que_manager_->Dequeue();
  if ((que->size().width == 0) || (que->size().height == 0)) {
    DEBUG_PRINT("size w:%d h:%d\n", que->size().width, que->size().height);
    return;
  }
  if (que->cols == 0 || que->rows == 0) {
    DEBUG_PRINT("size cols:%d rows:%d\n", que->cols, que->rows);
    return;
  }

  if (video_writer_flag_ == true) {
    DEBUG_PRINT("Init video writer :%d rows:%d\n", que->cols, que->rows);
    InitVideoWriter(static_cast<int>(que->cols), static_cast<int>(que->rows));
    video_writer_flag_ = false;
  }

  if (que->cols != writer_width_ || que->rows != writer_height_) {
    DEBUG_PRINT("size error. size cols:%d rows:%d\n", que->cols, que->rows);
    return;
  }

  if (writer_ == NULL) {
    return;
  }

  if (!writer_->isOpened()) {
    return;
  }
  *writer_ << *que;
}

/**
 * @brief
 * Post local event(CAPTURE_END) for destroy the screen on own thread.
 */
void SaveToAviWnd::PostCapture(void) {
  DEBUG_PRINT("SaveToAviWnd::PostCapture\n");
  wxCommandEvent event(CAPTURE_UPDATE);
  event.SetString(wxT("This is the data"));
  wxPostEvent(this, event);
}

/**
 * @brief
 * The handler function for local event(CAPTURE_INITIALIZE).
 * Initialize the screen settings.
 */
void SaveToAviWnd::OnCaptureInit(wxCommandEvent &event) {}

/**
 * @brief
 * Post local event(CAPTURE_INITIALIZE) for initialize the screen settings on
 * own thread.
 */
void SaveToAviWnd::PostCaptureInit(void) {
  DEBUG_PRINT("SaveToAviWnd::PostCaptureInit\n");

  wxCommandEvent event(CAPTURE_INITIALIZE);

  event.SetString(wxT("This is the init"));

  wxPostEvent(this, event);
}

/**
 * @brief
 * The handler function for local event(CAPTURE_END).
 * destroy the screen.
 */
void SaveToAviWnd::OnCaptureEnd(wxCommandEvent &event) {
  if (writer_ != NULL) {
    delete writer_;
    writer_ = NULL;
    writer_width_ = 0;
    writer_height_ = 0;
  }
}

/**
 * @brief
 * Post local event(CAPTURE_END) for destroy the screen on own thread.
 */
void SaveToAviWnd::PostCaptureEnd(void) {
  DEBUG_PRINT("SaveToAviWnd::PostCaptureEnd\n");
  wxCommandEvent event(CAPTURE_END);
  event.SetString(wxT("This is the end"));
  wxPostEvent(this, event);
}

/**
 * @brief
 * The handler function for BTN_SELECT_AVI_FILE_WND_ID.
 * Save dialog to select AVI file.
 */
void SaveToAviWnd::OpenAviFile(wxCommandEvent &event) {
  wxFileDialog *OpenDialog =
      new wxFileDialog(this, _("Select file"), wxEmptyString, wxEmptyString,
                       wxT("*.avi"), wxFD_SAVE, wxDefaultPosition);

  wx_text_ctrl_file_path_->SetValue(wxT(""));
  if (OpenDialog->ShowModal() == wxID_OK) {
    /* file open*/
    video_writer_path_ = OpenDialog->GetPath();
    if (video_writer_path_.Find(wxT(".avi")) == wxNOT_FOUND) {
      video_writer_path_ += wxT(".avi");
    }
  }
  wx_text_ctrl_file_path_->SetValue(video_writer_path_);
}

void SaveToAviWnd::OnSliderFps(wxScrollEvent &event) {
  DEBUG_PRINT("SaveToAviWnd::OnSliderFps\n");
  wxString value;
  float value2;
  value2 = static_cast<float>(slider_fps_->GetValue());
  value = wxString::Format(wxT("%.1f"), (value2 / 10));
  static_text_fps_->SetLabel(value);
}

/**
 * @brief
 * The handler function for BTN_APPLY_ID.
 * Reflect the settings, and then start to save the data.
 */
void SaveToAviWnd::OnUpdate(wxCommandEvent &event) {
  DEBUG_PRINT("SaveToAviWnd::OnUpdate\n");
  static_text_fps_->GetLabel().ToDouble(&fps_);
  video_writer_flag_ = true;
  WriteSettingsToFile(wxT(kSaveToAviConfigFile));
  this->Show(false);
}

/**
 * @brief
 * Set the list of parameter setting string for the SaveToAvi plugin.
 * @param params [in] settings string.
 */
void SaveToAviWnd::SetPluginSettings(std::vector<wxString> params) {
  // avi file
  video_writer_path_ = params[0];
  wx_text_ctrl_file_path_->SetValue(video_writer_path_);

  // slider FPS
  wxString fps;
  long value;
  float value2;

  params[1].ToLong(&value);
  slider_fps_->SetValue(static_cast<int>(value));
  value2 = static_cast<float>(value);
  fps = wxString::Format(wxT("%.1f"), (value2 / 10));
  static_text_fps_->SetLabel(fps);
  static_text_fps_->GetLabel().ToDouble(&fps_);

  WriteSettingsToFile(wxT(kSaveToAviConfigFile));
}

/**
 * @brief
 * Load the parameters from the file.
 * @param file_path [in] file path.
 * @return If true, reading the file success
 */
bool SaveToAviWnd::LoadSettingsFromFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    ret = text_file.Open(file_path);
    if (ret == false) {
      DEBUG_PRINT("Could not open file =%s\n",
                  (const char *)file_path.mb_str());
      return false;
    }
  } else {
    DEBUG_PRINT("File does not exist =%s\n", (const char *)file_path.mb_str());
    return false;
  }

  ret = text_file.Eof();
  if (ret == true) {
    DEBUG_PRINT("Blank init file\n");
    text_file.Close();
    return false;
  } else {
    // avi file
    line_str = text_file.GetFirstLine();
    video_writer_path_ = line_str;
    wx_text_ctrl_file_path_->SetValue(video_writer_path_);

    // slider FPS
    wxString fps;
    long value;
    float value2;

    line_str = text_file.GetNextLine();
    line_str.ToLong(&value);
    slider_fps_->SetValue(static_cast<int>(value));
    value2 = static_cast<float>(value);
    fps = wxString::Format(wxT("%.1f"), (value2 / 10));
    static_text_fps_->SetLabel(fps);
    static_text_fps_->GetLabel().ToDouble(&fps_);
  }
  text_file.Close();
  return true;
}

/**
 * @brief
 * Write the parameters to the file.
 * @param file_path [in] file path.
 * @return If true, writing the file success
 */
bool SaveToAviWnd::WriteSettingsToFile(wxString file_path) {
  wxTextFile text_file;
  bool ret = false;
  wxString line_str;

  ret = wxFile::Exists(file_path);
  if (ret == true) {
    printf("SaveToAviWnd file exist \n");
    ret = text_file.Open(file_path);
    if (ret == false) {
      ret = text_file.Create(file_path);
      if (ret == false) {
        printf("Fail to create file = %s \n", (const char *)file_path.mb_str());
        return false;
      }
    }
  } else {
    printf("SaveToAviWnd file not exist \n");
    ret = text_file.Create(file_path);
    if (ret == false) {
      printf("Fail to create file = %s \n", (const char *)file_path.mb_str());
      return false;
    }
  }
  text_file.Clear();
  save_to_avi_->ClearPluginSettings();

  // avi file
  line_str = video_writer_path_;
  save_to_avi_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  // slider FPS
  int value;
  line_str = wxT("");

  value = slider_fps_->GetValue();
  line_str << value;
  save_to_avi_->AddLinePluginSettings(line_str);
  text_file.AddLine(line_str);

  if (save_to_avi_->is_cloned() == false) {
    text_file.Write();
  }
  text_file.Close();

  return true;
}
