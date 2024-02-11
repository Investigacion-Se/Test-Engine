#include "core/application.h"
#include "game_types.h"

#include "platform/platform.h"
#include "core/logger.h"

typedef struct application_state {
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 application_create(game* game_inst) {
    if (initialized) {
        KERROR("Application already initialized");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    initialize_logging();

    // TODO: eliminar
    KFATAL("Esto es una prueba de mensaje: %i", 123);
    KERROR("Esto es una prueba de mensaje: %i", 123);
    KWARN("Esto es una prueba de mensaje: %i", 123);
    KINFO("Esto es una prueba de mensaje: %i", 123);
    KDEBUG("Esto es una prueba de mensaje: %i", 123);
    KTRACE("Esto es una prueba de mensaje: %i", 123);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!platform_startup(
        &app_state.platform, 
        game_inst->app_config.name, 
        game_inst->app_config.start_pos_x, 
        game_inst->app_config.start_pos_y, 
        game_inst->app_config.start_width, 
        game_inst->app_config.start_height)) {

        KERROR("Failed to start platform");
        return FALSE;
    }

    if (!app_state.game_inst->initialize(app_state.game_inst)) {
        KERROR("Failed to initialize game");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialized = TRUE;
    return TRUE;
}

b8 application_run() {
    while (app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0)) {
                KFATAL("Failed to update game");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (f32)0)) {
                KFATAL("Failed to render game");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;
    
    platform_shutdown(&app_state.platform);
    return TRUE;
}