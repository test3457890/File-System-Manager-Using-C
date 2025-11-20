#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h> 
#include <errno.h>
#include <pthread.h>
#include "logger.h"

// --- Configuration and Data Structures ---

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Internal log variables
static FILE *log_file = NULL;
static char log_filename[256] = ""; 
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static LogLevel min_log_level = LOG_INFO;
static int log_to_file = 1;
static int log_to_console = 1;
static int logger_initialized = 0;

// ANSI colors for console output
#define COLOR_RESET  "\x1b[0m"
#define COLOR_RED    "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_CYAN   "\x1b[36m"

// --- Logger Control Functions ---

void set_log_level(LogLevel level) {
    pthread_mutex_lock(&log_mutex);
    min_log_level = level;
    pthread_mutex_unlock(&log_mutex);
}

// Initialize logger
int init_logger(const char *filename) {
    pthread_mutex_lock(&log_mutex);

    if (logger_initialized) {
        fprintf(stderr, "Warning: Logger already initialized.\n");
        pthread_mutex_unlock(&log_mutex);
        return 1;
    }

    if (filename) {
        strncpy(log_filename, filename, sizeof(log_filename) - 1);
        log_filename[sizeof(log_filename) - 1] = '\0';
    }

    if (log_filename[0] != '\0') {
        log_file = fopen(log_filename, "a");
        if (!log_file) {
            perror("Failed to open log file");
            pthread_mutex_unlock(&log_mutex);
            return 0;
        }

        time_t now = time(NULL);
        struct tm tm_info;
        if (localtime_r(&now, &tm_info)) {
            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_info);
            fprintf(log_file, "\n===== Log started: %s (%s) =====\n", timebuf, log_filename);
            fflush(log_file);
        }

        printf("Logger initialized. Writing to: %s\n", log_filename);
    } else {
        printf("Logger initialized. Console-only logging.\n");
        log_to_file = 0; 
    }

    logger_initialized = 1;
    pthread_mutex_unlock(&log_mutex);
    return 1;
}

// Core logging function
void _log_message_internal(LogLevel level, const char *file, int line, const char *func, const char *format, ...) {
    if (level < min_log_level || !logger_initialized) return;

    char timebuf[64];
    time_t now = time(NULL);
    struct tm tm_info;
    if (localtime_r(&now, &tm_info) == NULL) {
        strncpy(timebuf, "Time Error", sizeof(timebuf));
    } else {
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_info);
    }

    pthread_mutex_lock(&log_mutex);

    const char *level_str;
    const char *color;
    switch(level) {
        case LOG_DEBUG:   level_str = "DEBUG"; color = COLOR_CYAN; break;
        case LOG_INFO:    level_str = "INFO";  color = COLOR_GREEN; break;
        case LOG_WARNING: level_str = "WARN";  color = COLOR_YELLOW; break;
        case LOG_ERROR:   level_str = "ERROR"; color = COLOR_RED; break;
        default: level_str = "UNKNOWN"; color = COLOR_RESET; break;
    }

    char prefix_buf[512];
    snprintf(prefix_buf, sizeof(prefix_buf), "[%s] [%s] (%s:%d:%s)", timebuf, level_str, file, line, func);

    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (len < 0) {
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    char *message = malloc(len + 1);
    if (!message) {
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    va_start(args, format);
    vsnprintf(message, len + 1, format, args);
    va_end(args);

    if (log_to_file && log_file) {
        fprintf(log_file, "%s %s\n", prefix_buf, message);
        fflush(log_file);
    }

    if (log_to_console) {
        FILE *out = (level >= LOG_WARNING) ? stderr : stdout;
        fprintf(out, "%s%s %s%s\n", color, prefix_buf, message, COLOR_RESET);
    }

    free(message);
    pthread_mutex_unlock(&log_mutex);
}

// Close logger
void close_logger(void) {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        time_t now = time(NULL);
        struct tm tm_info;
        if (localtime_r(&now, &tm_info)) {
            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_info);
            fprintf(log_file, "===== Log closed: %s =====\n", timebuf);
        }
        fclose(log_file);
        log_file = NULL;
    }
    logger_initialized = 0;
    pthread_mutex_unlock(&log_mutex);
}

// --- Convenience Macros ---
#define LOG_DEBUG(...) _log_message_internal(LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(...)  _log_message_internal(LOG_INFO,  __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN(...)  _log_message_internal(LOG_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(...) _log_message_internal(LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_ERROR_SYS(msg) \
    _log_message_internal(LOG_ERROR, __FILE__, __LINE__, __func__, "%s: %s", msg, strerror(errno))

#define LOG_PATH_OP(level, op, path, result) \
    _log_message_internal(level, __FILE__, __LINE__, __func__, "%s '%s' - Result: %s", op, path, result)

// --- Test main ---
int main() {
     if (!init_logger("file_manager_log.txt")) {
        fprintf(stderr, "Failed to initialize logger.\n");
        return 1;
    }
    set_log_level(LOG_DEBUG);

    char *new_folder = "/home/user/NewFolder";
    LOG_INFO("Starting recursive directory scan...");
    LOG_PATH_OP(LOG_INFO, "CREATE", new_folder, "SUCCESS");

    LOG_WARN("Skipping inaccessible file in user directory.");

    errno = EACCES;
    LOG_ERROR_SYS("Failed to open configuration file at /etc/config/app.conf");

    char *target_file = "/root/important_data.dat";
    LOG_PATH_OP(LOG_ERROR, "READ", target_file, "FAILED (No Permissions)");

    close_logger();
    return 0;
}
