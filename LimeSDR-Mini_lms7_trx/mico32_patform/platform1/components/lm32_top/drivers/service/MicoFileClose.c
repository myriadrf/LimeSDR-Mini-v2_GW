/****************************************************************************
**
**  Name: MicoFileClose.c
**
**  Description:
**        Implements _close that is called by Newlib C library when
**        performing fclose.  This function is NOT re-entrant
**        and isn't thread-safe.
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

#include <sys/errno.h>
#include <sys/types.h>
#include "MicoFileDevices.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************
 *                                                                *
 * Implements _read required by NewLibC's _read_r function        *
 *                                                                *
 ******************************************************************/
int _close(int fd)
{
    MicoFileDesc_t *pFD;

    /* given the file-id, fetch the associated file-descriptor */
    if(MicoGetFDEntry(fd, &pFD) != 0)
        return(-1);


    /* give the device a chance to close the file */
    if(pFD->pFileOpsTable->close)
        pFD->pFileOpsTable->close(pFD);


    /* free-up the descriptor */
    MicoFreeFDEntry(fd);


    return(0);
}

#ifdef __cplusplus
}
#endif

