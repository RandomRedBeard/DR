#include "dr_vis.h"

dr_rect2d_t dr_point_to_rect_raw(const dr_vis_t *v, const dr_point2d_t p) {
  dr_rect2d_t rect;
  rect.x = p.x;
  rect.y = p.y;
  rect.w = v->blockw;
  rect.h = v->blockh;
  return rect;
}

dr_rect2d_t dr_point_to_rect(const dr_vis_t *v, const dr_point2d_t p) {
  dr_rect2d_t rect;
  rect.x = (p.x * v->blockw) + v->camera.x;
  rect.y = (p.y * v->blockh) + v->camera.y;
  rect.w = v->blockw;
  rect.h = v->blockh;
  return rect;
}

dr_rect2d_t dr_rect_to_rect(const dr_vis_t *v, const dr_rect2d_t rect) {
  dr_rect2d_t r;
  r.x = (rect.x * v->blockw) + v->camera.x;
  r.y = (rect.y * v->blockh) + v->camera.y;
  r.w = rect.w * v->blockw;
  r.h = rect.h * v->blockh;
  return r;
}
