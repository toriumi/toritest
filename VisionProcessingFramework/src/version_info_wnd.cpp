/**
 * @file      version_info_wnd.cpp
 * @brief     Source for VersionInfoWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include <wx/font.h>
#include <string>
#include <vector>

#include "./version_info_wnd.h"
#include "./version_info_wnd_define.h"

//BEGIN_EVENT_TABLE(VersionInfoWnd, wxDialog)
BEGIN_EVENT_TABLE(VersionInfoWnd, wxFrame)
EVT_CLOSE(VersionInfoWnd::OnClose)
EVT_BUTTON(kButtonCloseId, VersionInfoWnd::OnCloseWnd)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor.
 * @param major_ver [in] major version.
 * @param target_if_ver [in] target interface version.
 * @param min_if_ver [in] minimum interface version.
 */
VersionInfoWnd::VersionInfoWnd(int major_ver, int target_if_ver, int min_if_ver)
//    : wxDialog(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
      : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
               wxSize(kWndSizeW, kWndSizeH)) {
  /* Create version info string */
  std::stringstream ss1, ss2;
  ss1 << "Vision Processing Framework " << vpf_major_ver << "." << vpf_minor_ver
      << "." << vpf_maintenance_ver;
  std::string application_name_str = ss1.str();
  wxString application_name_wxstr(application_name_str.c_str(), wxConvUTF8);
  ss2 << "Plugin Version information:\n";
  ss2 << "  Major version\t\t\t\t\t\t: ";
  ss2 << major_ver;
  ss2 << "\n";
  ss2 << "  Target Plugin I/F version\t\t\t\t: ";
  ss2 << target_if_ver;
  ss2 << "\n";
  ss2 << "  Minimum Required Plugin I/F version\t: ";
  ss2 << min_if_ver;
  std::string if_info_str = ss2.str();
  wxString if_info_wxstr(if_info_str.c_str(), wxConvUTF8);

  /* UI creation of the main window.*/
  /* Creating a version info static text object.*/
  static_text_application_name_ = new wxStaticText(
      this, kStaticTextApplicationNameId, application_name_wxstr,
      wxPoint(kStaticTextApplicationNamePointX,
              kStaticTextApplicationNamePointY),
      wxSize(kStaticTextApplicationNameSizeW, kStaticTextApplicationNameSizeH));

  static_text_copyright_ = new wxStaticText(
      this, kStaticTextCopyRightId, wxT(kCopyRightText),
      wxPoint(kStaticTextCopyRightPointX, kStaticTextCopyRightPointY),
      wxSize(kStaticTextCopyRightSizeW, kStaticTextCopyRightSizeH),
      wxALIGN_CENTRE);

  static_text_version_info_ = new wxStaticText(
      this, kStaticTextVersionInfoId, if_info_wxstr,
      wxPoint(kStaticTextVersionInfoPointX, kStaticTextVersionInfoPointY),
      wxSize(kStaticTextVersionInfoSizeW, kStaticTextVersionInfoSizeH));
  wxFont font = static_text_application_name_->GetFont();
  font.SetPointSize(18);
  font.SetWeight(wxFONTWEIGHT_BOLD);
  static_text_application_name_->SetFont(font);

  /* Creating a close button object.*/
  button_close_ = new wxButton(this, kButtonCloseId, wxT(kButtonCloseName),
                               wxPoint(kButtonClosePointX, kButtonClosePointY),
                               wxSize(kButtonCloseSizeW, kButtonCloseSizeH));
}

/**
 * @brief
 * Destructor.
 */
VersionInfoWnd::~VersionInfoWnd() {}

void VersionInfoWnd::OnClose(wxCloseEvent &event) { this->Show(false); }

void VersionInfoWnd::OnCloseWnd(wxCommandEvent &event) {
  DEBUG_PRINT("MainWnd::OnCloseWnd\n");
  this->Show(false);
}
