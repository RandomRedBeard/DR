#include <dr_argparser.h>
#include <dr_map.h>
#include <dr_pathfinder.h>
#include <dr_monster.h>
#include <dr_player.h>
#include <dr_level.h>
#include <dr_game.h>
#include <stdlib.h>
#include <time.h>

#define NCURSES_MOUSE_VERSION 2
#include <curses.h>

void render_map(dr_map_t *m)
{
  dr_point2d_t p;
  char c;
  unsigned int len = m->width * m->height, i;

  for (i = 0; i < len; i++)
  {
    c = *(m->map_buf + i);

    dr_point2d_set(&p, i % m->width, i / m->width);
    mvaddch(p.y, p.x, c);
  }
}

void draw_person(dr_point2d_t cur) { mvaddch(cur.y, cur.x, '@'); }

void draw_monster(dr_monster_t *m) { mvaddch(m->pt.y, m->pt.x, 'M'); }

void draw_path(dr_map_t *m, cosmic_list_t *l)
{
  unsigned int i;
  long index;
  dr_point2d_t pt;

  ssize_t len = cosmic_list_size(l);

  for (i = 0; i < len; i++)
  {
    index = cosmic_list_get_tt(l, i, long);
    dr_point2d_set(&pt, index % m->width, index / m->width);
    mvaddch(pt.y, pt.x, '*');
  }
}

void draw_monsters(dr_level_t *level)
{
  int len = cosmic_list_size(level->monster_list);
  for (int i = 0; i < len; i++)
  {
    dr_monster_t *monster = cosmic_list_get_tt(level->monster_list, i, dr_monster_t *);
    draw_monster(monster);
  }
}

int main()
{
  srand(time(0));
  dr_player_t player;
  dr_point2d_t dest, mv;
  dr_game_t *game = dr_game_new(80, 20, 3, 3, 5, &player);
  int xdir, ydir;
  int camera_drag = 0;
  cosmic_list_t *path = cosmic_vector_new(10);
  int c;
  MEVENT event;

  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  dr_game_next_level(game);

  do
  {
    xdir = ydir = 0;
    render_map(game->level->map);
    draw_person(player.pt);
    draw_monsters(game->level);
    draw_path(game->level->map, path);
    refresh();

    c = mvgetch(player.pt.y, player.pt.x);
    switch (c)
    {
    case KEY_MOUSE:
      if (getmouse(&event) == OK)
      { /* When the user clicks left mouse button */
        if (event.bstate & (BUTTON1_CLICKED | BUTTON1_PRESSED))
        {
          dr_point2d_set(&dest, event.x, event.y);
          mvprintw(game->level->map->height + 1, 0, "Mouse");
          dr_game_init_path(game, player.pt, &path, dest);
          mvprintw(game->level->map->height + 2, 0, "After");
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

    if (c == 'q')
    {
      break;
    }

    if (c == 'n')
    {
      dr_game_next_level(game);
      continue;
    }

    dr_game_move_monsters(game);

    if (xdir == 0 && ydir == 0)
    {
      continue;
    }

    dr_point2d_set(&mv, player.pt.x + xdir, player.pt.y + ydir);
    if (dr_level_check_move(game->level, mv))
    {
      player.pt.x += xdir;
      player.pt.y += ydir;
    }

    dr_game_set_monsters_path(game);

  } while (1);

  endwin();
  dr_game_free(game);

  return 0;
}
