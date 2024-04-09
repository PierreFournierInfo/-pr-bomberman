#ifndef GAME_H

typedef struct game game;
typedef struct board board;
typedef enum ACTION ACTION;

game *setup_game();
void update_game(game *, ACTION a);
void render_game(game *);
void run_game(game *);

int game_add_player(game *, player *); // Return 1 if successful 0 if game full;

void free_game(game *);

#endif // !GAME_H
