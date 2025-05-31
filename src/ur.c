#include "ur.h"

// Path of player's pawns. LUT for path index to board pos.
const vec2i_t pawn_path[N_PLAYERS][N_PATH] = {
	{
		{0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0},
		{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},
		{0, 7}, {0, 6}, {0, 5}
	}, {
		{2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0},
		{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},
		{2, 7}, {2, 6}, {2, 5}
	}
};

// Player path indices with stars.
const int star_indices[] = {
	4, 8, 14
};

void board_init(board_t* board) {
	memset(board, 0, sizeof(board_t));
	board->roll = get_roll();
	board->err_msg = "";
	board->players[0].pawn_char = '&';
	board->players[1].pawn_char = '^';
	board->players[0].board_color_pair = 2;
	board->players[1].board_color_pair = 3;
	board->players[0].pawn_color_pair = 2;
	board->players[1].pawn_color_pair = 3;
	board->winner = -1;
}

// Gets a dice roll. Returns the sum of four binary dice.
int get_roll() {
	switch(rand() % 16) {
		case 0:
			return 0;
		case 1: case 2: case 4: case 8:
			return 1;
		case 3: case 5: case 6: case 9: case 10: case 12:
			return 2;
		case 7: case 11: case 13: case 14:
			return 3;
		case 15:
			return 4;
	}
}

// Find number of player's pawns at pos.
// Optionally returns an index player's pawn at that posiiton.
int n_pawns(player_t* player, int player_num, vec2i_t pos, int* pawn_index) {
	int count = 0;
	if(pawn_index) *pawn_index = 0;
	for(int i = 0; i < N_PAWNS; ++i) {
		if (pawn_path[player_num][player->pawn_positions[i]].x == pos.x &&
			pawn_path[player_num][player->pawn_positions[i]].y == pos.y) {
			if(pawn_index) *pawn_index = i;
			count += 1;
		}
	}
	return count;
}

bool skip_turn(board_t* board) {
	board->current_player = (board->current_player + 1 ) % N_PLAYERS;
	board->roll = get_roll();
	return true;
}

bool attempt_turn(board_t* board) {
	int other_player_num = (board->current_player + 1) % N_PLAYERS;
	int pawn_index, new_index, other_pawn_index;
	bool new_index_on_star;
	int n_pawn = n_pawns(&(board->players[board->current_player]), board->current_player, board->players[board->current_player].selected_pos, &pawn_index);
	board-> err_msg = "";

	if(board->roll == 0) {
		board-> err_msg = "Womp Womp!";
		return skip_turn(board);
	}

	if(n_pawn == 0) {
		board-> err_msg = "You have no pawns at selected position!";
		return false;
	}

	if ((new_index = board->players[board->current_player].pawn_positions[pawn_index] + board->roll) >= N_PATH) {
		board->err_msg = "Selected pawn cannot move that far!";
		return false;
	}

	if (new_index != N_PATH-1 && n_pawns(&(board->players[board->current_player]), board->current_player, pawn_path[board->current_player][new_index], NULL)) {
		board->err_msg = "You cannot jump your own pawn!";
		return false;
	}

	new_index_on_star = false;
	for(int i = 0; i < N_STAR_PATH; i++) {
		if(star_indices[i] == new_index) new_index_on_star = true;
	}

	int n_other = n_pawns(&(board->players[other_player_num]), other_player_num, pawn_path[board->current_player][new_index], &other_pawn_index);

	if(new_index_on_star && n_other) {
		board->err_msg = "Other player is on a safe place!";
		return false;
	}

	// Able to move. Move and check for jumping other player and stars.
	board->players[board->current_player].pawn_positions[pawn_index] = new_index;

	if(n_other) {
		board->err_msg = "You jumped other player's pawn!";
		board->players[other_player_num].pawn_positions[other_pawn_index] = 0;
	}

	if (!new_index_on_star)
		board->current_player = (board->current_player + 1 ) % N_PLAYERS;
	board->roll = get_roll();
	return true;
}

bool update(board_t* board) {
	for(int i = 0; board->winner == -1 && i < N_PLAYERS; i++) {
		if (n_pawns(&(board->players[i]), i, pawn_path[i][N_PATH-1], NULL) == 6) {
			board->winner = i;
			return true;
		}
	}

	if((board->c = getch()) == 'q') return false;

	if(board->winner != -1) {
		return true;
	}

	int selection_y = board->players[board->current_player].selected_pos.y;
	int selection_x = board->players[board->current_player].selected_pos.x;

	switch(board->c) {
	/*case '0' ... '5':
		board->players[board->current_player].board_color_pair=c-'0'; break; */
	case KEY_UP:
		selection_y = (selection_y - 1 < 0) ? selection_y : selection_y - 1; break;
	case KEY_DOWN:
		selection_y = (selection_y + 1 >= 3) ? selection_y : selection_y + 1; break;
	case KEY_LEFT:
		selection_x = (selection_x - 1 < 0) ? selection_x : selection_x - 1; break;
	case KEY_RIGHT:
		selection_x = (selection_x + 1 >= 8) ? selection_x : selection_x + 1; break;
	case KEY_ENTER: case '\n': attempt_turn(board); break;
	case '.': skip_turn(board); break;
	default: break;
	}
	board->players[board->current_player].selected_pos.y = selection_y;
	board->players[board->current_player].selected_pos.x = selection_x;
	return true;
}
