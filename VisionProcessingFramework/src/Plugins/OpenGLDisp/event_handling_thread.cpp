/**
 * @file      event_handling_thread.cpp
 * @brief     EventHandlingThread class.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./event_handling_thread.h"

/**
 * @brief
 * Constructor of the ImageProcessingThread.
 * @param display [in] Pointer to the Display class
 * @param window [in] Pointer to the Window class
 * @param gc [in] Pointer to the GC class
 * @param disp_info [in] Pointer to the DisplayInfo class
 */
EventHandlingThread::EventHandlingThread(Display* display, Window window, GC gc,
                                         DisplayInfo* disp_info)
    : wxThread(wxTHREAD_JOINABLE) {
  DEBUG_PRINT("EventHandlingThread::EventHandlingThread\n");
  display_ = display;
  window_ = window;
  gc_ = gc;
  disp_info_ = disp_info;
}

/**
 * @brief
 * Destructor of the ImageProcessingThread.
 */
EventHandlingThread::~EventHandlingThread() { ; }

/**
 * @brief
 * Entry point of EventHandlingThread.
 */
wxThread::ExitCode EventHandlingThread::Entry() {
  printf("[EventHandlingThread] Start\n");
  XEvent event;
  while (!TestDestroy()) {
    while (XPending(display_)) {
      XNextEvent(display_, &event);
      switch (event.type) {
        case KeyPress:
          printf("key_press detect code = %u\n", event.xkey.keycode);
          KeySym keysym = XKeycodeToKeysym(display_, event.xkey.keycode, 0);
          disp_info_->is_update = true;
          switch (keysym) {
            // Return to the default
            case XK_Escape:
              disp_info_->disp_state = kNormal;
              disp_info_->current_alpha = disp_info_->default_alpha;
              disp_info_->current_width = disp_info_->default_width;
              disp_info_->current_height = disp_info_->default_height;
              break;
            // Hide display
            case XK_BackSpace:
              disp_info_->disp_state = kNotDisp;
              disp_info_->current_alpha = 0;
              break;
            // Move to the left
            case XK_Left:
              if (disp_info_->disp_state == kNormal ||
                  disp_info_->disp_state == kMini) {
                if (-disp_info_->disp_width + 100 < disp_info_->current_x) {
                  disp_info_->current_x -= MOVE_HORIZON;
                }
              }
              break;
            // Move to the right
            case XK_Right:
              if (disp_info_->disp_state == kNormal ||
                  disp_info_->disp_state == kMini) {
                if (disp_info_->current_x < disp_info_->disp_width) {
                  disp_info_->current_x += MOVE_HORIZON;
                }
              }
              break;
            // Move to the up
            case XK_Up:
              if (disp_info_->disp_state == kNormal ||
                  disp_info_->disp_state == kMini) {
                if (-disp_info_->disp_height + 100 < disp_info_->current_y) {
                  disp_info_->current_y -= MOVE_VERTICAL;
                }
              }
              break;
            // Move to the down
            case XK_Down:
              if (disp_info_->disp_state == kNormal ||
                  disp_info_->disp_state == kMini) {
                if (disp_info_->current_y < disp_info_->disp_height) {
                  disp_info_->current_y += MOVE_VERTICAL;
                }
              }
              break;
            // Mini display
            case XK_F10:
              disp_info_->disp_state = kMini;
              disp_info_->current_alpha = disp_info_->default_alpha;
              disp_info_->current_x = 0;
              disp_info_->current_y =
                  disp_info_->disp_height - MINI_DISP_HEIGHT;
              disp_info_->current_width = MINI_DISP_WIDTH;
              disp_info_->current_height = MINI_DISP_HEIGHT;
              break;
            // Full dcreen
            case XK_F11:
              disp_info_->disp_state = kAllDisp;
              disp_info_->current_alpha = disp_info_->default_alpha;
              disp_info_->current_x = 0;
              disp_info_->current_y = 0;
              disp_info_->current_width = disp_info_->disp_width;
              disp_info_->current_height = disp_info_->disp_height;
              break;
          }
          break;
      }
    }
  }
  return (wxThread::ExitCode)0;
} /* NOLINT */