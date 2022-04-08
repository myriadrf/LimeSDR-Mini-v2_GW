/****************************************************************************
**
**  Name: LatticeMico32.c
**
**  Description:
**        Implements LatticeMico32 processor specific functions:
**
**          LatticeMico32Init: processor initialization function for
**               registering it as an i/o device (for stdio)
**
**          MicoFlushDataCache: function for invalidating data cache
**
**          MicoFlushInstrCache: function for invalidating instruction cache
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

#include "LatticeMico32.h"
#include "LatticeMico32Uart.h"


#ifdef __cplusplus
extern "C" void asmMicoFlushDataCache(void);
extern "C" void asmMicoFlushInstrCache(void);
#else
extern void asmMicoFlushDataCache(void);
extern void asmMicoFlushInstrCache(void);
#endif


/*
 *************************************************
 *                                               *
 * Initializes processor-driver specific stuff   *
 *                                               *
 *************************************************
*/
void LatticeMico32Init(LatticeMico32Ctx_t *pCPU)
{

#ifdef __LM32_DEBUG_ROM_ENABLED__   /* THIS ONE IS AUTOMATICALLY GENERATED BASED ON CPU CONFIG */
#ifndef _MICOCPU_FILESUPPORT_DISABLED_
#ifndef _MICOFILE_SUPPORT_DISABLED_
    /*
     * REGISTER THE PROCESSOR AS AN AVAILABLE FILE-I/O DEVICE
     */
    LatticeMico32_JTAGFileInit(pCPU);
#endif
#endif
#endif

    return;
}



/*
 ***********************************
 *
 * Flushes data cache.
 *
 ************************************
*/
void MicoFlushDataCache(void)
{
        asmMicoFlushDataCache();
}


/*
 ***********************************
 *
 * Flushes instruction cache.
 *
 ************************************
*/
void MicoFlushInstrCache(void)
{
        asmMicoFlushInstrCache();
}


