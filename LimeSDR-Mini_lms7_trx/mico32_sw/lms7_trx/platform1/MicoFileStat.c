/****************************************************************************
**
**  Name: MicoFileStat.c
**
**  Description:
**        Implements _fstat, called by Newlib C library file functions
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

#include <_ansi.h>
#include <_syslist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "MicoFileDevices.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************
 *                                                                *
 * Implements _read required by NewLibC's _read_r function        *
 *                                                                *
 ******************************************************************/
int _fstat(int fd, struct stat *pstat)
{
    MicoFileDesc_t *pFD;
    int retValue =-1;


    /* given the file-id, fetch the associated file-descriptor */
    if(MicoGetFDEntry(fd, &pFD) != 0)
        return(-1);


    /* ask the device to write the data if it is capable of writing data */
    if(pFD->pFileOpsTable->stat)
        retValue = pFD->pFileOpsTable->stat(pFD, pstat);


    /* all done */
    return(retValue);

}

#ifdef __cplusplus
}
#endif

