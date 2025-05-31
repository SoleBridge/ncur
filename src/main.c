#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>

#include "ur.h"
#include "draw.h"

int main(void) {
	board_t board;
	if(!ncurses_init()) return 1;
	board_init(&board);

	draw(&board);
	while (update(&board)) {
		draw(&board);
	}

	ncurses_end();
	return 0;
}
