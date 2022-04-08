/****************************************************************************
**
**  Name: MicoFileDevices.c
**
**  Description:
**        Implements file-redirection layer for Newlib C file
**        operations.  These routines manage various devices that
**        register themselves as file-devices.  The actual function
**        implementations of the functions called by Newlib C library
**        are implemented in other files.
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

#include "MicoFileDevices.h"
#include "MicoUtils.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern const char *MICO_STDIN_DEV_NAME;
extern const char *MICO_STDOUT_DEV_NAME;
extern const char *MICO_STDERR_DEV_NAME;


/* ENFORCE HAVING ATLEAST 3 FILE DESCRIPTORS (stdi, stdo,stderr) */
#if (MICO_FILE_DEVICES_MAX_DESCRIPTORS < 3)
#undef MICO_FILE_DEVICE_MAX_DESCRIPTORS
#define MICO_FILE_DEVICES_MAX_DESCRIPTORS   (3)
#endif



/******************************************************************************
 *                                                                            *
 *                       TYPE DEFINITIONS                                     *
 *                                                                            *
 ******************************************************************************/
    typedef struct st_MicoFDTableEntry_t{
        unsigned int i_InUse;       /* helps identify if the entry is in use i.e. device is opened  */
        MicoFileDesc_t fd;          /* file descriptor associated with this entry                   */
    }MicoFDTableEntry_t;



/******************************************************************************
 *                                                                            *
 *                       DATA MEMBERS                                         *
 *                                                                            *
 ******************************************************************************/
    static MicoFileDevice_t *s_FDeviceLinkHd;                                   /* used to track the list of file-devices */
    static MicoFDTableEntry_t s_MicoFDTable[MICO_FILE_DEVICES_MAX_DESCRIPTORS]; /* file-descriptor table */
    static MicoFileDevice_t *s_DefaultFileDevice;                               /* DEFAULT FILE DEVICE */


/******************************************************************************
 *                                                                            *
 *                          null i/o device                                   *
 *                                                                            *
 ******************************************************************************/
    static struct st_MicoFileFnTable_t s_FileDeviceFnTbl_NULL = {
        0,  /* no open routine      */
        0,  /* no close routine     */
        0,  /* no read routine      */
        0,  /* no write routine     */
        0,  /* no lseek routine     */
        0,  /* no unlink routine    */
        0,  /* no link routine      */
        0,  /* no stat routine      */
        0,  /* no ioctl routine     */
        0   /* no isatty routine    */
    };

    static struct st_MicoFileDevice_t s_FileDevice_NULL = {
        "MICO_NULL_DEVICE",
        &s_FileDeviceFnTbl_NULL
    };

#ifdef __cplusplus
}
#endif /* __cplusplus */



/*
 *----------------------------------------------------*
 *                                                    *
 * UTILITY FUNCTION:                                  *
 * PARAMETERS:                                        *
 * char *dName: Pointer to buffer to contain          *
 *    device-name                                     *
 * int iDLimit: length of dName buffer (in bytes)     *
 * char *fName: Pointer to buffer to contain          *
 *    file-name                                       *
 * int iFLimit: length of fName buffer (in bytes)     *
 * const char *filename: device/filename pair (must   *
 *     be null-terminated.                            *
 *                                                    *
 *----------------------------------------------------*
 */
void GetDeviceName(char *dName, int iDLimit, char *fName, int iFLimit, const char *filename)
{
	int i;
	int j;

	fName[0] 	= '\0';
	dName[0] 	= '\0';

	/* make sure filename is atleast 2 characters long */
	if(cStrLen(filename) < 2)
		return;

	j = 0;
	/* make sure filename is actually referencing a device */
	if(	(filename[0] == '\\') && (filename[1] == '\\')){

		/* extract device-name */
		j = 2;
		for(i = 0; i < (iDLimit-1); i++, j++){
			/* if we've reached end of string, there's no filename */
			if(filename[j] == '\0'){
				dName[i] = '\0';
				return;
			}
			/* if we've reached a delimiter, we've got our filename */
			if(filename[j] == '\\'){
				break;
			}
			/* keep copying device-name character */
			dName[i] = filename[j];
		}

		/* null-terminate device-name */
		dName[i] = '\0';

		/* 
		 * See if we exited because we reached the file-marker or
		 * we reached device-limit
		 */
		while(1){
			if(filename[j] == '\0')
				return;
			if(filename[j] == '\\')
				break;
			j++;
		}

		/* move to the next location i.e. filename-start-location */
		j++;
	}

	/* extract filename */
	for(i = 0; i < iFLimit-1; i++, j++){
		if(filename[j] == '\0'){
			fName[i] = '\0';
            if(dName[0] == '\0')
			    return;
            else
                return;
		}
		fName[i] = filename[j];
	}

	/* nullterminate filename */
	fName[i] = '\0';

	return;
}


/***************************************************************
 *                                                             *
 *     initializes file-device service                         *
 *                                                             *
 ***************************************************************/
static void MicoInitFileDevicesService(void)
{
    unsigned int i;
    static unsigned int s_iInitialized = 0;

    if(s_iInitialized == 0){
        s_iInitialized = 1;

        /* initialize link-list head */
        s_FDeviceLinkHd = 0;

        /* load null-device into the standard file devices */
        for(i = 0; i < 3; i++){
            s_MicoFDTable[i].i_InUse = 1;
            s_MicoFDTable[i].fd.pFileOpsTable = &s_FileDeviceFnTbl_NULL;
            s_MicoFDTable[i].fd.mode = 0;
            s_MicoFDTable[i].fd.flags = 0;
            s_MicoFDTable[i].fd.pDevice = &s_FileDevice_NULL;
            s_MicoFDTable[i].fd.pData = 0;
        }



        /* initialize fd-table */
        for(i = 3; i < MICO_FILE_DEVICES_MAX_DESCRIPTORS; i++){
            s_MicoFDTable[i].i_InUse = 0;
        }

        /* set default file-device to null */
        s_DefaultFileDevice = 0;

    }

    /* all done */
    return;
}



/*****************************************************************
 *                                                               *
 *   Fetches file-descriptor, given the "file-id"                *
 *   Returns 0 if successful                                     *
 *                                                               *
 *****************************************************************/
int MicoGetFDEntry(int fd, MicoFileDesc_t **pFD)
{
    /* make sure the file-descriptor is within range */
    if((fd < 0) || (fd >= MICO_FILE_DEVICES_MAX_DESCRIPTORS))
        return(ENOENT);

    /* make sure the file is in use */
    if(s_MicoFDTable[fd].i_InUse == 0)
        return(ENOENT);

    *pFD = &(s_MicoFDTable[fd].fd);

    return(0);
}



/*******************************************************************
 *                                                                 *
 *   Allocates a file-descriptor (other than standard io streams   *
 *   Returns 0 if successful                                       *
 *                                                                 *
 *******************************************************************/
int MicoAllocFDEntry(int *fd, MicoFileDesc_t **pFD)
{
    unsigned int i;

    /* make sure there are enough descriptors anyways */
    if(MICO_FILE_DEVICES_MAX_DESCRIPTORS <= 3)
        return(ENFILE);


    /* scour through the table and see if there's an empty slot */
    for(i = 3; i < MICO_FILE_DEVICES_MAX_DESCRIPTORS; i++){
        if(s_MicoFDTable[i].i_InUse ==0){
            *fd = i;
            *pFD = &(s_MicoFDTable[i].fd);
            s_MicoFDTable[i].i_InUse = 1;
            return(0);
        }
    }


    /* too many open files, no more space left */
    return(ENFILE);
}



/*******************************************************************
 *                                                                 *
 *   Frees a file descriptor (other than standard io streams       *
 *                                                                 *
 *******************************************************************/
void MicoFreeFDEntry(int fd)
{

    /* 
     * Make sure no one's attempting to free the standard stream
     * and that it is within range
     */
    if((fd <= 0) || (fd >= MICO_FILE_DEVICES_MAX_DESCRIPTORS))
        return;

    /* make sure the descriptor was really in use */
    if(s_MicoFDTable[fd].i_InUse != 0){
        s_MicoFDTable[fd].i_InUse = 0;
    }

    return;
}



/*
 *****************************************************************
 *                                                               *
 *   Fetches file-device context of the registered device with   *
 *   matching name                                               *
 *                                                               *
 *****************************************************************
 */
MicoFileDevice_t* MicoGetFileDevice(const char *name)
{
    MicoFileDevice_t *pDevice;

    /* Initialize file-devices service if not already initialized */
    MicoInitFileDevicesService();

    pDevice = s_FDeviceLinkHd;

    if((pDevice == 0) || (name == 0))
        return(0);

    do{
        if(nMicoStrCompare(name, pDevice->name, MICO_FILE_DEVICES_MAX_DEV_NAME_LEN) == 0)
            return(pDevice);
        pDevice = (MicoFileDevice_t *)pDevice->next;
    }while(pDevice != s_FDeviceLinkHd);

    return(0);
}





/***************************************************************
 *                                                             *
 *     registers a file-device; returns 0 if successful        *
 *                                                             *
 ***************************************************************/
unsigned int MicoRegisterFileDevice(MicoFileDevice_t *pDevCtx)
{
    /* Initialize file-devices service if not already initialized */
    MicoInitFileDevicesService();


    /* check parameters */
    if(pDevCtx == 0)
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* name cannot be null-pointer */
    if(pDevCtx->name == 0)
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* name cannot be anonymous */
    if(pDevCtx->name[0] == '\0')
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* function-table must exist */
    if(pDevCtx->p_FileFnTable == 0)
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* Make sure this device isn't already registered */
    if(MicoGetFileDevice(pDevCtx->name) != 0)
        return(MICO_FILE_ERR_DEVICE_EXISTS);


    /* go ahead and attach this file-device to our list of devices */
    if(s_FDeviceLinkHd == 0){
        s_FDeviceLinkHd         = pDevCtx;
        s_FDeviceLinkHd->next   = pDevCtx;
        s_FDeviceLinkHd->prev   = pDevCtx;
    } else {
        /* put the new timer at the end of the list and arrange links */
        pDevCtx->prev = s_FDeviceLinkHd->prev;
        ((MicoFileDevice_t * )pDevCtx->prev)->next = pDevCtx;
        pDevCtx->next = s_FDeviceLinkHd;
        s_FDeviceLinkHd->prev = pDevCtx;
    }


    /* Check if this device is to be the standard input device */
    if(nMicoStrCompare(MICO_STDIN_DEV_NAME, pDevCtx->name, MICO_FILE_DEVICES_MAX_DEV_NAME_LEN) == 0){
        MicoFileRedirIO(0, pDevCtx->name);
    }

    /* Check if this device is to be the standard output device */
    if(nMicoStrCompare(MICO_STDOUT_DEV_NAME, pDevCtx->name, MICO_FILE_DEVICES_MAX_DEV_NAME_LEN) == 0){
        MicoFileRedirIO(1, pDevCtx->name);
    }

    /* Check if the device is to be the standard error device */
    if(nMicoStrCompare(MICO_STDERR_DEV_NAME, pDevCtx->name, MICO_FILE_DEVICES_MAX_DEV_NAME_LEN) == 0){
        MicoFileRedirIO(2, pDevCtx->name);
    }


    /* done! */
    return(0);
}



/*
 *****************************************************************
 *                                                               *
 *   Fetches default file-device                                 *
 *                                                               *
 *****************************************************************
 */
MicoFileDevice_t* MicoFileGetDefFileDevice(void)
{
    /* Initialize file-devices service if not already initialized */
    MicoInitFileDevicesService();

    return(s_DefaultFileDevice);
}



/***************************************************************
 *                                                             *
 *     Sets default file-device                                *
 *                                                             *
 *-------------------------------------------------------------*
 *  returns 0 if successful                                    *
 *                                                             *
 ***************************************************************/
int MicoFileSetDefFileDevice(const char *deviceName)
{
    MicoFileDevice_t* pDevice;

    /* Initialize file-devices service if not already initialized */
    MicoInitFileDevicesService();

    /* make sure argument is valid */
    if(deviceName == 0)
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* make sure device exists */
    pDevice = MicoGetFileDevice(deviceName);
    if(pDevice == 0)
        return(MICO_FILE_ERR_DEVICE_NOT_FOUND);


    /* change the default file-device */
    s_DefaultFileDevice = pDevice;


    /* done! */
    return(0);
}





/***************************************************************
 *                                                             *
 *     Redirects standard input/output                         *
 *                                                             *
 *-------------------------------------------------------------*
 *  returns 0 if successful                                    *
 *                                                             *
 ***************************************************************/
int MicoFileRedirIO(int StreamId, const char *deviceName)
{
    MicoFileDevice_t* pDevice;
    MicoFileDesc_t pPrev;
    MicoFileDesc_t *pPrevFD;
    int retvalue;


    /* make sure it's pointing to a standard stream */
    if(StreamId > 3)
        return(MICO_FILE_ERR_INVALID_STREAM_ID);


    /* make sure the deviceName pointer is non-null */
    if(deviceName == 0)
        return(MICO_FILE_ERR_PARAMETER_INVALID);


    /* see if the named-device is registered */
    pDevice = MicoGetFileDevice(deviceName);
    if(pDevice == 0)
        return(MICO_FILE_ERR_DEVICE_NOT_FOUND);



    /* save the current stream information */
    pPrevFD             = &s_MicoFDTable[StreamId].fd;
    pPrev.pFileOpsTable = pPrevFD->pFileOpsTable;
    pPrev.pDevice       = pPrevFD->pDevice;
    pPrev.pData         = pPrevFD->pData;
    pPrev.priv          = pPrevFD->priv;
    pPrev.special       = pPrevFD->special;



    /* set this device for handling the desired standard stream */
    s_MicoFDTable[StreamId].i_InUse             = 1;
    s_MicoFDTable[StreamId].fd.pFileOpsTable    = pDevice->p_FileFnTable;
    s_MicoFDTable[StreamId].fd.pDevice          = pDevice;
    s_MicoFDTable[StreamId].fd.pData            = 0;
    s_MicoFDTable[StreamId].fd.priv             = pDevice->priv;
    s_MicoFDTable[StreamId].fd.special          = StreamId;



    /* 
     * Since we're opening a standard-stream, the filename is null and
     * special is set to the desired stream-ID
     */
    retvalue = 0;
    if(pDevice->p_FileFnTable->open)
        retvalue = pDevice->p_FileFnTable->open(&s_MicoFDTable[StreamId].fd, "");



    /* see if the device opened successfully */
    if(retvalue != 0){
        /* since the new device did not open successfully, restore old information */
        pPrevFD->pDevice            = pPrev.pDevice;
        pPrevFD->pData              = pPrevFD->pData;
        pPrevFD->pFileOpsTable      = pPrev.pFileOpsTable;
        pPrevFD->priv               = pPrev.priv;
        pPrevFD->special            = pPrev.special;
        return(retvalue);
    }



    /* 
     * since we've replaced the old device with the new device, tell the old device to close this
     * file operator
     */
    if(pPrev.pFileOpsTable->close)
        pPrev.pFileOpsTable->close(&pPrev);



    /* 
     * all done
     */
    return(retvalue);
}

