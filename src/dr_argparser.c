#include "dr_argparser.h"

struct dr_argparser_arg *dr_argparser_arg_new(cosmic_any_t o,
                                              enum dr_argparser_arg_type type) {
  struct dr_argparser_arg *arg = malloc(sizeof(struct dr_argparser_arg));
  arg->o = o;
  arg->type = type;
  return arg;
}

void dr_argparser_arg_free(cosmic_pair_t p) { free(p.v.vp); }

dr_argparser_t dr_argparser_new() {
  return cosmic_dmap_new((cosmic_cmp)strcmp);
}

void dr_argparser_free(dr_argparser_t da) {
  cosmic_dmap_free(da, (cosmic_map_dealloc)dr_argparser_arg_free);
}

ssize_t dr_argparser_add_flag(dr_argparser_t da, const char *k, int *flag) {
  struct dr_argparser_arg *arg =
      dr_argparser_arg_new(COSMIC_ANY(flag), DR_ARG_FLAG);
  if (cosmic_map_add(da, COSMIC_ANY_CONST(k), COSMIC_ANY(arg)) < 0) {
    free(arg);
    return -1;
  }

  return 0;
}

ssize_t dr_argparser_add_string(dr_argparser_t da, const char *k,
                                char **string) {
  struct dr_argparser_arg *arg =
      dr_argparser_arg_new(COSMIC_ANY(string), DR_ARG_STRING);
  if (cosmic_map_add(da, COSMIC_ANY_CONST(k), COSMIC_ANY(arg)) < 0) {
    free(arg);
    return -1;
  }

  return 0;
}

ssize_t dr_argparser_add_double(dr_argparser_t da, const char *k,
                                double *number) {
  struct dr_argparser_arg *arg =
      dr_argparser_arg_new(COSMIC_ANY(number), DR_ARG_DOUBLE);
  if (cosmic_map_add(da, COSMIC_ANY_CONST(k), COSMIC_ANY(arg)) < 0) {
    free(arg);
    return -1;
  }

  return 0;
}

ssize_t dr_argparser_add_long(dr_argparser_t da, const char *k,
                                long *number) {
  struct dr_argparser_arg *arg =
      dr_argparser_arg_new(COSMIC_ANY(number), DR_ARG_LONG);
  if (cosmic_map_add(da, COSMIC_ANY_CONST(k), COSMIC_ANY(arg)) < 0) {
    free(arg);
    return -1;
  }

  return 0;
}

ssize_t dr_argparser_parse(dr_argparser_t da, int argc, char **argv) {
  int i;
  char *arg = NULL;
  struct dr_argparser_arg *darg = NULL;

  for (i = 1; i < argc; i++) {
    arg = *(argv + i);

    if (*arg != '-') {
      continue;
    }

    arg++;
    if (*arg == '-') {
      arg++;
    }

    if (cosmic_map_get_value(da, COSMIC_ANY_CONST(arg), PCOSMIC_ANY(&darg)) <
        0) {
      continue;
    }

    switch (darg->type) {
    case DR_ARG_FLAG:
      *((int *)darg->o.vp) = 1;
      break;
    case DR_ARG_DOUBLE:
      if (i + 1 < argc) {
        *((double *)darg->o.vp) = strtod(*(argv + ++i), NULL);
      }
      break;
    case DR_ARG_LONG:
      if (i + 1 < argc) {
        *((long *)darg->o.vp) = strtol(*(argv + ++i), NULL, 10);
      }
      break;
    case DR_ARG_STRING:
      if (i + 1 < argc) {
        *((char **)darg->o.vp) = *(argv + ++i);
      }
      break;
    }
  }

  return argc;
}
