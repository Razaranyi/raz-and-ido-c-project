#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"


static LogLevel log_level;

void print_log_by_level(LogLevel level, const char* prefix, const char *message, int line) {
    char log_res[1024];

    /* Check if the log level is high enough to log the message */
    if (level < log_level) {
        return;
    }

    /* Format the log message and store it in log_res */
    sprintf(log_res, "%s%s (line: %d)\n", prefix, message, line);

    /* Print the log message to stdout */
    printf("%s", log_res);
}

void debug(char* message, int line) {
    print_log_by_level(DEBUG, "[DEBUG] ", message, line);
}

void warn(char* message, int line) {
    print_log_by_level(WARN, "[WARN] ", message, line);
}

void error(char* message, int line) {
    print_log_by_level(ERROR, "[ERROR] ", message, line);
}
void errorf(int line, const char *format, ...) {
    char message[1024];
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    error(message, line);
}

void fatal(char* message, int line) {
    print_log_by_level(FATAL, "[FATAL] ", message, line);
}
