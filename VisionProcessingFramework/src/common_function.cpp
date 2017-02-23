/**
 * @file      common_function.cpp
 * @brief     Source for CommonFunction class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./common_function.h"
#include <string>

/**
 * @brief
 * This method is not implemented.
 * @param file_type [in]
 * @param path      [in]
 * @return true
 */
bool CommonFunction::SaveAsImage(FileType file_type, const std::string path) {
  DEBUG_PRINT("CommonFunction::SaveAsImage\n");
  return true;
}

 /**
  * @brief
  * This method is not implemented.
  * @param address [in]
  * @param data    [in]
  */
void CommonFunction::WriteRegister(int address, int data) {
  DEBUG_PRINT("CommonFunction::WriteRegister\n");
}

 /**
  * @brief
  * This method is not implemented.
  * @param address [in]
  * @return register value
  */
int CommonFunction::ReadRegister(int address) {
  DEBUG_PRINT("CommonFunction::ReadRegister\n");
  return 1;
}

/**
 * @brief
 * This method is not implemented.
 */
void CommonFunction::StartMonitor(void) {
  DEBUG_PRINT("CommonFunction::StartMonitor\n");
}

/**
 * @brief
 * This method is not implemented.
 */
void CommonFunction::StopMonitor(void) {
  DEBUG_PRINT("CommonFunction::StopMonitor\n");
}

/**
 * @brief
 * This method is not implemented.
 */
void CommonFunction::PauseMonitor(void) {
  DEBUG_PRINT("CommonFunction::SaveAsImagePauseMonitorn");
}

/**
 * @brief
 * This method is not implemented.
 * @return monitoring state
 */
int CommonFunction::GetMonitorState(void) {
  DEBUG_PRINT("CommonFunction::GetMonitorState\n");
  return 1;
}

/**
 * @brief
 * This method is not implemented.
 */
void CommonFunction::InitPlugin(void) {
  DEBUG_PRINT("CommonFunction::InitPlugin\n");
}

/**
 * @brief
 * This method is not implemented.
 */
void CommonFunction::DoPlugin(void) {
  DEBUG_PRINT("CommonFunction::DoPlugin\n");
}
