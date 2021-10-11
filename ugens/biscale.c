/* biscale.c: map [-1,1] -> [min,max] */
#include <usporth.h>

static usp_flt
biscale(usp_flt in, usp_flt min, usp_flt max)
{
  return min + FL(0.5)*(max-min)*(in+1);
}

ugen_status
ugen_biscale_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt min, max, val;

  max = usp_pop_flt(ctx);
  min = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);
  usp_push_flt(ctx, biscale(val, min, max));

  return UGEN_OK;
}

ugen_status
ugen_biscale_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt min, max, val;

  max = usp_pop_flt(ctx);
  min = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);
  usp_push_flt(ctx, biscale(val, min, max));

  return UGEN_OK;
}

void ugen_biscale_free(ugen_instance ugen) { }
