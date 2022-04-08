/****************************************************************************
**
**  Name: LatticeMicoUtils.S
**
**  Description:
**        Implements MicoFlushDataCache and MicoFlushInstrCache for
**        invalidating the instruction and data caches.
**        these functions can be called even though the processor
**        may not have been configured with caches enabled as these
**        instructions in the invalidate functions get treated as
**        nops if there is no cache
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
        .section .text
        .align 4
        .global asmMicoFlushDataCache
        .type   asmMicoFlushDataCache, @function
asmMicoFlushDataCache:
.L1:
        addi r1, r0, 1
        wcsr DCC, r1
        nop
        nop
        nop
        nop
        b    ra
        .size   asmMicoFlushDataCache, .-asmMicoFlushDataCache



/************************************************************************
 * void MicoFlushInstrCache(void)                                       *
 *                                                                      *
 * Flushes LatticeMico32's instruction cache (entirely)                 *
 ************************************************************************/
        .global asmMicoFlushInstrCache
        .type   asmMicoFlushInstrCache, @function
asmMicoFlushInstrCache:
.L2:
        addi r1, r0, 1
        wcsr ICC, r1
        nop
        nop
        nop
        nop
        b    ra
        .size   asmMicoFlushInstrCache, .-asmMicoFlushInstrCache

