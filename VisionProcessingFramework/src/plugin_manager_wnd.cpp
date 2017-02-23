/**
 * @file      plugin_manager_wnd.cpp
 * @brief     Source for PluginManagerWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./plugin_manager_wnd.h"
#include <string>
#include <vector>
#include "./logger.h"
#include "./plugin_edit_canvas.h"
#include "./plugin_manager_wnd_define.h"

BEGIN_EVENT_TABLE(PluginManagerWnd, wxFrame)
EVT_CLOSE(PluginManagerWnd::OnClose)
EVT_BUTTON(kButtonReloadId, PluginManagerWnd::OnReloadPluginList)
EVT_BUTTON(kButtonCommonId, PluginManagerWnd::OnOpenPluginSettingsWnd)
EVT_BUTTON(kButtonSavePluginFlowId, PluginManagerWnd::OnSavePluginFlow)
EVT_BUTTON(kButtonLoadPluginFlowId, PluginManagerWnd::OnLoadPluginFlow)
END_EVENT_TABLE()

/**
 * @brief
 * Constructor.
 * @param plugin_manager [in] pointer to the plugin_manager class.
 * @param thread_running_cycle_manager [in] pointer to the
 * ThreadRunningCycleManager class.
 */
PluginManagerWnd::PluginManagerWnd(
    PluginManager *plugin_manager,
    ThreadRunningCycleManager *thread_running_cycle_manager)
    : wxFrame(NULL, kWndId, wxT(kWndTitle), wxPoint(kWndPointX, kWndPointY),
              wxSize(kWndSizeW, kWndSizeH), wxDEFAULT_FRAME_STYLE) {
  /* PluginManager class object creation.*/
  plugin_manager_ = plugin_manager;

  /* UI creation */
  list_box_plugin_list_ =
      new wxListBox(this, kListBoxPluginListId, wxDefaultPosition,
                    wxSize(kListBoxPluginListSizeW, kListBoxPluginListSizeH));
  button_reload_ =
      new wxButton(this, kButtonReloadId, wxT(kButtonReloadName),
                   wxPoint(kButtonReloadPointX, kButtonReloadPointY),
                   wxSize(kButtonReloadSizeW, kButtonReloadSizeH));

  button_plugin_settings_ =
      new wxButton(this, kButtonCommonId, wxT(kButtonCommonName),
                   wxPoint(kButtonCommonPointX, kButtonCommonPointY),
                   wxSize(kButtonCommonSizeW, kButtonCommonSizeH));

  button_save_plugin_flow_ = new wxButton(
      this, kButtonSavePluginFlowId, wxT(kStaticTextSaveFlowName),
      wxPoint(kButtonReloadPointX + kButtonReloadSizeW +
                  (kEditCanvasScreenWidth - kButtonSaveLoadFlowSizeW * 2),
              kButtonReloadPointY),
      wxSize(kButtonSaveLoadFlowSizeW, kButtonReloadSizeH));

  button_load_plugin_flow_ = new wxButton(
      this, kButtonLoadPluginFlowId, wxT(kStaticTextLoadFlowName),
      wxPoint(kButtonReloadPointX + kButtonReloadSizeW +
                  (kEditCanvasScreenWidth - kButtonSaveLoadFlowSizeW),
              kButtonReloadPointY),
      wxSize(kButtonSaveLoadFlowSizeW, kButtonReloadSizeH));

  flow_edit_canvas_ =
      new PluginEditCanvas(this, wxID_ANY, thread_running_cycle_manager);
  flow_edit_canvas_->SetPluginManagerInfo(plugin_manager);
  flow_edit_canvas_->Show(true);

  wxBoxSizer *sizer_top = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *sizer_button_col1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *sizer_button_col2 = new wxBoxSizer(wxHORIZONTAL);
  sizer_button_col1->Add(button_plugin_settings_, 0, wxALL, 3);
  sizer_button_col1->Add(button_reload_, 0, wxALL, 3);
  sizer_button_col2->Add(button_save_plugin_flow_, 0, wxALL, 3);
  sizer_button_col2->Add(button_load_plugin_flow_, 0, wxALL, 3);

  wxFlexGridSizer *sizer_flex = new wxFlexGridSizer(2, 3);
  sizer_flex->Add(new wxStaticText(this, kStaticTextPluginListId,
                                   wxT(kStaticTextPluginListName)),
                  0, wxALIGN_LEFT | wxALL, 3);
  sizer_flex->Add(
      new wxStaticText(this, kStaticTextFlowId, wxT(kStaticTextFlowName)), 0,
      wxALIGN_LEFT | wxALL, 3);
  sizer_flex->Add(list_box_plugin_list_, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 3);
  sizer_flex->Add(flow_edit_canvas_, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 3);
  sizer_flex->Add(sizer_button_col1, 0, wxALIGN_RIGHT | wxALL, 3);
  sizer_flex->Add(sizer_button_col2, 0, wxALIGN_RIGHT | wxALL, 3);
  sizer_flex->AddGrowableCol(1);
  sizer_flex->AddGrowableRow(1);
  sizer_top->Add(sizer_flex, 1, wxALL | wxEXPAND);
  SetSizerAndFit(sizer_top);
  this->SetSize(wxSize(kWndSizeW, kWndSizeH));
}

/**
 * @brief
 * Destructor.
 */
PluginManagerWnd::~PluginManagerWnd() { delete flow_edit_canvas_; }

void PluginManagerWnd::OnClose(wxCloseEvent &event) {
  this->Show(false);
  flow_edit_canvas_->Reset();
}

void PluginManagerWnd::OnReloadPluginList(wxCommandEvent &event) {
  DEBUG_PRINT("PluginManagerWnd::OnReloadPluginList\n");

  LOG_STATUS("Reload all plugins");
  flow_edit_canvas_->Reset();
  ReloadPluginListFromFile();
}

void PluginManagerWnd::OnOpenPluginSettingsWnd(wxCommandEvent &event) {
  unsigned int list_selection_num = wxNOT_FOUND;
  std::string plugin_name;
  PluginBase *plugin;

  DEBUG_PRINT("PluginManagerWnd::OnOpenCommonSettings\n");

  list_selection_num = list_box_plugin_list_->GetSelection();

  if (list_selection_num != wxNOT_FOUND) {
    plugin_name = std::string(
        list_box_plugin_list_->GetString(list_selection_num).mb_str());
    plugin = plugin_manager_->GetPlugin(plugin_name);

    plugin->OpenSettingWindow(image_proc_state_);
  }
}

/**
 * @brief
 * Add a plugin into the list box.
 * @param plugin_name [in] name of the target plugin.
 */
void PluginManagerWnd::AddPluginNameToPluginList(
    const std::string plugin_name) {
  wxArrayString wx_array_string;
  wxString wx_string(plugin_name.c_str(), wxConvUTF8);
  wx_array_string.Add(wx_string);
  list_box_plugin_list_->InsertItems(wx_array_string, 0);
}

/**
 * @brief
 * Remove a plugin from the list box.
 * @param plugin_name [in] name of the target plugin.
 */
void PluginManagerWnd::RemovePluginNameFromPluginList(
    const std::string plugin_name) {
  wxString wx_string(plugin_name.c_str(), wxConvUTF8);
  int index = wxNOT_FOUND;

  index = list_box_plugin_list_->FindString(wx_string);

  if (index != wxNOT_FOUND) {
    list_box_plugin_list_->Delete(index);
  }
}

/**
 * @brief
 * Find a plugin from the list box.
 * @param plugin_name [in] name of the target plugin.
 * @return true, if the plugin is found.
 */
bool PluginManagerWnd::FindPluginNameFromPluginList(
    const std::string plugin_name) {
  wxString wx_string(plugin_name.c_str(), wxConvUTF8);
  int index = wxNOT_FOUND;

  index = list_box_plugin_list_->FindString(wx_string);

  if (index == wxNOT_FOUND) {
    return false;
  }
  return true;
}

/**
 * @brief
 * Remove all plugin from the list box.
 */
void PluginManagerWnd::RemoveAllPluginNameFromPluginList(void) {
  list_box_plugin_list_->Clear();
}

/**
 * @brief
 * Set the ImageProcessingState.
 * @param image_proc_state [in] ImageProcessingState.
 * @return image processing state.
 */
void PluginManagerWnd::set_image_proc_state(
    ImageProcessingState image_proc_state) {
  image_proc_state_ = image_proc_state;
  if (image_proc_state_ == kStop) {
    button_reload_->Enable(true);
    flow_edit_canvas_->Enable(true);
    button_save_plugin_flow_->Enable(true);
    button_load_plugin_flow_->Enable(true);
  } else if ((image_proc_state_ == kRun) || (image_proc_state_ == kPause)) {
    button_reload_->Disable();
    flow_edit_canvas_->Reset();
    button_save_plugin_flow_->Disable();
    button_load_plugin_flow_->Disable();
  }
  flow_edit_canvas_->set_image_proc_state(image_proc_state);
  flow_edit_canvas_->PostUpdateCanvasEvent();
}

/**
 * @brief
 * Get the ImageProcessingState.
 * @return image processing state.
 */
ImageProcessingState PluginManagerWnd::image_proc_state(void) {
  return image_proc_state_;
}

/**
 * @brief
 * Replad all plugins.
 * @return image processing state.
 */
void PluginManagerWnd::ReloadPluginListFromFile(void) {
  std::string prev_root_plugin_name = "";
  // get root plugin name before reload process.
  if (plugin_manager_->root_plugin() != NULL) {
    prev_root_plugin_name = plugin_manager_->root_plugin()->plugin_name();
  } else {
    return;
  }

  /* Reload Plugin list.*/
  plugin_manager_->LoadPlugins(kPluginPath, true);

  /* Set root plugin according to the following priority. */
  /* 1.Previous root plugin */
  /* 2.Bin plugin */
  /* 3.Sensor plugin */
  /* If these plugins do not exist, not set root plugin. */
  PluginBase *prev_root_plugin =
      plugin_manager_->GetPlugin(prev_root_plugin_name);
  bool is_success = false;

  if (prev_root_plugin != NULL) {
    if (prev_root_plugin_name != "") {
      is_success = plugin_manager_->set_root_plugin(prev_root_plugin_name);
      if (is_success) {
        DEBUG_PRINT(
            "PluginManagerWnd::ReloadPluginList update root plugin(same as "
            "prev) success.\n");
      } else {
        DEBUG_PRINT(
            "PluginManagerWnd::ReloadPluginList update root plugin(same as "
            "prev) fail.\n");
      }
    }
  }
  flow_edit_canvas_->SetPluginManagerInfo(plugin_manager_);
  flow_edit_canvas_->PostUpdateCanvasEvent();
}

/**
 * @brief
 * Reset a list box and canvas.
 * @return image processing state.
 */
void PluginManagerWnd::ResetPluginList(void) {
  flow_edit_canvas_->RemovePluginFlowList(wxPoint(0, 0));
  /*  RemoveAllPluginNameFromPluginList();*/
  flow_edit_canvas_->SetPluginManagerInfo(plugin_manager_);
  flow_edit_canvas_->PostUpdateCanvasEvent();
}

void PluginManagerWnd::OnSavePluginFlow(wxCommandEvent &event) {
  LOG_STATUS("Save plugin flow");

  wxFileDialog *SaveDialog = new wxFileDialog(
      this, _("Save flow file"), wxEmptyString, wxEmptyString, wxT("*.flow"),
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

  /* Creates a "open file" dialog*/
  if (SaveDialog->ShowModal() == wxID_OK) {
    wxString wxPathName;
    /* file open*/
    wxPathName = SaveDialog->GetPath();
    if (wxPathName.Find(wxT(".flow")) == wxNOT_FOUND) {
      wxPathName += wxT(".flow");
    }

    flow_edit_canvas_->SavePluginFlowToFile(wxPathName);
  }
}

void PluginManagerWnd::OnLoadPluginFlow(wxCommandEvent &event) {
  LOG_STATUS("Load plugin flow");

  wxFileDialog *LoadDialog = new wxFileDialog(
      this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
      wxT("*.flow"), wxFD_OPEN, wxDefaultPosition);

  /* Creates a "open file" dialog*/
  if (LoadDialog->ShowModal() == wxID_OK) {
    wxString wxPathName;
    /* file open*/
    wxPathName = LoadDialog->GetPath();

    flow_edit_canvas_->LoadPluginFlowFromFile(wxPathName);
  }
}
