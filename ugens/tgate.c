/* tgate.c: trigger gate signal */
#include <stdlib.h>
#include <usporth.h>

typedef struct {
  unsigned long countdown;
} ugen_tgate;

ugen_status
ugen_tgate_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_tgate* tgate;

  tgate = calloc(1, sizeof(ugen_tgate));
  if (!tgate)
    return UGEN_ERR;
  tgate->countdown = 0;
  *pugen = tgate;

  usp_pop_flt(ctx); /* time */
  usp_pop_flt(ctx); /* trig */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_tgate_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_tgate* tgate = ugen;
  usp_flt time, trig, out = 0;

  time = usp_pop_flt(ctx);
  trig = usp_pop_flt(ctx);

  if (trig != 0)
    tgate->countdown = time * ctx->sr;
  if (tgate->countdown != 0) {
    out = 1;
    tgate->countdown--;
  }
  usp_push_flt(ctx, out);

  return UGEN_OK;
}

void
ugen_tgate_free(ugen_instance ugen)
{
  free(ugen);
}
