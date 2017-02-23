/**
 * @file      common_function.h
 * @brief     Header for CommonFunction class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <string>
#include "./include.h"

/**
 * @class CommonFunction
 * @brief This class implements the public APIs.
 */
class CommonFunction {
 public:
 /**
  * @brief
  * This method is not implemented.
  * @param file_type [in]
  * @param path      [in]
  * @return true
  */
  bool SaveAsImage(FileType file_type, const std::string path);

 /**
  * @brief
  * This method is not implemented.
  * @param address [in]
  * @param data    [in]
  */
  void WriteRegister(int address, int data);

 /**
  * @brief
  * This method is not implemented.
  * @param address [in]
  * @return register value
  */
  int ReadRegister(int address);

 /**
  * @brief
  * This method is not implemented.
  */
  void StartMonitor(void);

 /**
  * @brief
  * This method is not implemented.
  */
  void StopMonitor(void);

 /**
  * @brief
  * This method is not implemented.
  */
  void PauseMonitor(void);

 /**
  * @brief
  * This method is not implemented.
  * @return monitoring state
  */
  int GetMonitorState(void);

 /**
  * @brief
  * This method is not implemented.
  */
  void InitPlugin(void);

 /**
  * @brief
  * This method is not implemented.
  */
  void DoPlugin(void);
};

#endif /* _COMMON_FUNCTION_H_ */
