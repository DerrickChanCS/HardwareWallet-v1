#include "screen.h"
#include "rand.h"

void unlock_screen_load(void *data) {
	uint8_t rand[9];
	rand_generate(rand,9);

	char row[6];
	char digits[] = {'1','2','3','4','5','6','7','8','9'};
	shuffle((uint8_t*)digits, rand, 9);

    oled_clear();
	oled_write(SCREEN_WIDTH/3, 0, "Unlock");
	for (uint8_t y=0; y<3; y++) {
		row[0] = digits[y*3 + 0];
		row[1] = ' ';
		row[2] = digits[y*3 + 1];
		row[3] = ' ';
		row[4] = digits[y*3 + 2];
		row[5] = '\0';
		oled_write(SCREEN_WIDTH/3, 1+y, row);
	}
    oled_redraw();
}

screen_t unlock_screen = {
    .on_load = unlock_screen_load,
    .on_unload = NULL,
    .on_evt = NULL
};


