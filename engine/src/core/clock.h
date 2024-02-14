#pragma once

#include "defines.h"

typedef struct clock {
    f64 start_time;
    f64 elapsed;
} clock;

// Actualiza el clock y debería llamarse justo antes de chequear el elapsed time
void clock_update(clock* clock);

// Inicializa el clock y resetea el elapsed time
void clock_start(clock* clock);

// Detiene el clock pero no resetea el elapsed time
void clock_stope(clock* clock);