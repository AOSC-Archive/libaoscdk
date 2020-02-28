/**
 * @file msg.h
 * @author Junde Yhi <lmy441900@aosc.xyz>
 * @copyright (C) 2019-2020 Anthon Open Source Community
 *
 * Definition of the data structures and the corresponding methods of the
 * messages going through the aynchronous queue inside the libaoscdk
 * logging infrastructure.
 */

#ifndef LIBAOSCDK_LOG_MSG_H
#define LIBAOSCDK_LOG_MSG_H

#include <log.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * Type of the message going through #log_queue_g.
 */
struct DkLogMsg {
  bool worker_exit;      ///< Whether the logging worker should exit.
  enum DkLogLevel level; ///< Logging level.
  char *file;            ///< The name of file where the log is sent.
  unsigned int line;     ///< The number of line where the log is sent.
  char *func;            ///< The name of function where the log is sent.
  char *log;             ///< The log message.
};

/**
 * Create a #DkLogMsg instance.
 *
 * @param level [in] Logging level.
 * @param file  [in] The name of file where the log is sent.
 * @param line  [in] The number of line where the log is sent.
 * @param func  [in] The name of function where the log is sent.
 * @param msg   [in] The log message.
 * @return A #DkLogMsg with the corresponding fields initialized.
 */
struct DkLogMsg *dk_log_msg_new(const enum DkLogLevel level, const char *file, const unsigned int line, const char *func, const char *msg);

/**
 * Create a #DkLogMsg instance, with DkLogMsg::log be created using variadic
 * arguments.
 *
 * @param level    [in] Logging level.
 * @param file     [in] The name of file where the log is sent.
 * @param line     [in] The number of line where the log is sent.
 * @param func     [in] The name of function where the log is sent.
 * @param log_fmt  [in] A `printf`-like format string.
 * @param log_args [in] A `va_list`.
 * @return A #DkLogMsg with the corresponding fields initialized.
 */
struct DkLogMsg *dk_log_msg_new_v(const enum DkLogLevel level, const char *file, const unsigned int line, const char *func, const char *log_fmt, va_list log_args);

/**
 * Create a #DkLogMsg instance, but with only DkLogMsg::worker_exit set.
 *
 * This is used to signal the worker to quit.
 *
 * @return A #DkLogMsg with only DkLogMsg::worker_exit set to `true`.
 */
struct DkLogMsg *dk_log_msg_new_worker_exit(void);

/**
 * Free the memory occupied by a #DkLogMsg.
 *
 * @param msg [in] A #DkLogMsg.
 */
void dk_log_msg_free(struct DkLogMsg *msg);

#endif
