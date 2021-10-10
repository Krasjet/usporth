/* basic.c: basic ugens */
#include "usporth.h"

/* 1 2 add => 3 */
ugen_status usp_add_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2+v1);
  return UGEN_OK;
}
ugen_status usp_add_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2+v1);
  return UGEN_OK;
}
void usp_add_free(ugen_instance ugen) {  }

/* 1 2 sub => -1 */
ugen_status usp_sub_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2-v1);
  return UGEN_OK;
}
ugen_status usp_sub_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2-v1);
  return UGEN_OK;
}
void usp_sub_free(ugen_instance ugen) {  }

/* 1 2 mul => 2 */
ugen_status usp_mul_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2*v1);
  return UGEN_OK;
}
ugen_status usp_mul_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2*v1);
  return UGEN_OK;
}
void usp_mul_free(ugen_instance ugen) {  }

/* 1 2 div => 0.5 */
ugen_status usp_div_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2/v1);
  return UGEN_OK;
}
ugen_status usp_div_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2/v1);
  return UGEN_OK;
}
void usp_div_free(ugen_instance ugen) {  }

/* 1 2 drop => 1 */
ugen_status usp_drop_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_pop_flt(ctx);
  return UGEN_OK;
}
ugen_status usp_drop_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_pop_flt(ctx);
  return UGEN_OK;
}
void usp_drop_free(ugen_instance ugen) {  }

/* 1 2 3 rot => 2 3 1 */
ugen_status usp_rot_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2, v3;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  v3 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v3);
  return UGEN_OK;
}
ugen_status usp_rot_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2, v3;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  v3 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v3);
  return UGEN_OK;
}
void usp_rot_free(ugen_instance ugen) {  }

/* 1 dup => 1 1 */
ugen_status usp_dup_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v = usp_pop_flt(ctx);
  usp_push_flt(ctx, v);
  usp_push_flt(ctx, v);
  return UGEN_OK;
}
ugen_status usp_dup_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v = usp_pop_flt(ctx);
  usp_push_flt(ctx, v);
  usp_push_flt(ctx, v);
  return UGEN_OK;
}
void usp_dup_free(ugen_instance ugen) {  }

/* 1 2 dup2 => 1 2 1 2 */
ugen_status usp_dup2_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  return UGEN_OK;
}
ugen_status usp_dup2_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  usp_push_flt(ctx, v1);
  return UGEN_OK;
}
void usp_dup2_free(ugen_instance ugen) {  }

/* 1 2 swap => 2 1 */
ugen_status usp_swap_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  return UGEN_OK;
}
ugen_status usp_swap_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  return UGEN_OK;
}
void usp_swap_free(ugen_instance ugen) {  }
