/**
 * @file      plugin_manager.h
 * @brief     Header for PluginManager class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_MANAGER_H_
#define _PLUGIN_MANAGER_H_

#include <dirent.h>
#include <dlfcn.h>
#include <cstdlib>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "./include.h"
#include "./plugin_base.h"

#define PLUGIN_INTERFACE_VERSION 1
#define MIN_REQUIRED_PLUGIN_INTERFACE_VERSION 1
#define MAJOR_VERSION 1

typedef struct {
  void* handle;
  PluginBase* plugin;
} PluginData;

/**
 * @class PluginManager
 * @brief This class execute the plugin load/unload, version management,
 *        and the connection control between the plugins.
 */
class PluginManager {
 public:
  /**
   * @brief
   * Constructor of the PluginManager.
   */
  PluginManager(void);

  /**
   * @brief
   * Destructor of the PluginManager.
   */
  ~PluginManager(void);

  /**
   * @brief
   * Set the first plugin on the Flow.
   * @param plugin_name [in] plugin name.
   * @return If true, success to set root plugin.
   */
  virtual bool set_root_plugin(const std::string plugin_name);

  /**
   * @brief
   * Get the first plugin on the Flow.
   * @return first plugin on the Flow.
   */
  virtual PluginBase* root_plugin(void);

  /**
   * @brief
   * Load the stored plugins in the specified directory.
   * @param path   [in] file path
   * @param reload [in] whether to reload
   * @return If true, success to load plugin.
   */
  virtual bool LoadPlugins(const std::string path, bool reload = false);

  /**
   * @brief
   * Get the plugin from plugin name.
   * @param plugin_name [in] plugin name
   */
  virtual PluginBase* GetPlugin(const std::string plugin_name);

  /**
   * @brief
   * Get the plugin name that can be connected to the plugin name
   * that is specified in the argument.
   * @param prev_plugin_name [in] plugin name to be connected to previous
   * @param next_plugin_name [in] plugin name to be connected to next
   * @return connectable plagins names.
   */
  virtual std::vector<std::string> GetConnectablePluginNames(
      const std::string prev_plugin_name, const std::string next_plugin_name);

  /**
   * @brief
   * Get the plugin name that can be connected to the plugin name
   * that is specified in the argument.
   * @param prev_plugin_name [in] plugin name to be connected to previous.
   * @param next_plugin_name [in] plugin name to be connected to next.
   *                              (It can be specified more than one)
   * @return connectable plagins names.
   */
  virtual std::vector<std::string> GetConnectablePluginNames(
      const std::string prev_plugin_name,
      const std::vector<std::string> next_plugin_names);

  /**
   * @brief
   * Connect the plugin and plugin.
   * @param prev_plugin_name [in] plugin name to connect to the previous
   * @param target_plugin_name [in] plugin name to be connected
   * @param next_plugin_name [in] plugin name to connect to the next
   *                              (It can be specified more than one)
   * @return If true, success connect.
   */
  virtual bool ConnectPlugin(const std::string prev_plugin_name,
                             const std::string target_plugin_name,
                             const std::vector<std::string> next_plugin_names);

  /**
   * @brief
   * Replace the plugin on the Flow.
   * @param prev_plugin_name [in] plugin name to connect to the previous
   * @param old_target_plugin_name [in] replacement before the plugin name
   * @param new_target_plugin_name [in] replacement after the  plugin name
   * @param next_plugin_name [in] plugin name to connect to the next
   *                              (In the case of a branch, you can specified
   * more than one.)
   * @return If true, success replacement.
   */
  virtual bool ReplacePlugin(const std::string prev_plugin_name,
                             const std::string old_target_plugin_name,
                             const std::string new_target_plugin_name,
                             const std::vector<std::string> next_plugin_names);

  /**
   * @brief
   * Release the connection of the specified plugin.
   * @param target_plugin_name [in] plugin name to release the connection
   */
  virtual void DisconnectAllPlugin(const std::string target_plugin_name);

  /**
   * @brief
   * Release the connection of specified branch for plugin
   * @param target_plugin_name [in] plugin name of the previous branch
   * @param branch_plugin_name [in] plugin name after the branching
   */
  virtual void DisconnectPlugin(const std::string target_plugin_name,
                                const std::string branch_plugin_name);

  /**
   * @brief
   * Check whether it is possible to image processing by the current
   * of the Flow and setting of each plug-in.
   * @param target_plugin [in] Pointer to the PluginBase class
   * @return If true, image processing can be executed.
   */
  virtual bool CheckExecuteSetting(PluginBase* target_plugin);

  /**
   * @brief
   * Get the newest plugin interface version that can be VPF to process.
   * @return newest plugin interface version.
   */
  virtual int TargetPluginIFVersion(void);

  /**
   * @brief
   * Get the most oldest plugin interface version that can be VPF to process.
   * @return oldest plugin interface version.
   */
  virtual int MinimumRequiredPluginIFVersion(void);

  /**
   * @brief
   * Get the version of the VPF.
   * @return version of the VPF.
   */
  virtual int MajorVersion(void);

  /**
   * @brief
   * Get the plugin names from the type of plugin.
   * @param type [in] type of plugin
   * @return plugin names of the specified plugin type.
   */
  virtual std::vector<std::string> GetPluginNames(PluginType plugin_type);

  /**
   * @brief
   * Notify the VPN state to the each plugin.
   * @param state [in] state of the VPF
   */
  virtual void NotifyState(ImageProcessingState state);

  /**
   * @brief
   * Get the image type information of OpenCV from PlaneType.
   * @param state [in] PlaneType
   * @return image type information of OpenCV
   */
  static int GetDepthAndChannelType(PlaneType plane_type);

  /**
   * @brief
   * Replicate the specified plugin.
   * @param target_plugin [in] Pointer to the IPlugin class
   * @return pointer to the replicated plugin.
   */
  virtual IPlugin* ClonePlugin(IPlugin* target_plugin);

  /**
   * @brief
   * Check whether the specified plugin is existing on the Flow.
   * @param plugin [in] Pointer to the PluginBase class
   * @return If true, exist plugin.
   */
  bool IsExistPluginForFlow(PluginBase* plugin);

  /**
   * @brief
   * Release the resource of specified plugin.
   * @param target_plugin [in] Pointer to the PluinBase class
   */
  void ReleasePlugin(PluginBase* target_plugin);

  /**
   * @brief
   * Release the resources of cloned plugins.
   */
  void ReleaseAllClonePlugin(void);

 private:
  /*! list of the loaded plugin */
  std::vector<PluginData> all_plugins_;

  /*! start plugin pointer of the Flow */
  PluginBase* root_plugin_;

  /*! directory where the plugins are stored */
  std::string plugin_root_dir_;

  /*! enable change root plugin */
  bool enable_change_root_plugin_;

  /**
   * @brief
   * Remove the SO file that was created when the plugin cloned.
   * @param path [in] file path
   * @param plugin_type [in] target plugin type.
   */
  void DeleteClonedPlugins(const std::string path, PluginType plugin_type);

  /**
   * @brief
   * Load the plugins for the specified PluginType.
   * @param path [in] file path
   * @param plugin_type [in] plugin type for read.
   * @param reload [in] whether to reload
   * @return If true, success to read plugin.
   */
  bool LoadPluginWithPluginType(const std::string path, PluginType plugin_type,
                                bool reload = false);

  /**
   * @brief
   * Load the plugins for the specified PluginType and name.
   * @param path [in] file path
   * @param plugin_type [in] plugin type for read
   * @param plugin_name [in] plugin name to be set
   * @return If true, success to load plugin.
   */
  bool LoadPlugin(const std::string path, PluginType plugin_type,
                  const std::string plugin_name = "");

  /**
   * @brief
   * Release resource of all plugins.
   * @param reload [in] whether to reload
   */
  void ReleasePlugins(bool reload = false);

  /**
   * @brief
   * If duplicate plugin name, rename it automatically.
   * @param target_plugin_name [in] target plugin name
   * @return plugin name after rename
   */
  std::string CheckDuplicateAndRenamePlugin(std::string target_plugin_name);

  /**
   * @brief
   * Check whether there is the same name of the specified plugin name
   * to the plugins that was already loaded .
   * @param plugin [in] plugin name
   * @return If true, duplicate.
   */
  bool IsDuplicatePluginName(std::string plugin_name);

  /**
   * @brief
   * Check whether the plugin interface version that can be executed.
   * @param [in] plugin interface version
   * @return If true, executable.
   */
  bool IsExecutablePluginInterfaceVersion(int plugin_if_ver);

  /**
   * @brief
   * Update the availability of the output port.
   * @param plugin [in] plugin
   */
  void UpdateOutputPortSpec(PluginBase* plugin);

  /**
   * @brief
   * Check whether the image processing can be executed, by the input-output
   * relation and output port availability.
   * @param prev_plugin [in] Pointer to the PluginBase class for prev plugin
   * @param next_plugin [in] Pointer to the PluginBase class for next plugin
   * return If true, next_plugin has been set to the output port
   *        that can be image processing.
   */
  bool CheckPortRelation(PluginBase* prev_plugin, PluginBase* next_plugin);

  /**
   * @brief
   * Copy the file in the specified path.
   * @param src_file_name [in] source file path
   * @param dst_file_name [in] dest file path
   * @return If true, succeed to copy
   */
  bool CopyFile(std::string src_file_name, std::string dst_file_name);
};

#endif /*_PLUGIN_MANAGER_H_*/
