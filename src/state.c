
#ifdef TEST
#include "mock_gb/mock_crash_handler.h"
#else
#include <gb/crash_handler.h>
#endif

#include "state.h"

void assign(
    state *target,
    void (*init)(void),
    uint8_t (*step)(void),
    uint8_t (*exit)(void)
) {
    target->state = 0;
    target->init = init;
    target->step = step;
    target->exit = exit;
}

uint8_t run(state *target) {
    switch (target->state) {
        case 0:
            if (target->init)
                target->init();
            target->state = 1;
        case 1:
            if (target->step)
                if (!target->step())
                    return 0;
        case 2:
            target->state = 0;
            if (target->exit)
                return target->exit();
        default:
            __HandleCrash();
    }
    return 0;
}
