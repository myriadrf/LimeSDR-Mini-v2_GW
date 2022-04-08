/****************************************************************************
**
**  Name: MicoFileWrite.c
**
**  Description:
**        Implements _write, called by Newlib C library functions
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
 * Implements MicoFileWrite for writing to file-devices           *
 * This maps to NewlibC's _write function, called by _write_r     *
 *                                                                *
 ******************************************************************/
int _write(int fd, const void *buf, unsigned int cnt)
{
    MicoFileDesc_t *pFD;
    int retValue =0;


    /* given the file-id, fetch the associated file-descriptor */
    if(MicoGetFDEntry(fd, &pFD) != 0)
        return(-1);


    /* ask the device to write the data if it is capable of writing data */
    if(pFD->pFileOpsTable->write)
        retValue = pFD->pFileOpsTable->write(pFD, (const char *)buf, cnt);


    /* all done */
    return(retValue);
}

#ifdef __cplusplus
}
#endif

