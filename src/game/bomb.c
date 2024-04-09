bomb *create_bomb(int x, int y) {
    bomb *res = malloc(sizeof(bomb));
    res->pos_x = x;
    res->pos_y = y;
    res->time_remaining = 100; // game loop a 30 fps. 100 = 3s.
    return res;
}

void update_bomb(bomb *b, float) {
    if (b->time_remaining == 0) {
        b->has_exploded = 1;
    }
    else {
        --b->time_remaining;
    }
}

