/**
 * @file      event_handling_thread.h
 * @brief     EventHandlingThread class.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <wx/thread.h>
#include "./include.h"

typedef enum {
  kNormal = 0,
  kMini,
  kAllDisp,
  kNotDisp,
} DisplayState;

typedef struct {
  int default_x;
  int default_y;
  int default_width;
  int default_height;
  uint32_t default_alpha;
  int current_x;
  int current_y;
  int current_width;
  int current_height;
  uint32_t current_alpha;
  int disp_width;
  int disp_height;
  bool is_disp;
  bool is_update;
  DisplayState disp_state;
} DisplayInfo;

#define MOVE_HORIZON 100
#define MOVE_VERTICAL 100
#define MINI_DISP_WIDTH 320
#define MINI_DISP_HEIGHT 180

typedef bool ThreadFunc(void);

/**
 * @class EventHandlingThread
 * @brief This class implements a thread process to handle event.
 */
class EventHandlingThread : public wxThread {
 public:
  /**
   * @brief
   * Constructor of the ImageProcessingThread.
   * @param display [in] Pointer to the Display class
   * @param window [in] Pointer to the Window class
   * @param gc [in] Pointer to the GC class
   * @param disp_info [in] Pointer to the DisplayInfo class
   */
  EventHandlingThread(Display* display, Window window, GC gc,
                      DisplayInfo* disp_info);

  /**
   * @brief
   * Destructor of the ImageProcessingThread.
   */
  virtual ~EventHandlingThread(void);

  /**
   * @brief
   * Entry point of EventHandlingThread.
   */
  virtual wxThread::ExitCode Entry(void);

 private:
  /*! Pointer to the Display class (not own) */
  Display* display_;
  /*! Pointer to the Window class (not own */
  Window window_;
  /*! Pointer to the GC class (not own) */
  GC gc_;
  /*! Pointer to the DisplayInfo class (not own) */
  DisplayInfo* disp_info_;
};
