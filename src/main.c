//
// Launcher menu for Mega Duck zwenergy flash cart
//


#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>

#include "menu.h"


static void cart_launch_game(uint8_t selected_game);
void init_gfx(void);


// Writes the specified game slot to the cart register
// which should trigger a reboot with that game enabled
//
// Expected values: 1 for game slot 1, 2 for game slot 2
//
static volatile uint8_t AT(0x0002) CART_GAMELOAD_REG;
//
static void cart_launch_game(uint8_t selected_game) {

    menu_show_loading_message(selected_game);

    CART_GAMELOAD_REG = selected_game;
}



void init_gfx(void) {
    DISPLAY_OFF;

    // Clear the screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0);

    menu_draw();

    vsync();
    SHOW_BKG;
    DISPLAY_ON;
}


void main(void) {

    init_gfx();

    while(1) {
        vsync();
        uint8_t selected_game = menu_run();
        cart_launch_game(selected_game);
    }
}

