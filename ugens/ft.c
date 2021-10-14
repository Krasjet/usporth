/* ft.c: function tables */
#include <string.h>
#include <stdlib.h>
#include <usporth.h>
#include <util.h>
#include "ext.h"

ftmap *
ftmap_new(usp_ctx *ctx, const char *name, size_t cap)
{
  ftmap *ft = xcalloc(1, sizeof(ftmap));
  ft->name = name;
  ft->len = 0;
  if (cap <= 0)
    cap = 8;
  ft->capacity = cap;
  ft->tbl = xcalloc(cap, sizeof(usp_flt));
  /* add to ctx */
  ft->next = ctx->ext[EXT_FTMAP];
  ctx->ext[EXT_FTMAP] = ft;
  return ft;
}

void
ftmap_push(ftmap *ft, usp_flt val)
{
  if (ft->capacity < ft->len + 1) {
    ft->capacity *= 2;
    ft->tbl = xrealloc(ft->tbl, ft->capacity);
  }
  ft->tbl[ft->len++] = val;
}

ftmap *
ftmap_find(usp_ctx *ctx, const char *name)
{
  ftmap *curr = ctx->ext[EXT_FTMAP];
  while (curr) {
    if (strcmp(name, curr->name) == 0)
      return curr;
    curr = curr->next;
  }
  return NULL;
}

void
ftmap_free(ftmap *ft)
{
  if (!ft)
    return;
  free(ft->tbl);
  free(ft);
}
