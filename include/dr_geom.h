#ifndef DR_GEOM_H
#define DR_GEOM_H

#include <cosmic/cosmic_json.h>
#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int x, y;
} dr_point2d_t;

void dr_point2d_set(dr_point2d_t *, int, int);
int dr_point2d_eq(const dr_point2d_t, const dr_point2d_t);

cosmic_json_t *dr_point2d_to_json(const dr_point2d_t);
/**
 * 1d index for 2d point
 */
unsigned int dr_point2d_index(const dr_point2d_t, unsigned int);
double dr_point2d_dist(const dr_point2d_t, const dr_point2d_t);
unsigned int dr_point2d_l1dist(const dr_point2d_t, const dr_point2d_t);

typedef struct {
  double x, y, m;
} dr_vector2d_t;

/**
 * Recommended for magnitude setting
 */
void dr_vector2d_set(dr_vector2d_t *, double, double);

typedef struct {
  dr_point2d_t p1, p2;
} dr_line2d_t;

cosmic_json_t *dr_line2d_to_json(const dr_line2d_t);

typedef struct {
  int x, y, h, w;
} dr_rect2d_t;

int dr_rect2d_contains(const dr_rect2d_t, const dr_point2d_t);
cosmic_json_t *dr_rect2d_to_json(const dr_rect2d_t);

#ifdef __cplusplus
}

#endif

#endif
