
#include <stdio.h>
#include "../obj.h"
#include "../gfx.h"
#include "splash.h"

extern uint8_t ttt_title_tiles[];
extern uint8_t title_tilemap[];

typedef struct logoData_ {
    uint8_t count;
    uint8_t x, y;
} logoData;

obj *logo;
logoData logo_data;

void splash_init(void) {
    HIDE_BKG;
    move_bkg(0, 0);
    clear_bkg();
    set_bkg_data(2, 18, ttt_title_tiles);
    set_bkg_tiles(5, 5, 9, 2, title_tilemap);
    SHOW_BKG;
    logo_data.count = 0;
    logo = obj_create(NULL, NULL, NULL, NULL, &logo_data);
}

uint8_t splash_step(void) {
    uint8_t input = joypad();
    if (input & (J_START | J_A)) {
        return 1;
    }
    scroll_bkg(0, 1);
    return 0;
}

uint8_t splash_exit(void) {
    obj_release_all();
    return 2;
}
