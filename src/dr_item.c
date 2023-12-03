#include "dr_item.h"

void dr_item_free(dr_item_t* item) {
    item->dealloc(item);
}