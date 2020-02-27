/**
 * @file msg.c
 * @author Junde Yhi <lmy441900@aosc.xyz>
 * @copyright (C) 2019-2020 Anthon Open Source Community
 *
 * Implementation of the data structures and the corresponding methods of the
 * messages going through the aynchronous queue inside the libaoscdk
 * logging infrastructure.
 */

#include "msg.h"
#include <log.h>
#include <glib.h>
#include <stdbool.h>

struct DkLogMsg *dk_log_msg_new(const enum DkLogLevel level, const char *file, const unsigned int line, const char *func, const char *log)
{
  struct DkLogMsg *msg = g_malloc0(sizeof(struct DkLogMsg));

  msg->worker_exit = false;
  msg->level = level;
  msg->file  = g_strdup(file);
  msg->line  = line;
  msg->func  = g_strdup(func);
  msg->log   = g_strdup(log);

  return msg;
}

struct DkLogMsg *dk_log_msg_new_v(const enum DkLogLevel level, const char *file, const unsigned int line, const char *func, const char *log_fmt, va_list log_args)
{
  char *log = g_strdup_vprintf(log_fmt, log_args);

  struct DkLogMsg *msg = dk_log_msg_new(level, file, line, func, log);

  g_free(log);
  return msg;
}

struct DkLogMsg *dk_log_msg_new_worker_exit(void)
{
  struct DkLogMsg *msg = g_malloc0(sizeof(struct DkLogMsg));
  msg->worker_exit = true;
  return msg;
}

void dk_log_msg_free(struct DkLogMsg *msg)
{
  msg->worker_exit = false;
  msg->level = DK_LOG_LEVEL_FATAL; // XXX
  g_clear_pointer(&msg->file, g_free);
  msg->line = 0;
  g_clear_pointer(&msg->func, g_free);
  g_clear_pointer(&msg->log, g_free);

  g_free(msg);
}
