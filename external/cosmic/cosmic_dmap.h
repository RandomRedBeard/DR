#ifndef COSMIC_DMAP_H
#define COSMIC_DMAP_H

#include <stdlib.h>

#include <cosmic/cosmic_map.h>

#ifdef __cplusplus
extern "C" {
#endif

COSMIC_DLL cosmic_map_t *cosmic_dmap_new(cosmic_cmp);
COSMIC_DLL void cosmic_dmap_free(cosmic_map_t *, cosmic_map_dealloc);

#ifdef __cplusplus
}
#endif

#endif
