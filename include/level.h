#ifndef LEVEL_H
#define LEVEL_H




SemaphoreHandle_t  Level;


void init_levels();
void next_level();
void game_over();

#define LEVEL_CYCLE 11
#define LEVEL_INVADER_DISTANCE 10

void init_level(int level);



#endif /* LEVEL_H */