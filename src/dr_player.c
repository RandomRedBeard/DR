#include "dr_player.h"

dr_player_t *dr_player_new()
{
    dr_player_t *player = malloc(sizeof(dr_player_t));
    player->inventory = cosmic_vector_new(10);
    player->mhand = player->ohand = NULL;
    return player;
}

void dr_player_free(dr_player_t *player)
{
    cosmic_vector_free(player->inventory, (cosmic_list_dealloc)dr_item_free);
    free(player);
}