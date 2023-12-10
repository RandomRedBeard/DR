#include "test_base.h"
#include <stdio.h>
#include <time.h>
#include <dr_dice.h>

void test_dice() {
    dr_dice_t d = {.cnt=1, .val=4};
    printf("%d\n", dr_dice_roll(d));
}

int main() {
    srand(time(0));
    test_dice();
    return 0;
}