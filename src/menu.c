#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>

#include "input.h"
#include "menu.h"

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

#define TITLE_X 1u
#define TITLE_Y 1u

#define CURSOR_X 1u
#define MENU_X 4u
#define MENU_Y 4u
#define MENU_Y_ENTRY_SPACING 2u  // Reduce to 1u for no line gaps in menu entries


void draw_menu(void);

uint8_t cursor_num = 0u;
uint8_t cursor_num_last = 0u;


const char * menu_entries[] = {
  // 16 characters long max
  // 01234567890123456
    "Load game 1",
    "Load game 2",
// Uncommenting additional entries below will enable them and increase menu range

//    "Load game 3",
//    "Load game 4",
//    "Load game 5",
};

#define CURSOR_MIN 0
#define CURSOR_MAX (ARRAY_LEN(menu_entries) - 1u)

#define LOAD_GAME_MSG_X 1u
#define LOAD_GAME_MSG_Y (MENU_Y + (MENU_Y_ENTRY_SPACING * (ARRAY_LEN(menu_entries) + 1)))




void menu_show_loading_message(uint8_t selected_game) {
    gotoxy(LOAD_GAME_MSG_X, LOAD_GAME_MSG_Y);
    printf("Loading game %hu...\n", selected_game);
}



void menu_draw_cursor(void) {

    // Erase the last cursor location
    gotoxy(CURSOR_X, MENU_Y + (MENU_Y_ENTRY_SPACING * cursor_num_last));
    printf("  ");

    // Draw CURSOR_X last cursor location
    gotoxy(CURSOR_X, MENU_Y + (MENU_Y_ENTRY_SPACING * cursor_num));
    printf("->");

    // Save last cursor position
    cursor_num_last = cursor_num;
}


void menu_draw(void) {

     gotoxy(TITLE_X, TITLE_Y);
     printf("MegaDuck FlashCart");

    // Draw all menu entries
    uint8_t draw_y = MENU_Y;
    for (uint8_t c = 0; c < ARRAY_LEN(menu_entries); c++) {
        gotoxy(MENU_X, draw_y);
        printf("%s", menu_entries[c]);
        draw_y += MENU_Y_ENTRY_SPACING;
    }

    menu_draw_cursor();
}



// Returns selected game number (1 based, not 0 based)
uint8_t menu_run(void) {

    while(1) {
        UPDATE_KEYS();

        if (KEY_TICKED(J_UP)) {
            if (cursor_num > CURSOR_MIN) cursor_num--;
            else cursor_num = CURSOR_MAX;

            menu_draw_cursor();
        }
        else if (KEY_TICKED(J_DOWN)) {
            if (cursor_num < CURSOR_MAX) cursor_num++;
            else cursor_num = CURSOR_MIN;

            menu_draw_cursor();
        }
        else if (KEY_TICKED(J_START | J_A | J_B)) {
            return (cursor_num + 1);
        }

        vsync();
    }
}


