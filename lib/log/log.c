/**
 * @file log.c
 * @author Junde Yhi <lmy441900@aosc.xyz>
 * @copyright (C) 2019-2020 Anthon Open Source Community
 *
 * Implementation of the logging infrastructure for libaoscdk.
 */

#include "config.h"
#include "log.h"

/**
 * Defines the logging domain for g_log functions, which should be the name of
 * the project.
 */
#define G_LOG_DOMAIN PROJECT_NAME

// They need to be included after the #G_LOG_DOMAIN definition
#include <glib.h>
#include <gio/gio.h>

/**
 * Private type indicating the current state of log output.
 */
enum DkLogOutput {
  DK_LOG_OUTPUT_FILE,
  DK_LOG_OUTPUT_G_LOG,
};

/**
 * The current state of log output.
 */
static enum DkLogOutput log_output_g = DK_LOG_OUTPUT_G_LOG;

/**
 * If log is outputted to a file, this is it.
 */
static GFile *log_file_g = NULL;

/**
 * Output stream of #log_file_g.
 */
static GFileOutputStream *log_file_stream_g = NULL;

/********** Private APIs **********/

/**
 * Log to a file.
 *
 * @param level [in] Level of the log.
 * @param file  [in] The name of file where the log is written.
 * @param line  [in] The number of line where the log is written.
 * @param func  [in] The name of function in which the log is written.
 * @param fmt   [in] A `printf`-like format string.
 * @param args  [in] A variadic list containing all arguments.
 */
static void dk_logv_file(const enum DkLogLevel level, const char *file, const int line, const char *func, const char *fmt, va_list args)
{
  g_return_if_fail(log_file_g);

  gchar *msg = g_strdup_vprintf(fmt, args);
  gboolean r = FALSE;
  GError *error = NULL;

  switch (level) {
    case DK_LOG_LEVEL_DEBUG:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Debug: %s:%d %s: %s\n", file, line, func, msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write a debugging log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }
      break;
    case DK_LOG_LEVEL_INFO:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Info: %s:%d %s: %s\n", file, line, func, msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write an informational log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }
      break;
    case DK_LOG_LEVEL_MESSAGE:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Message: %s\n", msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write a message-log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }
      break;
    case DK_LOG_LEVEL_WARNING:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Warning: %s\n", msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write a warning-log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }
      break;
    case DK_LOG_LEVEL_ERROR:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Error: %s:%d %s: %s\n", file, line, func, msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write an erroneous log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }
      break;
    case DK_LOG_LEVEL_FATAL:
      r = g_output_stream_printf(G_OUTPUT_STREAM(log_file_stream_g), NULL, NULL, &error, "Fatal Error: %s:%d %s: %s\n", file, line, func, msg);
      if (!r && error) {
        dk_log_set_output_g_log();
        dk_error("Failed to write a fatally erroneous log to file: %s. The log message is \"%s\". Falling back to GLog logging method.", error->message, msg);
        dk_log_file_close();
        g_clear_error(&error);
      }

      G_BREAKPOINT(); // Maintain consistency with dk_logv_glog()
      break;
    default:
      g_warn_if_reached();
      break;
  }

  g_free(msg);
}

/**
 * Log using the `g_log` facilities provided by GLib.
 *
 * @param level [in] Level of the log.
 * @param file  [in] The name of file where the log is written.
 * @param line  [in] The number of line where the log is written.
 * @param func  [in] The name of function in which the log is written.
 * @param fmt   [in] A `printf`-like format string.
 * @param args  [in] A variadic list containing all arguments.
 */
static void dk_logv_glog(const enum DkLogLevel level, const char *file, const int line, const char *func, const char *fmt, va_list args)
{
  gchar *line_str    = g_strdup_printf("%d", line);
  gchar *log_message = g_strdup_vprintf(fmt, args);

  switch (level) {
    case DK_LOG_LEVEL_DEBUG:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    case DK_LOG_LEVEL_INFO:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_LEVEL_INFO,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    case DK_LOG_LEVEL_MESSAGE:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    case DK_LOG_LEVEL_WARNING:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_LEVEL_WARNING,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    case DK_LOG_LEVEL_ERROR:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    case DK_LOG_LEVEL_FATAL:
      g_log_structured(
        G_LOG_DOMAIN, G_LOG_FLAG_FATAL,
        "CODE_FILE", file,
        "CODE_LINE", line_str,
        "CODE_FUNC", func,
        "MESSAGE", log_message
      );
      break;
    default:
      g_warn_if_reached();
      break;
  }

  g_free(log_message);
  g_free(line_str);
}

/********** Public APIs **********/

void dk_log(enum DkLogLevel level, const char *file, const int line, const char *func, const char *fmt, ...)
{
  g_return_if_fail(file != NULL);
  g_return_if_fail(line >= 0);
  g_return_if_fail(func != NULL);
  g_return_if_fail(fmt != NULL);

  va_list args;
  va_start(args, fmt);

  switch (log_output_g) {
    case DK_LOG_OUTPUT_FILE:
      dk_logv_file(level, file, line, func, fmt, args);
      break;
    case DK_LOG_OUTPUT_G_LOG:
      dk_logv_glog(level, file, line, func, fmt, args);
      break;
    default:
      g_warn_if_reached();
      break;
  }

  va_end(args);
}

int dk_log_set_output_file(const char *path)
{
  g_return_val_if_fail(path, 0);

  int r = 0;
  GError *error = NULL;

  dk_debug("Opening log file at %s", path);

  if (log_output_g == DK_LOG_OUTPUT_FILE) {
    // Close previous opened files
    r = dk_log_file_close();
    if (!r)
      return r;
  }

  GFile *log_file = g_file_new_for_path(path);
  GFileOutputStream *log_file_stream = g_file_replace(log_file, NULL, TRUE, G_FILE_CREATE_NONE, NULL, &error);
  if (!log_file_stream && error) {
    dk_warning("Failed to create a log file: %s", error->message);
    g_clear_error(&error);
    return 0;
  }

  log_file_g = log_file;
  log_file_stream_g = log_file_stream;
  log_output_g = DK_LOG_OUTPUT_FILE;

  dk_debug("Opened log file at %s", path);

  return 1;
}

int dk_log_set_output_g_log(void)
{
  dk_debug("Switching to g_log logging method...");

  log_output_g = DK_LOG_OUTPUT_G_LOG;

  dk_debug("Switched to g_log logging method.");

  return 1;
}

int dk_log_file_close(void)
{
  gboolean r = FALSE;
  GError *error = NULL;

  dk_info("Closing the log file");

  // The file is going to be closed, switch logging to g_log
  dk_log_set_output_g_log();

  if (log_file_stream_g) {
    if (!g_output_stream_is_closed(G_OUTPUT_STREAM(log_file_stream_g))) {
      r = g_output_stream_close(G_OUTPUT_STREAM(log_file_stream_g), NULL, &error);
      if (!r && error) {
        dk_warning("File stream cannot be closed: %s. Anyway.", error->message);
        g_clear_error(&error);
      }
    }

    g_clear_object(&log_file_stream_g);
  }

  if (log_file_g) {
    g_clear_object(&log_file_g);
  }

  return 1;
}
