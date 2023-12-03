#ifndef DR_MONSTER_H
#define DR_MONSTER_H

#include <dr_geom.h>
#include <cosmic/cosmic_list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    dr_point2d_t pt;
    cosmic_list_t* curpath;
} dr_monster_t;

dr_monster_t* dr_monster_new();

#ifdef __cplusplus
}
#endif

#endif