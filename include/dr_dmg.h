#ifndef DR_DMG_H
#define DR_DMG_H

#include <dr_elem.h>
#include <dr_dice.h>

typedef struct
{
    dr_dice_t die;
    dr_elem_e type;
} dr_dmg_t;

#define dr_dmg_roll(d) dr_dice_roll(d.die)

#endif