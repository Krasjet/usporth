/* metro.c: generate clock signal (impulse train) */
#include <stdlib.h>
#include <usporth.h>

typedef struct {
  usp_flt phs;
  usp_flt invsr;
  int init;
} ugen_metro;

ugen_status
ugen_metro_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_metro* metro;

  metro = calloc(1, sizeof(ugen_metro));
  if (!metro)
    return UGEN_ERR;
  metro->phs = 0;
  metro->invsr = 1/ctx->sr;
  metro->init = 1;
  *pugen = metro;

  usp_pop_flt(ctx); /* freq */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_metro_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_metro* metro = ugen;
  usp_flt phs, freq, out = 0;

  phs = metro->phs;
  freq = usp_pop_flt(ctx);

  if (metro->init) { /* trigger on first sample */
    out = 1;
    metro->init = 0;
  } else {
    phs += freq * metro->invsr;
    if (phs >= 1) {
      out = 1;
      phs -= 1.0;
    }
  }
  usp_push_flt(ctx, out);
  metro->phs = phs;

  return UGEN_OK;
}

void
ugen_metro_free(ugen_instance ugen)
{
  free(ugen);
}
