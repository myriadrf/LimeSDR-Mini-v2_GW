/****************************************************************************
**
**  Name: OpenCoresI2CMaster.c
**
**  Description:
**       Sample driver routines for Opencores I2C Master RTL (written by
**  Richard Herveille, available at Opencores.org)  The RTL is modified to
**  support 32-bit framework used by LatticeMico32 platforms.
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
**  3.1   Jun-10-2008  Initial
**
**---------------------------------------------------------------------------
*****************************************************************************/

#include "OpenCoresI2CMaster.h"
#include "OpenCoresI2CMasterService.h"
#include "MicoMacros.h"

// Slave Address
#define OCI2CM_RW_MASK                  (0x01) // 1=Read,0=Write
#define OCI2CM_ADDR_MASK                (0xFE)


/* MACRO FOR WAITING TILL CURRENT TRANSFER IS IN PROGRESS */
#define OCI2CM_PEND_FOR_TIP_DONE(ADDR)  \
    while( *(volatile unsigned int *)(ADDR + 0x10) & OCI2CM_STATUS_TRANSFER )

/*
 ****************************************************************************
 *
 * void OpenCoresI2CMasterInit(OpenCoresI2CMasterCtx_t *ctx );
 *
 * 
 * Initializes Opencores I2C Master instance, specified by the
 * device context parameter (ctx).  This function enables Opencores I2C Master
 * on initialization, keeping interrupts disabled
 *
 * 
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 * 
 * Return Value:
 *    none
 *****************************************************************************
 */
void OpenCoresI2CMasterInit(OpenCoresI2CMasterCtx_t *ctx )
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);
    
    /* Calculate processed-speed here */
    ctx->speed = ((MICO32_CPU_CLOCK_MHZ/(5*ctx->speed*1000)-1));            /* Calculate I2C SCL Speed */
    pDevStruct->Control = OCI2CM_CTL_CORE_ENABLE;                           /* enable core, disable ints */
    pDevStruct->PrescaleLo = ctx->speed;                                    /* Load Prescaler LSB */
    pDevStruct->PrescaleHi = 0;                                             /* Load Prescaler MSB */
    pDevStruct->Data = 0;                                                   /* Clear Tx Data */
    pDevStruct->StatusCommand = 0;                                          /* Clear out all commands */


#ifndef _OPENCORES_I2C_NOT_LOOKUPABLE_

    /* register this device for lookuop "OCI2CMService" */
    ctx->lookupReg.name = ctx->name;
    ctx->lookupReg.deviceType = "OCI2CMDevice";
    ctx->lookupReg.priv = ctx;
    MicoRegisterDevice(&(ctx->lookupReg));
#endif

    return;
}


/*
 *****************************************************************************
 *
 * void OpencoresI2CMasterDisable( OpenCoresI2CMasterCtx_t *ctx );
 *
 *
 * Disables OpenCores I2C Master instance specified by the device context
 * parameter (ctx).  Subsequent Opencores I2C Master operations will fail
 * unless the core is explicitly enabled
 *
 * Note: Be careful when disabling the core as it can hang the bus based on
 *    the current state of the core (and any pending transactions)
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 *
 * Return Value:
 *    none
 *****************************************************************************
 */
void OpenCoresI2CMasterDisable( OpenCoresI2CMasterCtx_t *ctx )
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);
    pDevStruct->Control &= ~OCI2CM_CTL_CORE_ENABLE; 
    return;
}


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterEnable( OpenCoresI2CMasterCtx_t *ctx );
 * 
 * 
 * Enables OpenCores I2C Master instance specified by the device context
 * parameter (ctx).  Subsequent Opencores I2C Master operations will fail
 * unless the core is explicitly enabled
 * 
 * 
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 * Return Value:
 *    none
 *****************************************************************************
 */
void OpenCoresI2CMasterEnable( OpenCoresI2CMasterCtx_t *ctx )
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);    
    pDevStruct->Control |= OCI2CM_CTL_CORE_ENABLE;  
    return;
}

/*
 *****************************************************************************
 *
 * int OpenCoresI2CMasterStart( OpenCoresI2CMasterCtx_t *ctx )
 *
 * 
 * Issues a start; this must be done prior to performing any write or
 * read operations.  Separating start from a read or write operation
 * allows for 
 *
 * 
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 * 
 * Return Value:
 *    0 if successful
 *    non-zero if failure (arbitration lost), in which case you must
 *       re-issue a start
 *****************************************************************************
 */
int OpenCoresI2CMasterStart( OpenCoresI2CMasterCtx_t *ctx )
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);

    /* wait for pending transaction to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* issue a start */
    pDevStruct->StatusCommand = OCI2CM_CMD_START;

    /*
     * see if arbitration was lost; return 0 if arbitration
     * wasn't lost i.e. start was successful
     */
    return ( pDevStruct->StatusCommand & OCI2CM_STATUS_ARB_LOST );
}

/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterStop( OpenCoresI2CMasterCtx_t *ctx )
 * 
 * 
 * Issues a stop i.e. relinquishes the bus.  This must be done once 
 * all relevant read/write operations are complete.  Issuing a stop 
 * gives up ownership of the bus that was acquired when issuing a start
 *
 * Note: This function does not stop the core i.e. does not disable the
 * core; rather it relinquishes control of the bus to allow other masters
 * to arbitrate for bus-access.
 * 
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 * 
 * Return Value:
 *    none
 *****************************************************************************
 */
void OpenCoresI2CMasterStop( OpenCoresI2CMasterCtx_t *ctx )
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);

    /* wait for pending transaction to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* issue a stop */
    pDevStruct->StatusCommand = OCI2CM_CMD_STOP;

    /* all done */
    return;
}


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterWrite( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned int buffersize,
 *                               unsigned char *data)
 *
 * performs block-write; it is contingent on a successful start i.e. ownership
 * of the bus.
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for write operation
 *    unsigned int buffersize     : number of bytes to write
 *    unsigned char *data         : pointer to bytes to write
 *
 * Return Value:
 *    int:
 *         0 => successful writes
 *        -1 => failed to receive ack during addressing
 *        -2 => failed to receive ack when writing data
 *        -3 => arbitration lost during the operation
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterWrite( OpenCoresI2CMasterCtx_t *ctx,
                                unsigned int address,
                                unsigned int buffersize,
                                unsigned char *data)
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);

    unsigned int i;

    /* load address, set-up a write to write the address out to the bus */
    pDevStruct->Data = ((address<<1) & ~OCI2CM_RW_MASK);

    /* initiate a start (repeated) and write out the address */
    pDevStruct->StatusCommand = (OCI2CM_CMD_START|OCI2CM_CMD_WRITE);
    //pDevStruct->StatusCommand = (OCI2CM_CMD_WRITE);

    /* wait for transfer to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* check if ack is received */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
    {
        return -1;
    }

    /* transfer the bytes */
    for ( i = 0; i < buffersize; ++i, ++data )
    {
        /* load data in register */
        pDevStruct->Data = (unsigned int)(*data);


        /* write the data out to the bus  */
        pDevStruct->StatusCommand = OCI2CM_CMD_WRITE;


        /* wait for current transfer to complete */
        OCI2CM_PEND_FOR_TIP_DONE(ctx->base);


        /* check for ack */
        if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
        {
            return -2;
        }
    }

    /* see if arbitration was lost during this transaction */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_ARB_LOST )
    {
        return -3;
    }
    else
    {
        return 0;
    }
}


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterWriteByte( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned char data)
 *
 * performs a single-byte write
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for write operation
 *    unsigned char data          : byte to write
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 * Return Value:
 *    int:
 *         0 => successful writes
 *        -1 => failed to receive ack during writes
 *        -2 => failed to receive ack when writing data
 *        -3 => arbitration lost during the operation
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterWriteByte( OpenCoresI2CMasterCtx_t *ctx,
                                    unsigned int address,
                                    unsigned char data)
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);

    /* load address, set-up a write to write the address out to the bus */
    pDevStruct->Data = ((address<<1) & ~OCI2CM_RW_MASK);

    /* initiate a start (repeated) and write out the address */
    pDevStruct->StatusCommand = (OCI2CM_CMD_START|OCI2CM_CMD_WRITE);

    /* wait for transfer to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* check if ack is received */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
    {
        return -1;
    }

    /* load data in register */
    pDevStruct->Data = (unsigned int)data;

    /* write the data out to the bus */
    pDevStruct->StatusCommand = OCI2CM_CMD_WRITE;

    /* wait for current transfer to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* check for ack  and if arb was lost during this transaction */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
    {
        return -2;
    }
    else if( pDevStruct->StatusCommand & OCI2CM_STATUS_ARB_LOST )
    {
        return -3;
    }
    else
    {
        return 0;
    }
}


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterRead( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned int buffersize,
 *                               unsigned char* data)
 *
 * performs block-read
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for read operation
 *    unsigned int buffersize     : bytes to read
 *    unsigned char *data         : pointer for storing bytes read; must have
 *                                  sufficient space to read buffersize bytes
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 *
 * Return Value:
 *    int:
 *         returns number of bytes actually read.
 *         the return value may be less than the desired if an error is
 *         encountered while reading
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterRead( OpenCoresI2CMasterCtx_t *ctx,
                                unsigned int address,
                                unsigned int buffersize,
                                unsigned char *data)
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);
    unsigned int i;

    /* load address, set-up a write to write the address out to the bus */
    pDevStruct->Data = ((address<<1) | (OCI2CM_RW_MASK) );

    /* initiate a start (repeated) and write out the address */
    pDevStruct->StatusCommand = (OCI2CM_CMD_START|OCI2CM_CMD_WRITE);

    /* wait for transfer to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* check if ack is received */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
    {
        return -1;
    }

    /* begin reading data */
    for (i = 0; i < buffersize; ++i, ++data)
    {
        /* read data; send ACK on all reads except the last one */
        if( i == (buffersize - 1) )
        {
            pDevStruct->StatusCommand = (OCI2CM_CMD_READ|OCI2CM_CMD_NACK);
        }
        else
        {
            pDevStruct->StatusCommand = (OCI2CM_CMD_READ|OCI2CM_CMD_ACK);
        }

        /* wait while transfer is in progress */
        OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

        /* read data */
        *data = (unsigned char)(pDevStruct->Data);
    }

    /* check for ack  and if arb was lost during this transaction */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_ARB_LOST )
    {
        return -3;
    }
    else
    {
        return i;
    }
}


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterReadByte( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned char* data)
 *
 * performs read single byte
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for read operation
 *    unsigned char *data         : pointer for storing the read byte
 *
 * Return Value:
 *    int:
 *         returns number of bytes actually read.
 *         the return value may be less than the desired if an error is
 *         encountered while reading.
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterReadByte( OpenCoresI2CMasterCtx_t *ctx,
                                    unsigned int address,
                                    unsigned char *data)
{
    volatile OCI2CMDev_t *pDevStruct;
    pDevStruct = (volatile OCI2CMDev_t *)(ctx->base);

    /* load address, set-up a write to write the address out to the bus */
    pDevStruct->Data = ((address<<1) | (OCI2CM_RW_MASK) );

    /* initiate a start (repeated) and write out the address */
    pDevStruct->StatusCommand = (OCI2CM_CMD_START|OCI2CM_CMD_WRITE);

    /* wait for transfer to complete */
    OCI2CM_PEND_FOR_TIP_DONE(ctx->base);

    /* check if ack is received */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_RX_ACK )
    {
        return -1;
    }

    /* issue a read with a NACK */
    pDevStruct->StatusCommand = (OCI2CM_CMD_READ | OCI2CM_CMD_NACK);

    /* wait till transfer is in progress */
    while(pDevStruct->StatusCommand & OCI2CM_STATUS_TRANSFER);

    /* read data */
    *data = (unsigned char)(pDevStruct->Data);

    /* check for ack  and if arb was lost during this transaction */
    if( pDevStruct->StatusCommand & OCI2CM_STATUS_ARB_LOST )
    {
        return -3;
    }
    else
    {
        return 1;
    }
}


