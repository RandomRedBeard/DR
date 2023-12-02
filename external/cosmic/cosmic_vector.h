#ifndef COSMIC_VECTOR_H
#define COSMIC_VECTOR_H

#include <stdlib.h>

#include <cosmic/cosmic_list.h>

#ifdef __cplusplus
extern "C" {
#endif

COSMIC_DLL cosmic_list_t *cosmic_vector_new(size_t);
COSMIC_DLL void cosmic_vector_free(cosmic_list_t *, cosmic_list_dealloc);

#ifdef __cplusplus
}
#endif

#endif
