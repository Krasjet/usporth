/* ftset.c: set table value */
#include <stdlib.h>
#include <stdio.h>
#include <usporth.h>
#include <util.h>
#include "ext.h"

typedef struct {
  ftmap *ft;
} ugen_ftset;

ugen_status
ugen_ftset_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_ftset* self;
  const char *name;
  size_t index;
  usp_flt val;

  *pugen = self = xcalloc(1, sizeof(ugen_ftset));

  name = usp_pop_str(ctx);
  index = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);

  self->ft = ftmap_find(ctx, name);
  if (!self->ft) {
      fprintf(stderr, "[warn] ftset: fail to find table %s\n", name);
      return UGEN_ERR;
  }
  self->ft->tbl[index] = val;
  return UGEN_OK;
}

ugen_status
ugen_ftset_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_ftset* self = ugen;
  size_t index;
  usp_flt val;

  index = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);

  self->ft->tbl[index] = val;
  return UGEN_OK;
}

void
ugen_ftset_free(ugen_instance ugen)
{
  free(ugen);
}
