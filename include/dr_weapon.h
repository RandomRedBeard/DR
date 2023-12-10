#ifndef DR_WEAPON_H
#define DR_WEAPON_H

#include <dr_item.h>
#include <dr_dmg.h>

typedef struct
{
    dr_item_t super;
    dr_dmg_t dmg;
    int thanded;
} dr_weapon_t;

#endif