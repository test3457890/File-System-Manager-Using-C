#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

int init_logger(const char *filename);
void close_logger(void);
void set_log_level(LogLevel level);
void _log_message_internal(LogLevel level, const char *file, int line, const char *func, const char *format, ...);

#define LOG_DEBUG(...) _log_message_internal(LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(...)  _log_message_internal(LOG_INFO,  __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN(...)  _log_message_internal(LOG_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(...) _log_message_internal(LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR_SYS(msg) \
    _log_message_internal(LOG_ERROR, __FILE__, __LINE__, __func__, "%s: %s", msg, strerror(errno))
#define LOG_PATH_OP(level, op, path, result) \
    _log_message_internal(level, __FILE__, __LINE__, __func__, "%s '%s' - Result: %s", op, path, result)

#endif
