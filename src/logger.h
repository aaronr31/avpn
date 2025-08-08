#ifndef LOGGER
#define LOGGER

void log_stdout(const char *level, const char *format, ...);
void log_debug(const char *format, ...);
void log_error(const char *format, ...);

#endif