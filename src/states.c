
#ifdef TEST
#include "mock_gb/mock_gb.h"
#else
#include <gb/gb.h>
#endif

#include "state.h"
#include "states.h"
#include "states/splash.h"
#include "states/main_menu.h"
#include "states/game.h"
#include "states/result.h"

void assign_splash(state *target) {
    assign(
        target,
        &splash_init,
        &splash_step,
        &splash_exit
    );
}

void assign_main_menu(state *target) {
    assign(
        target,
        &main_menu_init,
        &main_menu_step,
        &main_menu_exit
    );
}

void assign_game(state *target) {
    assign(
        target,
        &game_init,
        &game_step,
        &game_exit
    );
}

void assign_result(state *target) {
    assign(
        target,
        &result_init,
        &result_step,
        &result_exit
    );
}
