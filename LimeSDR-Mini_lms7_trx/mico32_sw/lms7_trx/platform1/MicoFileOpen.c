/****************************************************************************
**
**  Name: MicoFileOpen.c
**
**  Description:
**        Implements _open that is called by Newlib C library file
**        functions.
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

/*
 * OPEN HANDLER
 */
int _open(const char *file, int flags, int mode)
{
    MicoFileDevice_t *pDevice;
    MicoFileDesc_t *pFD;
    int fileId;
    int rValue;
    char dName[MICO_FILE_DEVICES_MAX_DEV_NAME_LEN];
    char fName[MICO_FILE_DEVICES_MAX_FILE_NAME_LEN];


    /* extract device-name and file-name from the provided name */
    GetDeviceName(  dName, MICO_FILE_DEVICES_MAX_DEV_NAME_LEN, 
                    fName, MICO_FILE_DEVICES_MAX_FILE_NAME_LEN,
                    file);


    /* see if the device-name is okay */
    if(dName[0] == '\0'){
        /* since there's no device-name, make sure there's a filename */
        if(fName[0] == '\0'){
            return(-1);
        }

        /* get the default file-device */
        pDevice = MicoFileGetDefFileDevice();

    }else{
        /* Search for a device with matching name */
        pDevice = MicoGetFileDevice(dName);
    }


    /* make sure it's a valid device */
    if(pDevice == 0){
        return(-1);
    }


    /* fetch an existing descriptor */
    rValue = MicoAllocFDEntry(&fileId, &pFD);
    if(rValue != 0){
        return(-1);
    }


    /* populate the file-descriptor data */
    pFD->mode           = mode;
    pFD->flags          = flags;
    pFD->special        = -1;   /* MAKE SURE THIS IS NOT 0,1 or 2 AS THOSE ARE RESERVED */
    pFD->pDevice        = pDevice;
    pFD->priv           = pDevice->priv;
    pFD->pFileOpsTable  = pDevice->p_FileFnTable;


    /* inform the device that it needs to open the given file */
    if(pFD->pFileOpsTable->open){
        if(pFD->pFileOpsTable->open(pFD, fName) != 0){

            /* free-up the file-descriptor data */
            MicoFreeFDEntry(fileId);
            return(-1);
        }
    }


    /* return the file-ID */
    return(fileId);
}


#ifdef __cplusplus
}
#endif

