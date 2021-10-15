/* var.c: variables */
#include <string.h>
#include <stdlib.h>
#include <usporth.h>
#include <usputil.h>
#include "ext.h"

var *
var_new(usp_ctx *ctx, const char *name, usp_flt val)
{
  var *v = xcalloc(1, sizeof(var));
  v->name = name;
  v->val = val;
  /* add to ctx */
  v->next = ctx->ext[EXT_VAR];
  ctx->ext[EXT_VAR] = v;
  return v;
}

var *
var_find(usp_ctx *ctx, const char *name)
{
  var *curr = ctx->ext[EXT_VAR];
  while (curr) {
    if (strcmp(name, curr->name) == 0)
      return curr;
    curr = curr->next;
  }
  return NULL;
}

void
var_free(var *v)
{
  free(v);
}
