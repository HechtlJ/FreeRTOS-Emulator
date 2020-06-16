#define PLAYER_Y_COORD 400
//TODO: add include guards

typedef struct {
    unsigned int Life;
    unsigned int x_coord;
} player_t;




void paint_player(player_t *player);
void player_move_right(player_t *player);
void player_move_left(player_t *player);
void player_shoot(player_t *player);
void player_reduce_life(player_t *player);
