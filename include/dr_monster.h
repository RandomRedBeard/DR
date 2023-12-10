#ifndef DR_MONSTER_H
#define DR_MONSTER_H

#include <dr_geom.h>
#include <cosmic/cosmic_list.h>

typedef struct
{
    dr_point2d_t pt;
    cosmic_list_t *curpath;
    int chp, mhp;
} dr_monster_t;

dr_monster_t *dr_monster_new();

#endif