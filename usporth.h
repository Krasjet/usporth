/* usporth.h: a minimal rewrite of sporth core */
#ifndef USPORTH_H
#define USPORTH_H
#include <stddef.h>

/* data type for floats */
typedef float usp_flt;
/* cast any constant floats to native type. essential for
 * speed since float conversions cannot be optimized out */
#define FL(x) ((usp_flt)(x))
/* float functions */
#define strtoflt strtof
#define sinflt sinf

/* data type for stack values */
typedef enum usp_valtype {
  VAL_FLOAT, VAL_STRING
} usp_valtype;
typedef struct usp_val {
  union { usp_flt f; const char *s; } val;
  usp_valtype type;
} usp_val;

#define USP_STACK_SIZE 32
/* runtime context for usporth that can be
 * queried by ugens */
typedef struct usp_ctx {
  /* sample rate */
  usp_flt sr;
  /* TODO: ftmap, reg */

  /* stack */
  size_t stack_top;
  usp_val stack[USP_STACK_SIZE];
} usp_ctx;

/* stack operations */
void usp_push_flt(usp_ctx *ctx, usp_flt f);
usp_flt usp_pop_flt(usp_ctx *ctx);
void usp_push_str(usp_ctx *ctx, const char *s);
const char *usp_pop_str(usp_ctx *ctx);

typedef enum {
  UGEN_ERR = 0, UGEN_OK = 1
} ugen_status;
/* handle for ugen's internal state */
typedef void * ugen_instance;
typedef struct usp_ugen {
  const char *name;
  ugen_status (*init)(usp_ctx *, ugen_instance *);
  ugen_status (*tick)(usp_ctx *, ugen_instance);
  void (*free)(ugen_instance);
} usp_ugen;

/* ugen mapping, can be overwritten */
extern usp_ugen usp_ugens[];
/* ugen indices */
enum {
#define USPORTH_UGEN(name, macro, f_init, f_tick, f_free) macro,
#include "ugens.h"
#undef USPORTH_UGEN
  UGEN_SIZE
};

/* pipe is an *instance* of an element in usporth code that
 * can pump out values repeatedly
      1 2 *
   =>
      +f--+  +f--+  +u--+
      | 1 +=>| 2 +=>| * +=>
      +---+  +---+  +---+        */
typedef enum pipe_type {
  PIPE_FLOAT, PIPE_STRING, PIPE_UGEN
} pipe_type;
typedef struct usp_pipe {
  pipe_type type;
  union {
    struct {
      size_t index;
      ugen_instance handle;
    } u;
    struct str* s;
    usp_flt f;
  } data;
  struct usp_pipe *next;
} usp_pipe;

/* append pipe to the end of the pipeline. returns new head */
usp_pipe *pipes_append(usp_pipe *head, usp_pipe *pipe);
/* run pipeline in sequence */
void pipes_init(usp_ctx *ctx, usp_pipe *head);
void pipes_tick(usp_ctx *ctx, usp_pipe *head);
void pipes_free(usp_pipe *head);

/* init runtime context */
void usporth_init_ctx(usp_ctx *ctx, int sr);
/* evaluate source code to construct pipeline */
usp_pipe *usporth_eval(const char *src);

#endif
