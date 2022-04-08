/****************************************************************************
**
**  Name: MicoFileSeek.c
**
**  Description:
**        Implements _lseek, called by Newlib C library functions
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

#include <sys/types.h>
#include "MicoFileDevices.h"
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************
 *                                                                *
 * Implements _read required by NewLibC's _read_r function        *
 *                                                                *
 ******************************************************************/
long _lseek(int fd, long pos, int origin)
{
    MicoFileDesc_t *pFD;
    int retvalue = -1;

    /* make sure origin i.e. whence is okay */
    if( (origin != SEEK_SET) &&
        (origin != SEEK_CUR) &&
        (origin != SEEK_END))
        return(-1);


    /* given the file-id, fetch the associated file-descriptor */
    if(MicoGetFDEntry(fd, &pFD) != 0){
        return(-1);
    }


    if(pFD->pFileOpsTable->lseek)
        retvalue = pFD->pFileOpsTable->lseek(pFD, pos, origin);

    return(retvalue);
}

#ifdef __cplusplus
}
#endif

