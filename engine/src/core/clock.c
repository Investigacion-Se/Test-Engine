#include "core/clock.h"

#include "platform/platform.h"
#include "core/logger.h"

void clock_update(clock* clock) {
#if KRELEASE != 1
    if (!clock) {
        KERROR("Trying to update a NULL clock");
        return;
    }
#endif

    if (clock->start_time != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->start_time;
    }
}

void clock_start(clock* clock) {
#if KRELEASE != 1
    if (!clock) {
        KERROR("Trying to start a NULL clock");
        return;
    }
#endif

    clock->start_time = platform_get_absolute_time();
    clock->elapsed = 0;
}

void clock_stope(clock* clock) {
#if KRELEASE != 1
    if (!clock) {
        KERROR("Trying to stop a NULL clock");
        return;
    }
#endif

    clock->start_time = 0;
}