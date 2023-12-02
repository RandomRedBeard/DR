#include "dr_pathfinder.h"

void dr_pathfinder_visit(char *v, unsigned int index) {
  *(v + (index / 8)) |= (1 << (index % 8));
}

int dr_pathfinder_is_visited(char *v, unsigned int index) {
  return *(v + (index / 8)) & (1 << (index % 8));
}

int dr_pathfinder_uw(int *pred, int *nodes, unsigned int len, unsigned int src,
                     unsigned int dest) {
  unsigned int vlen = (len / 8) + 1;
  unsigned int i;
  CLONG l = 0;
  int n;
  int found = -1;
  char *visited = alloca(vlen);
  cosmic_list_t *queue = cosmic_vector_new(10);

  memset(visited, 0, vlen);

  for (i = 0; i < len; i++) {
    *(pred + i) = -1;
  }

  dr_pathfinder_visit(visited, src);
  cosmic_list_add(queue, COSMIC_ANY_L(src));

  while (cosmic_list_size(queue) > 0) {
    cosmic_list_pop(queue, PCOSMIC_ANY(&l));
    if (l == dest) {
      found = 1;
      break;
    }
    for (i = 0; i < 4; i++) {
      n = nodes[l * 4 + i];
      if (n == -1) {
        continue;
      }
      if (!dr_pathfinder_is_visited(visited, n)) {
        pred[n] = l;
        dr_pathfinder_visit(visited, n);
        cosmic_list_add(queue, COSMIC_ANY_L(n));
      }
    }
  }

  cosmic_vector_free(queue, NULL);

  return found;
}
