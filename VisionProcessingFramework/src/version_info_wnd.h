/**
 * @file      version_info_wnd.h
 * @brief     Header for VersionInfoWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _VERSION_INFO_WND_H_
#define _VERSION_INFO_WND_H_

#include "./include.h"

/**
 * @class VersionInfoWnd
 * @brief Version information window class
 */
//lass VersionInfoWnd : public wxDialog {
class VersionInfoWnd : public wxFrame {
 private:
 public:
  /**
   * @brief
   * Constructor.
   * @param major_ver [in] major version.
   * @param target_if_ver [in] target interface version.
   * @param min_if_ver [in] minimum interface version.
   */
  VersionInfoWnd(int major_ver, int target_if_ver, int min_if_ver);

  /**
   * @brief
   * Destructor.
   */
  virtual ~VersionInfoWnd(void);

  virtual void OnClose(wxCloseEvent &event);      /* NOLINT */
  virtual void OnCloseWnd(wxCommandEvent &event); /* NOLINT */

 protected:
  wxStaticText *static_text_application_name_;
  wxStaticText *static_text_copyright_;
  wxStaticText *static_text_version_info_;
  wxButton *button_close_;

  static const int vpf_major_ver = 1;
  static const int vpf_minor_ver = 0;
  static const int vpf_maintenance_ver = 2;

 private:
  DECLARE_EVENT_TABLE();
};

#endif /* _VERSION_INFO_WND_H_*/
