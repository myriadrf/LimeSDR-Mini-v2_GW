/****************************************************************************
**
**  Name: MicoFileIsAtty.c
**
**  Description:
**        Implements isatty, called by NewlibC file operations
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

int isatty(int fd)
{
    MicoFileDesc_t *pFD;
    int retValue =0;


    /* given the file-id, fetch the associated file-descriptor */
    if(MicoGetFDEntry(fd, &pFD) != 0)
        return(0);


    /* ask the device to write the data if it is capable of writing data */
    if(pFD->pFileOpsTable->isatty)
        retValue = pFD->pFileOpsTable->isatty(pFD);


    /* all done */
    return(retValue);

}

#ifdef __cplusplus
}
#endif

