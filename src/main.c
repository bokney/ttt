
#ifdef TEST
#include "mock_gb/mock_crash_handler.h"
#else
#include <gb/crash_handler.h>
#include <stdio.h>
#include <string.h>
#endif

#include "obj.h"
#include "state.h"
#include "states.h"


int main(void) {
    obj_pool_init();
    state currentState;
    assign_splash(&currentState);

    for (;;) {
        switch (run(&currentState)) {
            case 0:
                break;
            case 1:
                assign_splash(&currentState);
                break;
            case 2:
                assign_main_menu(&currentState);
                break;
            case 3:
                assign_game(&currentState);
                break;
            case 4:
                assign_result(&currentState);
                break;
            default:
                __HandleCrash();
                break;
        }
        vsync();
    }
}
