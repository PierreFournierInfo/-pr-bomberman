#ifndef STRUCTS_H
#include "globals.h"

struct player {
    int pos_x, pos_y,
        id,
        team_id,
        has_bomb;
};
typedef struct player player;

struct bomb {
    int pos_x, pos_y, next_x, next_y,
        time_remaining,
        has_exploded;
};
typedef struct bomb bomb;

struct board {
    char* grid;
    int w;
    int h;
};
typedef struct board board;

struct game {
    GAMESTATE game_state;
    size_t player_count;
    player *players_list[4];
    bomb *bomb_list[4];
    board *b;
};
typedef struct game game;

#endif // !STRUCTS_H
