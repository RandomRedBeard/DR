#ifndef COSMIC_LLIST_H
#define COSMIC_LLIST_H

#include <stdlib.h>

#include <cosmic/cosmic_list.h>

#ifdef __cplusplus
extern "C" {
#endif

COSMIC_DLL cosmic_list_t *cosmic_llist_new();
COSMIC_DLL void cosmic_llist_free(cosmic_list_t *, cosmic_list_dealloc);

#ifdef __cplusplus
}
#endif

#endif
