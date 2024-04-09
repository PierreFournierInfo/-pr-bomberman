// Build with -lncurses option

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "protocol/messages/protocol_client_messages.h"
#include "headers/structs.h"
#include "game/headers/game.h"
#include "game/headers/bomb.h"
#include "game/headers/player.h"

#define TEXT_SIZE 255

typedef enum GAMESTATE GAMESTATE;
typedef enum ACTION ACTION;

GAMESTATE game_state;
int running,
    chat_type;

typedef struct line {
    char data[TEXT_SIZE];
    int cursor;
} line;

typedef struct pos {
    int x;
    int y;
} pos;

void free_board(board* board) {
    free(board->grid);
}

void refresh_menu(board* b, line* l) {}

    
ACTION control(line* l) {
    int c;
    int prev_c = ERR;
    // We consume all similar consecutive key presses
    while ((c = getch()) != ERR) { // getch returns the first key press in the queue
        if (prev_c != ERR && prev_c != c) {
            ungetch(c); // put 'c' back in the queue
            break;
        }
        prev_c = c;
    }
    ACTION a = NONE;
    switch (prev_c) {
        case ERR: break;
        case KEY_LEFT:
            a = LEFT; break;
        case KEY_RIGHT:
            a = RIGHT; break;
        case KEY_UP:
            a = UP; break;
        case KEY_DOWN:
            a = DOWN; break;
        case '~':
            a = QUIT; break;
        case KEY_BACKSPACE:
            if (l->cursor > 0) l->cursor--;
            break;
        case KEY_ENTER:
            a = ENTER; break;
        default:
            if (prev_c >= ' ' && prev_c <= '~' && l->cursor < TEXT_SIZE)
                l->data[(l->cursor)++] = prev_c;
            break;
    }
    return a;
}

void setup_ncurses() {
    // NOTE: All ncurses operations (getch, mvaddch, refresh, etc.) must be done on the same thread.
    initscr(); /* Start curses mode */
    raw(); /* Disable line buffering */
    intrflush(stdscr, FALSE); /* No need to flush when intr key is pressed */
    keypad(stdscr, TRUE); /* Required in order to get events from keyboard */
    nodelay(stdscr, TRUE); /* Make getch non-blocking */
    noecho(); /* Don't echo() while we do getch (we will manually print characters when relevant) */
    curs_set(0); // Set the cursor to invisible
    start_color(); // Enable colors
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Define a new color style (text is yellow, background is black)
}

int main() {
    setup_ncurses();

    line *l = malloc(sizeof(line));
    game *game = setup_game();
    player *player = setup_player();

    game_add_player(game, player);

    while(running) {
	    ACTION a = control(l);

        player_handle_input(player, a);

    	run_game(game);
    }

    curs_set(1); // Set the cursor to visible again
    endwin(); /* End curses mode */

    free_game(game);

    return 0;
}
