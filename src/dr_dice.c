#include "dr_dice.h"

int dr_dice_roll(dr_dice_t d)
{
    int r = 0;
    for (int i = 0; i < d.cnt; i++)
    {
        r += (1 + rand() % d.val);
    }
    return r;
}
