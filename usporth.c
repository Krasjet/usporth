/* usporth.c: a minimal rewrite of sporth */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "usporth.h"
#include "util.h"

/* declarations for ugens */
#define USPORTH_UGEN(name, macro, f_init, f_tick, f_free) \
  ugen_status f_init(usp_ctx *, ugen_instance *); \
  ugen_status f_tick(usp_ctx *, ugen_instance); \
  void f_free(ugen_instance);
#include "ugens.h"
#undef USPORTH_UGEN

/* define default ugens */
usp_ugen usp_ugens[] = {
#define USPORTH_UGEN(name, macro, f_init, f_tick, f_free) \
  {name, f_init, f_tick, f_free},
#include "ugens.h"
#undef USPORTH_UGEN
};

/* ----------- stack ----------- */

void
usp_push_flt(usp_ctx *ctx, usp_flt f)
{
  ctx->stack[ctx->stack_top].val.f = f;
  ctx->stack[ctx->stack_top].type = VAL_FLOAT;
  ctx->stack_top++;
}

usp_flt
usp_pop_flt(usp_ctx *ctx)
{
  return ctx->stack[--ctx->stack_top].val.f;
}

void
usp_push_str(usp_ctx *ctx, const char *s)
{
  ctx->stack[ctx->stack_top].val.s = s;
  ctx->stack[ctx->stack_top].type = VAL_STRING;
  ctx->stack_top++;
}

const char *
usp_pop_str(usp_ctx *ctx)
{
  return ctx->stack[--ctx->stack_top].val.s;
}

/* ----------- pipes ----------- */

static usp_pipe *
pipe_newflt(usp_flt f)
{
  usp_pipe *pipe = xcalloc(1, sizeof(usp_pipe));
  pipe->type = PIPE_FLOAT;
  pipe->data.f = f;
  pipe->next = NULL;
  return pipe;
}

static usp_pipe *
pipe_newstr(struct str *s)
{
  usp_pipe *pipe = xcalloc(1, sizeof(usp_pipe));
  pipe->type = PIPE_STRING;
  pipe->data.s = s;
  pipe->next = NULL;
  return pipe;
}

static usp_pipe *
pipe_newugen(size_t index)
{
  usp_pipe *pipe = xcalloc(1, sizeof(usp_pipe));
  pipe->type = PIPE_UGEN;
  pipe->data.u.index = index;
  pipe->next = NULL;
  return pipe;
}

static void
pipe_free(usp_pipe *pipe)
{
  if (!pipe)
    return;
  switch (pipe->type) {
  case PIPE_FLOAT:
    free(pipe);
    break;
  case PIPE_STRING:
    str_free(pipe->data.s);
    free(pipe);
    break;
  case PIPE_UGEN:
    usp_ugens[pipe->data.u.index].free(pipe->data.u.handle);
    free(pipe);
    break;
  }
}

usp_pipe *
pipes_append(usp_pipe *head, usp_pipe *pipe)
{
  usp_pipe *curr = head;

  if (!head)
    /* pipe is new head */
    return pipe;

  while (curr->next)
    curr = curr->next;
  curr->next = pipe;

  return head;
}

void
pipes_init(usp_ctx *ctx, usp_pipe *head)
{
  usp_pipe *curr = head;

  while (curr) {
    switch (curr->type) {
    case PIPE_FLOAT:
      usp_push_flt(ctx, curr->data.f);
      break;
    case PIPE_STRING:
      usp_push_str(ctx, curr->data.s->str);
      break;
    case PIPE_UGEN:
      usp_ugens[curr->data.u.index].init(ctx, &curr->data.u.handle);
      break;
    }
    curr = curr->next;
  }
  ctx->stack_top = 0; /* pop any vals on stack */
}

void
pipes_tick(usp_ctx *ctx, usp_pipe *head)
{
  usp_pipe *curr = head;

  while (curr) {
    switch (curr->type) {
    case PIPE_FLOAT:
      usp_push_flt(ctx, curr->data.f);
      break;
    case PIPE_STRING:
      usp_push_str(ctx, curr->data.s->str);
      break;
    case PIPE_UGEN:
      usp_ugens[curr->data.u.index].tick(ctx, curr->data.u.handle);
      break;
    }
    curr = curr->next;
  }
}

void
pipes_free(usp_pipe *head)
{
  usp_pipe *curr = head, *next;

  if (!head)
    return;

  while (curr) {
    next = curr->next;
    pipe_free(curr);
    curr = next;
  }
}

/* ----------- tokenize ----------- */

typedef enum usp_toktype {
  TK_INVALID = -1,
  TK_FLOAT, TK_STRING, TK_UGEN, TK_EOF
} usp_toktype;

typedef struct usp_token {
  usp_toktype type;
  union {
    usp_flt f;
    struct str *s;
    size_t ugen;
    const char *err_msg;
  } val;
} usp_token;

static const char* usp_delims = " \n()\t\r\v\f";

static usp_token
tok_eof(void)
{
  usp_token tk = { .type = TK_EOF };
  return tk;
}

static usp_token
tok_flt(const char **pcurr)
{
  usp_token tk;
  const char *curr = *pcurr;
  char *end_num;

  /* find end of current token */
  char *end_tok = strpbrk(curr, usp_delims);

  /* convert token to float */
  tk.type = TK_FLOAT;
  tk.val.f = strtoflt(curr, &end_num);

  if (end_num != end_tok) {
    /* conversion failed */
    tk.type = TK_INVALID;
    tk.val.err_msg = "misformatted number";
    return tk;
  }
  *pcurr = end_tok; /* update progress */
  return tk;
}

static usp_token
tok_str(const char **pcurr, char delim)
{
  usp_token tk;
  const char *curr = *pcurr + 1; /* skip begin " */

  /* find end delimiter */
  char *end = strchr(curr, delim);
  if (!end) {
    tk.type = TK_INVALID;
    tk.val.err_msg = "missing end delimiter";
    return tk;
  }

  tk.type = TK_STRING;
  tk.val.s = str_init();
  while (*curr && curr < end) {
    /* alllow line continuation with \ at end of line */
    if ( curr[0] == '\\' &&
        (curr[1] == '\n' || curr[1] == '\r')) {
      curr++; /* skip \ */
      while (*curr == '\n' || *curr == '\r')  /* skip linebreak */
        curr++;
      if (curr == end) /* quote at begining of new line */
        break;
    }
    /* end of line but no continuation */
    if (*curr == '\n' || *curr == '\r') {
      tk.type = TK_INVALID;
      str_free(tk.val.s);
      tk.val.err_msg = "missing end delimiter";
    }
    str_append(tk.val.s, *curr);
    curr++;
  }
  *pcurr = curr + 1; /* update progress (skip end ") */
  return tk;
}

static usp_token
tok_word(const char **pcurr)
{
  usp_token tk;
  const char *curr = *pcurr + 1; /* skip _ */

  /* find end of current token */
  char *end = strpbrk(curr, usp_delims);

  tk.type = TK_STRING;
  tk.val.s = str_init();
  while (*curr && curr < end) {
    str_append(tk.val.s, *curr);
    curr++;
  }
  *pcurr = curr; /* update progress */
  return tk;
}

static usp_token
tok_ugen(const char **pcurr)
{
  usp_token tk;
  const char *curr = *pcurr;

  /* find end of current token */
  char *end = strpbrk(curr, usp_delims);
  size_t n = end ? (size_t)(end-curr) : strlen(curr);

  /* find ugen with matching name (binary search) */
  size_t min = 0, max = UGEN_SIZE;
  while (min < max) {
    size_t i = min + (max-min)/2;
    int cmp = strncmp(usp_ugens[i].name, curr, n);
    if (cmp == 0)
      cmp = usp_ugens[i].name[n] - '\0'; /* check ending byte as well */
    if (cmp == 0) {
      tk.type = TK_UGEN;
      tk.val.ugen = i;
      *pcurr = end; /* update progress */
      return tk;
    } else if (cmp < 0)
      min = i + 1;
    else
      max = i;
  }
  tk.type = TK_INVALID;
  tk.val.err_msg = "undefined ugen";
  return tk;
}

static usp_token
next_tok(const char **pcurr)
{
  for (;;) {
    const char *curr = *pcurr;
    switch (curr[0]) {
    case '_':                               /* _word */
      return tok_word(pcurr);
    case '\'': case '"':                    /* "str" */
      return tok_str(pcurr, curr[0]);
    case '0': case '1': case '2': case '3': /* 1.23 */
    case '4': case '5': case '6': case '7':
    case '8': case '9':
      return tok_flt(pcurr);
    case '+': case '-':                     /* -.23 */
      if (isdigit(curr[1]) || curr[1] == '.')
        return tok_flt(pcurr);
      else
        return tok_ugen(pcurr);
    case '.':                               /* .23 */
      if (isdigit(curr[1]))
        return tok_flt(pcurr);
      else
        return tok_ugen(pcurr);
    case '(':  case ')':                    /* spaces */
    case ' ':  case '\t': case '\v':
    case '\n': case '\r': case '\f':
      (*pcurr)++;
      break;
    case '#':                               /* comments */
      while (*curr && *curr != '\n' && *curr != '\r')
        curr++;
      *pcurr = curr;
      break;
    case '\0':                              /* eof */
      return tok_eof();
    default:
      return tok_ugen(pcurr);
    }
  }
}

/* ----------- interface ----------- */

void
usporth_init_ctx(usp_ctx *ctx, int sr)
{
  ctx->stack_top = 0;
  ctx->sr = sr;
}

usp_pipe *
usporth_eval(const char *s)
{
  usp_token tok;
  usp_pipe *pipes = NULL;

  for (tok = next_tok(&s); tok.type != TK_EOF;
       tok = next_tok(&s)) {
    switch(tok.type) {
    case TK_FLOAT:
      pipes = pipes_append(pipes, pipe_newflt(tok.val.f));
      break;
    case TK_STRING:
      pipes = pipes_append(pipes, pipe_newstr(tok.val.s));
      break;
    case TK_UGEN:
      pipes = pipes_append(pipes, pipe_newugen(tok.val.ugen));
      break;
    default:
      fprintf(stderr, "[error] parse: %s\n", tok.val.err_msg);
      return NULL;
    }
  }
  return pipes;
}
