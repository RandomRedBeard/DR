#include "dr_level.h"

void dr_level_rand_point(dr_level_t* level, dr_point2d_t* pt) {
    const dr_room_t* room = dr_map_rand_room(level->map);
    pt->x = room->rect.x + rand() % room->rect.w;
    pt->y = room->rect.y + rand() % room->rect.h;
}

int dr_level_has_monster(dr_level_t* level, dr_point2d_t pt) {
    int len = cosmic_list_size(level->monster_list);
    for (int i = 0; i < len; i++) {
        dr_monster_t* m = cosmic_list_get_tt(level->monster_list, i, dr_monster_t*);
        if (dr_point2d_eq(m->pt, pt)) {
            return 1;
        }
    }
    return 0;
}

int dr_level_check_move(dr_level_t* level, dr_point2d_t pt) {
    return dr_map_check_move(level->map, pt) && !dr_level_has_monster(level, pt);
}

void dr_level_init_monster(dr_level_t* level, dr_monster_t* monster) {
  dr_level_rand_point(level, &monster->pt);
}

dr_level_t* dr_level_new(unsigned int w, unsigned int h, unsigned int n,
    unsigned int m, unsigned int mcnt) {
    dr_level_t* level = malloc(sizeof(dr_level_t));
    level->map = dr_map_new(w, h, n, m);
    level->monster_list = cosmic_vector_new(mcnt);
    for (int i = 0; i < mcnt; i++) {
        dr_monster_t* monster = dr_monster_new();
        dr_level_init_monster(level, monster);
        cosmic_list_add(level->monster_list, COSMIC_ANY(monster));
    }

    return level;
}

void dr_level_free(dr_level_t* level) {
    dr_map_free(level->map);
    cosmic_vector_free(level->monster_list, free);
}