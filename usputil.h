/* util.h: utility functions */
#ifndef USPUTIL_H
#define USPUTIL_H
#include <stddef.h>

void die(const char *msg, ...);

/* die when out of memory */
void *xcalloc(size_t n, size_t s);
void *xrealloc(void *p, size_t s);

/* simple dynamic string */
struct str {
  char *str;
  size_t len, capacity;
};
struct str *str_init(void);
void str_free(struct str *s);
void str_append(struct str *s, char ch);

#endif
