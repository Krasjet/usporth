/* ftgen.c: generate function table */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <usporth.h>
#include <usputil.h>
#include "ext.h"

typedef struct {
  ftmap *ft;
} ugen_ftgen;

ugen_status
ugen_ftgen_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_ftgen* self;
  const char *name, *start;
  char *end;

  *pugen = self = xcalloc(1, sizeof(ugen_ftgen));

  start = usp_pop_str(ctx);
  name = usp_pop_str(ctx);

  self->ft = ftmap_new(ctx, name, 0);

  for (;;) {
    usp_flt v = strtoflt(start, &end);
    if (start == end)
      break;
    ftmap_push(self->ft, v);
    start = end;
  }
  /* make sure there're no leftover characters to parse */
  while (*end) {
    if (!isspace(*end)) {
      fprintf(stderr, "[error] ftgen: values must only contain numbers\n");
      return UGEN_ERR;
    }
    end++;
  }

  return UGEN_OK;
}

ugen_status
ugen_ftgen_tick(usp_ctx *ctx, ugen_instance ugen)
{
  return UGEN_OK;
}

void
ugen_ftgen_free(ugen_instance ugen)
{
  ugen_ftgen* self = ugen;
  if (!self)
    return;
  ftmap_free(self->ft);
  free(self);
}
