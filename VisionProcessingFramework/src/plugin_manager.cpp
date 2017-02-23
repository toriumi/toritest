/**
 * @file      plugin_manager.cpp
 * @brief     Source for PluginManager class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./plugin_manager.h"
#include <errno.h>
#include <queue>
#include <string>
#include <vector>
#include "./logger.h"

#define VPF_WORKAROUND_RELOAD_PLUGIN  // workaround

static const PlaneTypeInfo plane_type_info_array[] = {
    {kGRAY8, 1, 8, CV_8UC1},
    {kGRAY16, 1, 16, CV_16UC1},
    {kRGB444, 3, 12, CV_8UC3},
    {kBGR444, 3, 12, CV_8UC3},
    {kRGB555, 3, 15, CV_8UC3},
    {kBGR555, 3, 15, CV_8UC3},
    {kRGB565, 3, 16, CV_8UC3},
    {kBGR565, 3, 16, CV_8UC3},
    {kRGB888, 3, 24, CV_8UC3},
    {kBGR888, 3, 24, CV_8UC3},
    {kARGB, 4, 32, CV_8UC4},
    {kRGBA, 4, 32, CV_8UC4},
    {kABGR, 4, 32, CV_8UC4},
    {kBGRA, 4, 32, CV_8UC4},
    {kRGB48, 3, 48, CV_16UC3},
    {kBGR48, 3, 48, CV_16UC3},
    {kRGBA64, 4, 64, CV_16UC4},
    {kBGRA64, 4, 64, CV_16UC4},

    // If you add a new definition, to add a PlaneType.
    {kNone, 0, 0, -1},
};

/**
 * @brief
 * Constructor of the PluginManager.
 */
PluginManager::PluginManager() {
  root_plugin_ = NULL;
  enable_change_root_plugin_ = true;
  plugin_root_dir_ = "";
}

/**
* @brief
* Destructor of the PluginManager.
*/
PluginManager::~PluginManager() {
  DEBUG_PRINT("PluginManager::~PluginManager()\n");
  ReleasePlugins();
}

/**
 * @brief
 * Get the plugin from plugin name.
 * @param plugin_name [in] plugin name
 */
PluginBase* PluginManager::GetPlugin(const std::string plugin_name) {
  std::vector<PluginData>::iterator itr;
  PluginBase* plugin;
  for (itr = all_plugins_.begin(); itr != all_plugins_.end(); itr++) {
    plugin = (*itr).plugin;
    if (plugin_name == plugin->plugin_name()) {
      return plugin;
    }
  }
  return NULL;
}

/**
 * @brief
 * Set the first plugin on the Flow.
 * @param plugin_name [in] plugin name.
 * @return If true, success to set root plugin.
 */
bool PluginManager::set_root_plugin(const std::string plugin_name) {
  if (enable_change_root_plugin_ == false) {
    return false;
  }

  PluginBase* plugin;
  for (int i = 0; i < all_plugins_.size(); i++) {
    plugin = all_plugins_[i].plugin;
    if (plugin_name == plugin->plugin_name()) {
      root_plugin_ = plugin;
      return true;
    }
  }
  // Not exist "plugin_name" plugin.
  root_plugin_ = NULL;
  return false;
}

/**
 * @brief
 * Get the first plugin on the Flow.
 * @return first plugin on the Flow.
 */
PluginBase* PluginManager::root_plugin(void) { return root_plugin_; }

/**
 * @brief
 * Get the plugin names from the type of plugin.
 * @param type [in] type of plugin
 * @return plugin names of the specified plugin type
 */
std::vector<std::string> PluginManager::GetPluginNames(PluginType type) {
  int debug_count = 0;
  std::vector<std::string> names;
  std::vector<PluginData>::iterator itr;
  PluginBase* plugin;
  for (itr = all_plugins_.begin(); itr != all_plugins_.end(); itr++) {
    plugin = (*itr).plugin;
    if (type == plugin->plugin_type()) {
      names.push_back(plugin->plugin_name());
      DEBUG_PRINT("Plugin name = %s, count = %d\n",
                  plugin->plugin_name().c_str(), debug_count);
    }
    debug_count++;
  }
  return names;
}

/**
 * @brief
 * Notify the VPN state to each plugin.
 * @param state [in] state of the VPF
 */
void PluginManager::NotifyState(ImageProcessingState state) {
  if (state == kStop) {
    enable_change_root_plugin_ = true;
  } else {
    enable_change_root_plugin_ = false;
  }

  for (int i = 0; i < all_plugins_.size(); i++) {
    PluginBase* plugin = all_plugins_[i].plugin;
    plugin->set_image_processing_state(state);
  }
}

/**
 * @brief
 * Get the plugin name that can be connected to the plugin name that is
 specified in the argument.
 * @param prev_plugin_name [in] plugin name to be connected to previous
 * @param next_plugin_name [in] plugin name to be connected to next
                                (It can be specified more than one)
 * @return connectable plagins names.
 */
std::vector<std::string> PluginManager::GetConnectablePluginNames(
    const std::string prev_plugin_name,
    const std::vector<std::string> next_plugin_names) {
  std::vector<std::string> connectable_plugins;

  if (next_plugin_names.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames next_plugin_names size "
        "zero\n");
    return connectable_plugins;
  }
  std::vector<std::string>
      tmp_connectable_plugins_arr[next_plugin_names.size()];

  // Find the plugin that can be connected between the prev_plugin_name
  // and next_plugin_names
  // If prev = A, next = B,C, find plugins A and B, A and C.
  for (int i = 0; i < next_plugin_names.size(); i++) {
    tmp_connectable_plugins_arr[i] =
        GetConnectablePluginNames(prev_plugin_name, next_plugin_names[i]);
    // If the size is 0, not inserted before branching.
    if (tmp_connectable_plugins_arr[i].size() == 0) {
      DEBUG_PRINT(
          "PluginManager::GetConnectablePluginNames %s connectable zero\n",
          next_plugin_names[i].c_str());
      return connectable_plugins;
    }
  }
  // If next_plugin_names is only one, It returns the first element
  // of the tmp_connectable_plugins_arr.
  if (next_plugin_names.size() == 1) {
    return tmp_connectable_plugins_arr[0];
  }

  // Below, if next_plugin_names is 2 or more
  // The plugins of the first element of the tmp_connectable_plugins_arr
  // a search target.
  std::vector<std::string> target_connectable_plugin_names =
      tmp_connectable_plugins_arr[0];
  for (int i = 0; i < target_connectable_plugin_names.size(); i++) {
    std::string target_plugin_name = target_connectable_plugin_names[i];

    bool all_ok = true;

    // check whether there was a target_plugin_name to the elements
    // of the tmp_connectable_plugins_arr.
    for (int j = 1; j < next_plugin_names.size(); j++) {
      std::vector<std::string> tmp_plugin_names =
          tmp_connectable_plugins_arr[j];
      int k;
      for (k = 0; k < tmp_plugin_names.size(); k++) {
        std::string tmp_plugin_name = tmp_plugin_names[k];
        if (tmp_plugin_name == target_plugin_name) {
          break;
        }
      }

      if (k == tmp_plugin_names.size()) {
        all_ok = false;
        break;
      }
    }
    // If there is a target_plugin_name to elements of all of
    // tmp_connectable_plugins_arr
    if (all_ok) {
      connectable_plugins.push_back(target_plugin_name);
    }
  }
  return connectable_plugins;
}

/**
 * @brief
 * Get the plugin name that can be connected to the plugin name
 * that is specified in the argument.
 * @param prev_plugin_name [in] plugin name to be connected to previous
 * @param next_plugin_name [in] plugin name to be connected to next
 * @return connectable plagins names.
 */
std::vector<std::string> PluginManager::GetConnectablePluginNames(
    const std::string prev_plugin_name, const std::string next_plugin_name) {
  // It can be connected to the plugin "prev_plugin_name"
  std::vector<std::string> prev_connectable_plugins;
  // It can be connected to the plug-in "next_plugin_name"
  std::vector<std::string> next_connectable_plugins;
  // It can be connected to the plug-in "prev_plugin_name" and
  // "next_plugin_name"
  std::vector<std::string> connectable_plugins;

  PluginBase* next_plugin;

  PluginBase* prev_plugin = GetPlugin(prev_plugin_name);
  // Flag to indicate the "addition" or "insertion"
  bool is_insert = false;

  if (prev_plugin == NULL) {
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames prev_plugin == NULL\n");
    return connectable_plugins;
  }

  std::vector<PortSpec*> prev_output_port_candidate_specs =
      prev_plugin->output_port_candidate_specs();
  // Not exist output port spec
  if (prev_output_port_candidate_specs.size() <= 0) {
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames prev_plugin not exist output "
        "port\n");
    return connectable_plugins;
  }

  // In the case of insertion
  if (next_plugin_name.size() > 0) {
    next_plugin = GetPlugin(next_plugin_name);
    if (next_plugin == NULL) {
      DEBUG_PRINT(
          "PluginManager::GetConnectablePluginNames next_plugin == NULL\n");
      return connectable_plugins;
    }
    // If there is no next_plugin input port
    if (next_plugin->input_port_candidate_specs().size() > 0) {
      is_insert = true;
    } else {
      DEBUG_PRINT(
          "PluginManager::GetConnectablePluginNames next_plugin not exist "
          "input port\n");
      return connectable_plugins;
    }
  }

  // Find the plugin that can be connected to the prev_plugin.
  for (int i = 0; i < all_plugins_.size(); i++) {
    // Find plugins except prev_plugin.
    if (all_plugins_[i].plugin != prev_plugin) {
      PluginBase* tmp_plugin = all_plugins_[i].plugin;
      std::string prev_plugin_name;
      if (prev_plugin->is_cloned()) {
        prev_plugin_name = prev_plugin->original_plugin_name();
      } else {
        prev_plugin_name = prev_plugin->plugin_name();
      }
      // If was the same name as the prev_plugin
      if (prev_plugin_name == tmp_plugin->plugin_name()) {
        continue;
      }

      // Input port that can be connected to check whether there is a
      // prev_plugin.
      std::vector<PortSpec*> tmp_input_port_candidate_specs =
          tmp_plugin->input_port_candidate_specs();
      bool is_find = false;
      if (tmp_input_port_candidate_specs.size() > 0) {
        for (int j = 0; j < tmp_input_port_candidate_specs.size(); j++) {
          PortSpec* tmp_input_port_spec = tmp_input_port_candidate_specs[j];
          PlaneType tmp_input_plane_type = tmp_input_port_spec->plane_type();

          for (int k = 0; k < prev_output_port_candidate_specs.size(); k++) {
            PortSpec* prev_output_port_spec =
                prev_output_port_candidate_specs[k];
            PlaneType prev_output_plane_type =
                prev_output_port_spec->plane_type();
            // If there is a compatible port. (only once)
            if (is_find == false &&
                prev_output_plane_type == tmp_input_plane_type) {
              // clone plugin is excluded.
              if (!tmp_plugin->is_cloned()) {
                prev_connectable_plugins.push_back(tmp_plugin->plugin_name());
              }
              is_find = true;
            }
          }
        }
      }
    }
  }
  // If there is no plugins that can be connected to the prev_plugin,
  if (prev_connectable_plugins.size() <= 0) {
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames not exist connectable plugin "
        "for prev\n");
    return connectable_plugins;
  }

  // In the case of insertion, find a plugin that can be connected to the
  // next_plugin
  if (is_insert) {
    DEBUG_PRINT("PluginManager::GetConnectablePluginNames is_insert == true\n");
    std::vector<PortSpec*> next_input_port_candidate_specs =
        next_plugin->input_port_candidate_specs();
    // If the input port of next_plugin does not exist,
    if (next_input_port_candidate_specs.size() == 0) {
      DEBUG_PRINT(
          "PluginManager::GetConnectablePluginNames next_plugin not exist "
          "input port\n");
      return connectable_plugins;
    }
    for (int i = 0; i < all_plugins_.size(); i++) {
      // Find plugins except next_plugin.
      if (all_plugins_[i].plugin != next_plugin) {
        bool is_find = false;
        PluginBase* tmp_plugin = all_plugins_[i].plugin;
        std::string next_plugin_name;
        if (next_plugin->is_cloned()) {
          next_plugin_name = next_plugin->original_plugin_name();
        } else {
          next_plugin_name = next_plugin->plugin_name();
        }
        if (next_plugin_name == tmp_plugin->plugin_name()) {
          continue;
        }
        std::vector<PortSpec*> tmp_output_port_candidate_specs =
            tmp_plugin->output_port_candidate_specs();
        if (tmp_output_port_candidate_specs.size() > 0) {
          for (int j = 0; j < tmp_output_port_candidate_specs.size(); j++) {
            PortSpec* tmp_output_port_spec = tmp_output_port_candidate_specs[j];
            PlaneType tmp_output_plane_type =
                tmp_output_port_spec->plane_type();

            for (int k = 0; k < next_input_port_candidate_specs.size(); k++) {
              PortSpec* next_input_port_spec =
                  next_input_port_candidate_specs[k];
              PlaneType next_input_plane_type =
                  next_input_port_spec->plane_type();
              // If there is a compatible port. (only once)
              if (is_find == false &&
                  next_input_plane_type == tmp_output_plane_type) {
                // clone plugin is excluded.
                if (!tmp_plugin->is_cloned()) {
                  next_connectable_plugins.push_back(tmp_plugin->plugin_name());
                }
                is_find = true;
              }
            }
          }
        }
      }
    }
    // If there is no plugins that can be connected to the next_plugin,
    if (next_connectable_plugins.size() <= 0) {
      DEBUG_PRINT(
          "PluginManager::GetConnectablePluginNames not exist connectable "
          "plugin for next\n");
      return connectable_plugins;
    }
    // Find the plugin that can be connected to the prev_plugin and next_plugin
    for (int i = 0; i < prev_connectable_plugins.size(); i++) {
      PluginBase* prev_plugin = GetPlugin(prev_connectable_plugins[i]);
      for (int j = 0; j < next_connectable_plugins.size(); j++) {
        PluginBase* next_plugin = GetPlugin(next_connectable_plugins[j]);
        // If the prev_plugin and next_plugin pointer was the same to add.
        if (prev_plugin == next_plugin) {
          connectable_plugins.push_back(prev_connectable_plugins[i]);
        }
      }
    }
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames is_insert == true "
        "connectable_plugins size = %d\n",
        connectable_plugins.size());
    return connectable_plugins;
  } else {  // In the case of additional
    DEBUG_PRINT(
        "PluginManager::GetConnectablePluginNames is_insert == false "
        "connectable_plugins size = %d\n",
        connectable_plugins.size());
    return prev_connectable_plugins;
  }
}

/**
 * @brief
 * Connect the plugin and plugin.
 * @param prev_plugin_name [in] plugin name to connect to the previous
 * @param target_plugin_name [in] plugin name to be connected
 * @param next_plugin_name [in] plugin name to connect to the next
 *                              (It can be specified more than one)
 * @return If true, success connect.
 */
bool PluginManager::ConnectPlugin(
    const std::string prev_plugin_name, const std::string target_plugin_name,
    const std::vector<std::string> next_plugin_names) {
  PluginBase* prev_plugin = GetPlugin(prev_plugin_name);
  PluginBase* target_plugin = GetPlugin(target_plugin_name);
  if (prev_plugin == NULL || target_plugin == NULL) return false;

  std::vector<PluginBase*> next_plugins;
  for (int i = 0; i < next_plugin_names.size(); i++) {
    PluginBase* next_plugin = GetPlugin(next_plugin_names[i]);
    if (next_plugin != NULL) {
      next_plugins.push_back(next_plugin);
    } else {
      return false;
    }
  }

  // If there is no output port of prev_plugin, return false.
  std::vector<PortSpec*> prev_output_port_specs =
      prev_plugin->output_port_candidate_specs();
  if (prev_output_port_specs.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::ConnectPlugin prev_plugin input port num is zero\n");
    return false;
  }
  // If there is no input port of target_plugin, return false.
  std::vector<PortSpec*> target_input_port_specs =
      target_plugin->input_port_candidate_specs();
  if (target_input_port_specs.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::ConnectPlugin target_plugin input port num is zero\n");
    return false;
  }

  // Check prev_plugin and target_plugin can be connected.
  bool prev_target_connectable = false;
  for (int i = 0; i < prev_output_port_specs.size(); i++) {
    PortSpec* prev_output_spec = prev_output_port_specs[i];
    for (int j = 0; j < target_input_port_specs.size(); j++) {
      PortSpec* target_input_spec = target_input_port_specs[j];
      if (prev_output_spec->plane_type() == target_input_spec->plane_type()) {
        prev_target_connectable = true;
        break;
      }
    }
  }

  // When adding to the end of the Flow
  if (next_plugins.size() == 0) {
    // Connect the prev_plugin and target_plugin, update the output port of
    // prev_plugin.
    if (prev_target_connectable) {
      prev_plugin->AddNextPlugin(target_plugin);
      UpdateOutputPortSpec(prev_plugin);
      DEBUG_PRINT("PluginManager::ConnectPlugin success. %s and %s\n",
                  prev_plugin_name.c_str(), target_plugin_name.c_str());
      return true;
    } else {
      DEBUG_PRINT(
          "PluginManager::ConnectPlugin fail. prev and next plugin not "
          "connectable.\n");
      return false;
    }
  } else {  // In the case of insertion
    // If prev_plugin and target_plugin can be connected
    if (prev_target_connectable) {
      // If there is no output port of target_plugin, return false.
      std::vector<PortSpec*> target_output_port_specs =
          target_plugin->output_port_candidate_specs();
      if (target_output_port_specs.size() == 0) {
        DEBUG_PRINT(
            "PluginManager::ConnectPlugin target_plugin output port num is "
            "zero\n");
        return false;
      }
      // If there is no input port of next_plugin, return false.
      for (int i = 0; i < next_plugins.size(); i++) {
        std::vector<PortSpec*> next_input_port_specs =
            next_plugins[i]->input_port_candidate_specs();
        if (next_input_port_specs.size() == 0) {
          DEBUG_PRINT(
              "PluginManager::ConnectPlugin next_plugin(%s) input port num is "
              "zero\n",
              next_plugins[i]->plugin_name().c_str());
          return false;
        }
      }
      // Check target_plugin and next_plugin can be connected.
      bool* target_next_connectable = new bool(next_plugins.size());
      bool all_target_next_connectable = true;
      for (int i = 0; i < next_plugins.size(); i++) {
        target_next_connectable[i] = false;
      }
      for (int i = 0; i < next_plugins.size(); i++) {
        std::vector<PortSpec*> next_input_port_specs =
            next_plugins[i]->input_port_candidate_specs();
        for (int j = 0; j < next_input_port_specs.size(); j++) {
          PortSpec* next_input_port_spec = next_input_port_specs[j];
          for (int k = 0; k < target_output_port_specs.size(); k++) {
            PortSpec* target_output_port_spec = target_output_port_specs[k];
            if (target_output_port_spec->plane_type() ==
                next_input_port_spec->plane_type()) {
              target_next_connectable[i] = true;
            }
          }
        }
      }
      for (int i = 0; i < next_plugins.size(); i++) {
        if (!target_next_connectable[i]) {
          DEBUG_PRINT(
              "PluginManager::ConnectPlugin Not connectable %s and %s\n",
              target_plugin->plugin_name().c_str(),
              next_plugins[i]->plugin_name().c_str());
          all_target_next_connectable = false;
        }
      }
      // If target_plugin and next_plugin can not connect, return false.
      if (!all_target_next_connectable) {
        return false;
      }
      delete[] target_next_connectable;
      // Disconnect the prev_plugin.
      for (int i = 0; i < next_plugins.size(); i++) {
        prev_plugin->ClearNextPlugins(next_plugins[i]->plugin_name());
      }
      // Connect the prev_plugin and target_plugin.
      prev_plugin->AddNextPlugin(target_plugin);
      // Connect the target_plugin and next_plugin.
      for (int i = 0; i < next_plugins.size(); i++) {
        target_plugin->AddNextPlugin(next_plugins[i]);
      }
      DEBUG_PRINT("PluginManager::ConnectPlugin Success connect %s and ",
                  target_plugin->plugin_name().c_str());
      for (int i = 0; i < next_plugins.size(); i++) {
        if (i == next_plugins.size() - 1) {
          DEBUG_PRINT("%s\n", next_plugins[i]->plugin_name().c_str());
        } else {
          DEBUG_PRINT("%s, ", next_plugins[i]->plugin_name().c_str());
        }
      }
      // Update output port prev_plugin and target_plugin
      UpdateOutputPortSpec(prev_plugin);
      UpdateOutputPortSpec(target_plugin);

      return true;
    } else {
      DEBUG_PRINT(
          "PluginManager::ConnectPlugin fail. prev and next plugin not "
          "connectable.\n");
    }
  }
  return false;
}

/**
 * @brief
 * Replace the plug on the Flow.
 * @param prev_plugin_name [in] plugin name to connect to the previous
 * @param old_target_plugin_name [in] replacement before the plugin name
 * @param new_target_plugin_name [in] replacement after the  plugin name
 * @param next_plugin_name [in] plugin name to connect to the next
 *                              (In the case of a branch, you can specified more
 * than one.)
 * @return If true, success replacement.
 */
bool PluginManager::ReplacePlugin(
    const std::string prev_plugin_name,
    const std::string old_target_plugin_name,
    const std::string new_target_plugin_name,
    const std::vector<std::string> next_plugin_names) {
  PluginBase* prev_plugin = GetPlugin(prev_plugin_name);
  PluginBase* old_target_plugin = GetPlugin(old_target_plugin_name);
  PluginBase* new_target_plugin = GetPlugin(new_target_plugin_name);
  if (prev_plugin == NULL || old_target_plugin == NULL ||
      new_target_plugin == NULL) {
    DEBUG_PRINT("[PluginManager::ReplacePlugin] plugin = NULL\n");
    return false;
  }
  // If there is no input port of next_plugin, return false.
  std::vector<PluginBase*> next_plugins;
  for (int i = 0; i < next_plugin_names.size(); i++) {
    PluginBase* next_plugin = GetPlugin(next_plugin_names[i]);
    if (next_plugin != NULL) {
      next_plugins.push_back(next_plugin);
    } else {
      DEBUG_PRINT("[PluginManager::ReplacePlugin] next_plugin == NULL");
      return false;
    }
  }
  // If there is no output port of prev_plugin, return false.
  std::vector<PortSpec*> prev_output_port_specs =
      prev_plugin->output_port_candidate_specs();
  if (prev_output_port_specs.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::ReplacePlugin prev_plugin input port num is zero\n");
    return false;
  }
  // If there is no input port of new_target_plugin, return false.
  std::vector<PortSpec*> target_input_port_specs =
      new_target_plugin->input_port_candidate_specs();
  if (target_input_port_specs.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::ReplacePlugin new_target_plugin input port num is "
        "zero\n");
    return false;
  }

  // Check prev_plugin and new_target_plugin can be connected
  bool prev_target_connectable = false;
  for (int i = 0; i < prev_output_port_specs.size(); i++) {
    PortSpec* prev_output_spec = prev_output_port_specs[i];
    for (int j = 0; j < target_input_port_specs.size(); j++) {
      PortSpec* target_input_spec = target_input_port_specs[j];
      if (prev_output_spec->plane_type() == target_input_spec->plane_type()) {
        prev_target_connectable = true;
        break;
      }
    }
  }

  // When adding to the end of the Flow
  if (next_plugins.size() == 0) {
    // Connect the prev_plugin and new_target_plugin, update the output port.
    if (prev_target_connectable) {
      prev_plugin->ClearNextPlugins(old_target_plugin_name);
      old_target_plugin->ClearNextPlugins();
      prev_plugin->AddNextPlugin(new_target_plugin);
      UpdateOutputPortSpec(old_target_plugin);
      UpdateOutputPortSpec(prev_plugin);
      DEBUG_PRINT("PluginManager::ReplacePlugin success. %s and %s\n",
                  prev_plugin_name.c_str(), new_target_plugin_name.c_str());
      return true;
    } else {
      DEBUG_PRINT(
          "PluginManager::ReplacePlugin fail. prev and next plugin not "
          "connectable.\n");
      return false;
    }
  } else {  // In the case of insertion
    if (prev_target_connectable) {
      // If there is no output port of new_target_plugin, return false.
      std::vector<PortSpec*> new_target_output_port_specs =
          new_target_plugin->output_port_candidate_specs();
      if (new_target_output_port_specs.size() == 0) {
        DEBUG_PRINT(
            "PluginManager::ReplacePlugin target_plugin output port num is "
            "zero\n");
        return false;
      }
      // If there is no input port of next_plugin, return false.
      for (int i = 0; i < next_plugins.size(); i++) {
        std::vector<PortSpec*> next_input_port_specs =
            next_plugins[i]->input_port_candidate_specs();
        if (next_input_port_specs.size() == 0) {
          DEBUG_PRINT(
              "PluginManager::ReplacePlugin next_plugin(%s) input port num is "
              "zero\n",
              next_plugins[i]->plugin_name().c_str());
          return false;
        }
      }
      // Check target_plugin and next_plugin can be connected.
      bool* target_next_connectable = new bool(next_plugins.size());
      bool all_target_next_connectable = true;
      for (int i = 0; i < next_plugins.size(); i++) {
        target_next_connectable[i] = false;
      }
      for (int i = 0; i < next_plugins.size(); i++) {
        std::vector<PortSpec*> next_input_port_specs =
            next_plugins[i]->input_port_candidate_specs();
        for (int j = 0; j < next_input_port_specs.size(); j++) {
          PortSpec* next_input_port_spec = next_input_port_specs[j];
          for (int k = 0; k < new_target_output_port_specs.size(); k++) {
            PortSpec* new_target_output_port_spec =
                new_target_output_port_specs[k];
            if (new_target_output_port_spec->plane_type() ==
                next_input_port_spec->plane_type()) {
              target_next_connectable[i] = true;
            }
          }
        }
      }

      for (int i = 0; i < next_plugins.size(); i++) {
        if (!target_next_connectable[i]) {
          DEBUG_PRINT(
              "PluginManager::ReplacePlugin Not connectable %s and %s\n",
              new_target_plugin->plugin_name().c_str(),
              next_plugins[i]->plugin_name().c_str());
          all_target_next_connectable = false;
        }
      }
      // If target_plugin and next_plugin can not connect, return false.
      if (!all_target_next_connectable) {
        return false;
      }
      delete[] target_next_connectable;

      // Disconnect the prev_plugin and old_target_plugin.
      prev_plugin->ClearNextPlugins(old_target_plugin_name);
      old_target_plugin->ClearNextPlugins();
      // Connect the prev_plugin and new_target_plugin.
      prev_plugin->AddNextPlugin(new_target_plugin);
      // Connect the new_target_plugin and next_plugins.
      for (int i = 0; i < next_plugins.size(); i++) {
        new_target_plugin->AddNextPlugin(next_plugins[i]);
      }
      DEBUG_PRINT("PluginManager::ReplacePlugin Success connect %s and ",
                  new_target_plugin->plugin_name().c_str());
      for (int i = 0; i < next_plugins.size(); i++) {
        if (i == next_plugins.size() - 1) {
          DEBUG_PRINT("%s\n", next_plugins[i]->plugin_name().c_str());
        } else {
          DEBUG_PRINT("%s, ", next_plugins[i]->plugin_name().c_str());
        }
      }
      // Updates the output port.
      UpdateOutputPortSpec(old_target_plugin);
      UpdateOutputPortSpec(prev_plugin);
      UpdateOutputPortSpec(new_target_plugin);

      return true;
    } else {
      DEBUG_PRINT(
          "PluginManager::ReplacePlugin fail. prev and next plugin not "
          "connectable.\n");
    }
  }
  return false;
}

/**
 * @brief
 * Release the connection of the specified plugin.
 * @param target_plugin_name [in] plugin name to release the connection
 */
void PluginManager::DisconnectAllPlugin(const std::string target_plugin_name) {
  PluginBase* target_plugin = GetPlugin(target_plugin_name);
  if (target_plugin == NULL) {
    return;
  }

  // If the target_plugin is not connected, update the output port, then return.
  std::vector<IPlugin*> target_next_plugins = target_plugin->next_plugins();
  if (target_next_plugins.size() <= 0) {
    std::vector<PortSpec*> output_port_candidate_specs =
        target_plugin->output_port_candidate_specs();
    for (int i = 0; i < output_port_candidate_specs.size(); i++) {
      PortSpec* output_port = output_port_candidate_specs[i];
      output_port->set_available(true);
    }
    return;
  }
  target_plugin->RemoveNextPlugin();

  return;
}

/**
 * @brief
 * Release the connection of specified branch for plugin
 * @param target_plugin_name [in] plugin name of the previous branch
 * @param branch_plugin_name [in] plugin name after the branching
 */
void PluginManager::DisconnectPlugin(const std::string target_plugin_name,
                                     const std::string branch_plugin_name) {
  PluginBase* target_plugin = GetPlugin(target_plugin_name);
  if (target_plugin == NULL) {
    return;
  }

  // If the target_plugin is not connected, update the output port, then return.
  std::vector<IPlugin*> target_next_plugins = target_plugin->next_plugins();
  if (target_next_plugins.size() <= 0) {
    return;
  }

  // Release the connection of the specified plugin.
  for (int i = 0; i < target_next_plugins.size(); i++) {
    PluginBase* target_next_plugin =
        reinterpret_cast<PluginBase*>(target_next_plugins[i]);
    std::string next_plugin_name = target_next_plugin->plugin_name();
    if (branch_plugin_name == next_plugin_name) {
      target_plugin->RemoveNextPlugin(branch_plugin_name);
    }
  }
  UpdateOutputPortSpec(target_plugin);
}

/**
 * @brief
 * Check whether it is possible to image processing by the current
 * of the Flow and setting of each plug-in.
 * @param target_plugin [in] Pointer to the PluginBase class
 * @return If true, image processing can be executed.
 */
bool PluginManager::CheckExecuteSetting(PluginBase* target_plugin) {
  DEBUG_PRINT("PluginManager::CheckExecuteSetting() start\n");
  if (target_plugin == NULL) {
    return false;
  }
  PluginBase* plugin = target_plugin;
  PluginBase* next_plugin = NULL;
  PluginBase* temp_next_plugin = NULL;
  std::queue<PluginBase*> branch_plugins;
  bool ret = true;

  // Check between all of the plugin in Flow.
  while (1) {
    if (plugin->next_plugins().size() > 0) {
      next_plugin = reinterpret_cast<PluginBase*>(plugin->next_plugins()[0]);
      if (!CheckPortRelation(plugin, next_plugin)) {
        ret = false;
        break;
      }
      temp_next_plugin = next_plugin;
      for (int i = 1; i < plugin->next_plugins().size(); i++) {
        next_plugin = reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
        if (!CheckPortRelation(plugin, next_plugin)) {
          ret = false;
          break;
        }
        branch_plugins.push(next_plugin);
      }
      plugin = temp_next_plugin;
    } else {
      if (!branch_plugins.empty()) {
        plugin = branch_plugins.front();
        branch_plugins.pop();
        if (plugin == NULL) {
          break;
        }
      } else {
        break;
      }
    }
  }
  DEBUG_PRINT("PluginManager::CheckExecuteSetting() end\n");
  return ret;
}

/**
 * @brief
 * Get the newest plugin interface version that can be VPF to process.
 * @return newest plugin interface version.
 */
int PluginManager::TargetPluginIFVersion() { return PLUGIN_INTERFACE_VERSION; }

/**
 * @brief
 * Get the oldest plugin interface version that can be VPF to process.
 * @return oldest plugin interface version.
 */
int PluginManager::MinimumRequiredPluginIFVersion() {
  return MIN_REQUIRED_PLUGIN_INTERFACE_VERSION;
}

/**
 * @brief
 * Get the version of the VPF.
 * @return version of the VPF.
 */
int PluginManager::MajorVersion(void) { return MAJOR_VERSION; }

/**
 * @brief
 * Load the plugins for the specified PluginType.
 * @param path [in] file path
 * @param plugin_type [in] plugin type for read.
 * @param reload [in] whether to reload
 * @return If true, success to read plugin.
 */
bool PluginManager::LoadPluginWithPluginType(const std::string path,
                                             PluginType plugin_type,
                                             bool reload) {
  DIR* dp;
  dirent* entry;
  void* handle;
  PluginBaseCreate* create;
  PluginBase* plugin;
  std::string file_name, plugin_dir_name, file_path;

  DEBUG_PRINT("Load LoadPluginWithPluginType type = %d.\n", plugin_type);

  // Create a directory name of each PluginType.
  switch (plugin_type) {
    case kInputPlugin:
      plugin_dir_name = "/input/";
      break;
    case kIspPlugin:
      plugin_dir_name = "/isp/";
      break;
    case kOutputPlugin:
      plugin_dir_name = "/output/";
      break;
    default:
      DEBUG_PRINT("Not correct PlaneType.\n");
      return false;
  }

  DEBUG_PRINT("LoadPluginWithPluginType plugin_dir_name = %s.\n",
              plugin_dir_name.c_str());
  DEBUG_PRINT("LoadPluginWithPluginType path + plugin_dir_name = %s.\n",
              (path + plugin_dir_name).c_str());

  // Open a directory where the SO file is stored.
  dp = opendir((path + plugin_dir_name).c_str());
  if (dp == NULL) {
    LOG_WARNING("Can't open dir:%s",
                wxString::FromUTF8((path + plugin_dir_name).c_str()).c_str());
    return false;
  }

  do {
    entry = readdir(dp);
    if (entry != NULL) {
      file_name = entry->d_name;
      if (static_cast<int>(file_name.find(".so")) < 0) {
        continue;
      }
#ifdef VPF_WORKAROUND_RELOAD_PLUGIN
      if (reload) {
        std::string::size_type index = file_name.find("Sensor");
        if ((index != std::string::npos) && (plugin_type == kInputPlugin)) {
          continue;  // Skip
        }
      }
#endif
      file_path = path + plugin_dir_name + file_name;
      DEBUG_PRINT("LoadPluginWithPluginType file_path = %s\n",
                  file_path.c_str());

      handle = dlopen(file_path.c_str(), RTLD_LAZY);
      if (handle == 0) {
        LOG_WARNING("Failed to dlopen:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to dlopen:%s\n", dlerror());
        continue;
      }

      create = reinterpret_cast<PluginBaseCreate*>(dlsym(handle, "Create"));
      if (create == 0) {
        LOG_WARNING("Failed to dlsym:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to dlsym:%s\n", dlerror());
        continue;
      }

      plugin = create();
      if (plugin == NULL) {
        LOG_WARNING("Failed to create plugin:%s",
                    wxString::FromUTF8(file_path.c_str()).c_str());
        DEBUG_PRINT("Failed to create plugin:%s\n", file_path.c_str());
        continue;
      }
      if (!IsExecutablePluginInterfaceVersion(
              plugin->plugin_interface_version())) {
        LOG_WARNING("Unmatch plugin interface version %s %d",
                    wxString::FromUTF8(plugin->plugin_name().c_str()).c_str(),
                    plugin->plugin_interface_version());
        LOG_WARNING(
            "System requirements for plugin interface version between %d and "
            "%d",
            MIN_REQUIRED_PLUGIN_INTERFACE_VERSION, PLUGIN_INTERFACE_VERSION);
        continue;
      }

      // Set new plugin name
      std::ostringstream stream;
      stream << plugin->plugin_name() << "-"
             << plugin->plugin_interface_version();
      std::string new_plugin_name = stream.str();
      if (IsDuplicatePluginName(new_plugin_name)) {
        LOG_WARNING("Failed to load %s file. %s is already loaded.",
                    wxString::FromUTF8(file_path.c_str()).c_str(),
                    wxString::FromUTF8(new_plugin_name.c_str()).c_str());
        continue;
      }
      plugin->set_plugin_name(new_plugin_name);

      // Set plugin type
      plugin->set_plugin_type(plugin_type);
      // Set plugin filepath
      plugin->set_file_path(file_path);
      // Set Log function
      plugin->set_logger_func(reinterpret_cast<void*>(&Logger::LogWrite));

      PluginData plugin_data;
      plugin_data.handle = handle;
      plugin_data.plugin = plugin;
      all_plugins_.push_back(plugin_data);

      LOG_MESSAGE("%s plugin loaded",
                  wxString::FromUTF8(file_path.c_str()).c_str());
    }
  } while (entry != NULL);

  return true;
}

/**
 * @brief
 * Load the plugins for the specified PluginType and name.
 * @param path [in] file path
 * @param plugin_type [in] plugin type for read
 * @param plugin_name [in] plugin name to be set
 * @return If true, success to load plugin.
 */
bool PluginManager::LoadPlugin(const std::string path, PluginType plugin_type,
                               const std::string plugin_name) {
  void* handle;
  PluginBaseCreate* create;
  PluginBase* plugin;
  std::string file_path = path;

  DEBUG_PRINT("LoadPlugin file_path = %s\n", file_path.c_str());

  handle = dlopen(file_path.c_str(), RTLD_LAZY);
  if (handle == 0) {
    LOG_WARNING("Failed to dlopen:%s", wxString::FromUTF8(dlerror()).c_str());
    DEBUG_PRINT("Failed to dlopen:%s\n", dlerror());
    return false;
  }

  create = reinterpret_cast<PluginBaseCreate*>(dlsym(handle, "Create"));
  if (create == 0) {
    LOG_WARNING("Failed to dlsym:%s", wxString::FromUTF8(dlerror()).c_str());
    DEBUG_PRINT("Failed to dlsym:%s\n", dlerror());
    return false;
  }

  plugin = create();
  if (plugin == NULL) {
    LOG_WARNING("Failed to create plugin:%s",
                wxString::FromUTF8(file_path.c_str()).c_str());
    DEBUG_PRINT("Failed to create plugin:%s\n", file_path.c_str());
    return false;
  }
  if (!IsExecutablePluginInterfaceVersion(plugin->plugin_interface_version())) {
    wxString temp_plugin_name(plugin->plugin_name().c_str(), wxConvUTF8);
    LOG_WARNING("Unmatch plugin interface version %s %d",
                wxString::FromUTF8(plugin->plugin_name().c_str()).c_str(),
                plugin->plugin_interface_version());
    LOG_WARNING(
        "System requirements for plugin interface version between %d and "
        "%d",
        MIN_REQUIRED_PLUGIN_INTERFACE_VERSION, PLUGIN_INTERFACE_VERSION);
    return false;
  }
  std::string new_plugin_name = plugin_name;
  // Set new plugin name
  if (new_plugin_name == "") {
    std::ostringstream stream;
    stream << plugin->plugin_name() << "-"
           << plugin->plugin_interface_version();
    new_plugin_name = stream.str();
    if (IsDuplicatePluginName(new_plugin_name)) {
      LOG_WARNING("Failed to load %s file. %s is already loaded.",
                  wxString::FromUTF8(file_path.c_str()).c_str(),
                  wxString::FromUTF8(new_plugin_name.c_str()).c_str());
      return false;
    }
  }
  plugin->set_plugin_name(new_plugin_name);

  // Set plugin type
  plugin->set_plugin_type(plugin_type);
  // Set plugin filepath
  plugin->set_file_path(file_path);
  // Set Log function
  plugin->set_logger_func(reinterpret_cast<void*>(&Logger::LogWrite));

  PluginData plugin_data;
  plugin_data.handle = handle;
  plugin_data.plugin = plugin;
  all_plugins_.push_back(plugin_data);

  LOG_MESSAGE("%s plugin loaded",
              wxString::FromUTF8(file_path.c_str()).c_str());

  return true;
}

/**
 * @brief
 * Load the stored plugins in the specified directory.
 * @param path   [in] file path
 * @param reload [in] whether to reload
 * @return If true, success to load plugin.
 */
bool PluginManager::LoadPlugins(const std::string path, bool reload) {
  plugin_root_dir_ = path;
  ReleasePlugins(reload);
  if (LoadPluginWithPluginType(path, kInputPlugin, reload) == false) {
    DEBUG_PRINT("LoadPlugins input plugin not loaded.\n");
    return false;
  }
  if (LoadPluginWithPluginType(path, kIspPlugin, reload) == false) {
    DEBUG_PRINT("LoadPlugins isp plugin not loaded.\n");
    return false;
  }
  if (LoadPluginWithPluginType(path, kOutputPlugin, reload) == false) {
    DEBUG_PRINT("LoadPlugins output plugin not loaded.\n");
    return false;
  }
  return true;
}

/**
 * @brief
 * Release resource of all plugins.
 * @param reload [in] whether to reload
 */
void PluginManager::ReleasePlugins(bool reload) {
  DEBUG_PRINT("PluginManager::ReleasePlugins()\n");
  std::vector<PluginData>::iterator itr;
  void* handle;
  PluginBase* plugin;
  PluginBaseDestroy* destroy;

#ifdef VPF_WORKAROUND_RELOAD_PLUGIN
  std::vector<PluginData> temp_plugins;
#endif
  for (itr = all_plugins_.begin(); itr != all_plugins_.end(); itr++) {
    PluginData plugin_data = (*itr);
    plugin = plugin_data.plugin;
    handle = plugin_data.handle;
#ifdef VPF_WORKAROUND_RELOAD_PLUGIN
    if (reload) {
      std::string::size_type index = plugin->plugin_name().find("Sensor");
      if ((index != std::string::npos) &&
          (plugin->plugin_type() == kInputPlugin)) {
        temp_plugins.push_back(plugin_data);  // Backup plugin data
        plugin->ClearNextPlugins();
        continue;  // Skip
      }
    }
#endif
    destroy = reinterpret_cast<PluginBaseDestroy*>(dlsym(handle, "Destroy"));
    if (destroy == 0) {
      LOG_WARNING("Failed to destroy:%s",
                  wxString::FromUTF8(dlerror()).c_str());
      DEBUG_PRINT("Failed to destroy:%s\n", dlerror());
      return;
    }

    destroy(plugin);

    if (dlclose(handle) != 0) {
      LOG_WARNING("Failed to dlclose:%s",
                  wxString::FromUTF8(dlerror()).c_str());
      DEBUG_PRINT("Failed to dlclose:%s\n", dlerror());
      return;
    }
  }

  DeleteClonedPlugins(plugin_root_dir_, kInputPlugin);
  DeleteClonedPlugins(plugin_root_dir_, kIspPlugin);
  DeleteClonedPlugins(plugin_root_dir_, kOutputPlugin);

  plugin_root_dir_ = "";
  all_plugins_.clear();
#ifdef VPF_WORKAROUND_RELOAD_PLUGIN
  for (itr = temp_plugins.begin(); itr != temp_plugins.end(); itr++) {
    PluginData plugin_data = (*itr);
    all_plugins_.push_back(plugin_data);  // ReEntry plugin data
  }
#endif
  root_plugin_ = NULL;
}

/**
 * @brief
 * Release the resource of specified plugin.
 * @param target_plugin [in] Pointer to the PluinBase class
 */
void PluginManager::ReleasePlugin(PluginBase* target_plugin) {
  DEBUG_PRINT("PluginManager::ReleasePlugin()\n");
  std::vector<PluginData>::iterator itr;
  void* handle;
  PluginBase* plugin;
  PluginBaseDestroy* destroy;

  for (itr = all_plugins_.begin(); itr != all_plugins_.end(); itr++) {
    plugin = (*itr).plugin;
    handle = (*itr).handle;
    if (plugin == target_plugin) {
      destroy = reinterpret_cast<PluginBaseDestroy*>(dlsym(handle, "Destroy"));
      if (destroy == 0) {
        LOG_WARNING("Failed to destroy:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to destroy:%s\n", dlerror());
        return;
      }

      destroy(plugin);

      if (dlclose(handle) != 0) {
        LOG_WARNING("Failed to dlclose:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to dlclose:%s\n", dlerror());
        return;
      }
      itr = all_plugins_.erase(itr);
      return;
    }
  }
}

/**
 * @brief
 * Release the resources of cloned plugins.
 */
void PluginManager::ReleaseAllClonePlugin(void) {
  DEBUG_PRINT("PluginManager::ReleasePlugin()\n");
  std::vector<PluginData>::iterator itr;
  void* handle;
  PluginBase* plugin;
  PluginBaseDestroy* destroy;

  for (itr = all_plugins_.begin(); itr != all_plugins_.end(); itr++) {
    plugin = (*itr).plugin;
    handle = (*itr).handle;
    if (plugin->is_cloned()) {
      destroy = reinterpret_cast<PluginBaseDestroy*>(dlsym(handle, "Destroy"));
      if (destroy == 0) {
        LOG_WARNING("Failed to destroy:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to destroy:%s\n", dlerror());
        return;
      }

      destroy(plugin);

      if (dlclose(handle) != 0) {
        LOG_WARNING("Failed to dlclose:%s",
                    wxString::FromUTF8(dlerror()).c_str());
        DEBUG_PRINT("Failed to dlclose:%s\n", dlerror());
        return;
      }
      all_plugins_.erase(itr);
      itr--;
    }
  }
}

/**
 * @brief
 * Check whether the plugin interface version that can be executed.
 * @param [in] plugin interface version
 * @return If true, executable.
 */
bool PluginManager::IsExecutablePluginInterfaceVersion(int plugin_if_ver) {
  if (MIN_REQUIRED_PLUGIN_INTERFACE_VERSION <= plugin_if_ver &&
      plugin_if_ver <= PLUGIN_INTERFACE_VERSION) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief
 * Get the image type information of OpenCV from PlaneType.
 * @param state [in] PlaneType
 * @return image type information of OpenCV
 */
int PluginManager::GetDepthAndChannelType(PlaneType plane_type) {
  int arr_size =
      sizeof(plane_type_info_array) / sizeof(plane_type_info_array[0]);
  for (int i = 0; i < arr_size; i++) {
    if (plane_type == plane_type_info_array[i].plane_type) {
      return plane_type_info_array[i].type;
    }
  }
  // Not found
  return plane_type_info_array[arr_size - 1].type;
}

/**
 * @brief
 * If duplicate plugin name, rename it automatically.
 * @param target_plugin_name [in] target plugin name
 * @return plugin name after rename
 */
std::string PluginManager::CheckDuplicateAndRenamePlugin(
    std::string target_plugin_name) {
  unsigned int duplicate_count = 0;
  std::string plugin_name = target_plugin_name;

  // If the plugin name is a duplicate, then rename.
  while (IsDuplicatePluginName(plugin_name)) {
    std::string suffix = ".";
    if (duplicate_count > 0) {
      int size = plugin_name.rfind(suffix);
      plugin_name.resize(size);
    }
    std::ostringstream stream;
    stream << plugin_name << suffix << ++duplicate_count;
    plugin_name = stream.str();
  }
  return plugin_name;
}

/**
 * @brief
 * Check whether there is the same name of the specified plugin name
 * to the plugins that was already loaded.
 * @param plugin [in] plugin name
 * @return If true, duplicate.
 */
bool PluginManager::IsDuplicatePluginName(std::string plugin_name) {
  bool ret = false;
  for (int i = 0; i < all_plugins_.size(); i++) {
    PluginData plugin_data = all_plugins_[i];
    std::string tmp_plugin_name = plugin_data.plugin->plugin_name();
    if (plugin_name == tmp_plugin_name) {
      ret = true;
      break;
    }
  }
  return ret;
}

/**
 * @brief
 * Check whether the specified plugin is existing on the Flow.
 * @param plugin [in] Pointer to the PluginBase class
 * @return If true, exist plugin.
 */
bool PluginManager::IsExistPluginForFlow(PluginBase* target_plugin) {
  if (root_plugin_ == NULL) {
    return false;
  }
  PluginBase* plugin = root_plugin_;
  PluginBase* next_plugin = NULL;
  PluginBase* temp_next_plugin = NULL;
  std::queue<PluginBase*> branch_plugins;
  bool is_exist = false;

  while (1) {
    if (plugin == target_plugin) {
      is_exist = true;
      DEBUG_PRINT("PluginManager::IsExistPluginForFlow %s is exist\n",
                  target_plugin->plugin_name().c_str());
      break;
    }
    if (plugin->next_plugins().size() > 0) {
      next_plugin = reinterpret_cast<PluginBase*>(plugin->next_plugins()[0]);
      temp_next_plugin = next_plugin;
      for (int i = 1; i < plugin->next_plugins().size(); i++) {
        next_plugin = reinterpret_cast<PluginBase*>(plugin->next_plugins()[i]);
        branch_plugins.push(next_plugin);
      }
      plugin = temp_next_plugin;
    } else {
      if (!branch_plugins.empty()) {
        plugin = branch_plugins.front();
        branch_plugins.pop();
        if (plugin == NULL) {
          break;
        }
      } else {
        break;
      }
    }
  }
  return is_exist;
}

/**
 * @brief
 * Update the availability of the output port.
 * @param plugin [in] plugin
 */
void PluginManager::UpdateOutputPortSpec(PluginBase* plugin) {
  DEBUG_PRINT("PluginManager::UpdateOutputPortSpec() name = %s\n",
              plugin->plugin_name().c_str());
  std::vector<PortSpec*> output_port_candidate_specs =
      plugin->output_port_candidate_specs();
  if (output_port_candidate_specs.size() == 0) {
    return;
  }

  // If the plug is not connected, it set to available of the
  // output port, then return.
  std::vector<IPlugin*> next_plugins = plugin->next_plugins();
  if (next_plugins.size() == 0) {
    for (int i = 0; i < output_port_candidate_specs.size(); i++) {
      PortSpec* output_port_candidate_spec = output_port_candidate_specs[i];
      output_port_candidate_spec->set_available(true);
    }
    return;
  }

  // If the plug is connected, re-set the output port.
  // Once, to disable all output port.
  for (int i = 0; i < output_port_candidate_specs.size(); i++) {
    PortSpec* output_port_candidate_spec = output_port_candidate_specs[i];
    output_port_candidate_spec->set_available(false);
  }

  // re-set the target of the output port.
  for (int i = 0; i < output_port_candidate_specs.size(); i++) {
    PortSpec* output_port_spec = output_port_candidate_specs[i];
    PlaneType output_plane_type = output_port_spec->plane_type();
    for (int j = 0; j < next_plugins.size(); j++) {
      PluginBase* next_plugin = reinterpret_cast<PluginBase*>(next_plugins[j]);
      std::vector<PortSpec*> input_port_candidate_specs =
          next_plugin->input_port_candidate_specs();
      for (int k = 0; k < input_port_candidate_specs.size(); k++) {
        PortSpec* next_input_port_spec = input_port_candidate_specs[k];
        PlaneType next_input_plane_type = next_input_port_spec->plane_type();
        if (output_plane_type == next_input_plane_type) {
          DEBUG_PRINT(
              "PluginManager::UpdateOutputPortSpec() false-->true plane_type = "
              "%d\n",
              output_port_spec->plane_type());
          output_port_spec->set_available(true);
        }
      }
    }
  }
}

/**
 * @brief
 * Check whether the image processing can be executed, by the input-output
 * relation and output port availability.
 * @param prev_plugin [in] Pointer to the PluginBase class for prev plugin
 * @param next_plugin [in] Pointer to the PluginBase class for next plugin
 * return If true, next_plugin has been set to the output port
 *        that can be image processing.
 */
bool PluginManager::CheckPortRelation(PluginBase* prev_plugin,
                                      PluginBase* next_plugin) {
  DEBUG_PRINT("PluginManager::CheckPortRelation start\n");
  if (prev_plugin == NULL || next_plugin == NULL) {
    DEBUG_PRINT("PluginManager::CheckPortRelation prev or next plugin NULL\n");
    return false;
  }
  DEBUG_PRINT("PluginManager::CheckPortRelation prev = %s, next = %s\n",
              prev_plugin->plugin_name().c_str(),
              next_plugin->plugin_name().c_str());
  // Check whether the prev_plugin output port is set correctly.
  PortSpec* prev_output_port_spec = prev_plugin->output_port_spec();
  if (prev_output_port_spec == NULL) {
    DEBUG_PRINT(
        "PluginManager::CheckPortRelation %s output port spec is NULL\n",
        prev_plugin->plugin_name().c_str());
    return false;
  }

  std::vector<PortRelation*> next_port_relation = next_plugin->port_relations();
  // If the port relationship of next_plugin does not exist, then return true.
  if (next_port_relation.size() == 0) {
    DEBUG_PRINT(
        "PluginManager::CheckPortRelation next_port_relation.size() == 0\n");
    return true;
  }

  // Find the next_plugin input port of the same as prev_plugin output port.
  // And then, it counts the port relationship.
  int num = 0;
  PortSpec* next_input_port;
  for (int i = 0; i < next_port_relation.size(); i++) {
    next_input_port = next_port_relation[i]->input_port;
    if (prev_output_port_spec->plane_type() == next_input_port->plane_type()) {
      num++;
    }
  }
  DEBUG_PRINT("PluginManager::CheckPortRelation relation num = %d\n", num);

  // If there is no port relationship.
  if (num == 0) {
    return false;
  }

  PlaneType next_input_plane_type = prev_output_port_spec->plane_type();
  bool is_exist_active_port = false;
  // If the port relationship is only one,
  if (num == 1) {
    PortSpec* next_output_port = NULL;
    // Get the output port of the port relation.
    for (int i = 0; i < next_port_relation.size(); i++) {
      if (next_input_plane_type ==
          next_port_relation[i]->input_port->plane_type()) {
        next_output_port = next_port_relation[i]->output_port;
      }
    }
    if (next_output_port == NULL) {
      DEBUG_PRINT(
          "PluginManager::CheckPortRelation relation num = 1 next_output_port "
          "== NULL\n");
      return false;
    } else {
      // Update the active index to the corresponding output port.
      std::vector<PortSpec*> next_output_port_candidate_specs =
          next_plugin->output_port_candidate_specs();
      for (int i = 0; i < next_output_port_candidate_specs.size(); i++) {
        PortSpec* tmp_next_output_port = next_output_port_candidate_specs[i];
        if (next_output_port->plane_type() ==
            tmp_next_output_port->plane_type()) {
          if (next_output_port->available()) {
            next_plugin->set_active_output_port_spec_index(i);
            DEBUG_PRINT(
                "PluginManager::CheckPortRelation relation num = 1 "
                "next_output_port->available() == true\n");
            is_exist_active_port = true;
            break;
          } else {
            DEBUG_PRINT(
                "PluginManager::CheckPortRelation relation num = 1 "
                "next_output_port->available() == false\n");
          }
        }
      }
      return is_exist_active_port;
    }
  } else {  // Port relationship is 2 or more.
    std::vector<PortSpec*> next_output_ports;
    for (int i = 0; i < next_port_relation.size(); i++) {
      if (next_input_plane_type ==
          next_port_relation[i]->input_port->plane_type()) {
        next_output_ports.push_back(next_port_relation[i]->output_port);
      }
    }

    if (next_output_ports.size() == 0) {
      DEBUG_PRINT(
          "PluginManager::CheckPortRelation relation num = %d "
          "next_output_ports.size() = %d\n",
          num, next_output_ports.size());
      return false;
    } else {
      int active_port_spec_index = next_plugin->active_output_port_spec_index();
      std::vector<PortSpec*> next_output_port_candidate_specs =
          next_plugin->output_port_candidate_specs();
      for (int i = 0; i < next_output_ports.size(); i++) {
        for (int index = 0; index < next_output_port_candidate_specs.size();
             index++) {
          if (next_output_ports[i]->plane_type() ==
              next_output_port_candidate_specs[index]->plane_type()) {
            if (index == active_port_spec_index) {
              if (next_output_port_candidate_specs[index]->available()) {
                DEBUG_PRINT(
                    "PluginManager::CheckPortRelation relation num = %d "
                    "next_output_port_spec->available() == true\n");
                is_exist_active_port = true;
                break;
              } else {
                DEBUG_PRINT(
                    "PluginManager::CheckPortRelation relation num = %d "
                    "next_output_port_spec->available() == false\n");
              }
            }
          }
        }
      }
    }
    if (is_exist_active_port == false) {
      LOG_ERROR("Not correct plugin connect : %s to %s",
                wxString::FromUTF8(prev_plugin->plugin_name().c_str()).c_str(),
                wxString::FromUTF8(next_plugin->plugin_name().c_str()).c_str());
    }
    return is_exist_active_port;
  }
}

/**
 * @brief
 * Replicate the specified plugin.
 * @param target_plugin [in] Pointer to the IPlugin class
 * @return pointer to the replicated plugin
 */
IPlugin* PluginManager::ClonePlugin(IPlugin* target_plugin) {
  IPlugin* new_plugin = NULL;
  if (target_plugin == NULL) return new_plugin;
  PluginBase* plugin = reinterpret_cast<PluginBase*>(target_plugin);
  // std::string new_plugin_name = plugin->plugin_name() + ".1";
  std::string new_plugin_name = plugin->plugin_name();
  new_plugin_name = CheckDuplicateAndRenamePlugin(new_plugin_name);

  std::string src_file_path = plugin->file_path();
  std::string dst_file_path = "";
  std::string::size_type index = src_file_path.rfind("/");
  if (index != std::string::npos) {
    std::string dir = src_file_path.substr(0, index);
    std::string file = src_file_path.substr(index + 1);
    index = new_plugin_name.rfind(".");
    std::string suffix = new_plugin_name.substr(index);
    dst_file_path = dir + "/" + "." + file + suffix;
  }
  if (CopyFile(src_file_path, dst_file_path)) {
    if (LoadPlugin(dst_file_path, plugin->plugin_type(), new_plugin_name)) {
      new_plugin = GetPlugin(new_plugin_name);
      (reinterpret_cast<PluginBase*>(new_plugin))
          ->SetCloneParameter(plugin->plugin_name());
      remove(dst_file_path.c_str());
    }
  }
  return new_plugin;
}

/**
 * @brief
 * Remove the SO file that was created when the plugin cloned.
 * @param path [in] file path
 * @param plugin_type [in] target plugin type.
 */
void PluginManager::DeleteClonedPlugins(const std::string path,
                                        PluginType plugin_type) {
  DIR* dp;
  dirent* entry;
  std::string file_name, plugin_dir_name, file_path;

  DEBUG_PRINT("DeleteClonedPlugins type = %d.\n", plugin_type);

  /* Create a directory name of each PluginType. */
  switch (plugin_type) {
    case kInputPlugin:
      plugin_dir_name = "/input/";
      break;
    case kIspPlugin:
      plugin_dir_name = "/isp/";
      break;
    case kOutputPlugin:
      plugin_dir_name = "/output/";
      break;
    default:
      DEBUG_PRINT("Not correct PlaneType.\n");
      return;
  }

  DEBUG_PRINT("DeleteClonedPlugins plugin_dir_name = %s.\n",
              plugin_dir_name.c_str());
  DEBUG_PRINT("DeleteClonedPlugins path + plugin_dir_name = %s.\n",
              (path + plugin_dir_name).c_str());

  /* Open a directory where the SO file is stored. */
  dp = opendir((path + plugin_dir_name).c_str());
  if (dp == NULL) {
    return;
  }

  do {
    entry = readdir(dp);
    if (entry != NULL) {
      file_name = entry->d_name;
      std::string first_charactor = file_name.substr(0, 1);
      if (first_charactor == ".") {
        if (static_cast<int>(file_name.find(".so")) > 0) {
          std::string file_path = path + plugin_dir_name + file_name;
          remove(file_path.c_str());
        }
      }
    }
  } while (entry != NULL);

  return;
}

/**
 * @brief
 * Copy the file in the specified path.
 * @param src_file_name [in] source file path
 * @param dst_file_name [in] dest file path
 * @return If true, succeed to copy
 */
bool PluginManager::CopyFile(std::string src_file_name,
                             std::string dst_file_name) {
  FILE* fp_src;
  FILE* fp_dst;
  int number;
  char c;
  fp_src = fopen(src_file_name.c_str(), "rb");
  if (fp_src == NULL) {
    number = errno;
    LOG_ERROR("Can't open file: %s(%s)",
              wxString::FromUTF8(src_file_name.c_str()).c_str(),
              wxString::FromUTF8(strerror(number)).c_str());
    return false;
  }

  fp_dst = fopen(dst_file_name.c_str(), "wb");
  if (fp_dst == NULL) {
    number = errno;
    LOG_ERROR("Can't open file: %s(%s)",
              wxString::FromUTF8(dst_file_name.c_str()).c_str(),
              wxString::FromUTF8(strerror(number)).c_str());
    return false;
  }
  while (1) {
    fread(&c, sizeof(c), 1, fp_src);
    if (feof(fp_src)) {
      break;
    }
    if (ferror(fp_src)) {
      number = errno;
      LOG_ERROR("Detect ferror while reading file: %s(%s)",
                wxString::FromUTF8(src_file_name.c_str()).c_str(),
                wxString::FromUTF8(strerror(number)).c_str());
      return false;
    }
    fwrite(&c, sizeof(c), 1, fp_dst);
  }
  if (fclose(fp_dst) == EOF) {
    number = errno;
    LOG_ERROR("Can't close file: %s(%s)",
              wxString::FromUTF8(dst_file_name.c_str()).c_str(),
              wxString::FromUTF8(strerror(number)).c_str());
    return false;
  }
  if (fclose(fp_src) == EOF) {
    number = errno;
    LOG_ERROR("Can't close file: %s(%s)",
              wxString::FromUTF8(src_file_name.c_str()).c_str(),
              wxString::FromUTF8(strerror(number)).c_str());
    return false;
  }
  return true;
}
