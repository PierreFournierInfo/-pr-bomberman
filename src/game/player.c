#include "headers/player.h"

player *setup_player() {
    player *res = malloc(sizeof(player));
    memset(res, 0, sizeof(player));
    
    res->has_bomb = 1;

    return res;
}

void player_handle_input(player *p, ACTION a) {
    switch(a) {
        case NONE:
            break;
        case UP:
            p->next_x = p->pos_x + 1;
            break;
        case DOWN:
            p->next_x = p->pos_x - 1;
            break;
        case LEFT:
            p->next_y = p->pos_y - 1;
            break;
        case RIGHT:
            p->next_y= p->pos_y + 1;
            break;
        case ENTER:
            //TODO
            break;
        case QUIT:
            //TODO
            break;
    }
}

void update_player(player *p) {
    p->pos_x = p->next_x;
    p->pos_y = p->next_y;
} 
