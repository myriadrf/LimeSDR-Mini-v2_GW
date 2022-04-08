#include "FIFO_Comp.h"

/* device initialization function */
void init_fifo_device( struct st_fifo_device * ctx )
{
 /* simply copy initialization data for reg_16
 * provided in the context structure to register-00 */
 FIFO_DEV_REGISTER(ctx->b_addr,0) = ctx->reg_16_value;
 
 return;
 
}