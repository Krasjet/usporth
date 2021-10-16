/* line.c: line segment */
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>

typedef struct {
  usp_flt curr;
  usp_flt step;
  int init;
  unsigned long nsteps;
} ugen_line;

ugen_status
ugen_line_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_line* self;
  usp_flt a, b, dur;

  *pugen = self = xcalloc(1, sizeof(ugen_line));

  b = usp_pop_flt(ctx);
  dur = usp_pop_flt(ctx);
  a = usp_pop_flt(ctx);
  usp_pop_flt(ctx); /* trigger */

  self->nsteps = dur * ctx->sr;
  self->step = (b-a)/self->nsteps;
  self->curr = a;
  self->init = 1;

  return UGEN_OK;
}

ugen_status
ugen_line_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_line* self = ugen;
  usp_flt a, b, dur, trig;
  usp_flt curr, step;
  unsigned long nsteps;

  curr = self->curr;
  step = self->step;
  nsteps = self->nsteps;

  b = usp_pop_flt(ctx);
  dur = usp_pop_flt(ctx);
  a = usp_pop_flt(ctx);
  trig = usp_pop_flt(ctx);

  if (nsteps <= 1)  /* for the final step, go directly to b */
    curr = b;
  else {
    /* do decrement before trigger check to keep first sample = a */
    nsteps--;
    curr += step;
  }

  if (trig != 0) { /* triggered, reinit */
    nsteps = dur*ctx->sr;
    step = (b-a)/nsteps;
    curr = a;
    self->init = 0;
  }

  if (self->init) /* not triggered yet, output a */
    curr = a;

  usp_push_flt(ctx, curr);

  self->curr = curr;
  self->step = step;
  self->nsteps = nsteps;

  return UGEN_OK;
}

void
ugen_line_free(ugen_instance ugen)
{
  free(ugen);
}
