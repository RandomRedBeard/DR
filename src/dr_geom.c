#include "dr_geom.h"

void dr_point2d_set(dr_point2d_t *p, int x, int y) {
  p->x = x;
  p->y = y;
}

int dr_point2d_eq(const dr_point2d_t p1, const dr_point2d_t p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

cosmic_json_t *dr_point2d_to_json(const dr_point2d_t p) {
  cosmic_json_t *j = cosmic_json_new_object();
  cosmic_json_add_kv_n(j, "x", p.x);
  cosmic_json_add_kv_n(j, "y", p.y);
  return j;
}

unsigned int dr_point2d_index(const dr_point2d_t p, unsigned int w) {
  return p.y * w + p.x;
}

double dr_point2d_dist(const dr_point2d_t p1, const dr_point2d_t p2) {
  return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

unsigned int dr_point2d_l1dist(const dr_point2d_t p1, const dr_point2d_t p2) {
  return abs(p2.x - p1.x) + abs(p2.y - p1.y);
}

void dr_vector2d_set(dr_vector2d_t *v, double x, double y) {
  double m = sqrt(pow(x, 2) + pow(y, 2));
  v->x = x / m;
  v->y = y / m;
  v->m = m;
}

cosmic_json_t *dr_line2d_to_json(const dr_line2d_t l) {
  cosmic_json_t *j = cosmic_json_new_object();
  cosmic_json_add_kv(j, "p1", dr_point2d_to_json(l.p1));
  cosmic_json_add_kv(j, "p2", dr_point2d_to_json(l.p2));
  return j;
}

int dr_rect2d_contains(const dr_rect2d_t r, const dr_point2d_t p) {
  return p.x >= r.x && p.x < r.x + r.w && p.y >= r.y && p.y < r.y + r.h;
}

cosmic_json_t *dr_rect2d_to_json(const dr_rect2d_t r) {
  cosmic_json_t *j = cosmic_json_new_object();
  cosmic_json_add_kv_n(j, "x", r.x);
  cosmic_json_add_kv_n(j, "y", r.y);
  cosmic_json_add_kv_n(j, "h", r.h);
  cosmic_json_add_kv_n(j, "w", r.w);
  return j;
}
