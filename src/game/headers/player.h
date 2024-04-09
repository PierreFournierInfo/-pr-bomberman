#ifndef PLAYER_H

typedef struct player player;
typedef enum ACTION ACTION;

player *setup_player();
void player_handle_input(player *p, ACTION);
void update_player(player *p);
int player_place_bomb(player *p);


#endif // !PLAYER_H
