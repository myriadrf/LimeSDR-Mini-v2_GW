/****************************************************************************
**
**  Name: MicoUartService.h
**
**  Description:
**        Implements functions that allows any LatticeMico32 UART instance
**        to be used for simple file operations such as printf, fprintf,
**        fgetc, gets etc.  Not all file operations are supported; the goal
**        is to provide minimal support for standard input/output support
**
**        User does not need to manipulate these functions.
**
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

#ifndef MICO32_UARTSERVICE_HEADER_FILE
#define MICO32_UARTSERVICE_HEADER_FILE

#include "DDStructs.h"
#include "LookupServices.h"
#include "MicoFileDevices.h"
#include "MicoUart.h"



/*
 * If not defined, defines maximum rs-232 uart
 * devices that can be registered as file-devices.
 */
#ifndef __RS232_UART_MAX_UART_FDEVS__
#define __RS232_UART_MAX_UART_FDEVS__       (3)
#endif



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
 ******************************************************************************
 ******************************************************************************
*/


    /* functions used by uart-file-service (user should not call them directly) */
    int LatticeMico32_UartFileRead(MicoFileDesc_t *fd, char *buffer, unsigned int bytes);
    int LatticeMico32_UartFileWrite(MicoFileDesc_t *fd, const char *buffer, unsigned int bytes);
    int LatticeMico32_UartFileIsAtty(MicoFileDesc_t *fd);
    int LatticeMico32_UartFileOpen(MicoFileDesc_t *fd, const char*filename);
    void LatticeMico32_UartFileInit(MicoUartCtx_t *pUart);


#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* MICO32_UARTSERVICE_HEADER_FILE */

