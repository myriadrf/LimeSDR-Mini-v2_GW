/****************************************************************************
**
**  Name: LatticeMico32.h
**
**  Description:
**        declares LatticeMico32 specific function prototypes
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

#ifndef LATTICE_LATTICEMICO32_HEADER_FILE
#define LATTICE_LATTICEMICO32_HEADER_FILE

#include "DDStructs.h"

/*
 ********************************************
 *                                          *
 * Processor-specific driver initialization *
 *                                          *
 ********************************************
 */

    void LatticeMico32Init(LatticeMico32Ctx_t *pCPU);


/*
 *************************
 * cache-flush functions *
 *************************
 */

    /*
     * Flushes instruction cache.
     */
    void MicoFlushInstrCache(void);


    /*
     * Flushes data-cache
    */
    void MicoFlushDataCache(void);


#endif

