#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>

#define N_PAWNS		6
#define N_PATH		16
#define N_PLAYERS	2
#define STR_SIZE	255
#define N_STAR_PATH	3

typedef struct {
	int y, x;
} vec2i_t;

typedef struct {
	// drawing:
	char name[STR_SIZE];
	int pawn_char;
	int board_color_pair;
	int pawn_color_pair;
	vec2i_t selected_pos;
	// logic:
	int pawn_positions[N_PAWNS];
} player_t;

typedef struct {
	// drawing:
	int width, height;
	char* err_msg;
	// logic:
	int c, roll, current_player, winner;
	player_t players[N_PLAYERS];
} board_t;

extern const int star_indices[N_STAR_PATH];
extern const vec2i_t pawn_path[N_PLAYERS][N_PATH];

void board_init(board_t* board);

int get_roll();

int n_pawns(player_t* player, int player_num, vec2i_t pos, int* pawn_index);

bool skip_turn(board_t* board);
bool attempt_turn(board_t* board);

bool update(board_t* board);
