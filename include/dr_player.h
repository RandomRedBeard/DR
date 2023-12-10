#ifndef DR_PLAYER_H
#define DR_PLAYER_H

#include <cosmic/cosmic_vector.h>
#include <dr_geom.h>
#include <dr_item.h>

typedef struct
{
    dr_point2d_t pt;
    dr_item_t* mhand, *ohand;
    cosmic_list_t *inventory;
} dr_player_t;

dr_player_t *dr_player_new();

int dr_player_equip_mhand(dr_player_t*, dr_item_t*);
int dr_player_equip_ohand(dr_player_t*, dr_item_t*);

int dr_player_unequip_mhand(dr_player_t*);
int dr_player_unequip_mhand(dr_player_t*);


void dr_player_free(dr_player_t *);

#endif