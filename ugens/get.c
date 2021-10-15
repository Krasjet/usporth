/* get.c: get variable value */
#include <stdlib.h>
#include <stdio.h>
#include <usporth.h>
#include <usputil.h>
#include "ext.h"

typedef struct {
  var *v;
} ugen_get;

ugen_status
ugen_get_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_get* self;
  const char *name;

  *pugen = self = xcalloc(1, sizeof(ugen_get));

  name = usp_pop_str(ctx);

  self->v = var_find(ctx, name);
  if (!self->v) {
    fprintf(stderr, "[error] get: fail to find variable %s\n", name);
    return UGEN_ERR;
  }
  usp_push_flt(ctx, self->v->val);

  return UGEN_OK;
}

ugen_status
ugen_get_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_get* self = ugen;
  usp_push_flt(ctx, self->v->val);
  return UGEN_OK;
}

void
ugen_get_free(ugen_instance ugen)
{
  free(ugen);
}
