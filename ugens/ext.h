/* ext.h: extension data shared by ugens */
#ifndef UGEN_EXT_H
#define UGEN_EXT_H
#include <usporth.h>

/* index into ctx.ext */
enum {
  EXT_FTMAP = 0
};

/* ----- function table mapping ----- */
typedef struct ftmap {
  const char *name;
  usp_flt *tbl;
  size_t len;
  size_t capacity;
  struct ftmap *next;
} ftmap;

/* new ftmap and add to context, set cap to 0 if not known */
ftmap *ftmap_new(usp_ctx *ctx, const char *name, size_t cap);
/* usually you would acess tbl directly, but if capacity is
 * not known at init time, this function can be helpful */
void ftmap_push(ftmap *ft, usp_flt val);
/* find ftmap matching name, returns NULL when not found */
ftmap *ftmap_find(usp_ctx *ctx, const char *name);
void ftmap_free(ftmap *ft);

#endif
