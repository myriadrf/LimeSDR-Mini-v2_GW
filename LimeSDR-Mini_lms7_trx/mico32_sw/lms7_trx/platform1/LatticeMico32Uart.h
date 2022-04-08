/****************************************************************************
**
**  Name: LatticeMico32Uart.h
**
**  Description:
**        declares routines required for treating
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

#ifndef _LATTICEMICO32_UART_HEADER_FILE_
#define _LATTICEMICO32_UART_HEADER_FILE_


#include "DDStructs.h"
#include "LookupServices.h"
#include "MicoFileDevices.h"
#include "LatticeMico32.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                 FILE (CHARACTER MODE) SERVICE                              *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *                                                                            *
 * LatticeMico32 processor uses its JTAG UART for communication over the      *
 * debug module with the GDB stub.                                            *
 *                                                                            *
 * The code for these routines is disabled if the debug module is not defined *
 * or if it is explicitly disabled.                                           *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
*/

    /* functions used by uart-file-service (user should not call them directly) */
    int LatticeMico32_JTAGFileRead(MicoFileDesc_t *fd, char *buffer, unsigned int bytes);
    int LatticeMico32_JTAGFileWrite(MicoFileDesc_t *fd, const char *buffer, unsigned int bytes);
    int LatticeMico32_JTAGFileIsAtty(MicoFileDesc_t *fd);
    int LatticeMico32_JTAGFileOpen(MicoFileDesc_t *fd, const char*filename);
    void LatticeMico32_JTAGFileInit(LatticeMico32Ctx_t *pCPU);



    /* functions defined elsewhere within the library */
    int LatticeMico32DbgRead(int, char *, int);
    int LatticeMico32DbgWrite(int, const char *, int);
    int LatticeMico32DbgOpen(const char *name, int flags, int mode);
    int LatticeMico32DbgClose(int file);

#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* _LATTICEMICO32_UART_HEADER_FILE_ */

