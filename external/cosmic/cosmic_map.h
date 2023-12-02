#ifndef COSMIC_MAP_H
#define COSMIC_MAP_H

#include <stdlib.h>

#include <cosmic/cosmic_any.h>
#include <cosmic/cosmic_iterator.h>
#include <cosmic/cosmic_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cosmic_map cosmic_map_t;
typedef void (*cosmic_map_dealloc)(cosmic_pair_t);

COSMIC_DLL ssize_t cosmic_map_size(const cosmic_map_t *);
COSMIC_DLL int cosmic_map_get_value(const cosmic_map_t *, const cosmic_any_t,
                                    cosmic_any_t *);
COSMIC_DLL ssize_t cosmic_map_add(cosmic_map_t *, cosmic_any_t, cosmic_any_t);
COSMIC_DLL int cosmic_map_remove(cosmic_map_t *, const cosmic_any_t,
                                 cosmic_pair_t *);
/**
 * Type is cosmic_pair_t
 */
COSMIC_DLL cosmic_iterator_t *cosmic_map_iterator(const cosmic_map_t *);

#ifdef USING_NAMESPACE_COSMIC
typedef cosmic_map_t map_t;
typedef cosmic_map_dealloc map_dealloc;
#endif

#ifdef __cplusplus
}
#endif

#endif
