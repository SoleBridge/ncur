#include "draw.h"

const char CHAR_STAR = '*';

const int padding_x = 2, padding_y = 3;
const int min_x = 3, min_y = 1;
const int dividor_size = 1;

bool ncurses_init() {
	initscr();
	if(!has_colors() || !can_change_color()) {
		endwin();
    	return false;
    }
    start_color();

    cbreak();
    noecho();
	raw();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
	curs_set(FALSE);

    init_pair(1, COLOR_WHITE,   COLOR_BLACK);
	init_pair(2, COLOR_RED,     COLOR_BLACK);
    init_pair(3, COLOR_BLUE,    COLOR_BLACK);
    init_pair(4, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    srand(time(NULL));

    return true;
}

bool ncurses_end() {
	endwin();
	return true;
}

bool draw_hline(int y0, int x0, int width, int repeat, int start_char, int end_char, int mid_char, int divider_char) {
	if(start_char)
		mvaddch(y0,x0,start_char);
	else
		move(y0,x0+1);

	for(int j = 0; j < repeat; j++) {
		for(int i = 0; i < width; i++) {
			addch(mid_char);
		}
		if(j != repeat-1)
			addch(divider_char);
	}
	if(end_char)
		addch(end_char);
	return true;
}

bool draw_vline(int y0, int x0, int height, int mid_char) {
	for(int i = 0; i < height; i++) {
		mvaddch(y0+i,x0,mid_char);
	}
	return true;
}

bool draw_selection(board_t *board) {
	int y0, x0, y1, x1;
	y0 = padding_y + dividor_size + (board->players[board->current_player].selected_pos.y)*(board->height+1);
	x0 = padding_x + dividor_size + (board->players[board->current_player].selected_pos.x)*(board->width+1);
	y1 = y0+board->height-1;
	x1 = x0+board->width-1;
	mvaddch(y0,x0,ACS_ULCORNER);
	mvaddch(y0,x1,ACS_URCORNER);
	mvaddch(y1,x0,ACS_LLCORNER);
	mvaddch(y1,x1,ACS_LRCORNER);
	return true;
}

bool draw_board(board_t* board) {
/*
1. [x] find max width, height
2. [-] choose a drawing width, height
3. [x] calculate positions
4. [x] change color
5. [x] draw hlines
6. [x] draw vlines
7. [x] change color, draw stars
8. [ ] change color, draw players?
*/
	// int color_pair, player_t* p, int* pwidth, int* pheight;

	int width = (COLS - (2*padding_x + 9) ) / 8;
	int height = (LINES - (2*padding_y + 4) ) / 3;
	board->width = width;
	board->height = height;

	if(width < 5 || height < 1) // too small
		return false;

	const int y_pos[] = {
		padding_y, padding_y+(height+1), padding_y+2*(height+1), padding_y+3*(height+1)
	};

	const int x_pos[] = {
		padding_x, padding_x+6*(width+1), padding_x+4*(width+1)
	};

	const int division_cells[] = { 4, 2, 2 };

	const int star_pos_x[] = {
		padding_x+2, padding_x+2 + 3*(width+1), padding_x+2 + 6*(width+1)
	};

	const int star_pos_y[] = {
		padding_y+1+(height/2), padding_y+1+(height/2)+1*(height+1), padding_y+1+(height/2)+2*(height+1)
	};

	attron(COLOR_PAIR(board->players[board->current_player].board_color_pair));

	draw_hline(y_pos[0], x_pos[0], width, division_cells[0], ACS_ULCORNER, ACS_URCORNER, ACS_HLINE, ACS_TTEE);
	draw_hline(y_pos[0], x_pos[1], width, division_cells[2], ACS_ULCORNER, ACS_URCORNER, ACS_HLINE, ACS_TTEE);

	draw_hline(y_pos[1], x_pos[0], width, division_cells[0], ACS_LTEE, ACS_PLUS, ACS_HLINE, ACS_PLUS);
	draw_hline(y_pos[1], x_pos[1], width, division_cells[2], ACS_PLUS, ACS_RTEE, ACS_HLINE, ACS_PLUS);
	draw_hline(y_pos[1], x_pos[2], width, division_cells[1], 0, 0, ACS_HLINE, ACS_TTEE);

	draw_hline(y_pos[2], x_pos[0], width, division_cells[0], ACS_LTEE, ACS_PLUS, ACS_HLINE, ACS_PLUS);
	draw_hline(y_pos[2], x_pos[1], width, division_cells[2], ACS_PLUS, ACS_RTEE, ACS_HLINE, ACS_PLUS);
	draw_hline(y_pos[2], x_pos[2], width, division_cells[1], 0, 0, ACS_HLINE, ACS_BTEE);

	draw_hline(y_pos[3], x_pos[0], width, division_cells[0], ACS_LLCORNER, ACS_LRCORNER, ACS_HLINE, ACS_BTEE);
	draw_hline(y_pos[3], x_pos[1], width, 2, ACS_LLCORNER, ACS_LRCORNER, ACS_HLINE, ACS_BTEE);

	for(int y = 0; y <= 2; y++) {
		for(int x = 0; x <= 8; x++) {
			if(x == 5 && y != 1) continue;
			draw_vline(y_pos[y]+1, x_pos[0]+x*(width+1), height, ACS_VLINE);
		}
	}

	attroff(COLOR_PAIR(board->players[board->current_player].board_color_pair));
	attron(COLOR_PAIR(4));
	mvaddch(star_pos_y[0], star_pos_x[0], CHAR_STAR);
	mvaddch(star_pos_y[0], star_pos_x[2], CHAR_STAR);
	mvaddch(star_pos_y[1], star_pos_x[1], CHAR_STAR);
	mvaddch(star_pos_y[2], star_pos_x[0], CHAR_STAR);
	mvaddch(star_pos_y[2], star_pos_x[2], CHAR_STAR);
	attroff(COLOR_PAIR(4));

	return true;
}

bool draw_pawns(board_t* board) {
	int y0, x0;
	for(int j = 0; j < N_PLAYERS; j++) {
		attron(COLOR_PAIR(board->players[j].pawn_color_pair));
		for(int i = 0; i < N_PAWNS; i++) {
			y0 = padding_y + dividor_size + (pawn_path[j][board->players[j].pawn_positions[i]].y)*(board->height+1) + (board->height / 2);
			x0 = padding_x + dividor_size + (pawn_path[j][board->players[j].pawn_positions[i]].x)*(board->width+1) + (board->width / 2);
			mvaddch(y0,x0,board->players[j].pawn_char);
		}
		attroff(COLOR_PAIR(board->players[j].pawn_color_pair));
	}
	return true;
}

bool draw(board_t* board) {
	erase();

	if(board->winner >= 0 && board->winner <= N_PLAYERS) {
		mvprintw(0,0,"Congrats, %s (%c), you won!", board->players[board->winner].name, board->players[board->winner].pawn_char);
		return false;
	}

	draw_board(board);
	draw_selection(board);
	draw_pawns(board);

	mvprintw(0,0,"%s, '%c', your roll is: %d.", board->players[board->current_player].name, board->players[board->current_player].pawn_char, board->roll);
	mvprintw(1,0,"%s", board->err_msg);

	refresh();
	return true;
}
