/****************************************************************************
**
**  Name: MicoFileDevices.h
**
**  Description:
**        Declares prototypes of functions for the file-redirection layer
**        These routines manage various devices that register themselves
**        as file-devices.  The actual function implementations of the
**        functions called by Newlib C library are implemented in other files.
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

#ifndef _MICO_FILE_DEV_HEADER_FILE_
#define _MICO_FILE_DEV_HEADER_FILE_


#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************
 *                                                               *
 * This file defines structures required of from file-devices.   *
 * It allows interfacing with newlib C library for file          *
 * manipulation.                                                 *
 *                                                               *
 *                                                               *
 * It does not support common drive operations as those are      *
 * not Newlib standard C functions.  You can however extend      *
 * the concept to create your own comprehensive drive concept    *
 *                                                               *
 * ------------------------------------------------------------- *
 *                                                               *
 * NOTE: devices such as uart, jtag-uart are also treated as     *
 * file-devices.                                                 *
 *                                                               *
 * Since devices may have peculiarities in opening a file,       *
 * this LatticeMico32 file-wrapper layer does not check open-    *
 * modes or flags.  these values are saved in the file descrip-  *
 * tor and the device must perform appropriate checks as needed  *
 *                                                               *
 *****************************************************************/

/***************************************************************
 *                                                             *
 * CONSTANTS                                                   *
 *                                                             *
 ***************************************************************/

    /************************************************************
     *                                                          *
     *     Declares max file descriptors i.e. max opened files  *
     *     MUST BE A MINIMUM OF 3 (stdio and stderr)            *
     *                                                          *
     ************************************************************/
#ifndef MICO_FILE_DEVICES_MAX_DESCRIPTORS
#define MICO_FILE_DEVICES_MAX_DESCRIPTORS   (5)
#endif




    /*
     ************************************************************
     *                                                          *
     *     Declares max device-name length (including null char)*
     *     MUST BE A MINIMUM OF 4.                              *
     *                                                          *
     *----------------------------------------------------------*
     *                                                          *
     * NOTE: THIS AFFECTS STACK-USAGE BY THE AMOUNT DECLARED    *
     *                                                          *
     ************************************************************
     */
#ifndef MICO_FILE_DEVICES_MAX_DEV_NAME_LEN
#define MICO_FILE_DEVICES_MAX_DEV_NAME_LEN  (12)
#else
/* enforce maximum device-name length to 12 (INCL NULL SPACE) */
#if (MICO_FILE_DEVICES_MAX_DEV_NAME_LEN < 12)
#undef MICO_FILE_DEVICES_MAX_DEV_NAME_LEN
#define MICO_FILE_DEVICES_MAX_DEV_NAME_LEN  (12)
#endif
#endif





    /*
     ************************************************************
     *                                                          *
     *     Declares max filename length (including null char)   *
     *     MUST BE A MINIMUM OF 4                               *
     *                                                          *
     *----------------------------------------------------------*
     *                                                          *
     * NOTE: THIS AFFECTS STACK-USAGE BY THE AMOUNT DECLARED    *
     *                                                          *
     ************************************************************
     */
#ifndef MICO_FILE_DEVICES_MAX_FILE_NAME_LEN
#define MICO_FILE_DEVICES_MAX_FILE_NAME_LEN (32)
#else
/* enforce maximum file-name length to 13 (INCL NULL SPACE) */
#if (MICO_FILE_DEVICES_MAX_FILE_NAME_LEN < 13)
#undef MICO_FILE_DEVICES_MAX_FILE_NAME_LEN
#define MICO_FILE_DEVICES_MAX_FILE_NAME_LEN (13)
#endif
#endif





    /*
     *************************************************************
     *                                                           *
     *  WELL-KNOWN RETURN VALUES FOR USER-FUNCTIONS SPECIFIC TO  *
     *                                                           *
     *************************************************************
     */
#define MICO_FILE_ERR_DEVICE_EXISTS         (0x1)
#define MICO_FILE_ERR_PARAMETER_INVALID     (0x2)
#define MICO_FILE_ERR_INVALID_STREAM_ID     (0x3)
#define MICO_FILE_ERR_DEVICE_NOT_FOUND      (0x4)




/***************************************************************
 *                                                             *
 * TYPE DECLARATIONS                                           *
 *                                                             *
 ***************************************************************/

    typedef struct st_MicoFileFnTable_t MicoFileFnTable_t;
    typedef struct st_MicoFileDevice_t MicoFileDevice_t;


    /**************************************************************
     *                                                            *
     * This structure defines a file-descriptor that is passed to *
     * registered devices for file-operations                     *
     * This file-descriptor is unique for each opened file        *
     * till the file is closed.                                   *
     *                                                            *
     **************************************************************/
    typedef struct st_MicoFileDesc_t{
        int mode;                           /* mode associated with file when performing open operation     */
        int flags;                          /* flags associated with file when calling the open operation   */
        int special;                        /* special purpose                                              */
        MicoFileFnTable_t *pFileOpsTable;   /* pointer to registered file-operations table                  */
        MicoFileDevice_t *pDevice;          /* device's registration information                            */
        void *priv;                         /* device-specific private data provided at registration time   */
        void *pData;                        /* additional file-specific storage pointer for user's use      */
    }MicoFileDesc_t;



    /***************************************************************
     *                                                             *
     * This structure defines the expected function-table from     *
     * (and hence the expected functionality) of file-type devices *
     *                                                             *
     ***************************************************************/
    struct st_MicoFileFnTable_t {
        int (*open)     (MicoFileDesc_t *fd, const char *name);                         /* open funtion-pointer         */
        int (*close)    (MicoFileDesc_t *fd);                                           /* close function-pointer       */
        int (*read)     (MicoFileDesc_t *fd, char *buffer, unsigned int bytes);         /* read function-pointer        */
        int (*write)    (MicoFileDesc_t *fd, const char *buffer, unsigned int bytes);   /* write function-pointer       */
        int (*lseek)    (MicoFileDesc_t *fd, int ptr, int dir);                         /* lseek implementation         */
        int (*unlink)   (MicoFileDesc_t *fd, char *name);                               /* unlink a named file entry    */
        int (*link)     (MicoFileDesc_t *fd, char *name);                               /* link a named file entry      */
        int (*stat)     (MicoFileDesc_t *fd, struct stat* buf);                         /* file-status implementation   */
        int (*ioctl)    (MicoFileDesc_t *fd, int req, void* arg);                       /* ioctl implementation         */
        int (*isatty)   (MicoFileDesc_t *fd);                                           /* isatty implementation        */
    };


    /******************************************************************************
     *                                                                            *
     *                                                                            *
     * This structure defines the elements needed when registering a file-device. *
     *                                                                            *
     *                                                                            *
     ******************************************************************************/
    struct st_MicoFileDevice_t{
        const char *name;                                               /* device's name                                         */
        MicoFileFnTable_t *p_FileFnTable;                               /* pointer to device's file ops table; MUST REMAIN VALID */
        void *priv;                                                     /* device-specific private data                          */
        void *next;                                                     /* USED INTERNALLY: DO NOT MODIFY                        */
        void *prev;                                                     /* USED INTERNALLY: DO NOT MODIFY                        */
    };



/***************************************************************
 *                                                             *
 *      USER ACCESSIBLE FUNCTIONS                              *
 *                                                             *
 ***************************************************************/

    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function registers a file-device.                         -
     -   NOTE: the pointer passed must remain valid for the duration    -
     -   of the program.                                                -
     -   It returns 0 if successful, else, returns a non-zero value     -
     --------------------------------------------------------------------
    */
    unsigned int MicoRegisterFileDevice(MicoFileDevice_t *pDevCtx);




    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function fetches the file-device context pointer of the   -
     -   registered device that has matching name.                      -
     -   It returns 0 if it cannot find a matching registered device    -
     -                                                                  -
     --------------------------------------------------------------------
    */
    MicoFileDevice_t* MicoGetFileDevice(const char *name);


    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function fetches the file-descriptor, given a file id     -
     -   It returns 0 if successful                                     -
     -                                                                  -
     --------------------------------------------------------------------
    */
    int MicoGetFDEntry(int fd, MicoFileDesc_t **pFD);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function allocates a file-descriptor and returns the file -
     -   id associated with the descriptor                              -
     -   It returns 0 if it successfully allocates a descriptor         -
     -                                                                  -
     --------------------------------------------------------------------
    */
    int MicoAllocFDEntry(int *fd, MicoFileDesc_t **pFD);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function deallocates a file-descriptor, given the file-id -
     -   associated with the file-id                                    -
     -   It returns 0 if it is successful.                              -
     -                                                                  -
     --------------------------------------------------------------------
    */
    void MicoFreeFDEntry(int fd);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function redirects desired standard stream to the appro.  -
     -   priate selected device                                         -
     -                                                                  -
     --------------------------------------------------------------------
    */
    int MicoFileRedirIO(int StreamId, const char *deviceName);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function sets the default file-device.  It returns 0      -
     -   if successful.                                                 -
     -                                                                  -
     --------------------------------------------------------------------
    */
    int MicoFileSetDefFileDevice(const char *deviceName);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   This function gets the default file-device.  User must check   -
     -   return-value to make sure the pointer isn't null i.e. no       -
     -   default file-device.                                           -
     -                                                                  -
     --------------------------------------------------------------------
    */
    MicoFileDevice_t* MicoFileGetDefFileDevice(void);



    /*
     --------------------------------------------------------------------
     -                                                                  -
     -   Utility function to extract devicename and filename from a     -
     -   device/filename pair.                                          -
     -                                                                  -
     -   Parameters:                                                    -
     -     char *dName: buffer to contain device-name                   -
     -     int iDLimit: length of dName buffer (chars)                  -
     -     char *fName: buffer to contain filename                      -
     -     int iFLimit: length of fName buffer                          -
     -     const char *filename: null-terminated, valid, device/filename-
     -         pair.                                                    -
     -                                                                  -
     --------------------------------------------------------------------
    */
    void GetDeviceName(char *dName, int iDLimit, char *fName, int iFLimit, const char *filename);



#ifdef __cplusplus
}
#endif


#endif /* _MICOFILEDESCRIPTOR_HEADER_FILE_ */

