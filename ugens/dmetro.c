/* dmetro.c: generate clock signal (impulse train) from duration */
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>

typedef struct {
  unsigned long count;
} ugen_dmetro;

ugen_status
ugen_dmetro_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_dmetro* self;

  *pugen = self = xcalloc(1, sizeof(ugen_dmetro));
  self->count = 0;

  usp_pop_flt(ctx); /* time */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_dmetro_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_dmetro* self = ugen;
  usp_flt time, out = 0;
  unsigned long count;

  count = self->count;
  time = usp_pop_flt(ctx);

  if (count == 0) {
    out = 1;
    count = ctx->sr * time;
  } else {
    count--;
  }

  usp_push_flt(ctx, out);
  self->count = count;

  return UGEN_OK;
}

void
ugen_dmetro_free(ugen_instance ugen)
{
  free(ugen);
}
