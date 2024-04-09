#include "headers/game.h"
#include "../headers/globals.h"

game *setup_game() {
    game *res = malloc(sizeof(game));
    game->w = GAME_WIDTH;
    game->h = GAME_HEIGHT;

    game->board = malloc(sizeof(board));
    game->board->h = lines - 2 - 1; // 2 rows reserved for border, 1 row for line
    game->board->w = columns - 2; // 2 columns reserved for border
    game->board->grid = calloc((board->w)*(board->h),sizeof(char));

    return res;
}

void run_game(game *game) {
    while (true) {
        switch (game->game_state) {
	        case MENU :
                break;
            case GAME :
                update_game(game); // Should only be done server-side
                render_game(game); // Should only be done client-side
                break;
            default : 
                break;
        usleep(30*1000);
	}
}

void render_game(game *g) {
    // Update grid
    int x,y;
    for (y = 0; y < g->board->h; y++) {
        for (x = 0; x < g->board->w; x++) {
            char c;
            switch (get_grid(g->board,x,y)) {
                case 0: // Free Space
                    mvaddch(y+1,x+1,' ');
                    break;
                case 1: // Wall
                    mvaddch(y+1,x+1,'■');
                    break;
                case 2: // Player
                    mvaddch(y+1,x+1, 'o');
                default:
                    mvaddch(y+1,x+1,'?');
                    break;
            }
        }
    } 
    // Borders
    for (x = 0; x < b->w+2; x++) {
        mvaddch(0, x, '-');
        mvaddch(b->h+1, x, '-');
    }
    for (y = 0; y < b->h+2; y++) {
        mvaddch(y, 0, '|');
        mvaddch(y, b->w+1, '|');
    }
    mvaddch(0, 0,'o');
    mvaddch(0, b->w + 1,'o');
    mvaddch(b->h + 1, 0,'o');
    mvaddch(b->h + 1, b->w + 1,'o');
    
    // Update chat text
    attron(COLOR_PAIR(1)); // Enable custom color 1
    attron(A_BOLD); // Enable bold
    for (x = 0; x < g->board->w+2; x++) {
        if (x >= TEXT_SIZE || x >= l->cursor)
            mvaddch(b->h+2, x, ' ');
        else
            mvaddch(b->h+2, x, l->data[x]);
    }
    attroff(A_BOLD); // Disable bold
    attroff(COLOR_PAIR(1)); // Disable custom color 1
    refresh(); // Apply the changes to the terminal
}

int get_grid(board* b, int x, int y) {
    return b->grid[y*b->w + x];
}

void set_grid(board* b, int x, int y, int v) {
        b->grid[y*b->w + x] = v;
}

void update_game(game *g) {
    for (int i = 0; i < g->player_count; i++) {
        set_grid(g->board, g->players_list[i]->pos_x, g->players_list[i]->pos_y, 0);
        update_player(g->players_list[i]);
        set_grid(g->board, g->players_list[i]->pos_x, g->players_list[i]->pos_y, 2);
    }


}

int game_add_player(game *g, player *p) {
    if (g->player_count >= 4) {
        return 0;
    }
    else {
        g->players_list[g->player_count] = p;
        g->player_count++;
        return 1;
   }
}

void free_game(game *game) {
    free(game->board->grid);
    free(game->board);
    free(game);
}
