/****************************************************************************
**
**  Name: MicoSPI.h
**
**  Description:
**        Declares prototypes of functions for manipulating LatticeMico32 SPI
**
**  Revision: 3.0
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

#ifndef MICOSPI_HEADER_FILE_
#define MICOSPI_HEADER_FILE_
#include "DDStructs.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**********************************************************
 * DATA TYPES, MACROS, CONSTANTS etc.                     *
 **********************************************************/
#define MICOSPI_ERR_SLAVE_DEVICE       (1)
#define MICOSPI_ERR_WOULD_BLOCK        (2)
#define MICOSPI_ERR_INVALID_PARAMETER  (3)

typedef void(*MicoSPIDataHandler_t)(void);



/**********************************************************
 * FUNCTIONS                                              *
 **********************************************************/
    void MicoSPIInit(MicoSPICtx_t *ctx);
    unsigned int MicoSPISetSlaveEnable(MicoSPICtx_t *ctx, unsigned int mask);
    unsigned int MicoSPIGetSlaveEnable(MicoSPICtx_t *ctx, unsigned int *pMask);
	unsigned int MicoSPIIsTxDone(MicoSPICtx_t *ctx);
    unsigned int MicoSPITxData(MicoSPICtx_t *ctx, unsigned int data, unsigned int bBlock);
    unsigned int MicoSPIRxData(MicoSPICtx_t *ctx, unsigned int *pData, unsigned int bBlock);
    unsigned int MicoSPIEnableTxIntr(MicoSPICtx_t *ctx, MicoSPIDataHandler_t handler);
    void MicoSPIDisableTxIntr(MicoSPICtx_t *ctx);
    unsigned int MicoSPIEnableRxIntr(MicoSPICtx_t *ctx, MicoSPIDataHandler_t handler);
    void MicoSPIDisableRxIntr(MicoSPICtx_t *ctx);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* MICOUART_HEADER_FILE_ */

