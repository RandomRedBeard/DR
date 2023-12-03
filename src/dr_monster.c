#include "dr_monster.h"

dr_monster_t* dr_monster_new() {
    dr_monster_t* m = malloc(sizeof(dr_monster_t));
    m->curpath = NULL;
    return m;
}