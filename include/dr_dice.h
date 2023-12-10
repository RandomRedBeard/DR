#ifndef DR_DICE_H
#define DR_DICE_H

#include <stdlib.h>

typedef struct
{
    int cnt, val;
} dr_dice_t;

int dr_dice_roll(dr_dice_t);

#endif