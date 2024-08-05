//
// Launcher menu for Mega Duck zwenergy flash cart
//


#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>

#include "input.h"

//#include "GBDK_2020_logo.h"

static void cart_launch_game(uint8_t selected_game);
void init_gfx(void);
void draw_menu(void);

static volatile uint8_t AT(0x0002) CART_GAMELOAD_REG;

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

#define CURSOR_X 1u

#define MENU_X 4u
#define MENU_Y 3u
#define MENU_Y_ENTRY_SPACING 2u
#define MENU_Y_ENTRY_1 (MENU_Y)
#define MENU_Y_ENTRY_2 (MENU_Y + MENU_Y_ENTRY_SPACING)

const char * menu_entries[] = {
  // 16 characters long max
  // 01234567890123456
    "Load slot 1",
    "Load slot 2",
};

#define CURSOR_MIN 0
#define CURSOR_MAX (ARRAY_LEN(menu_entries) - 1u)

#define LOAD_GAME_MSG_X 1u
#define LOAD_GAME_MSG_Y (MENU_Y + (MENU_Y_ENTRY_SPACING * (ARRAY_LEN(menu_entries) + 1)))

const uint8_t cursor_draw_ypos[] = { MENU_Y_ENTRY_1, MENU_Y_ENTRY_2 };
uint8_t cursor_num = 0u;
uint8_t cursor_num_last = 0u;





// Writes the specified game slot to the cart register
// which should trigger a reboot with that game enabled
static void cart_launch_game(uint8_t selected_game) {

    gotoxy(LOAD_GAME_MSG_X, LOAD_GAME_MSG_Y);
    printf("Loading game %hu...\n", selected_game);

    CART_GAMELOAD_REG = selected_game;
}


void draw_menu(void) {

    gotoxy(MENU_X, MENU_Y_ENTRY_1);
    printf("%s", menu_entries[0]);

    gotoxy(MENU_X, MENU_Y_ENTRY_2);
    printf("%s", menu_entries[1]);
}


void draw_cursor(void) {

    // Erase the last cursor location
    gotoxy(CURSOR_X, cursor_draw_ypos[cursor_num_last]);
    printf("  ");

    // Draw CURSOR_X last cursor location
    gotoxy(CURSOR_X, cursor_draw_ypos[cursor_num]);
    printf("->");

    // Save last cursor position
    cursor_num_last = cursor_num;
}


void init_gfx(void) {
    DISPLAY_OFF;

    // Clear the screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0);

    draw_menu();
    draw_cursor();

    vsync();
    SHOW_BKG;
    DISPLAY_ON;
}


void handle_menu(void) {

    UPDATE_KEYS();

    if (KEY_TICKED(J_UP)) {
        if (cursor_num > CURSOR_MIN) cursor_num--;
        else cursor_num = CURSOR_MAX;
        draw_cursor();
    }
    else if (KEY_TICKED(J_DOWN)) {
        if (cursor_num < CURSOR_MAX) cursor_num++;
        else cursor_num = CURSOR_MIN;
        draw_cursor();
    }
    else if (KEY_TICKED(J_START | J_A | J_B)) {
        cart_launch_game(cursor_num + 1);
    }
}


void main(void) {

    init_gfx();

    while(1) {
        vsync();
        handle_menu();
    }
}

