#include "dr_game.h"

dr_game_t *dr_game_new(unsigned int w, unsigned int h, unsigned int n, unsigned int m, unsigned int mcnt, dr_player_t *player)
{
    dr_game_t *game = malloc(sizeof(dr_game_t));
    game->w = w;
    game->h = h;
    game->n = n;
    game->m = m;
    game->mcnt = mcnt;
    game->player = player;
    game->level = NULL;
}

void dr_game_next_level(dr_game_t *game)
{
    if (game->level)
    {
        dr_level_free(game->level);
    }

    game->level = dr_level_new(game->w, game->h, game->n, game->m, game->mcnt);
    dr_level_rand_point(game->level, &game->player->pt);
    dr_game_set_monsters_path(game);
}

void dr_game_move_monster(dr_level_t *level, dr_monster_t *monster, dr_player_t *player)
{
    cosmic_any_t index;
    dr_point2d_t pt;
    if (cosmic_list_pop(monster->curpath, &index) < 0)
    {
        return;
    }
    dr_point2d_set(&pt, index.l % level->map->width, index.l / level->map->width);
    if (dr_level_check_move(level, pt) && !dr_point2d_eq(player->pt, pt))
    {
        monster->pt = pt;
    }
}

void dr_game_move_monsters(dr_game_t *game)
{
    int len = cosmic_list_size(game->level->monster_list);
    for (int i = 0; i < len; i++)
    {
        dr_monster_t *monster = cosmic_list_get_tt(game->level->monster_list, i, dr_monster_t *);
        dr_game_move_monster(game->level, monster, game->player);
    }
}

void dr_game_init_path(dr_game_t *game, dr_point2d_t src, cosmic_list_t **l,
                       dr_point2d_t dest)
{
    if (*l)
    {
        cosmic_vector_free(*l, NULL);
    }
    *l = cosmic_vector_new(10);
    dr_find_path(game->level->map, *l, src, dest);
}

void dr_game_set_monsters_path(dr_game_t *game)
{
    int len = cosmic_list_size(game->level->monster_list);
    for (int i = 0; i < len; i++)
    {
        dr_monster_t *monster = cosmic_list_get_tt(game->level->monster_list, i, dr_monster_t *);
        dr_game_init_path(game, monster->pt, &monster->curpath, game->player->pt);
    }
}

void dr_game_free(dr_game_t *game)
{
    dr_level_free(game->level);
    free(game);
}