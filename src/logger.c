#include "stdarg.h"
#include "stdio.h"
#include "time.h"

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2

#define LOG_LEVEL 0

void log_stdout(const char *level, const char *format, va_list args)
{
    time_t curr_time = time(NULL);
    struct tm *t = localtime(&curr_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", t);

    printf("%s - ", time_str);
    printf("[%s]: ", level);

    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void log_debug(const char *format, ...)
{
    if (LOG_LEVEL <= LOG_DEBUG)
    {
        va_list args;
        va_start(args, format);
        log_stdout("DEBUG", format, args);
        va_end(args);
    }
}
