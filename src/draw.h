#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>

#include "ur.h"

bool ncurses_init();
bool ncurses_end();

bool draw_hline(int y0, int x0, int width, int repeat, int start_char, int end_char, int mid_char, int divider_char);
bool draw_vline(int y0, int x0, int height, int mid_char);

bool draw_board(board_t* board);
bool draw_selection(board_t* board);
bool draw_pawns(board_t* board);

bool draw(board_t* board);
