#include "dr_level.h"

void dr_level_rand_point(dr_level_t *level, dr_point2d_t *pt)
{
    const dr_room_t *room = dr_map_rand_room(level->map);
    pt->x = room->rect.x + rand() % room->rect.w;
    pt->y = room->rect.y + rand() % room->rect.h;
}

dr_monster_t *dr_level_get_monster(dr_level_t *level, dr_point2d_t pt)
{
    int len = cosmic_list_size(level->monster_list);
    for (int i = 0; i < len; i++)
    {
        dr_monster_t *m = cosmic_list_get_tt(level->monster_list, i, dr_monster_t *);
        if (dr_point2d_eq(m->pt, pt))
        {
            return m;
        }
    }
    return NULL;
}

int dr_level_check_move(dr_level_t *level, dr_point2d_t pt)
{
    return dr_map_check_move(level->map, pt) && !dr_level_get_monster(level, pt);
}

void dr_level_init_monster(dr_level_t *level, dr_monster_t *monster)
{
    dr_level_rand_point(level, &monster->pt);
}

dr_item_t *dr_level_pickup_item(dr_level_t *level, dr_point2d_t pt)
{
    int len = cosmic_list_size(level->item_list);
    int i = 0;
    dr_item_t *item = NULL;
    for (i = 0; i < len; i++)
    {
        item = cosmic_list_get_tt(level->item_list, i, dr_item_t *);
        if (dr_point2d_eq(item->pt, pt))
        {
            break;
        }
    }

    if (i != len)
    {
        cosmic_list_remove(level->item_list, i, PCOSMIC_ANY(&item));
    }
    else
    {
        item = NULL;
    }

    return item;
}

void dr_level_move_monster_s(dr_level_t *level, dr_monster_t *monster, dr_player_t *player)
{
    cosmic_any_t index;
    dr_point2d_t pt;
    if (cosmic_list_pop(monster->curpath, &index) < 0)
    {
        return;
    }
    dr_point2d_set(&pt, index.l % level->map->width, index.l / level->map->width);
    if (dr_level_check_move(level, pt) && !dr_point2d_eq(player->pt, pt))
    {
        monster->pt = pt;
    }
}

void dr_level_move_monsters(dr_level_t *level)
{
    int len = cosmic_list_size(level->monster_list);
    for (int i = 0; i < len; i++)
    {
        dr_monster_t *monster = cosmic_list_get_tt(level->monster_list, i, dr_monster_t *);
        dr_level_move_monster_s(level, monster, level->player);
    }
}

int dr_level_remove_monster(dr_level_t* level, dr_monster_t* monster) {
    int len = cosmic_list_size(level->monster_list);
    for (int i = 0; i < len; i++) {
        dr_monster_t* m = cosmic_list_get_tt(level->monster_list, i, dr_monster_t*);
        if (m == monster) {
            cosmic_list_remove(level->monster_list, i, PCOSMIC_ANY(&m));
            return 1;
        }
    }
    return 0;
}

int dr_level_attack_monster(dr_level_t *level, dr_monster_t **monster, dr_dmg_t dmg)
{
    int rd = dr_dmg_roll(dmg);
    if ((*monster)->chp -= rd <= 0) {
        dr_level_remove_monster(level, *monster);
        free(*monster);
        *monster = NULL;
    }
    return rd;
}

void dr_level_init_path(dr_level_t *level, dr_point2d_t src, cosmic_list_t **l,
                        dr_point2d_t dest)
{
    if (*l)
    {
        cosmic_vector_free(*l, NULL);
    }
    *l = cosmic_vector_new(10);
    dr_find_path(level->map, *l, src, dest);
}

void dr_level_set_monsters_path(dr_level_t *level)
{
    int len = cosmic_list_size(level->monster_list);
    for (int i = 0; i < len; i++)
    {
        dr_monster_t *monster = cosmic_list_get_tt(level->monster_list, i, dr_monster_t *);
        dr_level_init_path(level, monster->pt, &monster->curpath, level->player->pt);
    }
}

dr_level_t *dr_level_new(unsigned int w, unsigned int h, unsigned int n,
                         unsigned int m, unsigned int mcnt)
{
    dr_level_t *level = malloc(sizeof(dr_level_t));
    level->map = dr_map_new(w, h, n, m);
    level->monster_list = cosmic_vector_new(mcnt);
    for (int i = 0; i < mcnt; i++)
    {
        dr_monster_t *monster = dr_monster_new();
        dr_level_init_monster(level, monster);
        cosmic_list_add(level->monster_list, COSMIC_ANY(monster));
    }

    // Initialize empty item list
    level->item_list = cosmic_vector_new(10);

    return level;
}

void dr_level_free(dr_level_t *level)
{
    dr_map_free(level->map);
    cosmic_vector_free(level->monster_list, (cosmic_list_dealloc)free);
}