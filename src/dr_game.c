#include "dr_game.h"

dr_game_t *dr_game_new(unsigned int w, unsigned int h, unsigned int n, unsigned int m, unsigned int mcnt, dr_player_t *player, int message_limit)
{
    dr_game_t *game = malloc(sizeof(dr_game_t));
    game->w = w;
    game->h = h;
    game->n = n;
    game->m = m;
    game->mcnt = mcnt;
    game->player = player;
    game->message_limit = message_limit;
    game->level = NULL;

    game->messages = cosmic_vector_new(message_limit);
    return game;
}

void dr_game_add_message(dr_game_t *game, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    char buf[128];
    vsnprintf(buf, 128, fmt, va);
    va_end(va);
    cosmic_any_t rm;

    while (cosmic_list_size(game->messages) >= game->message_limit)
    {
        cosmic_list_pop(game->messages, &rm);
        free(rm.cp);
    }

    cosmic_list_add(game->messages, COSMIC_ANY(strdup(buf)));
}

void dr_game_next_level(dr_game_t *game)
{
    if (game->level)
    {
        dr_level_free(game->level);
    }

    game->level = dr_level_new(game->w, game->h, game->n, game->m, game->mcnt);
    game->level->player = game->player;
    dr_level_rand_point(game->level, &game->player->pt);
    dr_level_set_monsters_path(game->level);
}

void dr_game_free(dr_game_t *game)
{
    dr_level_free(game->level);
    free(game);
}