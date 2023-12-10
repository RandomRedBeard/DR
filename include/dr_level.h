#ifndef DR_LEVEL_H
#define DR_LEVEL_H

#include <cosmic/cosmic_any.h>
#include <dr_monster.h>
#include <dr_map.h>
#include <dr_item.h>
#include <dr_dmg.h>
#include <dr_player.h>

typedef struct
{
    dr_player_t *player;
    cosmic_list_t *monster_list;
    cosmic_list_t *item_list;
    dr_map_t *map;
} dr_level_t;

dr_level_t *dr_level_new(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void dr_level_rand_point(dr_level_t *, dr_point2d_t *);
dr_monster_t *dr_level_get_monster(dr_level_t *, dr_point2d_t);
int dr_level_check_move(dr_level_t *, dr_point2d_t);
dr_item_t *dr_level_pickup_item(dr_level_t *, dr_point2d_t);

void dr_level_move_monsters(dr_level_t *);
int dr_level_attack_monster(dr_level_t *, dr_monster_t **, dr_dmg_t);
void dr_level_init_path(dr_level_t *, dr_point2d_t, cosmic_list_t **,
                        dr_point2d_t);
void dr_level_set_monsters_path(dr_level_t *);

void dr_level_free(dr_level_t *);

#endif