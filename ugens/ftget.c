/* ftget.c: read from table */
#include <usporth.h>
#include <stdlib.h>
#include <stdio.h>
#include "ext.h"

typedef struct {
  ftmap *ft;
} ugen_ftget;

ugen_status
ugen_ftget_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_ftget* self;
  const char *name;
  size_t index;

  self = calloc(1, sizeof(ugen_ftget));
  if (!self)
    return UGEN_ERR;
  *pugen = self;

  name = usp_pop_str(ctx);
  index = usp_pop_flt(ctx);

  self->ft = ftmap_find(ctx, name);
  if (!self->ft) {
      fprintf(stderr, "[warn] ftget: fail to find table %s\n", name);
      return UGEN_ERR;
  }
  usp_push_flt(ctx, self->ft->tbl[index]);
  return UGEN_OK;
}

ugen_status
ugen_ftget_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_ftget* self = ugen;
  size_t index = usp_pop_flt(ctx);

  usp_push_flt(ctx, self->ft->tbl[index]);
  return UGEN_OK;
}

void
ugen_ftget_free(ugen_instance ugen)
{
  free(ugen);
}
