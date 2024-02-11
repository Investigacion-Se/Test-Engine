#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "game_types.h"
#include "kerror.h"

// Definición externa de una función que crea un juego
extern b8 create_game(game* out_game);

// Nuevo entry point
int main(void) {
    kerror error = KERROR_NONE;

    game game_inst;
    if (!create_game(&game_inst)) {
        error = GAME_NOT_INITIALIZED;
        goto error_exit;
    }

    if (!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
        error = GAME_WITHOUT_FUNCTIONS;
        goto error_exit;
    }

    // Inicialización
    if (!application_create(&game_inst)) {
        error = APPLICATION_FAILED_CREATION;
        goto error_exit;
    }

    // Game loop
    if (!application_run()) {
        error = APPLICATION_SHUTDOWN_UNGRACEFULLY;
        goto error_exit;
    }

    return 0;

    error_exit:
        know_kerror(error);
        return -error;
}