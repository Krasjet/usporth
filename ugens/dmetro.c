/* dmetro.c: generate clock signal from duration (impulse train) */
#include <stdlib.h>
#include <usporth.h>

typedef struct {
  unsigned long count;
} ugen_dmetro;

ugen_status
ugen_dmetro_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_dmetro* dmetro;

  dmetro = calloc(1, sizeof(ugen_dmetro));
  if (!dmetro)
    return UGEN_ERR;
  dmetro->count = 0;
  *pugen = dmetro;

  usp_pop_flt(ctx); /* time */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_dmetro_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_dmetro* dmetro = ugen;
  usp_flt time, out = 0;
  unsigned long count;

  count = dmetro->count;
  time = usp_pop_flt(ctx);

  if (count == 0) {
    out = 1;
    count = ctx->sr * time;
  } else {
    count--;
  }

  usp_push_flt(ctx, out);
  dmetro->count = count;

  return UGEN_OK;
}

void
ugen_dmetro_free(ugen_instance ugen)
{
  free(ugen);
}