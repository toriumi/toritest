/**
 * @file      log_level.h
 * @brief     Header for LogLevel definition
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#pragma once

/** LogLevel enumration */
enum LogLevel {
  kLogFatalError = 0,
  kLogError,
  kLogWarning,
  kLogMessage,
  kLogVerbose,
  kLogStatus,
  //  kLogSysError,
  kLogDebug,
  //  kLogTrace,
};
