#ifndef COSMIC_LIB_H
#define COSMIC_LIB_H

#ifdef _WIN32
#ifdef COSMIC_DLL_EXPORT
#define COSMIC_DLL __declspec(dllexport)
#elif defined(COSMIC_STATIC)
#define COSMIC_DLL
#else
#define COSMIC_DLL __declspec(dllimport)
#endif
#else
#define COSMIC_DLL
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#define strdup _strdup
#else
#include <sys/types.h>
extern char *strdup(const char *);
#ifndef snprintf /* OSX defines snprintf */
extern int snprintf(char *, size_t, const char *, ...);
#endif
#endif

#endif
