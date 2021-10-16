/* at.c: static delay in samples */
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>

typedef struct {
  usp_flt *mem;
  size_t p;
  size_t N;
} ugen_at;

ugen_status
ugen_at_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_at* self;

  *pugen = self = xcalloc(1, sizeof(ugen_at));

  self->N = usp_pop_flt(ctx);
  usp_pop_flt(ctx); /* input */

  self->mem = xcalloc(self->N, sizeof(usp_flt));
  self->p = 0;

  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_at_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_at* self = ugen;
  usp_flt in;

  usp_pop_flt(ctx); /* delay */
  in = usp_pop_flt(ctx);

  if (self->N == 0) { /* special case: no delay */
    usp_push_flt(ctx, in);
    return UGEN_OK;
  }

  usp_push_flt(ctx, self->mem[self->p]);
  self->mem[self->p++] = in;
  if (self->p >= self->N)
    self->p -= self->N;

  return UGEN_OK;
}

void
ugen_at_free(ugen_instance ugen)
{
  ugen_at* self = ugen;
  if (!self)
    return;
  free(self->mem);
  free(self);
}
