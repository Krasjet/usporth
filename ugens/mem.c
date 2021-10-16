/* mem.c: 1 sample static delay */
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>

typedef struct {
  usp_flt mem;
} ugen_mem;

ugen_status
ugen_mem_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_mem* self;

  *pugen = self = xcalloc(1, sizeof(ugen_mem));
  self->mem = 0;

  usp_pop_flt(ctx); /* input */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_mem_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_mem* self = ugen;
  usp_flt mem;

  mem = usp_pop_flt(ctx);
  usp_push_flt(ctx, self->mem);
  self->mem = mem;

  return UGEN_OK;
}

void
ugen_mem_free(ugen_instance ugen)
{
  free(ugen);
}
