#include "test_base.h"
#include <stdio.h>
#include <time.h>
#include <dr_dmg.h>

void test_dmg() {
    dr_dice_t d = {.cnt=1, .val=4};
    dr_dmg_t dmg = {.die = d, .type = FIRE};
    int i = dr_dmg_roll(dmg);
    printf("%d\n", i);
}

int main() {
    srand(time(0));
    test_dmg();
    return 0;
}