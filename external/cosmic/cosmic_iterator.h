#ifndef COSMIC_ITERATOR_H
#define COSMIC_ITERATOR_H

#include <cosmic/cosmic_any.h>
#include <cosmic/cosmic_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cosmic_iterator cosmic_iterator_t;

COSMIC_DLL int cosmic_iterator_next(cosmic_iterator_t *, cosmic_any_t *);
COSMIC_DLL int cosmic_iterator_next_pair(cosmic_iterator_t *, cosmic_pair_t *);
COSMIC_DLL int cosmic_iterator_has_next(const cosmic_iterator_t *);
COSMIC_DLL void cosmic_iterator_close(cosmic_iterator_t *);

#ifdef USING_NAMESPACE_COSMIC
typedef cosmic_iterator_t iterator_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
