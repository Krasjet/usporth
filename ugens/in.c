/* in.c: input (dummy, to be overwritten by frontend) */
#include <usporth.h>

ugen_status
ugen_in_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_push_flt(ctx, 0.0f);
  return UGEN_OK;
}

ugen_status
ugen_in_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_push_flt(ctx, 0.0f);
  return UGEN_OK;
}

void ugen_in_free(ugen_instance ugen) { }
