//
// Launcher menu for Mega Duck zwenergy flash cart
//


#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>

#include "input.h"


#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

#define CURSOR_X 1u
#define MENU_X 4u
#define MENU_Y 2u
#define MENU_Y_ENTRY_SPACING 2u


static void cart_launch_game(uint8_t selected_game);
void init_gfx(void);
void draw_menu(void);

uint8_t cursor_num = 0u;
uint8_t cursor_num_last = 0u;


const char * menu_entries[] = {
  // 16 characters long max
  // 01234567890123456
    "Load slot 1",
    "Load slot 2",
//    "Load slot 3",
//    "Load slot 4",
//    "Load slot 5",
};

#define CURSOR_MIN 0
#define CURSOR_MAX (ARRAY_LEN(menu_entries) - 1u)

#define LOAD_GAME_MSG_X 1u
#define LOAD_GAME_MSG_Y (MENU_Y + (MENU_Y_ENTRY_SPACING * (ARRAY_LEN(menu_entries) + 1)))



// Writes the specified game slot to the cart register
// which should trigger a reboot with that game enabled
//
// Expected values: 1 for game slot 1, 2 for game slot 2
//
static volatile uint8_t AT(0x0002) CART_GAMELOAD_REG;
//
static void cart_launch_game(uint8_t selected_game) {

    gotoxy(LOAD_GAME_MSG_X, LOAD_GAME_MSG_Y);
    printf("Loading game %hu...\n", selected_game);

    CART_GAMELOAD_REG = selected_game;
}



void draw_menu(void) {

    uint8_t draw_y = MENU_Y;
    for (uint8_t c = 0; c < ARRAY_LEN(menu_entries); c++) {
        gotoxy(MENU_X, draw_y);
        printf("%s", menu_entries[c]);
        draw_y += MENU_Y_ENTRY_SPACING;
    }
}


void draw_cursor(void) {

    // Erase the last cursor location
    gotoxy(CURSOR_X, MENU_Y + (MENU_Y_ENTRY_SPACING * cursor_num_last));
    printf("  ");

    // Draw CURSOR_X last cursor location
    gotoxy(CURSOR_X, MENU_Y + (MENU_Y_ENTRY_SPACING * cursor_num));
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

