/**
 * @file      plugin_edit_canvas.h
 * @brief     Header for PluginEditCanvas class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_EDIT_CANVAS_H_
#define _PLUGIN_EDIT_CANVAS_H_

#include <list>
#include <string>
#include <vector>
#include "./include.h"
#include "./plugin_edit_canvas_define.h"
#include "./plugin_edit_item_info.h"
#include "./plugin_manager.h"
#include "./thread_running_cycle_manager.h"
#include "wx/wx.h"

class PluginManagerWnd;
class PluginEditCanvas;
class WxDlgPlusMenu;
class WxDlgSelectPlugin;
class WxDlgCycleMenu;
class WxDlgPluginMenu;

typedef struct {
  std::string file_plugin_name;
  PluginEditItemInfo *item_info;
} LoadPluginFlowInfo;

/**
 * @class PluginEditCanvas
 * @brief PluginEditCanvas manages
 *    all information about plugin edit canvas screen.
 */
class PluginEditCanvas : public wxScrolledWindow {
 public:
  /**
   * @brief
   * Constructor.
   * @param parent [in] parent class pointer.
   * @param id [in] wxWidgets window ID.
   * @param thread_running_cycle_manager [in]
   *        ThreadRunningCycleManager class pointer.
   */
  PluginEditCanvas(wxWindow *parent, const wxWindowID id,
                   ThreadRunningCycleManager *thread_running_cycle_manager);
  /**
   * @brief
   * Destructor.
   */
  ~PluginEditCanvas();

  void OnPaint(wxPaintEvent &event);  // NOLINT
  void OnMouseDown(wxMouseEvent &event);  // NOLINT
  void OnMouseUp(wxMouseEvent &event);  // NOLINT

  /**
   * @brief
   * Check action type(add, branch delete or replace ),
   * then update Plugin flow.
   */
  void UpdateCanvasPluginList(void);

  /**
   * @brief
   * The handler function for custom event(UPDATE_CANVAS).
   * Refresh of plugin edit canvas screen.
   * @param event [in] wxCommandEvent.
   */
  void OnUpdateCanvas(wxCommandEvent &event);  // NOLINT

  /**
   * @brief
   * Posts custom event(UPDATE_CANVAS) to PluginEditCanvas.
   * Call this function if fource refresh is required.
   */
  void PostUpdateCanvasEvent(void);

  /**
   * @brief
   * Get PluginManager class pointer to management plugin flow.
   * And set root plugin on plugin edit canvas.
   * @param plugin_manager [in] PluginManager class pointer.
   */
  void SetPluginManagerInfo(PluginManager *plugin_manager);

  /**
   * @brief
   * Set root plugin on plugin edit canvas,
   * when root plugin is set to plugin_manager.
   */
  void SetRootFlow(void);

  /**
   * @brief
   * Remove plugin flow after del_pos.
   * @param del_pos [in] delete position.
   */
  void RemovePluginFlowList(wxPoint del_pos);

  /**
   * @brief
   * Get display strings according to
   *  the action type(add, branch delete or replace).
   * @param type [in] action type.
   * @return diplay strings.
   */
  std::vector<std::string> GetDispString(FlowActionType type);

  /**
   * @brief
   * Clear the selected state of all of the plugin flow items.
   */
  void Reset();

  /**
   * @brief
   * Set image processing state.
   * @param state [in] Image processing state.
   */
  void set_image_proc_state(ImageProcessingState image_proc_state);

  /**
   * @brief
   * Update scroll bar state.
   *  - Enable horizontal scrollbars
   *      when x-length of plugin flow over the screen width.
   *  - Enable vertical scrollbars
   *      when y-length of plugin flow over the screen height.
   */
  void UpdateScrollbarState(void);

  /**
   * @brief
   * Get the name of a valid previous plugin
   * that are connected to the flow on the bases of the target_Item.
   * @param target_Item [in] target PluginEditItemInfo.
   * @return the name of a valid previous plugin.
   */
  std::string GetValidPrevPluginName(PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Get the names of a valid next plugin
   * that are connected to the flow on the bases of the target_Item.
   * @param target_Item [in] target PluginEditItemInfo.
   * @return the names of a next previous plugin.
   */
  std::vector<std::string> GetValidNextPluginNames(
      PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Get the name of a valid base next plugin
   * that are connected to the flow on the bases of the target_Item.
   * @param target_Item [in] target PluginEditItemInfo.
   * @return the name of a valid base next plugin.
   */
  std::string GetValidBaseNextPluginName(PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Remove Flow and Flow list
   * that are connected to the target_Item and target_Item itself.
   * @param target_Item [in] target PluginEditItemInfo.
   */
  void RemoveFlowAndFlowList(PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Get the PluginEditItemInfo of a valid previous plugin
   * that are connected to the flow on the bases of the target_Item.
   * @param target_Item [in] target PluginEditItemInfo.
   * @return the PluginEditItemInfo of a valid previous plugin.
   */
  PluginEditItemInfo *GetValidPrevItemInfo(PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Check the target_Item and next_item of target_Item include plugin named
   * plugin_name.
   * @param target_Item [in] target PluginEditItemInfo.
   * @return true  = target_Item or next_item of target_Item
   *                 include plugin named plugin_name.
   *         false = target_Item or next_item of target_Item
   *                 does not include plugin named plugin_name.
   */
  bool IsIncludePlugin(std::string plugin_name,
                       PluginEditItemInfo *target_Item);

  /**
   * @brief
   * Print debug logs all of plugin connection status(prev, current and next).
   */
  void EditCanvasDebugLog(void);

  /**
   * @brief
   * Save Plugin flow and settings to ".flow" file.
   * @param file_path [in] file path.
   * @return true  = succeeded to save file.
   *         false = failed to save file.
   */
  bool SavePluginFlowToFile(wxString file_path);

  /**
   * @brief
   * Load Plugin flow and settings from ".flow" file.
   * @param file_path [in] file path.
   * @return true  = succeeded to save file.
   *         false = failed to save file.
   */
  bool LoadPluginFlowFromFile(wxString file_path);

  /**
   * @brief
   * Add plugin to plugin flow.
   * @param target_name [in] plugin name to be added.
   * @param current_item_info
   *   [in] current item on the flow to add target_name plugin.
   * @return true  = succeeded to add plugin.
   *         false = failed to add plugin.
   */
  bool AddNewPluginToFlow(std::string target_name,
                          PluginEditItemInfo *current_item_info);

  /**
   * @brief
   * Insert plugin to plugin flow.
   * @param target_name [in] plugin name to be inserted.
   * @param current_item_info
   *   [in] current item on the flow to insert target_name plugin.
   * @return true  = succeeded to insert plugin.
   *         false = failed to insert plugin.
   */
  bool InsertNewPluginToFlow(std::string target_name,
                             PluginEditItemInfo *current_item_info);

  /**
   * @brief
   * Create blanch and insert plugin to plugin flow.
   * @param target_name [in] plugin name to be inserted.
   * @param current_item_info
   *   [in] current item on the flow to insert target_name plugin.
   * @return true  = succeeded to insert plugin.
   *         false = failed to insert plugin.
   */
  bool InsertNewBlanchPluginToFlow(std::string target_name,
                                   PluginEditItemInfo *current_item_info);

  /**
   * @brief
   * Replace plugin on plugin flow.
   * @param target_name [in] plugin name to be replaced.
   * @param current_item_info
   *   [in] current item on the flow to replace target_name plugin.
   * @return true  = succeeded to replace plugin.
   *         false = failed to replace plugin.
   */
  bool ReplacePlugin(std::string target_name,
                     PluginEditItemInfo *current_item_info);

  /**
   * @brief
   * Replace root plugin.
   * @param target_name [in] plugin name to be replaced.
   * @param current_item_info [in] current item of root plugin.
   * @return true  = succeeded to replace plugin.
   *         false = failed to replace plugin.
   */
  bool ReplaceRootPlugin(std::string target_name,
                         PluginEditItemInfo *current_item_info);

 private:
  /**
   * @brief
   * Get target plugin name.
   * if the same name of the plugin is already added,
   * create a clone plugin then return a different name.
   * This function should be used only for the purpose
   * to get the plugin name to be added(insert or replace).
   * @param src_target_name [in] plugin name.
   * @return name of target plugin name.
   */
  std::string GetTargetPluginName(std::string src_target_name);

  /*! List of the all plugin flow information. */
  std::vector<std::vector<PluginEditItemInfo *> > flow_plugin_list_;

  /*! PluginManagerWnd class pointer. */
  PluginManagerWnd *parent_;

  /*! WxDlgPlusMenu class pointer. */
  WxDlgPlusMenu *wxdlg_plus_menu_;

  /*! WxDlgCycleMenu class pointer. */
  WxDlgCycleMenu *wxdlg_cycle_menu_;

  /*! WxDlgPluginMenu class pointer. */
  WxDlgPluginMenu *wxdlg_plugin_menu_;

  /*! touched position on flow_plugin_list_ */
  wxPoint touched_plugin_num_;

  /*! PluginManager class pointer. */
  PluginManager *plugin_manager_;

  /*! ThreadRunningCycleManager class pointer. */
  ThreadRunningCycleManager *thread_running_cycle_manager_;

  /*! Set the scroll amount in the X direction */
  int scroled_x_;

  /*! Set the scroll amount in the Y direction */
  int scroled_y_;

  /* Event table of wxWidgets.*/
  DECLARE_EVENT_TABLE()
};

/**
 * @class WxDlgPlusMenu
 * @brief WxDlgPlusMenu manages popup window
 *   when a plus button touched on plugin edit canvas screen.
 */
class WxDlgPlusMenu : public wxDialog {
 public:
  /**
   * @brief
   * Constructor.
   * @param parent [in] wxWindow class pointer.
   * @param id [in] wxWidgets window ID.
   * @param str [in] window title strings.
   */
  WxDlgPlusMenu(wxWindow *parent, const wxWindowID id, const wxString &str);

  /**
   * @brief
   * Destructor.
   */
  ~WxDlgPlusMenu();

  void OnAddButton(wxCommandEvent &event);     // NOLINT
  void OnBranchButton(wxCommandEvent &event);  // NOLINT
  void OnDeleteButton(wxCommandEvent &event);  // NOLINT
  void OnShow(wxShowEvent &event);             // NOLINT

  /**
   * @brief
   * Get event_type(PlusMenuEventType).
   * return event type(PlusMenuEventType).
   */
  PlusMenuEventType event_type() { return event_type_; }

  /**
   * @brief
   * Clear the selected state of all of the plugin flow items.
   */
  void Reset(void);

  /**
   * @brief
   * Get plugin name which selected on the popup window.
   */
  std::string GetPluginName(void);

  /**
   * @brief
   * Update button enable/disable state of WxDlgPlusMenu.
   */
  void UpdateButtonState(PluginEditItemInfo *target_item_info,
                         int add_enable_state);

 private:
  PluginEditCanvas *parent_;
  wxButton *button_add_;
  wxButton *button_branch_;
  wxButton *button_delete_;

  /*! event type which selected on WxDlgPlusMenu dialog */
  PlusMenuEventType event_type_;

  WxDlgSelectPlugin *wxdlg_select_plugin_;

  // Event table of wxWidgets
  DECLARE_EVENT_TABLE();
};

/**
 * @class WxDlgSelectPlugin
 * @brief WxDlgSelectPlugin manages plugin selection window 
 * which select add or branch menu from WxDlgPlusMenu or
 * select replace menu from WxDlgPluginMenu.
 */
class WxDlgSelectPlugin : public wxDialog {
 public:
  /**
   * @brief
   * Constructor.
   * @param parent [in] wxWindow class pointer.
   * @param id [in] wxWidgets window ID.
   * @param str [in] window title strings.
   */
  WxDlgSelectPlugin(wxWindow *parent, const wxWindowID id, const wxString &str);

  /**
   * @brief
   * Destructor.
   */
  ~WxDlgSelectPlugin();

  void OnApply(wxCommandEvent &event);  // NOLINT

  /**
   * @brief
   * Get a plugin selected state on WxDlgSelectPlugin.
   * return true  : selected some plugin and need to update flow.
   *        false : did not select any plugin and not need to update flow.
   */
  bool is_update_plugin_list(void) { return is_update_plugin_list_; }

  /**
   * @brief
   * Clear the selected state of WxDlgSelectPlugin.
   */
  void Reset();

  /**
   * @brief
   * Get a plugin name which selected on WxDlgSelectPlugin.
   * return plugin name.
   */
  std::string GetPluginName(void);

  /**
   * @brief
   * There is a need to set the appropriate names 
   *  depending on the operation(add, branch or replace).
   * param disp_strings [in] name list which display on WxDlgSelectPlugin.
   */
  void SetDispString(std::vector<std::string> disp_strings);

 private:
  wxListBox *list_box_plugin_;
  wxButton *button_apply_;

  /*! plugin selected state of this dialog. */
  bool is_update_plugin_list_;

  /*! PluginManager class pointer. */
  PluginManager *plugin_manager_;

  // Event table of wxWidgets
  DECLARE_EVENT_TABLE();
};

/**
 * @class WxDlgPluginMenu
 * @brief WxDlgPluginMenu manages replase window.
 */
 class WxDlgPluginMenu : public wxDialog {
 public:
  /**
   * @brief
   * Constructor.
   * @param parent [in] wxWindow class pointer.
   * @param id [in] wxWidgets window ID.
   * @param str [in] window title strings.
   */
  WxDlgPluginMenu(wxWindow *parent, const wxWindowID id, const wxString &str);

  /**
   * @brief
   * Destructor.
   */
  ~WxDlgPluginMenu();

  void OnReplaceButton(wxCommandEvent &event);  // NOLINT
  void OnShow(wxShowEvent &event);              // NOLINT

  /**
   * @brief
   * Get event_type(PluginMenuEventType).
   * return event type(PluginMenuEventType).
   */
  PluginMenuEventType event_type() { return event_type_; }

  /**
   * @brief
   * Clear the selected state of all of the plugin flow items.
   */
  void Reset(void);

  /**
   * @brief
   * Get plugin name which selected on the popup window.
   */
  std::string GetPluginName(void);

 private:
  /*! PluginEditCanvas class pointer. */
  PluginEditCanvas *parent_;

  wxButton *button_replace_;

  /*! WxDlgSelectPlugin class pointer. */
  WxDlgSelectPlugin *wxdlg_select_plugin_;

  /*! event type which selected on WxDlgPluginMenu dialog */
  PluginMenuEventType event_type_;

  // Event table of wxWidgets
  DECLARE_EVENT_TABLE();
};

/**
 * @class WxDlgCycleMenu
 * @brief WxDlgCycleMenu manages cycle window.
 */
class WxDlgCycleMenu : public wxDialog {
 public:
  /**
   * @brief
   * Constructor.
   * @param parent [in] wxWindow class pointer.
   * @param id [in] wxWidgets window ID.
   * @param str [in] window title strings.
   */
  WxDlgCycleMenu(wxWindow *parent, const wxWindowID id, const wxString &str);

  /**
   * @brief
   * Destructor.
   */
  ~WxDlgCycleMenu();

  void OnClose(wxCloseEvent &event);          // NOLINT
  void OnApplyButton(wxCommandEvent &event);  // NOLINT

  /**
   * @brief
   * Set cycle value to display on WxDlgCycleMenu dialog.
   * @param cycle [in] cycle value to display.
   */
  void set_cycle(unsigned int cycle);

  /**
   * @brief
   * Get cycle value from WxDlgCycleMenu dialog.
   * return cycle value.
   */
  unsigned int cycle() { return cycle_; }

 private:
  /*! PluginEditCanvas class pointer. */
  PluginEditCanvas *parent_;

  wxStaticText *static_text_info_;
  wxTextCtrl *text_cycle_value_;
  wxButton *button_apply_;

  /*! cycle value set by WxDlgCycleMenu dialog. */
  unsigned int cycle_;

  // Event table of wxWidgets
  DECLARE_EVENT_TABLE();
};

#endif  // _PLUGIN_EDIT_CANVAS_H_
