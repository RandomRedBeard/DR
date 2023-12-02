#ifndef COSMIC_JSON_H
#define COSMIC_JSON_H

#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <cosmic/cosmic_any.h>
#include <cosmic/cosmic_io.h>
#include <cosmic/cosmic_io_mem.h>
#include <cosmic/cosmic_lib.h>
#include <cosmic/cosmic_llist.h>
#include <cosmic/cosmic_dmap.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COSMIC_NUMBER_BUFFER_LEN 32
#define COSMIC_DEFAULT_STRING_MAX_LEN 1024
#define COSMIC_DEFAULT_FPREC "%.4f"
#define COSMIC_JSON_WHITESPACE " \n\t\r"

typedef struct cosmic_json cosmic_json_t;

enum cosmic_json_type {
  COSMIC_NULL,
  COSMIC_NUMBER,
  COSMIC_STRING,
  COSMIC_BOOL,
  COSMIC_OBJECT,
  COSMIC_LIST,
  COSMIC_ERROR
};

enum cosmic_json_error {
  COSMIC_NONE,
  COSMIC_IO_ERROR,
  COSMIC_UNEXPECTED_CHAR,
  COSMIC_MAX_DEPTH_ERROR
};

typedef struct cosmic_json_error_st {
  enum cosmic_json_error error;
  ssize_t index;
  const char *func_name;
} cosmic_json_error_t;

COSMIC_DLL int cosmic_json_equals(const cosmic_json_t *, const cosmic_json_t *);
COSMIC_DLL enum cosmic_json_type cosmic_json_get_type(const cosmic_json_t *);
/**
 * Only applies to list
 */
COSMIC_DLL ssize_t cosmic_json_size(const cosmic_json_t *);

/**
 * New funcs
 */

COSMIC_DLL cosmic_json_t *cosmic_json_new_null();
COSMIC_DLL cosmic_json_t *cosmic_json_new_number(CDOUBLE);
COSMIC_DLL cosmic_json_t *cosmic_json_new_string(const char *);
COSMIC_DLL cosmic_json_t *cosmic_json_new_bool(CLONG);
COSMIC_DLL cosmic_json_t *cosmic_json_new_object();
COSMIC_DLL cosmic_json_t *cosmic_json_new_list();

COSMIC_DLL cosmic_json_t *cosmic_json_copy(const cosmic_json_t *);

/**
 * Free Func
 */
COSMIC_DLL void cosmic_json_free(cosmic_json_t *);

/**
 * Get raw type
 */

COSMIC_DLL CDOUBLE cosmic_json_get_number(const cosmic_json_t *);
COSMIC_DLL const char *cosmic_json_get_string(const cosmic_json_t *);
COSMIC_DLL CLONG cosmic_json_get_bool(const cosmic_json_t *);
COSMIC_DLL const cosmic_json_error_t *
cosmic_json_get_error(const cosmic_json_t *);
COSMIC_DLL enum cosmic_json_error
cosmic_json_get_error_code(const cosmic_json_t *);

/**
 * Get complex type
 */

COSMIC_DLL const cosmic_json_t *
cosmic_json_get_object_value(const cosmic_json_t *, const char *);
COSMIC_DLL CDOUBLE cosmic_json_get_object_value_n(const cosmic_json_t *,
                                                 const char *);
COSMIC_DLL const char *cosmic_json_get_object_value_s(const cosmic_json_t *,
                                                      const char *);
COSMIC_DLL CLONG cosmic_json_get_object_value_b(const cosmic_json_t *,
                                               const char *);
COSMIC_DLL const cosmic_json_t *
cosmic_json_get_list_value(const cosmic_json_t *, size_t);

/**
 * Set raw type
 */

COSMIC_DLL int cosmic_json_set_number(cosmic_json_t *, CDOUBLE);
COSMIC_DLL int cosmic_json_set_string(cosmic_json_t *, const char *);
COSMIC_DLL int cosmic_json_set_bool(cosmic_json_t *, CLONG);

/**
 * Add key-value pair to object
 * Assumes ownership of json_t*
 */
COSMIC_DLL ssize_t cosmic_json_add_kv(cosmic_json_t *, const char *,
                                      cosmic_json_t *);
COSMIC_DLL ssize_t cosmic_json_add_kv_n(cosmic_json_t *, const char *, CDOUBLE);
COSMIC_DLL ssize_t cosmic_json_add_kv_s(cosmic_json_t *, const char *,
                                        const char *);
COSMIC_DLL ssize_t cosmic_json_add_kv_b(cosmic_json_t *, const char *, CLONG);
/**
 * Add object to list
 * Assumes ownership
 */
COSMIC_DLL ssize_t cosmic_json_add(cosmic_json_t *, cosmic_json_t *);
/**
 * Insert object to list
 * Assumes ownership
 */
COSMIC_DLL ssize_t cosmic_json_insert(cosmic_json_t *, size_t, cosmic_json_t *);

/**
 * Remove key and value from json object
 * If dest is not null, the value will be returned
 * otherwise it will be freed
 */
COSMIC_DLL int cosmic_json_remove_key(cosmic_json_t *, const char *,
                                      cosmic_json_t **);

/**
 * Performs remove and add on json object
 * If dest is not null, the old value is returned
 */
COSMIC_DLL ssize_t cosmic_json_replace_key(cosmic_json_t *, const char *,
                                           cosmic_json_t *, cosmic_json_t **);

/**
 * Remove value from json list
 * if dest is not null, the value will be returned
 * otherwise it will be freed
 */
COSMIC_DLL int cosmic_json_remove(cosmic_json_t *, size_t, cosmic_json_t **);

/**
 * Iterator is either map or list iterator
 * If type is map, pair type is {char*, cosmic_json_t*}
 */
COSMIC_DLL cosmic_iterator_t *cosmic_json_iterator(const cosmic_json_t *);

/**
 * Parsing and Writing functions
 */

#define cosmic_json_write_stream(j, io, indent)                                \
  cosmic_json_write_stream_s(j, io, indent, COSMIC_DEFAULT_FPREC)
COSMIC_DLL ssize_t cosmic_json_write_stream_s(const cosmic_json_t *,
                                              cosmic_io_t *, const char *,
                                              const char *);

/**
 * write_stream(io_mem_new(buf, len));
 */
#define cosmic_json_write_buffer(j, buf, len, indent)                          \
  cosmic_json_write_buffer_s(j, buf, len, indent, COSMIC_DEFAULT_FPREC)
COSMIC_DLL ssize_t cosmic_json_write_buffer_s(const cosmic_json_t *, char *,
                                              size_t, const char *,
                                              const char *);

#define cosmic_json_read_stream(io, max_depth)                                 \
  cosmic_json_read_stream_s(io, max_depth, COSMIC_DEFAULT_STRING_MAX_LEN)
COSMIC_DLL cosmic_json_t *cosmic_json_read_stream_s(cosmic_io_t *, size_t,
                                                    size_t);

/**
 * read_stream(io_mem_new(buf, len))
 */
#define cosmic_json_read_buffer(buf, len, max_depth)                           \
  cosmic_json_read_buffer_s(buf, len, max_depth, COSMIC_DEFAULT_STRING_MAX_LEN)
COSMIC_DLL cosmic_json_t *cosmic_json_read_buffer_s(char *, size_t, size_t,
                                                    size_t);

#ifdef USING_NAMESPACE_COSMIC
typedef cosmic_json_t json_t;
typedef cosmic_json_error_t json_error_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
