#pragma once

/* Log level Enum */
typedef  enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
} LogLevel;


/*print log in level debug*/
void debug(char* message, int line);

/*print log in level info*/
void info(char* message, int line);

/*print log in level warning*/
void warn(char* message, int line);

/*print log in level error*/
void error(char* message, int line);

/*print log in level critical*/
void critical(char* message, int line);

