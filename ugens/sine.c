/* sine.c: sine oscillator */
#include <math.h>
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>
#ifndef M_PI
#define M_PI FL(3.14159265358979323846)
#endif

typedef struct {
  usp_flt phs;
  usp_flt invsr;
} ugen_sine;

ugen_status
ugen_sine_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_sine* self;

  *pugen = self = xcalloc(1, sizeof(ugen_sine));
  self->phs = 0;
  self->invsr = 1/ctx->sr;

  usp_pop_flt(ctx); /* freq */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_sine_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_sine* self = ugen;
  usp_flt phs, freq;

  phs = self->phs;
  freq = usp_pop_flt(ctx);
  usp_push_flt(ctx, sinflt(2*M_PI*phs));

  phs += freq * self->invsr;
  while (phs >= 1) phs--;
  while (phs < 0) phs++;
  self->phs = phs;

  return UGEN_OK;
}

void
ugen_sine_free(ugen_instance ugen)
{
  free(ugen);
}
