
#include <stdio.h>
#include "../gfx.h"
#include "main_menu.h"

typedef struct _main_menuData {
    uint8_t count;
} main_menuData;

main_menuData main_menu_data;

void main_menu_init(void) {
    HIDE_BKG;
    move_bkg(0, 0);
    clear_bkg();
    SHOW_BKG;
    main_menu_data.count = 0;
    printf("main_menu_init\n");
}

uint8_t main_menu_step(void) {
    printf("main_menu_step\n");
    main_menu_data.count += 1;
    if (main_menu_data.count > 4) return 1;
    return 0;
}

uint8_t main_menu_exit(void) {
    printf("main_menu_exit\n");
    return 3;
}
