#include "core/application.h"
#include "game_types.h"

#include "platform/platform.h"
#include "core/kmemory.h"
#include "core/logger.h"
#include "core/event.h"
#include "core/input.h"

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

// Event handlers
b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);

b8 application_create(game* game_inst) {
    if (initialized) {
        KERROR("Application already initialized");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    initialize_logging();
    input_initialize();

    // TODO: eliminar
    KFATAL("Esto es una prueba de mensaje: %i", 123);
    KERROR("Esto es una prueba de mensaje: %i", 123);
    KWARN("Esto es una prueba de mensaje: %i", 123);
    KINFO("Esto es una prueba de mensaje: %i", 123);
    KDEBUG("Esto es una prueba de mensaje: %i", 123);
    KTRACE("Esto es una prueba de mensaje: %i", 123);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!event_initialize()) {
        KERROR("Failed to initialize event system");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_BUTTON_RELEASED, 0, application_on_key);


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
    char* memory_stats = get_memory_usage_str();
    KINFO(memory_stats);
    platform_free(memory_stats, FALSE);

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

            // Se actualiza el input al final del frame
            input_update((f64)0);
        }
    }

    app_state.is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_BUTTON_RELEASED, 0, application_on_key);
    
    event_shutdown();
    input_shutdown();

    platform_shutdown(&app_state.platform);
    return TRUE;
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT received, shutting down");
            app_state.is_running = FALSE;
            return TRUE;
        } break;
    }

    return FALSE;
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_KEY_PRESSED: {
            u16 key_code = context.data.u16[0];
            if (key_code == KEY_ESCAPE) {
                event_context data = {};
                event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

                return TRUE;
            } else if (key_code == KEY_A) {
                KDEBUG("Explicit - A key pressed!");
            } else {
                KDEBUG("'%c' key pressed in window", key_code);
            }
        } break;
        case EVENT_CODE_BUTTON_RELEASED: {
            u16 key_code = context.data.u16[0];
            if (key_code == KEY_B) {
                KDEBUG("Explicit - B key released!");
            } else {
                KDEBUG("'%c' key released in window", key_code);
            }
        } break;
    }

    return FALSE;
}