/**
 * @file      thread_running_cycle_manager.cpp
 * @brief     Source for ThreadRunningCycleManager class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./thread_running_cycle_manager.h"
#include <string>
#include <vector>
#include "./include.h"

/**
 * @brief
 * Constructor.
 */
ThreadRunningCycleManager::ThreadRunningCycleManager() {}

/**
 * @brief
 * Destructor.
 */
ThreadRunningCycleManager::~ThreadRunningCycleManager() { DeleteAllCycle(); }

/**
 * @brief
 * Add new cycle value. 
 * If a plug-in name is already registered, overwrites a cycle value.
 * @param src_plugin_name [in] source plugin name.
 * @param dst_plugin_name [in] destination plugin name.
 * @param cycle [in] cycle value.
 */
void ThreadRunningCycleManager::AddCycle(std::string src_plugin_name,
                                         std::string dst_plugin_name,
                                         unsigned int cycle) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  ThreadRunningCycleInfo* info = NULL;
  for (itr = thread_cycle_info_.begin(); itr != thread_cycle_info_.end();
       itr++) {
    info = *itr;
    if (info) {
      if ((info->src_plugin_name == src_plugin_name) &&
          (info->dst_plugin_name == dst_plugin_name)) {
        info->cycle = cycle;
        return;
      }
    }
  }
  info = new ThreadRunningCycleInfo;
  info->src_plugin_name = src_plugin_name;
  info->dst_plugin_name = dst_plugin_name;
  info->cycle = cycle;
  thread_cycle_info_.push_back(info);
  return;
}


/**
 * @brief
 * Get a cycle value. 
 * @param src_plugin_name [in] target source plugin name.
 * @param dst_plugin_name [in] target destination plugin name.
 * @return cycle value.
 */
unsigned int ThreadRunningCycleManager::GetCycle(std::string src_plugin_name,
                                                 std::string dst_plugin_name) {
  unsigned int cycle = 0;
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  ThreadRunningCycleInfo* info = NULL;
  for (itr = thread_cycle_info_.begin(); itr != thread_cycle_info_.end();
       itr++) {
    info = *itr;
    if (info) {
      if ((info->src_plugin_name == src_plugin_name) &&
          (info->dst_plugin_name == dst_plugin_name)) {
        return info->cycle;
      }
    }
  }
  return cycle;
}

/**
 * @brief
 * Delete all cycle value. 
 */
void ThreadRunningCycleManager::DeleteAllCycle() {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      delete info;
    }
    itr++;
  }
  thread_cycle_info_.clear();
}

/**
 * @brief
 * Delete cycle value. 
 * @param src_plugin_name [in] target source plugin name.
 */
void ThreadRunningCycleManager::DeleteCycle(std::string src_plugin_name) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      if (info->src_plugin_name == src_plugin_name) {
        delete info;
        itr = thread_cycle_info_.erase(itr);
        continue;
      }
    }
    itr++;
  }
}


/**
 * @brief
 * Delete cycle value. 
 * @param src_plugin_name [in] target source plugin name.
 * @param dst_plugin_name [in] target destination plugin name.
 */
void ThreadRunningCycleManager::DeleteCycle(std::string src_plugin_name,
                                            std::string dst_plugin_name) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      if ((info->src_plugin_name == src_plugin_name) &&
          (info->dst_plugin_name == dst_plugin_name)) {
        delete info;
        itr = thread_cycle_info_.erase(itr);
        return;
      }
    }
    itr++;
  }
}

/**
 * @brief
 * Update a source plugin name. 
 * @param old_plugin_name [in] target source plugin name.
 * @param new_plugin_name [in] new source plugin name.
 */
void ThreadRunningCycleManager::UpdateSrcName(std::string old_plugin_name,
                                              std::string new_plugin_name) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      if (info->src_plugin_name == old_plugin_name) {
        info->src_plugin_name = new_plugin_name;
      }
    }
    itr++;
  }
}

/**
 * @brief
 * Update a destination plugin name. 
 * @param old_plugin_name [in] target destination plugin name.
 * @param new_plugin_name [in] new destination plugin name.
 */
void ThreadRunningCycleManager::UpdateDstName(std::string src_plugin_name,
                                              std::string old_plugin_name,
                                              std::string new_plugin_name) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      if ((info->src_plugin_name == src_plugin_name) &&
          (info->dst_plugin_name == old_plugin_name)) {
        info->dst_plugin_name = new_plugin_name;
      }
    }
    itr++;
  }
}

/**
 * @brief
 * Update a destination plugin name. 
 * @param src_plugin_name [in] target source plugin name.
 * @param old_plugin_name [in] target destination plugin name.
 * @param new_plugin_name [in] new destination plugin name.
 */
void ThreadRunningCycleManager::UpdateDstName(std::string old_plugin_name,
                                              std::string new_plugin_name) {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      if (info->dst_plugin_name == old_plugin_name) {
        info->dst_plugin_name = new_plugin_name;
      }
    }
    itr++;
  }
}

void ThreadRunningCycleManager::Dump() {
  std::vector<ThreadRunningCycleInfo*>::iterator itr;
  itr = thread_cycle_info_.begin();
  while (itr != thread_cycle_info_.end()) {
    ThreadRunningCycleInfo* info = *itr;
    if (info) {
      DEBUG_PRINT("[ThreadRunningCycleManager] src:%s, dst:%s, cycle:%d\n",
                  info->src_plugin_name.c_str(), info->dst_plugin_name.c_str(),
                  info->cycle);
    }
    itr++;
  }
}
