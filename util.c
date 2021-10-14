/* util.c: utility functions */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"

void
die(const char *msg, ...)
{
  va_list ap;
  fputs("[error] ", stderr);
  va_start(ap, msg);
    vfprintf(stderr, msg, ap);
  va_end(ap);
  putc('\n', stderr);
  exit(1);
}

void *
xcalloc(size_t n, size_t s)
{
  void *p = calloc(n, s);
  if (!p)
    die("out of memory");
  return p;
}

void *
xrealloc(void *p, size_t s)
{
  void *pnew = realloc(p, s);
  if (!pnew)
    die("out of memory");
  return pnew;
}

struct str *
str_init(void)
{
  struct str *s = xcalloc(1, sizeof(struct str));
  s->str = xcalloc(16, 1);
  s->capacity = 16;
  s->len = 0;
  return s;
}

void
str_free(struct str *s)
{
  if (!s) return;
  free(s->str);
  free(s);
}

void
str_append(struct str *s, char ch)
{
  if (s->capacity <= s->len + 1) {
    s->capacity *= 2;
    s->str = xrealloc(s->str, s->capacity);
  }
  s->str[s->len++] = ch;
  s->str[s->len] = '\0';
}
