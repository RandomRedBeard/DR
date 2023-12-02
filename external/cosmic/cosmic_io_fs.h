#ifndef COSMIC_IO_FS_H
#define COSMIC_IO_FS_H

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define read(fd, buf, len) _read(fd, buf, (unsigned int)len)
#define write(fd, buf, len) _write(fd, buf, (unsigned int)len)
#define close(fd) _close(fd)
#else
#include <unistd.h>
#endif

#include <cosmic/cosmic_io.h>
#include <cosmic/cosmic_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

COSMIC_DLL cosmic_io_t *cosmic_io_fs_new(int);
COSMIC_DLL void cosmic_io_fs_free(cosmic_io_t *);

#ifdef __cplusplus
}
#endif

#endif
