#pragma once

#include "defines.h"

typedef enum kerror {
    KERROR_NONE = 0,
    GAME_NOT_INITIALIZED,
    GAME_WITHOUT_FUNCTIONS,
    APPLICATION_FAILED_CREATION,
    APPLICATION_SHUTDOWN_UNGRACEFULLY,
    ERROR_COUNT
} kerror;

KAPI void know_kerror(kerror error);