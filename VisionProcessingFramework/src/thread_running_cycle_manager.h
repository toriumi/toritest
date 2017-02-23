/**
 * @file      thread_running_cycle_manager.h
 * @brief     Header for ThreadRunningCycleManager class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

typedef struct {
  std::string src_plugin_name;
  std::string dst_plugin_name;
  unsigned int cycle;
} ThreadRunningCycleInfo;

/**
 * @class ThreadRunningCycleManager
 * @brief This class manages the cycle value for branch point.
 */
class ThreadRunningCycleManager {
 public:
  /**
   * @brief
   * Constructor.
   */
  ThreadRunningCycleManager();

  /**
   * @brief
   * Destructor.
   */
  ~ThreadRunningCycleManager();

  /**
   * @brief
   * Add new cycle value. 
   * If a plug-in name is already registered, overwrites a cycle value.
   * @param src_plugin_name [in] source plugin name.
   * @param dst_plugin_name [in] destination plugin name.
   * @param cycle [in] cycle value.
   */
  void AddCycle(std::string src_plugin_name, std::string dst_plugin_name,
                unsigned int cycle);

  /**
   * @brief
   * Delete all cycle value. 
   */
  void DeleteAllCycle();

  /**
   * @brief
   * Delete cycle value. 
   * @param src_plugin_name [in] target source plugin name.
   */
  void DeleteCycle(std::string src_plugin_name);

  /**
   * @brief
   * Delete cycle value. 
   * @param src_plugin_name [in] target source plugin name.
   * @param dst_plugin_name [in] target destination plugin name.
   */
  void DeleteCycle(std::string src_plugin_name, std::string dst_plugin_name);

  /**
   * @brief
   * Dump cycle value table. 
   */
  void Dump();

  /**
   * @brief
   * Get a cycle value. 
   * @param src_plugin_name [in] target source plugin name.
   * @param dst_plugin_name [in] target destination plugin name.
   * @return cycle value.
   */
  unsigned int GetCycle(std::string src_plugin_name,
                        std::string dst_plugin_name);

  /**
   * @brief
   * Update a source plugin name. 
   * @param old_plugin_name [in] target source plugin name.
   * @param new_plugin_name [in] new source plugin name.
   */
  void UpdateSrcName(std::string old_plugin_name, std::string new_plugin_name);

  /**
   * @brief
   * Update a destination plugin name. 
   * @param old_plugin_name [in] target destination plugin name.
   * @param new_plugin_name [in] new destination plugin name.
   */
  void UpdateDstName(std::string old_plugin_name, std::string new_plugin_name);

  /**
   * @brief
   * Update a destination plugin name. 
   * @param src_plugin_name [in] target source plugin name.
   * @param old_plugin_name [in] target destination plugin name.
   * @param new_plugin_name [in] new destination plugin name.
   */
  void UpdateDstName(std::string src_plugin_name, std::string old_plugin_name,
                     std::string new_plugin_name);

 private:
  /*! Map table to manage cycle value */
  std::vector<ThreadRunningCycleInfo*> thread_cycle_info_;
};
