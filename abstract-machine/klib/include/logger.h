#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <am.h>

/**
 * @brief console colors
 */
#define FG_BLACK   "\033[1;30m"
#define FG_RED     "\033[1;31m"
#define FG_GREEN   "\033[1;32m"
#define FG_YELLOW  "\033[1;33m"
#define FG_BLUE    "\033[1;34m"
#define FG_PURPLE  "\033[1;35m"
#define FG_CYAN    "\033[1;36m"
#define FG_WHITE   "\033[1;37m"
#define BG_BLACK   "\033[1;40m"
#define BG_RED     "\033[1;41m"
#define BG_GREEN   "\033[1;42m"
#define BG_YELLOW  "\033[1;43m"
#define BG_BLUE    "\033[1;44m"
#define BG_PURPLE  "\033[1;45m"
#define BG_CYAN    "\033[1;46m"
#define BG_WHITE   "\033[1;47m"
#define COLOR_NONE "\033[0m"

/**
 * @brief Debug log control bit
 */
#define LOG_SUCCESS 0x01
#define LOG_INFO    0x02
#define LOG_WARN    0x04
#define LOG_ERROR   0x08

/**
 * @brief LOG_MASK control whether to output log message.
 *
 * Function log() will print the message only if
 * (type & LOG_MASK == true).
 *
 *  0: disable
 * 16: all
 * 12: only WARN and ERROR
 *  3: only INFO and SUCCESS
 *
 */
#ifndef LOG_MASK
#define LOG_MASK \
  (LOG_INFO | LOG_WARN | LOG_ERROR)  // defalt print INFO and above
#endif

extern const char* type_str[10];

#ifdef NOLOGGING
#define log(type, format, ...) ;
#else
#define log(type, format, ...) \
  if (type & LOG_MASK) { \
    printf("#%d %s: " format "\n", cpu_current(), type_str[type], \
           ##__VA_ARGS__); \
  }
#define log_detail(type, format, ...) \
  if (type & LOG_MASK) { \
    printf("#%d %s (%s:%d, %s):\n" format "\n", cpu_current(), type_str[type], \
           __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
  }
#define success(format, ...) log(LOG_SUCCESS, format, ##__VA_ARGS__)
#define info(format, ...)    log(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...)    log(LOG_WARN, format, ##__VA_ARGS__)
#define error(format, ...)   log(LOG_ERROR, format, ##__VA_ARGS__)
#define success_detail(format, ...) \
  log_detail(LOG_SUCCESS, format, ##__VA_ARGS__)
#define info_detail(format, ...)  log_detail(LOG_INFO, format, ##__VA_ARGS__)
#define warn_detail(format, ...)  log_detail(LOG_WARN, format, ##__VA_ARGS__)
#define error_detail(format, ...) log_detail(LOG_ERROR, format, ##__VA_ARGS__)
#endif

#ifdef DEBUG
#define Log(format, ...) \
  printf(COLOR_NONE BG_BLUE "[%d][%s,%d,%s] " format " \33[0m\n", _cpu(), \
         __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define CLog(color, format, ...) \
  printf("\33[0m" color "[%d][%s,%d,%s] " format " \33[0m\n", _cpu(), \
         __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define Log(format, ...) ;
#define CLog(color, format, ...)
#endif
#define Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
      printf("\33[0m" BG_RED "[%d][%s,%d,%s] " format " \33[0m\n", _cpu(), \
             __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
      assert(cond); \
    } \
  } while (0)

#define Panic(format, ...) \
  printf("\33[0m" BG_RED "[%d][%s,%d,%s] " format " \33[0m\n", _cpu(), \
         __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
  assert(0)

/**
 * @brief Control whether enable function trace
 */
#ifdef TRACE_F
#define TRACE_ENTRY printf("[trace] %s:entry\n", __func__)
#define TRACE_EXIT  printf("[trace] %s:exit\n", __func__)
#else
#define TRACE_ENTRY ((void)0)
#define TRACE_EXIT  ((void)0)
#endif

#endif
