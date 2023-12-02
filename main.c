#include <SDL2/SDL.h>
#include <dr_argparser.h>
#include <dr_map.h>
#include <dr_pathfinder.h>
#include <dr_vis.h>
#include <stdlib.h>
#include <time.h>

void zoom_in(dr_vis_t *vis, double ratio) {
  int x =
      (vis->camera.w / 2.0) - (((vis->camera.w / 2.0) - vis->camera.x) * ratio);
  int y =
      (vis->camera.h / 2.0) - (((vis->camera.h / 2.0) - vis->camera.y) * ratio);

  vis->camera.x = x;
  vis->camera.y = y;

  vis->blockw *= ratio;
  vis->blockh *= ratio;
}

void zoom_out(dr_vis_t *vis, double ratio) {
  int x =
      (vis->camera.w / 2.0) - (((vis->camera.w / 2.0) - vis->camera.x) / ratio);
  int y =
      (vis->camera.h / 2.0) - (((vis->camera.h / 2.0) - vis->camera.y) / ratio);

  vis->camera.x = x;
  vis->camera.y = y;

  vis->blockw /= ratio;
  vis->blockh /= ratio;
}

dr_point2d_t center(dr_vis_t *vis, dr_point2d_t src) {
  dr_point2d_t pt;
  pt.x = (vis->camera.w / 2.0) - (src.x * vis->blockw);
  pt.y = (vis->camera.h / 2.0) - (src.y * vis->blockh);
  return pt;
}

dr_point2d_t reduc_center(dr_vis_t *vis, int x, int y) {
  dr_point2d_t pt;
  pt.x = ((vis->camera.w / 2.0) - x) / vis->blockw;
  pt.y = ((vis->camera.h / 2.0) - y) / vis->blockh;
  return pt;
}

SDL_Rect cnv(dr_rect2d_t r) {
  SDL_Rect rect;
  rect.x = r.x;
  rect.y = r.y;
  rect.h = r.h;
  rect.w = r.w;
  return rect;
}

void render_map(dr_vis_t *v, dr_map_t *m, SDL_Renderer *renderer) {
  dr_point2d_t p;
  SDL_Rect rect;
  char c, fill;
  int i, j, si, sj, il, jl;

  si = (-1 * v->camera.x) / v->blockw;
  if (si < 0) {
    si = 0;
  }

  il = ((v->camera.w - v->camera.x) / v->blockw) + 1;
  if (il > (int)m->width) {
    il = m->width;
  }

  sj = ((-1 * v->camera.y) / v->blockh);
  if (sj < 0) {
    sj = 0;
  }

  jl = ((v->camera.h - v->camera.y) / v->blockh) + 1;
  if (jl > (int)m->height) {
    jl = m->height;
  }

  for (i = si; i < il; i++) {
    for (j = sj; j < jl; j++) {
      fill = 0;
      c = *(m->map_buf + ((j * m->width) + i));
      if (c == '|' || c == '-') {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0xFF);
      } else if (c == '.') {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      } else if (c == '+') {
        fill = 1;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0xFF);
      } else if (c == '#') {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0xFF);
      } else {
        continue;
      }

      dr_point2d_set(&p, i, j);
      rect = cnv(dr_point_to_rect(v, p));
      if (fill) {
        SDL_RenderFillRect(renderer, &rect);
      } else {
        SDL_RenderDrawRect(renderer, &rect);
      }
    }
  }
}

void draw_person(dr_vis_t *v, dr_point2d_t cur, SDL_Renderer *renderer) {
  SDL_Rect rect;
  dr_point2d_t pt;
  if (v->camera_lock) {
    pt.x = v->camera.w / 2;
    pt.y = v->camera.h / 2;
    rect = cnv(dr_point_to_rect_raw(v, pt));
  } else {
    rect = cnv(dr_point_to_rect(v, cur));
  }

  SDL_RenderFillRect(renderer, &rect);
}

void draw_path(dr_vis_t *v, dr_map_t *m, cosmic_list_t *l,
               SDL_Renderer *renderer) {
  unsigned int i;
  long index;
  SDL_Rect rect;
  dr_point2d_t pt;

  ssize_t len = cosmic_list_size(l);

  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0xFF);

  for (i = 0; i < len; i++) {
    index = cosmic_list_get_t(l, i, NULL).l;
    dr_point2d_set(&pt, index % m->width, index / m->width);
    rect = cnv(dr_point_to_rect(v, pt));
    SDL_RenderDrawRect(renderer, &rect);
  }
}

void draw_center(dr_vis_t *v, SDL_Renderer *renderer) {
  SDL_Rect rect;
  dr_point2d_t pt;
  pt.x = v->camera.w / 2;
  pt.y = v->camera.h / 2;
  rect.x = pt.x - 3;
  rect.y = pt.y - 3;
  rect.w = 6;
  rect.h = 6;

  SDL_RenderFillRect(renderer, &rect);
}

void init_path(dr_vis_t *v, dr_map_t *m, dr_point2d_t src, cosmic_list_t **l,
               SDL_Event e) {
  dr_point2d_t pt;
  cosmic_vector_free(*l, NULL);
  *l = cosmic_vector_new(10);

  dr_point2d_set(&pt, (e.motion.x - v->camera.x) / v->blockw,
                 (e.motion.y - v->camera.y) / v->blockh);

  dr_find_path(m, *l, src, pt);
}

void set_init_pt(dr_map_t *m, dr_point2d_t *p) {
  dr_room_t *r = NULL;
  cosmic_list_get(m->room_list, 0, PCOSMIC_ANY(&r));
  p->x = r->rect.x;
  p->y = r->rect.y;
}

int main(int argc, char **argv) {
  long width = 400, height = 200, hc = 3, wc = 3;
  dr_map_t *map = NULL;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Event e;
  dr_point2d_t cur, mv, inf_center;
  dr_vis_t vis;
  int xdir, ydir;
  int camera_drag = 0;
  cosmic_list_t *path = cosmic_vector_new(10);
  dr_argparser_t parser = dr_argparser_new();

  dr_argparser_add_long(parser, "h", &height);
  dr_argparser_add_long(parser, "w", &width);
  dr_argparser_add_long(parser, "hc", &hc);
  dr_argparser_add_long(parser, "wc", &wc);

  dr_argparser_parse(parser, argc, argv);

  vis.blockh = 40;
  vis.blockw = 20;
  vis.camera.w = 1200;
  vis.camera.h = 800;
  vis.camera_lock = 1;

  map = dr_map_new(width, height, wc, hc);

  set_init_pt(map, &cur);

  window =
      SDL_CreateWindow("DR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       vis.camera.w, vis.camera.h, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  do {
    if (vis.camera_lock) {
      inf_center = center(&vis, cur);
      /* Set Camera */
      vis.camera.x = inf_center.x;
      vis.camera.y = inf_center.y;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    render_map(&vis, map, renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
    draw_person(&vis, cur, renderer);
    draw_path(&vis, map, path, renderer);
    draw_center(&vis, renderer);
    SDL_RenderPresent(renderer);

    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      break;
    } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
      break;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
      if (e.button.button == SDL_BUTTON_RIGHT) {
        camera_drag = 1;
        vis.camera_lock = 0;
      } else if (e.button.button == SDL_BUTTON_LEFT) {
        init_path(&vis, map, cur, &path, e);
      }
    }

    if (e.type == SDL_MOUSEBUTTONUP) {
      if (e.button.button == SDL_BUTTON_RIGHT) {
        camera_drag = 0;
      }
    }

    if (camera_drag && e.type == SDL_MOUSEMOTION) {
      vis.camera.x += e.motion.xrel;
      vis.camera.y += e.motion.yrel;
    }

    if (e.type == SDL_MOUSEWHEEL) {
      if (e.wheel.y > 0) {
        zoom_in(&vis, 1.25);
      } else if (e.wheel.y < 0) {
        zoom_out(&vis, 1.25);
      }
    }

    if (e.type != SDL_KEYDOWN) {
      continue;
    }

    if (e.key.keysym.sym == SDLK_s) {
      vis.camera_lock = 1;
    }

    xdir = ydir = 0;
    if (e.key.keysym.sym == SDLK_RIGHT) {
      xdir = 1;
    } else if (e.key.keysym.sym == SDLK_LEFT) {
      xdir = -1;
    } else if (e.key.keysym.sym == SDLK_UP) {
      ydir = -1;
    } else if (e.key.keysym.sym == SDLK_DOWN) {
      ydir = 1;
    }

    if (xdir == 0 && ydir == 0) {
      continue;
    }

    dr_point2d_set(&mv, cur.x + xdir, cur.y + ydir);
    if (dr_map_check_move(map, mv)) {
      cur.x += xdir;
      cur.y += ydir;
    }

  } while (1);

  SDL_DestroyWindow(window);

  return 0;
}
