/**
 * @file      plugin_manager_wnd.h
 * @brief     Header for PluginManagerWnd class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_MANAGER_WND_H_
#define _PLUGIN_MANAGER_WND_H_

#include <string>
#include "./image_processing_thread.h"
#include "./include.h"
#include "./plugin_manager.h"
#include "./thread_running_cycle_manager.h"

class PluginEditCanvas;

/**
 * @class PluginManagerWnd
 * @brief PluginManagerWnd is window class to manage loaded plugins.
 * It contains a canvas which can control a connection of a plugin.
 */
class PluginManagerWnd : public wxFrame {
 private:
  /*! Pointer to the PluginManager class */
  PluginManager *plugin_manager_;
  /*! Pointer to the ImageProcessingThread class */
  ImageProcessingThread *image_proc_thread_;
  /*! ImageProcessingState object */
  ImageProcessingState image_proc_state_;
  /*! Pointer to the PluginEditCanvas class */
  //PluginEditCanvas *flow_edit_canvas_;

 public:
  /*! Pointer to the PluginEditCanvas class */
  PluginEditCanvas *flow_edit_canvas_;

  /**
   * @brief
   * Constructor.
   * @param plugin_manager [in] pointer to the plugin_manager class.
   * @param thread_running_cycle_manager [in] pointer to the
   * ThreadRunningCycleManager class.
   */
  PluginManagerWnd(PluginManager *plugin_manager,
                   ThreadRunningCycleManager *thread_running_cycle_manager);

  /**
   * @brief
   * Destructor.
   */
  virtual ~PluginManagerWnd(void);

  virtual void OnClose(wxCloseEvent &event);                   /* NOLINT */
  virtual void OnReloadPluginList(wxCommandEvent &event);      /* NOLINT */
  virtual void OnOpenPluginSettingsWnd(wxCommandEvent &event); /* NOLINT */
  virtual void OnSavePluginFlow(wxCommandEvent &event);        /* NOLINT */
  virtual void OnLoadPluginFlow(wxCommandEvent &event);        /* NOLINT */

  /**
   * @brief
   * Add a plugin into the list box.
   * @param plugin_name [in] name of the target plugin.
   */
  void AddPluginNameToPluginList(const std::string plugin_name);

  /**
   * @brief
   * Remove a plugin from the list box.
   * @param plugin_name [in] name of the target plugin.
   */
  void RemovePluginNameFromPluginList(const std::string plugin_name);

  /**
   * @brief
   * Find a plugin from the list box.
   * @param plugin_name [in] name of the target plugin.
   * @return true, if the plugin is found.
   */
  bool FindPluginNameFromPluginList(const std::string plugin_name);

  /**
   * @brief
   * Remove all plugin from the list box.
   */
  void RemoveAllPluginNameFromPluginList(void);

  /**
   * @brief
   * Set the ImageProcessingState.
   * @param image_proc_state [in] ImageProcessingState.
   * @return image processing state.
   */
  void set_image_proc_state(ImageProcessingState image_proc_state);

  /**
   * @brief
   * Get the ImageProcessingState.
   * @return image processing state.
   */
  ImageProcessingState image_proc_state(void);

  /**
   * @brief
   * Replad all plugins.
   * @return image processing state.
   */
  void ReloadPluginListFromFile(void);

  /**
   * @brief
   * Reset a list box and canvas.
   * @return image processing state.
   */
  void ResetPluginList(void);

 protected:
  wxListBox *list_box_plugin_list_;
  wxButton *button_reload_;
  wxButton *button_plugin_settings_;
  wxButton *button_save_plugin_flow_;
  wxButton *button_load_plugin_flow_;

 private:
  DECLARE_EVENT_TABLE();
};

#endif /* _PLUGIN_MANAGER_WND_H_ */
