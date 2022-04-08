#include "Reg_Comp.h"

/* device initialization function */
void init_reg_device( struct st_reg_device * ctx )
{
 /* simply copy initialization data for reg_08
 * provided in the context structure to register-00 */
 REG_DEV_REGISTER(ctx->b_addr,0) = ctx->reg_08_value;
 
 return;
 
}