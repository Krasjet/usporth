/* scale.c: map [0,1] -> [min,max] */
#include <usporth.h>

static usp_flt
scale(usp_flt in, usp_flt min, usp_flt max)
{
  return min + in*(max-min);
}

ugen_status
ugen_scale_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt min, max, val;

  max = usp_pop_flt(ctx);
  min = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);
  usp_push_flt(ctx, scale(val, min, max));

  return UGEN_OK;
}

ugen_status
ugen_scale_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt min, max, val;

  max = usp_pop_flt(ctx);
  min = usp_pop_flt(ctx);
  val = usp_pop_flt(ctx);
  usp_push_flt(ctx, scale(val, min, max));

  return UGEN_OK;
}

void ugen_scale_free(ugen_instance ugen) { }
