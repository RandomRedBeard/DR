#ifndef DR_PATHFINDER_H
#define DR_PATHFINDER_H

#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cosmic/cosmic_vector.h>

#ifdef __cplusplus
extern "C" {
#endif

int dr_pathfinder_uw(int *, int *, unsigned int, unsigned int, unsigned int);

#ifdef __cplusplus
}
#endif

#endif
