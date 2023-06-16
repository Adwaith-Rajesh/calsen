#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

// returns the ANSI color code for the corresponding log level
static inline const char *_log_level_to_color(LogLevelT level) {
    return ((level == INFO) ? C_INFO : ((level == WARN)) ? C_WARN: C_ERROR);
}

// clang-format on

void d_log(FILE *stream, LogLevelT level, const char *filename, int line_no, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // <ANSI-color-code>[enum as str]<ansi reset code> filename:lino fmt
    fprintf(stream, "%s[%5s]%s %s:%d: ", _log_level_to_color(level),
            (level == ERROR) ? "ERROR" : ((level == WARN) ? "WARN" : "INFO"),
            C_RESET, filename, line_no);
    vfprintf(stream, fmt, args);

    va_end(args);
}

void debug_print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
