#include <dr_argparser.h>
#include <dr_map.h>
#include <dr_pathfinder.h>
#include <dr_monster.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

void render_map(dr_map_t* m) {
  dr_point2d_t p;
  char c;
  unsigned int len = m->width * m->height, i;

  for (i = 0; i < len; i++) {
    c = *(m->map_buf + i);

    dr_point2d_set(&p, i % m->width, i / m->width);
    mvaddch(p.y, p.x, c);
  }
}

void draw_person(dr_point2d_t cur) { mvaddch(cur.y, cur.x, '@'); }

void draw_monster(dr_monster_t* m) { mvaddch(m->pt.y, m->pt.x, 'M'); }

void draw_path(dr_map_t* m, cosmic_list_t* l) {
  unsigned int i;
  long index;
  dr_point2d_t pt;

  ssize_t len = cosmic_list_size(l);

  for (i = 0; i < len; i++) {
    index = cosmic_list_get_tt(l, i, long);
    dr_point2d_set(&pt, index % m->width, index / m->width);
    mvaddch(pt.y, pt.x, '*');
  }
}

void init_path(dr_map_t* m, dr_point2d_t src, cosmic_list_t** l,
  dr_point2d_t dest) {
  cosmic_vector_free(*l, NULL);
  *l = cosmic_vector_new(10);
  dr_find_path(m, *l, src, dest);
}

void set_init_pt(dr_map_t* m, dr_point2d_t* p) {
  dr_room_t* r = NULL;
  cosmic_list_get(m->room_list, 0, PCOSMIC_ANY(&r));
  p->x = r->rect.x + rand() % r->rect.w;
  p->y = r->rect.y + rand() % r->rect.h;
}

void move_monster(dr_map_t* m, dr_monster_t* monster) {
  cosmic_any_t index;
  if (cosmic_list_pop(monster->curpath, &index) < 0) {
    return;
  }
  dr_point2d_set(&monster->pt, index.l % m->width, index.l / m->width);
}

void init_monster(dr_map_t* map, dr_monster_t* m, dr_point2d_t cur) {
  set_init_pt(map, &m->pt);
  m->curpath = cosmic_vector_new(10);
  init_path(map, m->pt, &m->curpath, cur);
}

int main() {
  srand(time(0));
  double width = 150, height = 40;
  dr_map_t* map = NULL;
  dr_point2d_t cur, mv, dest;
  cosmic_list_t* monster_list = cosmic_vector_new(1);
  int xdir, ydir;
  int camera_drag = 0;
  cosmic_list_t* path = cosmic_vector_new(10);
  int c;
  MEVENT event;

  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  map = dr_map_new(width, height, 5, 5);

  set_init_pt(map, &cur);

  for (int i = 0; i < 5; i++) {
    dr_monster_t* m = (dr_monster_t*)malloc(sizeof(dr_monster_t));
    init_monster(map, m, cur);
    cosmic_list_add(monster_list, COSMIC_ANY(m));
  }

  do {
    xdir = ydir = 0;
    render_map(map);
    draw_person(cur);
    for (int i = 0; i < cosmic_list_size(monster_list); i++) {
      dr_monster_t* m = cosmic_list_get_tt(monster_list, i, dr_monster_t*);
      draw_monster(m);
    }
    draw_path(map, path);
    refresh();

    c = mvgetch(cur.y, cur.x);
    switch (c) {
    case KEY_MOUSE:
      if (getmouse(&event) == OK) { /* When the user clicks left mouse button */
        if (event.bstate & (BUTTON1_CLICKED | BUTTON1_PRESSED)) {
          dr_point2d_set(&dest, event.x, event.y);
          mvprintw(map->height + 1, 0, "Mouse");
          init_path(map, cur, &path, dest);
          mvprintw(map->height + 2, 0, "After");
        }
      }
      break;
    case KEY_LEFT:
    case 'a':
      xdir = -1;
      break;
    case KEY_RIGHT:
    case 'd':
      xdir = 1;
      break;
    case KEY_DOWN:
    case 's':
      ydir = 1;
      break;
    case KEY_UP:
    case 'w':
      ydir = -1;
      break;
    }

    if (c == 'q') {
      break;
    }

    for (int i = 0; i < cosmic_list_size(monster_list); i++) {
      dr_monster_t* m = cosmic_list_get_tt(monster_list, i, dr_monster_t*);
      move_monster(map, m);
    }

    if (xdir == 0 && ydir == 0) {
      continue;
    }

    dr_point2d_set(&mv, cur.x + xdir, cur.y + ydir);
    if (dr_map_check_move(map, mv)) {
      cur.x += xdir;
      cur.y += ydir;
    }

    for (int i = 0; i < cosmic_list_size(monster_list); i++) {
      dr_monster_t* m = cosmic_list_get_tt(monster_list, i, dr_monster_t*);
      init_path(map, m->pt, &m->curpath, cur);
    }

  } while (1);

  endwin();
  dr_map_free(map);

  return 0;
}
