/****************************************************************************
**
**  Name: MicoSPI.c
**
**  Description:
**        Implements functions for manipulating LatticeMico32 SPI 
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

#include "MicoSPIService.h"
#include "MicoSPI.h"
#include "MicoInterrupts.h"
#include "MicoUtils.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /* status register masks */
    #define MICOSPI_STATUS_RX_ERR_MASK      (0x008)
    #define MICOSPI_STATUS_TX_ERR_MASK      (0x010)
    #define MICOSPI_STATUS_TX_RDY_MASK      (0x040)
    #define MICOSPI_STATUS_RX_RDY_MASK      (0x080)

    /* control-register masks */
    #define MICOSPI_CTL_RX_ERR_INTR_EN_MASK (0x008)
    #define MICOSPI_CTL_TX_ERR_INTR_EN_MASK (0x010)
    #define MICOSPI_CTL_TX_INTR_EN_MASK     (0x040)
    #define MICOSPI_CTL_RX_INTR_EN_MASK     (0x080)



	/****************************
	 * SPI Register Structure
	 ****************************/
	typedef struct st_MicoSPI{
        volatile unsigned int rx;
        volatile unsigned int tx;
        volatile unsigned int status;
        volatile unsigned int control;
        volatile unsigned int sSelect;
	}MicoSPI_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */


/* MICO-SPI Interrupt handler */
static void MicoSPIISRHandler(unsigned int intrLevel, void *data)
{
    unsigned int uiValue;
    volatile MicoSPI_t *spi;
    MicoSPICtx_t *ctx = (MicoSPICtx_t *)data;
    spi = (volatile MicoSPI_t *)(ctx->base);


    /* check the cause of interrupt */
    uiValue = spi->status;


    /* received new data */
    if(uiValue & MICOSPI_STATUS_RX_RDY_MASK){
        if(ctx->control & MICOSPI_CTL_RX_INTR_EN_MASK)
            ((MicoSPIDataHandler_t)ctx->onRx)();
    }


    /* transmitter's ready to accept new data */
    if(uiValue & MICOSPI_STATUS_TX_RDY_MASK){
        if(ctx->control & MICOSPI_CTL_TX_INTR_EN_MASK)
            ((MicoSPIDataHandler_t)ctx->onTx)();
    }


    return;
}



/*
 * Initializes SPI instance
 */
void MicoSPIInit(MicoSPICtx_t *ctx)
{
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);


    /* disable all interrupts and unforce sso if master */
    spi->control = 0;
    ctx->control = 0;


    /* if master, deselect all slaves */
    if(ctx->master != 0){
        spi->sSelect = 0;
    }


    /* register interrupt-handler */
    MicoRegisterISR(ctx->intrLevel, (void *)ctx, MicoSPIISRHandler);


    /* make this SPI device lookup-able */
    ctx->lookupReg.name = ctx->name;
    ctx->lookupReg.deviceType = "SPIDevice";
    ctx->lookupReg.priv = ctx;
    MicoRegisterDevice(&(ctx->lookupReg));


    /* all done! */
    return;
}



/*
 * Enables slave/s (applicable for masters only)
 */
unsigned int MicoSPISetSlaveEnable(MicoSPICtx_t *ctx, unsigned int mask)
{
    volatile MicoSPI_t *spi;

    if(ctx->master == 0)
        return(MICOSPI_ERR_SLAVE_DEVICE);

    spi = (volatile MicoSPI_t *)(ctx->base);
    spi->sSelect = mask;

    return(0);
}



/*
 * Fetches bit-mask for enabled-slaves
 */
unsigned int MicoSPIGetSlaveEnable(MicoSPICtx_t *ctx, unsigned int *pMask)
{
    unsigned int iMask;
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);

    if(ctx->master == 0)
        return(MICOSPI_ERR_SLAVE_DEVICE);

    iMask = spi->sSelect;

    if(pMask != 0)
        *pMask = iMask;

    return(0);
}



/*
 * Transmits data
 */
unsigned int MicoSPITxData(MicoSPICtx_t *ctx, unsigned int data, unsigned int bBlock)
{
    unsigned int uiValue;
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);

    do{
        /* read status register */
        uiValue = spi->status;
        if((uiValue & MICOSPI_STATUS_TX_RDY_MASK) != 0){
            spi->tx = data;
            return(0);
        }


        /* if non-blocking, return an error */
        if(bBlock == 0)
            return(MICOSPI_ERR_WOULD_BLOCK);

    }while(1);
}

/*
 * Checks if shift-register is done;
 * returns non-zero if tx shift register is empty
 * returns 0 otherwise
 */
unsigned int MicoSPIIsTxDone(MicoSPICtx_t *ctx)
{
	unsigned int uiValue;
	volatile MicoSPI_t *spi;
	spi = (volatile MicoSPI_t *)(ctx->base);
	uiValue = spi->status;
	return(uiValue & 0x20);
}


/*
 * receives data
 */
unsigned int MicoSPIRxData(MicoSPICtx_t *ctx, unsigned int *pData, unsigned int bBlock)
{
    unsigned int uiValue;
    volatile MicoSPI_t *spi;

    spi = (volatile MicoSPI_t *)(ctx->base);

    do{
        /* read status register */
        uiValue = spi->status;
        if( (uiValue & MICOSPI_STATUS_RX_RDY_MASK) != 0){
            if(pData != 0)
                *pData = spi->rx;

            return(0);
        }

        /* if non-blocking, return an error */
        if(bBlock == 0)
            return(MICOSPI_ERR_WOULD_BLOCK);
    }while(1);
}



/*
 * enables tx-ready interrupt handler
 */
unsigned int MicoSPIEnableTxIntr(MicoSPICtx_t *ctx, MicoSPIDataHandler_t handler)
{
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);


    if(handler == 0)
        return(MICOSPI_ERR_INVALID_PARAMETER);


    /* register the handler and enable transmit interrupts */
    ctx->onTx = (void *)handler;
    ctx->control |= MICOSPI_CTL_TX_INTR_EN_MASK;
    spi->control = ctx->control;
    return(0);
}



/*
 * disables tx-ready interrupt
 */
void MicoSPIDisableTxIntr(MicoSPICtx_t *ctx)
{
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);
    spi->control = 0;
    ctx->control &= (~MICOSPI_CTL_TX_INTR_EN_MASK);
    spi->control = ctx->control;
    ctx->onTx = (void *)0;
    return;
}



/*
 * enables rx-ready interrupt
 */
unsigned int MicoSPIEnableRxIntr(MicoSPICtx_t *ctx, MicoSPIDataHandler_t handler)
{
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);

    if(handler == 0)
        return(MICOSPI_ERR_INVALID_PARAMETER);

    /* register handler and enable receive interrupts */
    ctx->onRx = (void *)handler;
    ctx->control |= MICOSPI_CTL_RX_INTR_EN_MASK;
    spi->control = ctx->control;
    return(0);
}



/*
 * disables rx-ready interrupt
 */
void MicoSPIDisableRxIntr(MicoSPICtx_t *ctx)
{
    volatile MicoSPI_t *spi;
    spi = (volatile MicoSPI_t *)(ctx->base);
    spi->control = 0;
    ctx->control &= (~MICOSPI_CTL_RX_INTR_EN_MASK);
    spi->control = ctx->control;
    ctx->onRx = (void *)0;
    return;
}

