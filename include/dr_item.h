#ifndef DR_ITEM_H
#define DR_ITEM_H

#include <dr_geom.h>

typedef enum
{
    DR_ITEM_TYPE_WEAPON,
    DR_ITEM_TYPE_
} dr_item_type_e;

typedef struct
{
    dr_point2d_t pt;
    const char *name;
    char icon;
    dr_item_type_e type;
    void (*dealloc)(void *);
} dr_item_t;

void dr_item_free(dr_item_t *);

#endif