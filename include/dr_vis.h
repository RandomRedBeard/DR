#ifndef DR_VIS_H
#define DR_VIS_H

#include <dr_geom.h>
#include <dr_map.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dr_vis {
  dr_rect2d_t camera;
  double blockw, blockh;
  int camera_lock;
} dr_vis_t;

dr_rect2d_t dr_point_to_rect_raw(const dr_vis_t *, const dr_point2d_t);
dr_rect2d_t dr_point_to_rect(const dr_vis_t *, const dr_point2d_t);
dr_rect2d_t dr_rect_to_rect(const dr_vis_t *, const dr_rect2d_t);

#ifdef __cplusplus
}
#endif

#endif
