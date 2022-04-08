#include "Reg_Comp.h"
#include "MicoMacros.h"
#include "LookupServices.h"
/* device initialization function */
void init_reg_device( struct st_reg_device * ctx )
{
   ctx->lookupReg.name = ctx->name;
   ctx->lookupReg.deviceType = "GPODevice";
   ctx->lookupReg.priv = ctx;

   MicoRegisterDevice( &(ctx->lookupReg) );
    /* all done */
   return;
   
 return;
}