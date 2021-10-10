/* sine.c: sine oscillator */
#include <math.h>
#include <stdlib.h>
#include "usporth.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
  usp_flt phs;
} ugen_sine;

ugen_status
ugen_sine_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_sine* sine;

  sine = calloc(1, sizeof(ugen_sine));
  if (!sine)
    return UGEN_ERR;
  sine->phs = 0;
  *pugen = sine;

  usp_pop_flt(ctx); /* freq */
  usp_push_flt(ctx, 0.0f);

  return UGEN_OK;
}

ugen_status
ugen_sine_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_sine* sine = ugen;
  usp_flt phs, freq;

  phs = sine->phs;
  freq = usp_pop_flt(ctx);
  usp_push_flt(ctx, sinf(2*M_PI*phs));

  phs += freq/ctx->sr;
  while (phs >= 1) phs--;
  while (phs < 0) phs++;
  sine->phs = phs;

  return UGEN_OK;
}

void
ugen_sine_free(ugen_instance ugen)
{
  free(ugen);
}
