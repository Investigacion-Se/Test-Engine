#include "logger.h"
#include "asserts.h"

#include "platform/platform.h"

// TODO: temporario
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initialize_logging() {
    // TODO: crear log file
    return TRUE;
}

void shutdown_logging() {
    // TODO: poder limpiar el log file y escribir los entradas en la fila
}

void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: ",
    };
    b8 es_error = level < LOG_LEVEL_WARN;
    
    // Tecnicamente poniendo un limite de 32k caracteres por mensaje
    // no deberíamos estar haciendo esto
    const i32 msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    // Crear el formato del mensaje
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[msg_length];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    if (es_error) {
        platform_console_write_error(out_message2, level);
    } else {
        platform_console_write(out_message2, level);
    }
}

void report_assertion_failure(const char* expresion, const char* message, const char* file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion failed: %s, message: '%s', in file: %s, at line: %i", expresion, message, file, line);
}