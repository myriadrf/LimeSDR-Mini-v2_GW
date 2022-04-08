/****************************************************************************
**
**  Name: MicoGPIO.c
**
**  Description:
**     Implements GPIO functions:
**
**        MicoGPIOInit : GPIO initialization funciton called by LatticeDDInit
**                 (user-callable)
**        
**
**  $Revision: $
**
** Disclaimer:
**
**   This source code is intended as a design reference which
**   illustrates how these types of functions can be implemented.  It
**   is the user's responsibility to verify their design for
**   consistency and functionality through the use of formal
**   verification methods.  Lattice Semiconductor provides no warranty
**   regarding the use or functionality of this code.
**
** --------------------------------------------------------------------
**
**                     Lattice Semiconductor Corporation
**                     5555 NE Moore Court
**                     Hillsboro, OR 97214
**                     U.S.A
**
**                     TEL: 1-800-Lattice (USA and Canada)
**                          (503)268-8001 (other locations)
**
**                     web:   http://www.latticesemi.com
**                     email: techsupport@latticesemi.com
**
** --------------------------------------------------------------------------
**
**  Change History (Latest changes on top)
**
**  Ver    Date        Description
** --------------------------------------------------------------------------
**
**  3.0   Mar-25-2008  Added Header
**
**---------------------------------------------------------------------------
*****************************************************************************/

#include "MicoGPIO.h"
#include "MicoGPIOService.h"
#include "MicoMacros.h"
#include "LookupServices.h"


/******************************************************************************
 * Initializes a gpio                                                         *
 ******************************************************************************/
void MicoGPIOInit( MicoGPIOCtx_t *ctx )
{
    ctx->lookupReg.name = ctx->name;
    ctx->lookupReg.deviceType = "GPIODevice";
    ctx->lookupReg.priv = ctx;

    MicoRegisterDevice( &(ctx->lookupReg) );
    /* all done */
    return;
}

