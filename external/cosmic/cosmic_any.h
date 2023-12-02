#ifndef COSMIC_ANY_H
#define COSMIC_ANY_H

#include <cosmic/cosmic_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COSMIC_VOID COSMIC_ANY(NULL)

/* So many poor choices */
#ifdef _WIN32
typedef long long CLONG;
typedef long double CDOUBLE;
typedef unsigned long long CULONG;
#else
typedef long CLONG;
typedef double CDOUBLE;
typedef unsigned long CULONG;
#endif

typedef union cosmic_any {
  CDOUBLE d;
  CLONG l;
  CULONG ul;
  char *cp;
  void *vp;
  const void *cvp;
} cosmic_any_t;

typedef struct cosmic_pair {
  cosmic_any_t k, v;
} cosmic_pair_t;

/**`
 * Wrapper for cmp func
 */
typedef int (*cosmic_cmp)(const cosmic_any_t, const cosmic_any_t);

COSMIC_DLL cosmic_any_t COSMIC_ANY(void *);
COSMIC_DLL cosmic_any_t COSMIC_ANY_CONST(const void *);
COSMIC_DLL cosmic_any_t *PCOSMIC_ANY(void *);
COSMIC_DLL cosmic_any_t COSMIC_ANY_L(CLONG);
COSMIC_DLL cosmic_any_t COSMIC_ANY_D(CDOUBLE);
COSMIC_DLL cosmic_any_t COSMIC_ANY_UL(CULONG);

COSMIC_DLL cosmic_pair_t COSMIC_PAIR(cosmic_any_t, cosmic_any_t);

/**
 * Namespacing for verbosity
 */

#ifdef USING_NAMESPACE_COSMIC
typedef cosmic_any_t any_t;
typedef cosmic_pair_t pair_t;
typedef cosmic_cmp cmp;
#endif

#ifdef __cplusplus
}
#endif

#endif
