#include "line_indexing.h"


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
void set_log_level(LogLevel new_level) {
    log_level = new_level;
}