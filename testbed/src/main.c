#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    KFATAL("Esto es una prueba de mensaje: %i", 123);
    KERROR("Esto es una prueba de mensaje: %i", 123);
    KWARN("Esto es una prueba de mensaje: %i", 123);
    KINFO("Esto es una prueba de mensaje: %i", 123);
    KDEBUG("Esto es una prueba de mensaje: %i", 123);
    KTRACE("Esto es una prueba de mensaje: %i", 123);

    KASSERT(1 == 0);

    return 0;
}