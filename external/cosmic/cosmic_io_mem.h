#ifndef COSMIC_IO_MEM_H
#define COSMIC_IO_MEM_H

#include <stdlib.h>
#include <string.h>

#include <cosmic/cosmic_io.h>
#include <cosmic/cosmic_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Does not take ownership of char*
 * Will not attempt to free on io_mem_free
 */
COSMIC_DLL cosmic_io_t *cosmic_io_mem_new(char *, size_t);
COSMIC_DLL void cosmic_io_mem_free(cosmic_io_t *);
COSMIC_DLL ssize_t cosmic_io_mem_rsetpos(cosmic_io_t *, size_t);
COSMIC_DLL ssize_t cosmic_io_mem_wsetpos(cosmic_io_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
