#include <cosmic/cosmic_io_fs.h>
#include <cosmic/cosmic_socket.h>
#include <dr_argparser.h>
#include <dr_log.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  char *host = NULL;
  char *port = NULL;
  char buffer[128] = {0};
  int client = 0;
  double listen_backlog = 0;
  cosmic_io_t *s = NULL, *child = NULL, *sin = NULL;

  dr_argparser_t da = dr_argparser_new();

  dr_init_stdout_log(INFO);

  dr_argparser_add_string(da, "host", &host);
  dr_argparser_add_string(da, "port", &port);
  dr_argparser_add_flag(da, "client", &client);
  dr_argparser_add_number(da, "lback", &listen_backlog);
  dr_argparser_parse(da, argc, argv);

  dr_argparser_free(da);

  s = cosmic_socket_new();
  if (client) {
    if (cosmic_socket_connect_to_host(s, host, port) != 0) {
      dr_log(ERROR, errno, "Failed to connect %s - %s", host, port);
      exit(1);
    }
  } else {
    if (cosmic_socket_bind_to_host(s, host, port) != 0) {
      dr_log(ERROR, errno, "Failed to bind %s - %s", host, port);
      exit(1);
    }

    if (cosmic_socket_listen(s, listen_backlog) != 0) {
      dr_log(ERROR, errno, "Failed to listen %s - %s", host, port);
      exit(1);
    }

    if (cosmic_socket_wpoll(s) < 0) {
      dr_log(ERROR, errno, "Failed to poll\n");
      exit(1);
    }

    if ((child = cosmic_socket_accept(s)) == NULL) {
      dr_log(ERROR, errno, "Failed to accept %s - %s", host, port);
    }
  }

  sin = cosmic_io_fs_new(1);

  if (client) {
    cosmic_io_read(sin, buffer, 127);
    cosmic_io_write(s, buffer, strlen(buffer) - 1);
  } else {
    cosmic_io_read(child, buffer, 127);
    dr_log(INFO, 0, "Read \"%s\" from client", buffer);
  }

  cosmic_io_fs_free(sin);

  cosmic_socket_shutdown(s, O_RDWR);
  cosmic_io_close(s);

  cosmic_socket_free(s);

  dr_shutdown_log();

  return 0;
}
