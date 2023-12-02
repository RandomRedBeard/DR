#ifndef DR_MAP_H
#define DR_MAP_H

#include <cosmic/cosmic_json.h>
#include <cosmic/cosmic_llist.h>
#include <dr_geom.h>
#include <dr_pathfinder.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dr_hallway {
  cosmic_list_t *lsegs;
} dr_hallway_t;

dr_hallway_t *dr_hallway_new(cosmic_list_t *);
void dr_hallway_free(dr_hallway_t *);
cosmic_json_t *dr_hallway_to_json(const dr_hallway_t *);

typedef struct dr_room {
  dr_rect2d_t rect;
  /**
   * Doors
   */
  dr_point2d_t n, s, w, e;
} dr_room_t;

dr_room_t *dr_room_new();
void dr_room_free(dr_room_t *);
cosmic_json_t *dr_room_to_json(const dr_room_t *);

typedef struct dr_map {
  unsigned int height, width, n, m;
  cosmic_list_t *room_list;
  cosmic_list_t *hallway_list;
  char *map_buf;
} dr_map_t;

dr_map_t *dr_map_new(unsigned int, unsigned int, unsigned int, unsigned int);
void dr_map_free(dr_map_t *);
cosmic_json_t *dr_map_to_json(const dr_map_t *);
int dr_map_check_move(const dr_map_t *, const dr_point2d_t);

int dr_find_path(const dr_map_t *, cosmic_list_t *, dr_point2d_t, dr_point2d_t);

#ifdef __cplusplus
}

#endif

#endif
