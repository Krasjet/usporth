/* tseq.c: trigger controlled sequencer */
#include <stdlib.h>
#include <usporth.h>
#include <stdio.h>
#include "ext.h"

typedef struct {
  ftmap *ft;
  size_t pos;
  usp_flt val;
} ugen_tseq;

ugen_status
ugen_tseq_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_tseq* self;
  const char *name;

  self = calloc(1, sizeof(ugen_tseq));
  if (!self)
    return UGEN_ERR;
  *pugen = self;

  name = usp_pop_str(ctx);
  usp_pop_flt(ctx); /* trig */

  self->pos = -1; /* to start at pos 0 when triggered */
  self->val = 0;
  self->ft = ftmap_find(ctx, name);
  if (!self->ft) {
      fprintf(stderr, "[warn] tseq: fail to find table %s\n", name);
      return UGEN_ERR;
  }

  usp_push_flt(ctx, 0);
  return UGEN_OK;
}

ugen_status
ugen_tseq_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_tseq* self = ugen;
  usp_flt trig, val;
  size_t pos, ftlen;

  val = self->val;
  pos = self->pos;
  ftlen = self->ft->len;

  trig = usp_pop_flt(ctx);

  if (trig != 0) {
    pos++;
    if (pos >= ftlen)
      pos -= ftlen;
    val = self->ft->tbl[pos];
  }
  usp_push_flt(ctx, val);

  self->val = val;
  self->pos = pos;

  return UGEN_OK;
}

void
ugen_tseq_free(ugen_instance ugen)
{
  free(ugen);
}
