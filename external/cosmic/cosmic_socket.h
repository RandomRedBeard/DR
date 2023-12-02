#ifndef COSMIC_SOCKET_H
#define COSMIC_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cosmic/cosmic_io.h>
#include <cosmic/cosmic_lib.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#endif

#ifndef SHUT_WR
#define SHUT_WR SD_SEND
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif

#define poll WSAPoll
#else
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Global init of sockets (Thanks Windows) */
COSMIC_DLL int cosmic_socket_init();

COSMIC_DLL cosmic_io_t *cosmic_socket_new();
COSMIC_DLL void cosmic_socket_free(cosmic_io_t *);

/**
 * Connectors
 */

COSMIC_DLL int cosmic_socket_connect(cosmic_io_t *, const struct sockaddr *,
                                     socklen_t);
COSMIC_DLL int cosmic_socket_connect_to_host(cosmic_io_t *, const char *,
                                             const char *);
COSMIC_DLL int cosmic_socket_bind(cosmic_io_t *, const struct sockaddr *,
                                  socklen_t);
COSMIC_DLL int cosmic_socket_bind_to_host(cosmic_io_t *, const char *,
                                          const char *);
COSMIC_DLL int cosmic_socket_listen(cosmic_io_t *, int);
COSMIC_DLL cosmic_io_t *cosmic_socket_accept(cosmic_io_t *);

/**
 * Socket specific functions
 */

COSMIC_DLL int cosmic_socket_rpoll(cosmic_io_t *);
COSMIC_DLL int cosmic_socket_wpoll(cosmic_io_t *);
COSMIC_DLL int cosmic_socket_shutdown(cosmic_io_t *, int);

/**
 * Set underlying io features
 */

COSMIC_DLL int cosmic_socket_set_nonblock(cosmic_io_t *, int);
COSMIC_DLL void cosmic_socket_set_rpwait(cosmic_io_t *, int);
COSMIC_DLL void cosmic_socket_set_wpwait(cosmic_io_t *, int);

/* Return underlying handle */
#ifdef _WIN32
COSMIC_DLL SOCKET cosmic_socket_get_fd(cosmic_io_t *);
#else
COSMIC_DLL int cosmic_socket_get_fd(cosmic_io_t *);
#endif

#ifdef USING_NAMESPACE_COSMIC
typedef cosmic_io_t csocket_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
