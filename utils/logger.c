#include <stdio.h>
#include <stdarg.h>
#include "logger.h"
#include "line_indexing.h"
#define MSG_BUFFER  1024

/* ANSI color codes */
#define RESET_COLOR   "\033[0m"
#define DEBUG_COLOR   "\033[36m"
#define INFO_COLOR    "\033[32m"
#define WARN_COLOR    "\033[33m"
#define ERROR_COLOR   "\033[31m"
#define FATAL_COLOR   "\033[41m\033[30m"



static LogLevel log_level;

void print_log_by_level(LogLevel level, const char* prefix, const char *message, int line) {
    char log_res[MSG_BUFFER];
    char* color;

    /* Check if the log level is high enough to log the message */
    if (level < log_level) {
        return;
    }


    switch (level) {
        case DEBUG: color = DEBUG_COLOR; break;
        case INFO: color = INFO_COLOR; break;
        case WARN: color = WARN_COLOR; break;
        case ERROR: color = ERROR_COLOR; break;
        case FATAL: color = FATAL_COLOR; break;
        default: color = RESET_COLOR; break;
    }

    /* Format the log message and store it in log_res */
    if(line > 0){
        sprintf(log_res, "%s%s%s (line: %d)%s\n", color, prefix, message, line, RESET_COLOR);
    } else{
        sprintf(log_res, "%s%s%s %s\n", color, prefix, message, RESET_COLOR);
    }

    /* Print the log message to stdout */
    printf("%s", log_res);
}

void debug(char* message, int line) {
    print_log_by_level(DEBUG, "[DEBUG] ", message, line);
}


void debugf(int line, const char *format, ...) {
    char message[MSG_BUFFER];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    debug(message, line);
}

void info(char* message, int line){
    print_log_by_level(INFO, "[INFO] ", message, line);
}

void infof(int line, const char *format, ...) {
    char message[MSG_BUFFER];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    info(message, line);
}

void warn(char* message, int line) {
    print_log_by_level(WARN, "[WARN] ", message, line);
}

void error(char* message, int line) {
    print_log_by_level(ERROR, "[ERROR] ", message, line);
}
void errorf(int line, const char *format, ...) {
    char message[MSG_BUFFER];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    error(message, line);
}

void warnf(int line, const char *format, ...) {
    char message[MSG_BUFFER];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    warn(message, line);
}

void fatal(char* message, int line) {
    print_log_by_level(FATAL, "[FATAL] ", message, line);
}

void fatalf(int line, const char *format, ...) {
    char message[MSG_BUFFER];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    fatal(message, line);
}
