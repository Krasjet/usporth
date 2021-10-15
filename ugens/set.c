/* set.c: set or create variable */
#include <stdlib.h>
#include <stdio.h>
#include <usporth.h>
#include <usputil.h>
#include "ext.h"

typedef struct {
  var *v;
} ugen_set;

ugen_status
ugen_set_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_set* self;
  const char *name;
  usp_flt val;

  *pugen = self = xcalloc(1, sizeof(ugen_set));

  name = usp_pop_str(ctx);
  val = usp_pop_flt(ctx);

  self->v = var_find(ctx, name);
  if (!self->v) /* variable not defined, create new one */
    self->v = var_new(ctx, name, val);

  return UGEN_OK;
}

ugen_status
ugen_set_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_set* self = ugen;
  usp_flt val;

  val = usp_pop_flt(ctx);
  self->v->val = val;

  return UGEN_OK;
}

void
ugen_set_free(ugen_instance ugen)
{
  free(ugen);
}
