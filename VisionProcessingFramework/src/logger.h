/**
 * @file      logger.h
 * @brief     Header for Logger class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#pragma once

#include <wx/log.h>
#include <wx/string.h>
#include <wx/thread.h>
#include "./log_level.h"

/**
 * @class LogMessage
 * @brief This class has a log message and the log level.
  */
class LogMessage {
 public:
  /**
   * @brief
   * Constructor.
   * @param level   [in] log level.
   * @param message [in] log message.
   */
  LogMessage(enum LogLevel level, wxString message) {
    level_ = level;
    message_ = message;
  }

  /**
   * @brief
   * Get a log level.
   * @return log level,
   */
  enum LogLevel level() { return level_; }

  /**
   * @brief
   * Get a log message.
   * @return log message,
   */
  wxString message() { return message_; }

 private:
  /*! log level */
  enum LogLevel level_;

  /*! log message */
  wxString message_;
};

/**
 * @class Logger
 * @brief This class provides a method to output log message.
 */
class Logger {
 public:
  /**
   * @brief
   * Logger Initilaization.
   * Set a log output target to the log_text_ctrl object.
   * @param log_text_ctrl [in] Pointer to the wxLogTextCtrl class object.
   */
  static void Initialize(wxLogTextCtrl* log_text_ctrl);

  /**
   * @brief
   * Enqueue a log message.
   * @param level [in] log level.
   * @param module_name [in] module name.
   * @param message [in] log message.
   */
  static void LogWrite(enum LogLevel level, wxString module_name,
                       wxString message, ...);
  /**
   * @brief
   * Get a log message from a log queue.
   * @param level [out] log level.
   * @param message [out] log message.
   * @return false, no log message.
   */
  static bool GetLogMessage(enum LogLevel* level, wxString* message);
};

#define MODULE_NAME wxT("VPF")
// UserMacro
#define LOG_FATAL_ERROR(fmt, ...) \
  Logger::LogWrite(kLogFatalError, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) \
  Logger::LogWrite(kLogError, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...) \
  Logger::LogWrite(kLogWarning, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
#define LOG_MESSAGE(fmt, ...) \
  Logger::LogWrite(kLogMessage, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
#define LOG_VERBOSE(fmt, ...) \
  Logger::LogWrite(kLogVerbose, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
#define LOG_STATUS(fmt, ...) \
  Logger::LogWrite(kLogStatus, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
// #define LOG_SYS_ERROR(fmt, ...) Logger::LogWrite(kLogSysError,
//  MODULE_NAME, wxT(fmt), ## __VA_ARGS__);
#define LOG_DEBUG(fmt, ...) \
  Logger::LogWrite(kLogDebug, MODULE_NAME, wxT(fmt), ##__VA_ARGS__);
// #define LOG_TRACE(fmt, ...) Logger::LogWrite(kLogTrace,
//  MODULE_NAME, wxT(fmt), ## __VA_ARGS__);
