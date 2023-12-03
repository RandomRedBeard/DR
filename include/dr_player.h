#ifndef DR_PLAYER_H
#define DR_PLAYER_H

#include <cosmic/cosmic_vector.h>
#include <dr_geom.h>
#include <dr_item.h>

typedef struct
{
    dr_point2d_t pt;
    cosmic_list_t *inventory;
} dr_player_t;

dr_player_t *dr_player_new();
void dr_player_free(dr_player_t *);

#endif