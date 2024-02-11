#pragma once

#include "core/application.h"

typedef struct game {

    application_config app_config;

    // Inicializar
    b8 (*initialize)(struct game* game_inst);

    // Update loop
    b8 (*update)(struct game* game_inst, f32 delta_time);

    // Render loop
    b8 (*render)(struct game* game_inst, f32 delta_time);

    // Manejo de escalar la ventana
    void (*on_resize)(struct game* game_inst, u32 width, u32 height);

    void* state;
} game;