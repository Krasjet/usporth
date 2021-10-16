/* smoo.c: simple parameter smoothing with one pole filter */
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <usporth.h>
#include <usputil.h>

typedef struct {
  usp_flt mem;
} ugen_smoo;

ugen_status
ugen_smoo_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_smoo* self;

  *pugen = self = xcalloc(1, sizeof(ugen_smoo));
  self->mem = usp_pop_flt(ctx);
  usp_push_flt(ctx, self->mem);

  return UGEN_OK;
}

ugen_status
ugen_smoo_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_smoo* self = ugen;
  usp_flt in, mem;

  mem = self->mem;
  in = usp_pop_flt(ctx);

  mem = FL(0.001)*in + FL(0.999)*mem;

  usp_push_flt(ctx, mem);
  self->mem = mem;

  return UGEN_OK;
}

void
ugen_smoo_free(ugen_instance ugen)
{
  free(ugen);
}
