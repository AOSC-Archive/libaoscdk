/**
 * @file log.h
 * @author Junde Yhi <lmy441900@aosc.xyz>
 * @copyright (C) 2019-2020 Anthon Open Source Community
 *
 * Definition of the logging infrastructure for libaoscdk.
 */

#ifndef AOSC_DK_LOG_H
#define AOSC_DK_LOG_H

/**
 * Levels of logging.
 */
enum DkLogLevel {
  DK_LOG_LEVEL_DEBUG,   ///< Debugging message.
  DK_LOG_LEVEL_INFO,    ///< Information.
  DK_LOG_LEVEL_MESSAGE, ///< General message.
  DK_LOG_LEVEL_WARNING, ///< Warning.
  DK_LOG_LEVEL_ERROR,   ///< Error.
  DK_LOG_LEVEL_FATAL,   ///< Fatal error.
};

/**
 * Write a log message.
 *
 * Usually you don't want to use this function. Use the convenient macros
 * dk_debug(), dk_info(), dk_message(), dk_warning(), dk_error(), and dk_fatal()
 * instead.
 *
 * Note that on fatal message logging, this function will signal a breakpoint
 * (`SIGTRAP`). This is because GLib's `g_log` does the same thing, and since
 * you can switch to `g_log`, we make this behavior consistent across different
 * logging methods.
 *
 * @param level [in] Logging level.
 * @param file  [in] The name of source file where dk_log() is called.
 * @param line  [in] The number of line where dk_log() is called.
 * @param func  [in] The name of function in which dk_log() is called.
 * @param fmt   [in] A `printf`-like format string.
 */
void dk_log(const enum DkLogLevel level, const char *file, const int line, const char *func, const char *fmt, ...);

/**
 * Log a debugging message.
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_debug(fmt, ...) dk_log(DK_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Log an informational message.
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_info(fmt, ...) dk_log(DK_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Log a message.
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_message(fmt, ...) dk_log(DK_LOG_LEVEL_MESSAGE, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Log a warning message.
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_warning(fmt, ...) dk_log(DK_LOG_LEVEL_WARNING, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Log an error message.
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_error(fmt, ...) dk_log(DK_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Log a fatal error message.
 *
 * Note that this will signal a `SIGTRAP`. See dk_log().
 *
 * @param fmt [in] A `printf`-like format string.
 */
#define dk_fatal(fmt, ...) dk_log(DK_LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/**
 * Set log output to the designated file.
 *
 * If the file specified by `path` already exists, the original file will be
 * backed-up with suffix `~`, and a new log file will be created. If the logging
 * method is switched to another one without first closing the previous log
 * file, then if the latter path is identical to the former path, that log file
 * will be re-used; otherwise the old one will be closed and the new one will be
 * created.
 *
 * @param path [in] Path to the log file.
 * @return Non-0 if the operation succeed, or 0 if the file cannot be created.
 */
int dk_log_set_output_file(const char *path);

/**
 * Set log output to g_log.
 *
 * @return Non-0 if the operation succeed.
 */
int dk_log_set_output_g_log(void);

/**
 * Close the file opened by #dk_log_set_output_file().
 *
 * @return Non-0 if the operation succeed.
 */
int dk_log_file_close(void);

/**
 * Initialize the logging module.
 *
 * @return Non-0 if the operation succeed.
 */
int dk_log_init(void);

/**
 * Deinitialize the logging module.
 *
 * @return Non-0 if the operation succeed.
 */
int dk_log_deinit(void);

#endif
