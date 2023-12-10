#ifndef DR_ARGPARSER_H
#define DR_ARGPARSER_H

#include <stdio.h>
#include <string.h>

#include <cosmic/cosmic_dmap.h>

typedef cosmic_map_t *dr_argparser_t;

enum dr_argparser_arg_type
{
  DR_ARG_FLAG,
  DR_ARG_STRING,
  DR_ARG_DOUBLE,
  DR_ARG_LONG
};

struct dr_argparser_arg
{
  cosmic_any_t o;
  enum dr_argparser_arg_type type;
};

dr_argparser_t dr_argparser_new();
void dr_argparser_free(dr_argparser_t);

ssize_t dr_argparser_add_flag(dr_argparser_t, const char *, int *);
ssize_t dr_argparser_add_string(dr_argparser_t, const char *, char **);
ssize_t dr_argparser_add_double(dr_argparser_t, const char *, double *);
ssize_t dr_argparser_add_long(dr_argparser_t, const char *, long *);

ssize_t dr_argparser_parse(dr_argparser_t, int, char **);

#endif
