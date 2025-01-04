#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

#define MSG_BUFFER  1024

/* ANSI color codes */
#define RESET_COLOR   "\033[0m"
#define DEBUG_COLOR   "\033[36m"
#define INFO_COLOR    "\033[32m"
#define WARN_COLOR    "\033[33m"
#define ERROR_COLOR   "\033[31m"
#define FATAL_COLOR   "\033[41m\033[30m"

/* Define the log levels used in the logger */
typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
} LogLevel;

/* Function prototypes for logging at different levels */
void infof(int line, const char *format, ...);
void warnf(int line, const char *format, ...);
void errorf(int line, const char *format, ...);
void debugf(int line, const char *format, ...);
void fatalf(int line, const char *format, ...);
void set_log_level(LogLevel new_level);

#endif
