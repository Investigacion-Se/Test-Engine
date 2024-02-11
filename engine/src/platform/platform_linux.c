#include "platform/platform.h"

// Esta es la capa de linux, por lo que solo debería tener esta implementación
// si se esta usando linux
#if KPLATFORM_LINUX

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h> // nanosleep
#else
#include <unistd.h> // usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "core/logger.h"

typedef struct internal_state {
    Display *display;
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_window;
} internal_state;

b8 platform_startup(
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state* state = (internal_state*) plat_state->internal_state;

    // Conectar a X
    state->display = XOpenDisplay(NULL);

    // Apagar key repeats
    XAutoRepeatOff(state->display);

    // Conseguir la conección a la pantalla
    state->connection = XGetXCBConnection(state->display);

    if (xcb_connection_has_error(state->connection)) {
        KFATAL("Failed to connect to X server via XCB");
        return FALSE;
    }

    // Consseguir info del servidor de X
    const struct xcb_setup_t *setup = xcb_get_setup(state->connection);

    // Iterar sobre las pantallas
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    i32 screen_p = 0;
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&iter);
    }

    state->screen = iter.data;

    // crear XID para la ventana creada
    state->window = xcb_generate_id(state->connection);

    // registrar los evento a escuchar
    // XCB_CW_BACK_PIXEL = filling then window bg with a single colour
    // XCB_CW_EVENT_MASK es necesario
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Escucha por el teclado y los botones del mouse
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    u32 value_list[] = {
        state->screen->black_pixel,
        event_values,
    };

    // Crear la ventana
    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT,
        state->window,
        state->screen->root,
        x,
        y,
        width,
        height,
        0,  // sin bordes
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        state->screen->root_visual,
        event_mask,
        value_list);
    
    // Cambiar el titulo de la ventana
    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8, // la información tiene que verse a 8 bits en cada momento
        strlen(application_name),
        application_name);

    // Tell the server to notify when the window manager
    // attempts to destroy the window.
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        state->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        state->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
        state->connection,
        wm_delete_cookie,
        NULL);
    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
        state->connection,
        wm_protocols_cookie,
        NULL);
    state->wm_delete_window = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        wm_protocols_reply->atom,
        4,
        32,
        1,
        &wm_delete_reply->atom);

    // Map the window to the screen
    xcb_map_window(state->connection, state->window);

    // Flush the stream
    i32 stream_result = xcb_flush(state->connection);
    if (stream_result <= 0) {
        KFATAL("An error occurred when flusing the stream: %d", stream_result);
        return FALSE;
    }

    return TRUE;
}

void platform_shutdown(platform_state* plat_state) {
   // Simply cold-cast to the known type.
    internal_state* state = (internal_state*)plat_state->internal_state;

    // Turn key repeats back on since this is global for the OS... just... wow.
    XAutoRepeatOn(state->display);

    xcb_destroy_window(state->connection, state->window); 

    free(state);
}

b8 platform_pump_messages(platform_state* plat_state) {
    // Simply cold-cast to the known type.
    internal_state* state = (internal_state*)plat_state->internal_state;

    xcb_generic_event_t* event;
    xcb_client_message_event_t* cm;

    b8 quit_flagged = FALSE;

    // Poll for events until null is returned.
    while ((event = xcb_poll_for_event(state->connection))) {
        if (event == 0) {
            break;
        }
        // Input events
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                // TODO: Key presses and releases
            } break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                // TODO: Mouse button presses and releases
            }
            case XCB_MOTION_NOTIFY:
                // TODO: mouse movement
                break;

            case XCB_CONFIGURE_NOTIFY: {
                // TODO: Resizing
            }

            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t*)event;

                // Window close
                if (cm->data.data32[0] == state->wm_delete_window) {
                    quit_flagged = TRUE;
                }
            } break;
            default:
                // Something else
                break;
        }

        free(event);
    }

    return !quit_flagged;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}

void platform_free(void* block, b8 aligned) {
    free(block);
}

void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 colour) {
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    fprintf(stdout, "\033[%sm%s\033[0m", colour_strings[colour], message);
}

void platform_console_write_error(const char* message, u8 colour) {
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    fprintf(stderr, "\033[%sm%s\033[0m", colour_strings[colour], message);
}

f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

#endif