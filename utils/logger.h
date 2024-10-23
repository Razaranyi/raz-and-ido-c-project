#ifndef LOGGER_H
#define LOGGER_H

/* Define the log levels used in the logger */
typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
} LogLevel;

/* Function prototypes for logging at different levels */
void debug(char* message, int line);
void warn(char* message, int line);
void error(char* message, int line);
void fatal(char* message, int line);

#endif
