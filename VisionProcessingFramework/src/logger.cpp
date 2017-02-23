/**
 * @file      logger.cpp
 * @brief     Source for Logger class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./logger.h"
#include <queue>

#define MAX_LOG_MESSAGE_LENGTH 256
#define MAX_LOG_QUEUE_SIZE 256
static wxMutex mutex_output_log;
static std::queue<LogMessage*> log_queue;

/**
 * @brief
 * Logger Initilaization.
 * Set a log output target to the log_text_ctrl object.
 * @param log_text_ctrl [in] Pointer to the wxLogTextCtrl class object.
 */
void Logger::Initialize(wxLogTextCtrl* log_text_ctrl) {
  wxLog::SetVerbose(true);
  wxLog::SetActiveTarget(log_text_ctrl);
}

/**
 * @brief
 * Enqueue a log message.
 * @param level [in] log level.
 * @param module_name [in] module name.
 * @param message [in] log message.
 */
void Logger::LogWrite(enum LogLevel level, wxString module_name,
                      wxString message, ...) {
  va_list args;
  va_start(args, message);

  wxChar c[MAX_LOG_MESSAGE_LENGTH];
  wxVsnprintf(c, sizeof(c), message, args);
  wxString arg(c);
  wxString format = wxT("[") + module_name + wxT("] ") + arg;
  wxMutexLocker lock(mutex_output_log);
  if (lock.IsOk()) {
    LogMessage* log = NULL;
    if (log_queue.size() >= MAX_LOG_QUEUE_SIZE) {
      log = log_queue.front();
      log_queue.pop();
      if (log) {
        delete log;
      }
    }
    log = new LogMessage(level, format);
    log_queue.push(log);
  }
  va_end(args);
}

/**
 * @brief
 * Get a log message from a log queue.
 * @param level [out] log level.
 * @param message [out] log message.
 * @return false, no log message.
 */
bool Logger::GetLogMessage(enum LogLevel* level, wxString* message) {
  wxMutexLocker lock(mutex_output_log);
  if (level == NULL || message == NULL) return false;
  if (log_queue.empty()) return false;
  if (lock.IsOk()) {
    LogMessage* log = log_queue.front();
    log_queue.pop();
    if (log) {
      *level = log->level();
      *message = log->message();
      delete log;
    }
  }
  return true;
}
