#include "dr_map.h"

dr_hallway_t *dr_hallway_new(cosmic_list_t *lsegs) {
  dr_hallway_t *h = malloc(sizeof(dr_hallway_t));
  h->lsegs = lsegs;
  return h;
}

void dr_hallway_free(dr_hallway_t *h) {
  cosmic_llist_free(h->lsegs, (cosmic_list_dealloc)free);
  free(h);
}

cosmic_json_t *dr_hallway_to_json(const dr_hallway_t *h) {
  cosmic_json_t *j = cosmic_json_new_list();
  cosmic_iterator_t *iter = cosmic_list_iterator(h->lsegs);
  dr_line2d_t *l = NULL;
  while (cosmic_iterator_next(iter, PCOSMIC_ANY(&l)) >= 0) {
    cosmic_json_add(j, dr_line2d_to_json(*l));
  }
  return j;
}

dr_room_t *dr_room_new() { return calloc(1, sizeof(dr_room_t)); }

void dr_room_free(dr_room_t *r) { free(r); }

cosmic_json_t *dr_room_to_json(const dr_room_t *r) {
  cosmic_json_t *obj = cosmic_json_new_object();
  cosmic_json_add_kv(obj, "rect", dr_rect2d_to_json(r->rect));
  return obj;
}

dr_hallway_t *dr_hallway_build(const dr_point2d_t p1, const dr_point2d_t p2,
                               int dir) {
  int xleft = abs(p1.x - p2.x), yleft = abs(p1.y - p2.y);
  int xdir = 0, ydir = 0, cdir = dir;
  dr_point2d_t index = p1, last_choice;
  cosmic_list_t *lsegs = cosmic_llist_new();
  dr_line2d_t *l = NULL;

  if (p1.x < p2.x) {
    xdir = 1;
  } else if (p1.x > p2.x) {
    xdir = -1;
  }

  if (p1.y < p2.y) {
    ydir = 1;
  } else if (p1.y > p2.y) {
    ydir = -1;
  }

  last_choice = p1;
  while (!dr_point2d_eq(index, p2)) {
    if (xleft > rand() % (yleft + 1)) {
      dir = 0;
    } else {
      dir = 1;
    }

    if (dir != cdir) {
      cdir = dir;
      l = malloc(sizeof(dr_line2d_t));
      l->p1 = last_choice;
      l->p2 = index;
      cosmic_list_add(lsegs, COSMIC_ANY(l));
      last_choice = index;
    }

    if (dir == 0) {
      index.x += xdir;
      xleft--;
    } else if (dir == 1) {
      index.y += ydir;
      yleft--;
    }
  }

  l = malloc(sizeof(dr_line2d_t));
  l->p1 = last_choice;
  l->p2 = p2;
  cosmic_list_add(lsegs, COSMIC_ANY(l));
  return dr_hallway_new(lsegs);
}

int prob(double p) {
  int upperbound = 1;
  int rval = 0;
  if (p == 0) {
    return 0;
  }

  if (p >= 1) {
    return 1;
  }

  while (p < 1) {
    p *= 10;
    upperbound *= 10;
  }

  rval = rand() % upperbound;
  return rval < p;
}

int check_room_prob(int index, char *buf) {
  char c = *(buf + (index / 8));
  int t = index % 8;
  return c & (1 << t);
}

dr_room_t *find_room(dr_map_t *m, int index, char *buf) {
  int i, r = 0;
  dr_room_t *room = NULL;
  for (i = 0; i < index; i++) {
    if (check_room_prob(i, buf)) {
      r++;
    }
  }

  cosmic_list_get(m->room_list, r, PCOSMIC_ANY(&room));
  return room;
}

#define HWALL '-'
#define VWALL '|'
#define FLOOR '.'
#define HALLWAY '#'
#define ENTRANCE '+'

#define WALKABLE "#.+"

void dr_map_draw_room(dr_map_t *m, dr_room_t *r) {
  int i, j;
  int width = r->rect.w + 1;
  int height = r->rect.h + 1;
  dr_point2d_t p;

  for (i = 0; i < width; i++) {
    dr_point2d_set(&p, r->rect.x + i, r->rect.y - 1);
    *(m->map_buf + dr_point2d_index(p, m->width)) = HWALL;

    dr_point2d_set(&p, r->rect.x + i, r->rect.y + r->rect.h);
    *(m->map_buf + dr_point2d_index(p, m->width)) = HWALL;
  }

  for (i = 0; i < height; i++) {
    dr_point2d_set(&p, r->rect.x - 1, r->rect.y + i);
    *(m->map_buf + dr_point2d_index(p, m->width)) = VWALL;

    dr_point2d_set(&p, r->rect.x + r->rect.w, r->rect.y + i);
    *(m->map_buf + dr_point2d_index(p, m->width)) = VWALL;
  }

  for (i = 0; i < r->rect.w; i++) {
    for (j = 0; j < r->rect.h; j++) {
      dr_point2d_set(&p, r->rect.x + i, r->rect.y + j);
      *(m->map_buf + dr_point2d_index(p, m->width)) = FLOOR;
    }
  }

  if (r->n.x != 0) {
    *(m->map_buf + dr_point2d_index(r->n, m->width)) = ENTRANCE;
  }

  if (r->s.x != 0) {
    *(m->map_buf + dr_point2d_index(r->s, m->width)) = ENTRANCE;
  }

  if (r->e.x != 0) {
    *(m->map_buf + dr_point2d_index(r->e, m->width)) = ENTRANCE;
  }

  if (r->w.x != 0) {
    *(m->map_buf + dr_point2d_index(r->w, m->width)) = ENTRANCE;
  }
}

void dr_map_draw_hallway(dr_map_t *m, dr_hallway_t *h) {
  cosmic_iterator_t *literator = NULL;
  dr_line2d_t *l = NULL;
  dr_point2d_t p;

  int xdir, ydir;

  literator = cosmic_list_iterator(h->lsegs);
  while (cosmic_iterator_next(literator, PCOSMIC_ANY(&l)) >= 0) {
    xdir = ydir = 0;
    p = l->p1;
    if (p.x < l->p2.x) {
      xdir = 1;
    } else if (p.x > l->p2.x) {
      xdir = -1;
    }

    if (p.y < l->p2.y) {
      ydir = 1;
    } else if (p.y > l->p2.y) {
      ydir = -1;
    }

    while (!dr_point2d_eq(p, l->p2)) {
      *(m->map_buf + dr_point2d_index(p, m->width)) = HALLWAY;
      p.x += xdir;
      p.y += ydir;
    }
    *(m->map_buf + dr_point2d_index(p, m->width)) = HALLWAY;
  }
  cosmic_iterator_close(literator);
}

void connect_hallways(dr_map_t *m, dr_room_t *r, unsigned int roomProbIndex,
                      char *buf) {
  unsigned int tindex = roomProbIndex;
  unsigned int mod = tindex % m->n;
  dr_point2d_t p, p1, p2;
  dr_room_t *dest = NULL;
  dr_hallway_t *h = NULL;

  for (; mod < m->n - 1; mod++) {
    tindex++;
    if (!check_room_prob(tindex, buf)) {
      continue;
    }

    dr_point2d_set(&p, r->rect.x + r->rect.w,
                   r->rect.y + 1 + rand() % (r->rect.h - 1));

    r->e = p;
    dest = find_room(m, tindex, buf);
    p.x = dest->rect.x - 1;
    p.y = dest->rect.y + 1 + rand() % (dest->rect.h - 1);
    dest->w = p;

    dr_point2d_set(&p1, r->e.x + 1, r->e.y);
    dr_point2d_set(&p2, dest->w.x - 1, dest->w.y);

    h = dr_hallway_build(p1, p2, 1);
    cosmic_list_add(m->hallway_list, COSMIC_ANY(h));

    dr_map_draw_hallway(m, h);

    break;
  }

  tindex = roomProbIndex;
  mod = tindex / m->n;

  for (; mod < m->m - 1; mod++) {
    tindex += m->n;
    if (!check_room_prob(tindex, buf)) {
      continue;
    }

    dr_point2d_set(&p, r->rect.x + 1 + rand() % (r->rect.w - 1),
                   r->rect.y + r->rect.h);

    r->s = p;
    dest = find_room(m, tindex, buf);
    p.y = dest->rect.y - 1;
    p.x = dest->rect.x + 1 + rand() % (dest->rect.w - 1);

    dest->n = p;

    dr_point2d_set(&p1, r->s.x, r->s.y + 1);
    dr_point2d_set(&p2, dest->n.x, dest->n.y - 1);

    h = dr_hallway_build(p1, p2, 0);
    cosmic_list_add(m->hallway_list, COSMIC_ANY(h));

    dr_map_draw_hallway(m, h);

    break;
  }

  dr_map_draw_room(m, r);
}

void dr_map_build(dr_map_t *m) {
  int maxrh = m->height / m->m;
  int maxrw = m->width / m->n;
  unsigned int rcount = m->n * m->m;

  int tmidpth, tmidptw;

  unsigned int i, j;
  char *buf = alloca((rcount / 8) + 1);

  dr_point2d_t p, tl;
  dr_room_t *r;

  int numberOfAllowed = ((m->n - 1) * (m->m - 1)) + 2;
  int numberAllowedFalse = rcount - numberOfAllowed;
  int index = 0, wdel, hdel;

  int roomindex = 0;

  memset(buf, 0, (rcount / 8) + 1);

  /**
   * Fill out room matrix
   */
  for (i = 0; i < rcount; i++) {
    if (!prob(numberAllowedFalse / (rcount * 1.0))) {
      *(buf + (i / 8)) |= (1 << (i % 8));
    } else {
      numberAllowedFalse--;
    }
  }

  for (i = 0; i < m->m; i++) {
    for (j = 0; j < m->n; j++) {
      index = i * m->n + j;
      if (!check_room_prob(index, buf)) {
        continue;
      }

      tmidptw = maxrw * j;
      tmidpth = maxrh * i;

      dr_point2d_set(&p, 3 + rand() % (maxrw - 5), 3 + rand() % (maxrh - 5));

      wdel = maxrw - p.x;
      hdel = maxrh - p.y;

      dr_point2d_set(&tl, tmidptw + (1 + rand() % (wdel - 2)),
                     tmidpth + (1 + rand() % (hdel - 2)));

      r = dr_room_new();
      r->rect.x = tl.x;
      r->rect.y = tl.y;
      r->rect.w = p.x;
      r->rect.h = p.y;
      cosmic_list_add(m->room_list, COSMIC_ANY(r));
    }
  }

  for (i = 0; i < rcount; i++) {
    if (check_room_prob(i, buf)) {
      cosmic_list_get(m->room_list, roomindex, PCOSMIC_ANY(&r));
      connect_hallways(m, r, i, buf);
      roomindex++;
    }
  }
}

dr_map_t *dr_map_new(unsigned int width, unsigned int height, unsigned int n,
                     unsigned int m_) {
  dr_map_t *m = malloc(sizeof(dr_map_t));
  unsigned int rsize = width * height;
  m->width = width;
  m->height = height;

  m->n = n;
  m->m = m_;

  m->room_list = cosmic_llist_new();
  m->hallway_list = cosmic_llist_new();

  m->map_buf = malloc(rsize + 1);
  memset(m->map_buf, ' ', rsize);
  *(m->map_buf + rsize) = 0;

  dr_map_build(m);
  return m;
}

void dr_map_free(dr_map_t *m) {
  cosmic_llist_free(m->room_list, (cosmic_list_dealloc)dr_room_free);
  cosmic_llist_free(m->hallway_list, (cosmic_list_dealloc)dr_hallway_free);
  free(m->map_buf);
  free(m);
}

cosmic_json_t *dr_map_to_json(const dr_map_t *m) {
  cosmic_json_t *obj = cosmic_json_new_object(), *child = NULL;
  dr_hallway_t *h = NULL;

  cosmic_json_t *rlist = cosmic_json_new_list();
  cosmic_iterator_t *iter = cosmic_list_iterator(m->room_list);
  dr_room_t *r = NULL;

  cosmic_json_t *hlist = cosmic_json_new_list();

  cosmic_json_add_kv_n(obj, "h", m->height);
  cosmic_json_add_kv_n(obj, "w", m->width);

  while (cosmic_iterator_next(iter, PCOSMIC_ANY(&r)) >= 0) {
    child = dr_room_to_json(r);
    cosmic_json_add(rlist, child);
  }
  cosmic_iterator_close(iter);

  iter = cosmic_list_iterator(m->hallway_list);
  while (cosmic_iterator_next(iter, PCOSMIC_ANY(&h)) >= 0) {
    child = dr_hallway_to_json(h);
    cosmic_json_add(hlist, child);
  }
  cosmic_iterator_close(iter);

  cosmic_json_add_kv(obj, "rooms", rlist);
  cosmic_json_add_kv(obj, "hallways", hlist);

  return obj;
}

int dr_map_check_move(const dr_map_t *m, const dr_point2d_t dest) {
  unsigned int index = dr_point2d_index(dest, m->width);
  return index < m->width * m->height &&
         strchr(WALKABLE, *(m->map_buf + index));
}

const dr_room_t* dr_map_rand_room(dr_map_t* map) {
  return cosmic_list_get_tt(map->room_list, rand() %  cosmic_list_size(map->room_list), dr_room_t*);
}

int dr_find_path(const dr_map_t *m, cosmic_list_t *l, dr_point2d_t src,
                 dr_point2d_t dest) {
  int len = m->width * m->height;
  int i;
  int src_index = dr_point2d_index(src, m->width);
  int *pred = malloc(sizeof(int) * len);
  int *nodes = malloc(sizeof(int) * 4 * len);

  for (i = 0; i < len; i++) {
    if (i % m->width == 0 || !strchr(WALKABLE, *(m->map_buf + i - 1))) {
      nodes[i * 4] = -1;
    } else {
      nodes[i * 4] = i - 1;
    }

    if (i % m->width == m->width - 1 ||
        !strchr(WALKABLE, *(m->map_buf + i + 1))) {
      nodes[i * 4 + 1] = -1;
    } else {
      nodes[i * 4 + 1] = i + 1;
    }

    if (i - (int)m->width < 0 ||
        !strchr(WALKABLE, *(m->map_buf + i - m->width))) {
      nodes[i * 4 + 2] = -1;
    } else {
      nodes[i * 4 + 2] = i - m->width;
    }

    if (i + (int)m->width >= len ||
        !strchr(WALKABLE, *(m->map_buf + i + m->width))) {
      nodes[i * 4 + 3] = -1;
    } else {
      nodes[i * 4 + 3] = i + m->width;
    }
  }

  if (dr_pathfinder_uw(pred, nodes, len, dr_point2d_index(src, m->width),
                       dr_point2d_index(dest, m->width)) < 0) {
      free(pred);
      free(nodes);
    return -1;
  }
  i = dr_point2d_index(dest, m->width);
  while (i != src_index) {
    cosmic_list_insert(l, 0, COSMIC_ANY_L(i));
    i = pred[i];
  }

  free(pred);
  free(nodes);

  return 0;
}
