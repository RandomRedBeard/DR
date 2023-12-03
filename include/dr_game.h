#ifndef DR_GAME_H
#define DR_GAME_H

#include <dr_level.h>
#include <dr_player.h>

typedef struct
{
    dr_level_t *level;
    dr_player_t *player;
    unsigned int w, h, n, m, mcnt;
} dr_game_t;

dr_game_t *dr_game_new(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, dr_player_t *);
void dr_game_next_level(dr_game_t *);
void dr_game_move_monsters(dr_game_t *);
void dr_game_init_path(dr_game_t *, dr_point2d_t, cosmic_list_t **,
                       dr_point2d_t);
void dr_game_set_monsters_path(dr_game_t *);
void dr_game_free(dr_game_t *);

#endif