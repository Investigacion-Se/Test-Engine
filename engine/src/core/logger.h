#pragma once

#include "defines.h"

#define LOG_WARN_ENABLE 1
#define LOG_INFO_ENABLE 1
#define LOG_DEBUG_ENABLE 1
#define LOG_TRACE_ENABLE 1

// Desabilita debug y trace cuando se compila en release
#if KRELEASE == 1 
#define LOG_DEBUG_ENABLE 0
#define LOG_TRACE_ENABLE 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

b8 initialize_logging();
void shutdown_logging();

KAPI void log_output(log_level level, const char* message, ...);

// Logs un mensaje de nivel fatal
#define KFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef KERROR
// Logs un mensaje de nivel error
#define KERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLE == 1
// Logs un mensaje de nivel warning
#define KWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
// Se eliminan los comandos de logs
#define KWARN(message, ...) 
#endif

#if LOG_INFO_ENABLE == 1
// Logs un mensaje de nivel info
#define KINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
// Se eliminan los comandos de logs
#define KINFO(message, ...) 
#endif

#if LOG_DEBUG_ENABLE == 1
// Logs un mensaje de nivel debug
#define KDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
// Se eliminan los comandos de logs
#define KDEBUG(message, ...) 
#endif

#if LOG_TRACE_ENABLE == 1
// Logs un mensaje de nivel trace
#define KTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
// Se eliminan los comandos de logs
#define KTRACE(message, ...) 
#endif