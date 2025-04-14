
#include <stdio.h>
#include <gb/crash_handler.h>
#include "../obj.h"
#include "../gfx.h"
#include "game.h"

#define CELL_TILE_WIDTH 2
#define CELL_TILE_HEIGHT 2
#define INPUT_DELAY 8

extern uint8_t ttt_game_tiles[];
extern uint8_t ttt_game_sprites[];

typedef struct _boardData {
    uint8_t board[3][3];
    uint8_t turn;
    uint8_t winner;
} boardData;

typedef struct _playerData {
    uint8_t cursor_x, cursor_y;
} playerData;

boardData board_data;
playerData player_1_data, player_2_data;

obj *board;
obj *player_1, *player_2;

uint8_t counter;

void plot_board(void) {
    HIDE_BKG;
    clear_bkg();
    for (uint8_t ix = 6; ix < 14; ix++) {
        set_bkg_tile_xy(ix, 7, 10);
        set_bkg_tile_xy(ix, 10, 10);
    }
    set_bkg_tile_xy(5, 7, 9);
    set_bkg_tile_xy(5, 10, 9);
    set_bkg_tile_xy(14, 7, 11);
    set_bkg_tile_xy(14, 10, 11);
    for (uint8_t iy = 5; iy < 13; iy++) {
        set_bkg_tile_xy(8, iy, 13);
        set_bkg_tile_xy(11, iy, 13);
    }
    set_bkg_tile_xy(8, 4, 12);
    set_bkg_tile_xy(11, 4, 12);
    set_bkg_tile_xy(8, 13, 14);
    set_bkg_tile_xy(11, 13, 14);
    
    set_bkg_tile_xy(8, 7, 15);
    set_bkg_tile_xy(11, 7, 15);
    set_bkg_tile_xy(8, 10, 15);
    set_bkg_tile_xy(11, 10, 15);
    SHOW_BKG;
}

void plot_piece(uint8_t x, uint8_t y, uint8_t player) {
    HIDE_BKG;
    uint8_t base_x = 6 + x * 3;
    uint8_t base_y = 5 + y * 3;

    uint8_t tile_index;
    if (player == 1) {
        tile_index = 1;
    } else if (player == 2) {
        tile_index = 5;
    } else {
        tile_index = 0;
    }

    for (uint8_t row = 0; row < CELL_TILE_HEIGHT; row++) {
        for (uint8_t col = 0; col < CELL_TILE_WIDTH; col++) {
            set_bkg_tile_xy(base_x + col, base_y + row, tile_index++);
        }
    }
    SHOW_BKG;
}

void board_init(void *data) {
    boardData *bd = (boardData *)data;
    for (uint8_t ix = 0; ix < 3; ix++) {
        for (uint8_t iy = 0; iy < 3; iy++) {
            bd->board[ix][iy] = 0;
        }
    }
    bd->turn = 0;
    bd->winner = 0;

    plot_board();
}

void board_draw(void *data) {
    boardData *bd = (boardData *)data;
}

void player_init(void *data) {
    playerData *pd = (playerData *)data;
    pd->cursor_x = 1;
    pd->cursor_y = 1;
}

uint8_t player_1_step(void *data) {
    playerData *pd = (playerData *)data;
    static uint8_t input_timer = 0;
    uint8_t input = joypad();

    if (input_timer > 0) {
        input_timer--;
        return 0;
    }

    if (joypad() & J_LEFT && pd->cursor_x > 0) {
        pd->cursor_x--;
        input_timer = INPUT_DELAY;
    } else if (joypad() & J_RIGHT && pd->cursor_x < 2) {
        pd->cursor_x++;
        input_timer = INPUT_DELAY;
    } else if (joypad() & J_UP && pd->cursor_y > 0) {
        pd->cursor_y--;
        input_timer = INPUT_DELAY;
    } else if (joypad() & J_DOWN && pd->cursor_y < 2) {
        pd->cursor_y++;
        input_timer = INPUT_DELAY;
    }

    if (input & J_A) {
        if (board_data.board[pd->cursor_x][pd->cursor_y] == 0) {
            board_data.board[pd->cursor_x][pd->cursor_y] = 1;
            plot_piece(pd->cursor_x, pd->cursor_y, 1);
            board_data.turn++;
        }
    }

    return 0;
}

void player_draw(void *data) {
    playerData *pd = (playerData *)data;
    switch ((counter >> 4) & 1) {
        case 0:
            HIDE_SPRITES;
            move_sprite(0, 52 + 24 * pd->cursor_x, 52 + 24 * pd->cursor_y);
            move_sprite(1, 52 + 24 * pd->cursor_x + 16, 52 + 24 * pd->cursor_y);
            move_sprite(2, 52 + 24 * pd->cursor_x + 16, 52 + 24 * pd->cursor_y + 16);
            move_sprite(3, 52 + 24 * pd->cursor_x, 52 + 24 * pd->cursor_y + 16);
            SHOW_SPRITES;
            break;
        case 1:
            HIDE_SPRITES;
            move_sprite(0, 53 + 24 * pd->cursor_x, 53 + 24 * pd->cursor_y);
            move_sprite(1, 53 + 24 * pd->cursor_x + 14, 53 + 24 * pd->cursor_y);
            move_sprite(2, 53 + 24 * pd->cursor_x + 14, 53 + 24 * pd->cursor_y + 14);
            move_sprite(3, 53 + 24 * pd->cursor_x, 53 + 24 * pd->cursor_y + 14);
            SHOW_SPRITES;
            break;
        default:
            __HandleCrash();
    }
}

void game_init(void) {
    counter = 0;
    set_bkg_data(0, 16, ttt_game_tiles);
    set_sprite_data(0, 4, ttt_game_sprites);
    HIDE_SPRITES;
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 1);
    set_sprite_tile(2, 2);
    set_sprite_tile(3, 3);
    SHOW_SPRITES;
    board = obj_create(&board_init, NULL, NULL, NULL, NULL);
    player_1 = obj_create(&player_init, &player_1_step, NULL, &player_draw, &player_1_data);
    // player_2 = obj_create(NULL, NULL, NULL, NULL, NULL);
}

uint8_t game_step(void) {
    obj_run_cycle();
    counter++;
    return 0;
}

uint8_t game_exit(void) {
    obj_release_all();
    return 4;
}
