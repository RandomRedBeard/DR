#ifndef DR_GAME_H
#define DR_GAME_H

#include <stdarg.h>
#include <stdio.h>
#include <dr_level.h>
#include <dr_player.h>
#include <dr_dmg.h>

typedef struct
{
    dr_level_t *level;
    dr_player_t *player;
    unsigned int w, h, n, m, mcnt;
    cosmic_list_t* messages;
    int message_limit;
} dr_game_t;

dr_game_t *dr_game_new(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, dr_player_t *, int);
void dr_game_add_message(dr_game_t*, const char*, ...);
void dr_game_next_level(dr_game_t *);
void dr_game_free(dr_game_t *);

#endif