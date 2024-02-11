#include "kerror.h"
#include "core/logger.h"

void know_kerror(kerror error) {
    switch (error)
    {
        case GAME_NOT_INITIALIZED:
            KERROR("Game not initialized");
            break;
        case GAME_WITHOUT_FUNCTIONS:
            KERROR("Game without functions");
            break;
        case APPLICATION_FAILED_CREATION:
            KINFO("Application failed creation");
            break;
        case APPLICATION_SHUTDOWN_UNGRACEFULLY:
            KINFO("Application shutdown ungracefully");
            break;
        default: KERROR("Unknown error");
    }
}