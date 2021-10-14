/* adsr.c: adsr envelope (gate input) */
#include <stdlib.h>
#include <math.h>
#include <usporth.h>
#include <float.h>

/* flush denormal */
#define FLUSH(x) (x = fabs(x)<((double)FLT_MIN) ? 0 : x)

typedef enum {
  IDLE, ATTACK, DECAY, SUSTAIN, RELEASE
} State;

typedef struct {
  /* all are states from previous sample */
  State state;
  usp_flt gate;
  /* internal processing is done in 64-bit. unity gain one
   * pole filter with pole near 1 has a very bad round-off
   * error. using 32-bit resolution will cause large
   * transition time to be inaccurate. */
  double out;
  double target;
  /**
   * out = (1-pole)*target +pole*out
   *       |-------------|
   *            base
   */
  double pole;
  double base;
} ugen_adsr;

/* larger eps => more linear (dis)charging curve, can be
 * made into a parameter */
#define eps .001
/* attack needs to be cut off earlier to avoid the flat
 * slope later in the transition */
#define atk_eps .29

/* pole needed to reach ratio of jump in t sec */
static double
ratio2pole(double t, double ratio, double sr)
{
  return pow(ratio, 1/(t*sr));
}

ugen_status
ugen_adsr_init(usp_ctx *ctx, ugen_instance *pugen)
{
  ugen_adsr* self;

  self = calloc(1, sizeof(ugen_adsr));
  if (!self)
    return UGEN_ERR;
  self->state = IDLE;
  self->gate = 0;
  self->out = 0;
  self->target = 0;
  *pugen = self;

  usp_pop_flt(ctx); /* rel */
  usp_pop_flt(ctx); /* sus */
  usp_pop_flt(ctx); /* decay */
  usp_pop_flt(ctx); /* atk */
  usp_pop_flt(ctx); /* gate */
  usp_push_flt(ctx, 0);

  return UGEN_OK;
}

ugen_status
ugen_adsr_tick(usp_ctx *ctx, ugen_instance ugen)
{
  ugen_adsr* self = ugen;
  State state;
  usp_flt gate_old, gate;
  double atk, sus, decay, rel;
  double target, pole, base, sr, out;

  state = self->state;
  gate_old = self->gate;
  out = self->out;
  target = self->target;
  pole = self->pole;
  base = self->base;
  sr = ctx->sr;

  rel = usp_pop_flt(ctx);
  sus = usp_pop_flt(ctx);
  decay = usp_pop_flt(ctx);
  atk = usp_pop_flt(ctx);
  gate = usp_pop_flt(ctx);

  /* 1. update state based on current gate */
  if (gate > gate_old) {        /* 0 -> 1 */
    state = ATTACK;
    /*      ----- 1+atk_eps (target)
     *      +---- 1         (actual max)
     *     /
     *  --+       0         (min)
     */
    target = 1 + atk_eps;
    pole = ratio2pole(atk, atk_eps/target, sr);
    base = (1-pole)*target;
  } else if (gate < gate_old) { /* 1 -> 0 */
    state = RELEASE;
    /*
     * --+        sus   (max)
     *    \
     *     +----- 0     (actual min)
     *     ------ -eps  (target)
     */
    target = -eps;
    pole = ratio2pole(rel, eps/(sus+eps), sr);
    base = (1-pole)*target;
  }

  /* 2. move towards target */
  out = base + pole*out;
  FLUSH(out);

  /* 3. update based on current state */
  switch (state) {
  case IDLE:
    out = 0;
    break;
  case ATTACK:
    if (out >= 1) {
      out = 1;
      state = DECAY;
      /*
       * --+        1        (max)
       *    \
       *     +----- sus      (actual min)
       *     ------ sus-eps  (target)
       */
      target = sus-eps;
      pole = ratio2pole(decay, eps/(1-sus+eps), sr);
      base = (1-pole)*target;
    }
    break;
  case DECAY:
    if (out <= sus) {
      out = sus;
      state = SUSTAIN;
    }
    break;
  case SUSTAIN:
    out = sus;
    break;
  case RELEASE:
    if (out <= 0) {
      out = 0;
      state = IDLE;
    }
    break;
  }

  usp_push_flt(ctx, out);

  self->state = state;
  self->gate = gate;
  self->out = out;
  self->target = target;
  self->pole = pole;
  self->base = base;

  return UGEN_OK;
}

void
ugen_adsr_free(ugen_instance ugen)
{
  free(ugen);
}
