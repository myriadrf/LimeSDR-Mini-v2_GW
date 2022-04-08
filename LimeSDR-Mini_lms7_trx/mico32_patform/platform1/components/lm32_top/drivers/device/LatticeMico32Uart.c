/****************************************************************************
**
**  Name: LatticeMico32Uart.c
**
**  Description:
**        Implements routines required for treating
**        LatticeMico32 processor as a file device for
**        file operations.  User does not need to access
**        these functions.
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

#include "LatticeMico32Uart.h"


/*
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                 MAPPER: LOCAL FILE v/s GDB FILE                            *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *                                                                            *
 * This section implements mapping of local file-service to GDB file-service  *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/* declare MICO_GDB_MAX_FILES if it isn't already done so */
#ifndef MICO_GDB_MAX_FILES
#define MICO_GDB_MAX_FILES  (5)
#else
/* make sure there is space for atleast 3 files */
#if (MICO_GDB_MAX_FILES < 3)
#define MICO_GDB_MAX_FILES  (3)
#endif
#endif




    /* local-to-gdb mapping entry */
    typedef struct st_LM32ToGDBFMapEntry_t {
        int gdbId;                              /*  gdb file-id         */
        unsigned int iInUse;                    /*  entry-in-use        */
    }LM32ToGDBMapEntry_t;



    /* maintain a table for local-to-gdb file-id mapping */
    LM32ToGDBMapEntry_t LM32ToGDBMap[MICO_GDB_MAX_FILES];


    /* initializes map-entry */
    static void LM32GDBFile_InitializeMapEntry(void)
    {
        int i;


        /* initialize table */
        for (i = 0; i < MICO_GDB_MAX_FILES; i++){
            LM32ToGDBMap[i].iInUse = 0;
        }

        /* all done */
        return;
    }


    /* given a token, fetches GDBFileId */
    static int LM32GDBFile_GetGDBFileId(int token)
    {
        /* make sure token is reasonable */
        if(token >= MICO_GDB_MAX_FILES)
            return(-1);

        if(LM32ToGDBMap[token].iInUse != 0)
            return(LM32ToGDBMap[token].gdbId);
        else
            return(-1);
    }



    /* given a token, sets gdbId */
    static void LM32GDBFile_SetGDBFileId(int token, int gdbId)
    {
        /* make sure token is reasonable */
        if(token >= MICO_GDB_MAX_FILES)
            return;

        if(LM32ToGDBMap[token].iInUse != 0)
            LM32ToGDBMap[token].gdbId = gdbId;

        return;
    }


    /* allocates a map-entry */
    static int LM32GDBFile_Alloc(void)
    {
        int i;
        for(i = 0; i < MICO_GDB_MAX_FILES; i++){
            if(LM32ToGDBMap[i].iInUse == 0){
                LM32ToGDBMap[i].iInUse = 1;
                return(i);
            }
        }

        /* all locations are used up: too many files open */
        return(-1);
    }


    /* deallocates a map-entry */
    static void LM32GDBFile_Free(int token)
    {
        if(token >= MICO_GDB_MAX_FILES)
            return;

        /* nullify iInUse */
        LM32ToGDBMap[token].iInUse = 0;

        return;
    }


#ifdef __cplusplus
}
#endif /* __cplusplus */


/*
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                 FILE (CHARACTER MODE) SERVICE                              *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *                                                                            *
 * This section implements functionality for supporting local file-i/o        *
 * requests.                                                                  *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
*/


/*
 ******************************************************************************
 *                                                                            *
 * UART file-read function                                                    *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_JTAGFileRead(MicoFileDesc_t *fd, char *buffer, unsigned int bytes)
{

    int gdbId;
    gdbId = LM32GDBFile_GetGDBFileId((int)fd->priv);
    if(gdbId < 0)
        return(ENOENT);


	return(LatticeMico32DbgRead(gdbId, buffer, bytes));
}


/*
 ******************************************************************************
 *                                                                            *
 * UART file-write function                                                   *
 *                                                                            *
 ******************************************************************************
*/

int LatticeMico32_JTAGFileWrite(MicoFileDesc_t *fd, const char *buffer, unsigned int bytes)
{
    int gdbId;

    /* fetch gdb fileId */
    gdbId = LM32GDBFile_GetGDBFileId((int)fd->priv);
    if(gdbId < 0)
        return(ENOENT);

	return(LatticeMico32DbgWrite(gdbId, buffer, bytes));
}



/*
 ******************************************************************************
 *                                                                            *
 * Uart file-isatty function                                                  *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_JTAGFileIsAtty(MicoFileDesc_t *fd)
{
    /* we're a terminal io device */
    return(1);
}



/*
 ******************************************************************************
 *                                                                            *
 * Uart file-open handler                                                     *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_JTAGFileOpen(MicoFileDesc_t *fd, const char*filename)
{
    int gdbId;
    int token;

    token = LM32GDBFile_Alloc();
    if(token < 0)
        return(EMFILE);


    /*
     * If it is a standard-stream, don't bother opening, it's always open
     * and we support all standard streams
     */
    if((fd->special >= 0) && (fd->special < 3)){
        LM32GDBFile_SetGDBFileId(token, fd->special);
    }else{
        gdbId = LatticeMico32DbgOpen(filename, fd->flags, fd->mode);
        if(gdbId < 0){
            LM32GDBFile_Free(token);
            return(-1);
        }else{
            LM32GDBFile_SetGDBFileId(token, gdbId);
        }
    }


    fd->priv = (void *)token;
    return(0);
}



/*
 ******************************************************************************
 *                                                                            *
 * Uart file-close handler                                                    *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_JTAGFileClose(MicoFileDesc_t *fd)
{
    int gdbId;
    int rValue;

    /* fetch gdb fileId */
    gdbId = LM32GDBFile_GetGDBFileId((int)fd->priv);
    if(gdbId < 0)
        return(ENOENT);


    /*
     *-----------------------------------------------------------------
     * Standard streams are always open and hence we return success if 
     * the request is to close a standard stream
     *-----------------------------------------------------------------
     */
    if(gdbId > 2){
        rValue = LatticeMico32DbgClose(gdbId);
        if(rValue == 0){
            LM32GDBFile_Free(gdbId);
        }
        return(rValue);
    }
    else
        return(0);

}



/*
 ******************************************************************************
 *                                                                            *
 * Uart file-registration                                                     *
 *                                                                            *
 ******************************************************************************
*/
void LatticeMico32_JTAGFileInit(LatticeMico32Ctx_t *pCPU)
{
    static struct st_MicoFileDevice_t fileDevice;
    static struct st_MicoFileFnTable_t fileFnTbl;


    /*
     ****************************************************
     *        initialize mapping-table                  *
     ****************************************************
    */
    LM32GDBFile_InitializeMapEntry();


    /*
     ****************************************************
     * populate the file-device function-table entries
     ****************************************************
    */
    fileFnTbl.read      = LatticeMico32_JTAGFileRead;       /* read handler         */
    fileFnTbl.write     = LatticeMico32_JTAGFileWrite;      /* write handler        */
    fileFnTbl.isatty    = LatticeMico32_JTAGFileIsAtty;     /* isatty handler       */
    fileFnTbl.open      = LatticeMico32_JTAGFileOpen;       /* always open          */
    fileFnTbl.close     = LatticeMico32_JTAGFileClose;      /* nothing on close     */
    fileFnTbl.lseek     = 0;                                /* cannot seek          */
    fileFnTbl.unlink    = 0;                                /* cannot remove files  */
    fileFnTbl.link      = 0;                                /* cannot "link"        */
    fileFnTbl.stat      = 0;                                /* cannot stat          */
    fileFnTbl.ioctl     = 0;                                /* cannot ioctl         */



    /*
     ***************************************************
     * populate the file-device registration structure *
     ***************************************************
    */
    fileDevice.name             = pCPU->name;           /* file-device instance name   */
    fileDevice.p_FileFnTable    = &fileFnTbl;           /* function-table              */
    fileDevice.priv             = (void *)pCPU;         /* uart-instance info          */



    /*
     **************************************************
     * register as an available file-device
     **************************************************
    */
    MicoRegisterFileDevice(&fileDevice);


    /*
     **************************************************
     * Since this is the CPU, make it the default file
     * device.
     **************************************************
     */
    MicoFileSetDefFileDevice(pCPU->name);


    /*
     * all done
     */
    return; 
}

