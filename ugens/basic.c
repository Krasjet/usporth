/* basic.c: basic ugens */
#include <usporth.h>

/* 1 2 add => 3 */
ugen_status ugen_add_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2+v1);
  return UGEN_OK;
}
ugen_status ugen_add_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2+v1);
  return UGEN_OK;
}
void ugen_add_free(ugen_instance ugen) {  }

/* 1 2 sub => -1 */
ugen_status ugen_sub_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2-v1);
  return UGEN_OK;
}
ugen_status ugen_sub_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2-v1);
  return UGEN_OK;
}
void ugen_sub_free(ugen_instance ugen) {  }

/* 1 2 mul => 2 */
ugen_status ugen_mul_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2*v1);
  return UGEN_OK;
}
ugen_status ugen_mul_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2*v1);
  return UGEN_OK;
}
void ugen_mul_free(ugen_instance ugen) {  }

/* 1 2 div => 0.5 */
ugen_status ugen_div_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2/v1);
  return UGEN_OK;
}
ugen_status ugen_div_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v2/v1);
  return UGEN_OK;
}
void ugen_div_free(ugen_instance ugen) {  }

/* 1 2 drop => 1 */
ugen_status ugen_drop_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_pop_flt(ctx);
  return UGEN_OK;
}
ugen_status ugen_drop_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_pop_flt(ctx);
  return UGEN_OK;
}
void ugen_drop_free(ugen_instance ugen) {  }

/* 1 2 3 rot => 2 3 1 */
ugen_status ugen_rot_init(usp_ctx *ctx, ugen_instance *pugen)
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
ugen_status ugen_rot_tick(usp_ctx *ctx, ugen_instance ugen)
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
void ugen_rot_free(ugen_instance ugen) {  }

/* 1 dup => 1 1 */
ugen_status ugen_dup_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v = usp_pop_flt(ctx);
  usp_push_flt(ctx, v);
  usp_push_flt(ctx, v);
  return UGEN_OK;
}
ugen_status ugen_dup_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v = usp_pop_flt(ctx);
  usp_push_flt(ctx, v);
  usp_push_flt(ctx, v);
  return UGEN_OK;
}
void ugen_dup_free(ugen_instance ugen) {  }

/* 1 2 dup2 => 1 2 1 2 */
ugen_status ugen_dup2_init(usp_ctx *ctx, ugen_instance *pugen)
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
ugen_status ugen_dup2_tick(usp_ctx *ctx, ugen_instance ugen)
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
void ugen_dup2_free(ugen_instance ugen) {  }

/* 1 2 swap => 2 1 */
ugen_status ugen_swap_init(usp_ctx *ctx, ugen_instance *pugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  return UGEN_OK;
}
ugen_status ugen_swap_tick(usp_ctx *ctx, ugen_instance ugen)
{
  usp_flt v1, v2;
  v1 = usp_pop_flt(ctx);
  v2 = usp_pop_flt(ctx);
  usp_push_flt(ctx, v1);
  usp_push_flt(ctx, v2);
  return UGEN_OK;
}
void ugen_swap_free(ugen_instance ugen) {  }
