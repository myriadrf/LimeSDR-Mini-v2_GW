/****************************************************************************
**
**  Name: OpenCoresI2CMaster.h
**
**  Description:
**       Header file declaring structures/functions for 
**  Opencores I2C Master RTL (written by  Richard Herveille, available 
**  at Opencores.org) sample driver.  The RTL is modified to support
**  32-bit framework used by LatticeMico32 platforms.
**
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

#ifndef MICO32_OCI2CM_HEADER_FILE
#define MICO32_OCI2CM_HEADER_FILE

#include "DDStructs.h"

/****************************************************************************
 * The MicoI2CM driver provides the ability to use an OPENCORES I2C Master  *
 * in either the 100 Kbps Standard or 400 Kbps Fast mode.                   *
 *                                                                          *
 * Please refer to the provided Opencores I2C Master datasheet for more     *
 * information includnig licensing and terms of use.                        *
 * (Core developed by Richard Herveille, contributed to Opencores.org)      *
 *                                                                          *
 ****************************************************************************/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/******************************************************************************
 * 
 * Data Structures & manifest constants                                       *
 * 
 ******************************************************************************/
    /* I2CM callback function type */
    typedef void(*I2CMCallback_t)(void *);


    /*
     **************************************************************************
     *
     * I2C Component Register Structure
     * 
     * Note: The OpenCores I2C Master device has byte-aligned registers.
     *    However, modifications for LatticeMico32 (32-bit addressing)
     *    make the registers word-aligned (4-byte aligned).  Other than
     *    that, the register structure remains unchanged
     *
     **************************************************************************
     */
    typedef struct st_OCI2CMDev_t {
        /* Prescale register: lower byte, R/W */
        volatile unsigned int PrescaleLo;
        /* Prescale register: upper byte, R/W */
        volatile unsigned int PrescaleHi;
        /* Control register: R/W */
        volatile unsigned int Control;
        /* Data register: read = rxdata, write = txdata */
        volatile unsigned int Data;
        /* Command/Status register: read = status, write = command */
        volatile unsigned int StatusCommand;
    }OCI2CMDev_t;


    /*
     * Pertinent register bit-masks
     */
    /* Control Register */
    #define OCI2CM_CTL_CORE_ENABLE              (0x80) 
    #define OCI2CM_CTL_INT_ENABLE               (0x40)

    /* Status Register  */
    #define OCI2CM_STATUS_RX_ACK                (0x80)
    #define OCI2CM_STATUS_BUS_BUSY              (0x40)
    #define OCI2CM_STATUS_ARB_LOST              (0x20)
    #define OCI2CM_STATUS_TRANSFER              (0x02)
    #define OCI2CM_STATUS_INT_FLAG              (0x01)


    /* Command Register 0x04 */
    #define OCI2CM_CMD_START                    (0x80)
    #define OCI2CM_CMD_STOP                     (0x40)
    #define OCI2CM_CMD_READ                     (0x20)
    #define OCI2CM_CMD_WRITE                    (0x10)
    #define OCI2CM_CMD_NACK                     (0x08)
    #define OCI2CM_CMD_ACK                      (0x00)
    #define OCI2CM_CMD_INT_ACK                  (0x01)
    #define OCI2CM_CMD_CLEAR                    (0x00)


/*
 ******************************************************************************
 *                                                                            *
 *                           functions                                        *
 *                                                                            *
 ******************************************************************************
 */

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
void OpenCoresI2CMasterInit(OpenCoresI2CMasterCtx_t *ctx );


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
void OpencoresI2CMasterDisable( OpenCoresI2CMasterCtx_t *ctx );


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterEnable( OpenCoresI2CMasterCtx_t *ctx );
 * 
 * 
 * Enables OpenCores I2C Master instance specified by the device context
 * parameter (ctx).
 * 
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *
 * Return Value:
 *    none
 *****************************************************************************
 */
void OpenCoresI2CMasterEnable( OpenCoresI2CMasterCtx_t *ctx );

/*
 *****************************************************************************
 *
 * int OpenCoresI2CMasterStart( OpenCoresI2CMasterCtx_t *ctx )
 *
 * 
 * Issues a start; this must be done prior to performing any write or
 * read operations.  This is normally not required since reads/writes
 * issue a start before performing a read/write transaction.
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
int OpenCoresI2CMasterStart( OpenCoresI2CMasterCtx_t *ctx );


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
void OpenCoresI2CMasterStop( OpenCoresI2CMasterCtx_t *ctx );


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
                                unsigned char *data);



/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterWriteByte( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned char data)
 *
 * performs a single-byte write
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for write operation
 *    unsigned char data          : byte to write
 *
 *
 * Return Value:
 *    int:
 *         0 => successful writes
 *         0 => successful writes
 *        -1 => failed to receive ack during addressing
 *        -2 => failed to receive ack when writing data
 *        -3 => arbitration lost during the operation
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterWriteByte( OpenCoresI2CMasterCtx_t *ctx,
                                    unsigned int address,
                                    unsigned char data);


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
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for read operation
 *    unsigned int buffersize     : bytes to read
 *    unsigned char *data         : pointer for storing bytes read; must have
 *                                  sufficient space to read buffersize bytes
 *
 *
 * Return Value:
 *    int:
 *    buffersize => read requested number of bytes (buffersize)
 *    -1         => failed to receive ack during addressing
 *    -3         => arbitration lost during the operation
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterRead( OpenCoresI2CMasterCtx_t *ctx,
                                unsigned int address,
                                unsigned int buffersize,
                                unsigned char *data);


/*
 *****************************************************************************
 *
 * void OpenCoresI2CMasterReadByte( OpenCoresI2CMasterCtx_t *ctx, 
 *                               unsigned int address,
 *                               unsigned char* data)
 *
 * performs a single byte read
 *
 * Note: This function issues a start but does not issue a stop when done; 
 *   user application must explicitly issue a stop (OpenCoresI2CMasterStop)
 *    when ready to relinquish the bus.
 *
 * Arguments:
 *    OpenCoresI2CMasterCtx_t *ctx: Opencores I2C Master context
 *    unsigned int address        : slave device address for read operation
 *    unsigned char *data         : pointer for storing the read byte
 *
 * Return Value:
 *    int:
 *     1 : read a single byte
 *    -1 => failed to receive ack during addressing
 *    -3 => arbitration lost during the operation
 *
 *****************************************************************************
 */
int OpenCoresI2CMasterReadByte( OpenCoresI2CMasterCtx_t *ctx,
                                    unsigned int address,
                                    unsigned char *data);


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /*MICO32_OCI2CM_HEADER_FILE */

