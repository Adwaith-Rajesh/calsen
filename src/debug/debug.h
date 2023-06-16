#ifndef _INCLUDE_DEBUG_H
#define _INCLUDE_DEBUG_H

#ifndef DEBUG
#define DEBUG 1
#endif

#include <stdarg.h>
#include <stdio.h>

// ANSI color code for different log levels

#define C_INFO "\x1b[34m"   // COLOR_INFO
#define C_WARN "\x1b[33m"   // COLOR_WARN
#define C_ERROR "\x1b[31m"  // COLOR_ERROR
#define C_RESET "\x1b[0m"   // COLOR_RESET

typedef enum {
    INFO,
    WARN,
    ERROR,
} LogLevelT;

// execute B if DEBUG=1
#define DEBUG_RUN(B) \
    if (DEBUG) B

#define log_error(fmt, ...) \
    DEBUG_RUN(d_log(stderr, ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__);)

#define log_warn(fmt, ...) \
    DEBUG_RUN(d_log(stderr, WARN, __FILE__, __LINE__, fmt, __VA_ARGS__);)

#define log_info(fmt, ...) \
    DEBUG_RUN(d_log(stderr, INFO, __FILE__, __LINE__, fmt, __VA_ARGS__);)

void debug_print(const char *fmt, ...);
void d_log(FILE *stream, LogLevelT level, const char *filename, int line_no, const char *fmt, ...);

#endif
