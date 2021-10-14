/* metro.c: generate clock signal from frequency (impulse train) */
#include <stdlib.h>
#include <usporth.h>

typedef struct {
  usp_flt phs;
  usp_flt invsr;
} ugen_metro;

ugen_status
ugen_metro_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_metro* self;

  self = calloc(1, sizeof(ugen_metro));
  if (!self)
    return UGEN_ERR;
  self->phs = 1; /* to trigger on first sample */
  self->invsr = 1/ctx->sr;
  *pugen = self;

  usp_pop_flt(ctx); /* freq */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_metro_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_metro* self = ugen;
  usp_flt phs, freq, out = 0;

  phs = self->phs;
  freq = usp_pop_flt(ctx);

  if (phs >= 1) {
    out = 1;
    phs -= 1.0;
  }
  phs += freq * self->invsr;
  self->phs = phs;

  usp_push_flt(ctx, out);

  return UGEN_OK;
}

void
ugen_metro_free(ugen_instance ugen)
{
  free(ugen);
}
