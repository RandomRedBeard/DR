#ifndef DR_LEVEL_H
#define DR_LEVEL_H

#include <cosmic/cosmic_any.h>
#include <dr_monster.h>
#include <dr_map.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    cosmic_list_t* monster_list;
    dr_map_t* map;
} dr_level_t;

dr_level_t* dr_level_new(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
void dr_level_rand_point(dr_level_t*, dr_point2d_t*);
int dr_level_check_move(dr_level_t*, dr_point2d_t);
void dr_level_free(dr_level_t*);

#ifdef __cplusplus
}
#endif

#endif