#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(void) {
    KFATAL("Esto es una prueba de mensaje: %i", 123);
    KERROR("Esto es una prueba de mensaje: %i", 123);
    KWARN("Esto es una prueba de mensaje: %i", 123);
    KINFO("Esto es una prueba de mensaje: %i", 123);
    KDEBUG("Esto es una prueba de mensaje: %i", 123);
    KTRACE("Esto es una prueba de mensaje: %i", 123);

    platform_state state;
    if (platform_startup(&state, "Kohi Engine Testbed", 100, 100, 1280, 720)) {
        while (TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);

    return 0;
}