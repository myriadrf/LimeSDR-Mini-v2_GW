/*
 * Disclaimer:
 *
 *   This source code is intended as a design reference
 *   which illustrates how these types of functions can be implemented.
 *   It is the user's responsibility to verify their design for
 *   consistency and functionality through the use of formal
 *   verification methods.  Lattice Semiconductor provides no warranty
 *   regarding the use or functionality of this code.
 *
 * --------------------------------------------------------------------
 *
 *                     Lattice Semiconductor Corporation
 *                     5555 NE Moore Court
 *                     Hillsboro, OR 97214
 *                     U.S.A
 *
 *                     TEL: 1-800-Lattice (USA and Canada)
 *                          408-826-6000 (other locations)
 *
 *                     web: http://www.latticesemi.com/
 *                     email: techsupport@latticesemi.com
 *
 * --------------------------------------------------------------------
 *
 *  Project:           LatticeMico32 UART driver
 *  File:              MicoUart.h
 *  Description:       Header File for interacting with LatticeMico32 UART
 *
 *----------------------------------------------------------------------------
 *  Change History (Since Feb 12, 2007)
 *
 *  Ver   Date            Change
 * ---------------------------------------------------------------------------
 *  3.0  Mar-25-2008:     updated version field
 *
 *       Mar 05, 2008:    added LSR bits for PE and FE.  Also, added description
 *                        for user-callable functions
 *
 *----------------------------------------------------------------------------
 */


#ifndef MICOUART_HEADER_FILE_
#define MICOUART_HEADER_FILE_
#include "DDStructs.h"

/*
 *----------------------------------------------------------
 * UART SPECIFIC PREPROCESSOR DEFINES:
 *----------------------------------------------------------
 * _MICOUART_NO_INTERRUPTS_
 *   This precompiler definition, if defined, compiles-out
 *   code from the driver that relies on interrupt-driven
 *   behavior.  This precompiler setting affects ALL
 *   uart instances i.e. if defined, all uarts will operate
 *   in a non-interrupt mode.  This define tends to reduce
 *   the UART driver code-size as it compiles-out code.
 *   If any of your uart relies on interrupt-driven operation
 *   do not define this precompiler option.
 * 
 * 
 * _MICOUART_FILESUPPORT_DISABLED_
 *   This precompiler definition, if defined, compiles-out
 *   code specific to the UART that allows it to operate as a
 *   file-device.  This precompiler definition affects ALL
 *   uart instances i.e. if defined, none of the UART instances
 *   will be available for file-operations (printf, write, read,
 *   scanf); however, direct UART access through driver API can
 *   still be performed for sending/receiving data over the UART.
 *
 */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/***************************************************************
 ***************************************************************
 *                                                             *
 *  UART PHYSICAL DEVICE SPECIFIC INFORMATION                  *
 *                                                             *
 ***************************************************************
 ***************************************************************/

    /*
     ------------------------------------------------------
     - UART registers specific bit definitions used       -
     - in the driver implementation                       -
     ------------------------------------------------------
     */
    /* LINE STATUS REGISTER BITS USED IN THE CODE */
    #define MICOUART_LSR_RX_RDY_MASK    (0x01)
    #define MICOUART_LSR_PE_MASK        (0x04)
    #define MICOUART_LSR_FE_MASK        (0x08)
    #define MICOUART_LSR_TX_RDY_MASK    (0x20)


    /* INTERRUPT ENABLE REGISTER BITS USED IN THE CODE */
    #define MICOUART_IER_RX_INT_MASK    (0x01)
    #define MICOUART_IER_TX_INT_MASK    (0x02)


    /* INTERRUPT IDENTIFICATION REGISTER BITS USED IN THE CODE */
    #define MICOUART_IIR_RXRDY          (0x04)
    #define MICOUART_IIR_TXRDY          (0x02)


    /*
     ------------------------------------------------------
     -                                                    -
     - UART Device Register-map                           -
     -                                                    -
     ------------------------------------------------------
     */
    typedef struct st_MicoUart{
        volatile unsigned char rxtx;
        volatile unsigned char ier;
        volatile unsigned char iir;
        volatile unsigned char lcr;
        volatile unsigned char mcr;
        volatile unsigned char lsr;
        volatile unsigned char msr;
        volatile unsigned short div;
    }MicoUart_t;


/***************************************************************
 ***************************************************************
 *                                                             *
 *  UART SOFTWARE DRIVER INTERFACE                             *
 *                                                             *
 ***************************************************************
 ***************************************************************/
    /*
     ------------------------------------------------------
     -                                                    -
     - CONSTANTS AND TYPEDEFS                             -
     -                                                    -
     ------------------------------------------------------
     */

    #define MICOUART_ERR_WOULD_BLOCK        (1)
    #define MICOUART_ERR_DEVICE_ERROR       (2)
    #define MICOUART_ERR_INVALID_ARGUMENT   (3)


    /*
     ------------------------------------------------------
     -                                                    -
     - USER FUNCTIONS                                     -
     -                                                    -
     ------------------------------------------------------
     */
    /*
     **************************************************************
     * initializes a UART instance
     *
     * Argument:
     * MicoUartCtx_t *ctx: context to a UART (declared in DDStructs.h)
     *
     * Notes:
     * This function must be called before using a UART.
     * For managed make, this function is called by LatticeDDInit
     * and so this is done before main() automatically.
     *
     **************************************************************
     */
    void MicoUartInit(MicoUartCtx_t *ctx);


    /*
     **************************************************************
     * Put a character in a UART for transmission
     *
     * Argument:
     * MicoUartCtx_t *ctx: context to a UART (declared in DDStructs.h)
     * unsigned char ucChar: character to transmit
     *
     * Notes:
     * The blocking behavior is defined in the UART context and can
     * be controlled by MicoUart_setBlockMode function.  Please refer
     * to the UART documentation for more information
     *
     **************************************************************
     */
    unsigned int MicoUart_putC(MicoUartCtx_t *ctx, unsigned char ucChar);


    /*
     **************************************************************
     * get a character from a UART
     *
     * Argument:
     * MicoUartCtx_t *ctx: context to a UART (declared in DDStructs.h)
     * unsigned char *pucChar: pointer to unsigned char location for
     *      storing the received character
     *
     * Notes:
     * The blocking behavior is defined in the UART context and can
     * be controlled by MicoUart_setBlockMode function.  Please refer
     * to the UART documentation for more information
     *
     **************************************************************
     */
    unsigned int MicoUart_getC(MicoUartCtx_t *ctx, unsigned char *pucChar);


    /*
     **************************************************************
     * sets UART baud rate
     *
     * Argument:
     * MicoUartCtx_t *ctx: context to a UART (declared in DDStructs.h)
     * unsigned int baudrate: A supported baudrate value (e.g. 115200,
     *         9600 etc).
     *
     * Notes:
     * Please refer to the UART documentation for supported Baud rates.
     * This function uses floating point operations as required by the
     * UART RTL implementation and can add significant code-size.
     * You may opt to set the baudrate in the platform (MSB) if you don't
     * anticipate changing the baudrate at runtime.
     *
     **************************************************************
     */
    unsigned int MicoUart_setRate(MicoUartCtx_t *ctx, unsigned int baudrate);

    /*
     **************************************************************
     * sets blocking behavior for receive and transmit functions
     *
     * Argument:
     * MicoUartCtx_t *ctx: context to a UART (declared in DDStructs.h)
     * unsigned int uiBlock: Pass 1 if you want the rx/tx to block,
     *     else pass 0 if you don't want the rx/tx to block.
     *
     * Notes:
     * Blocking behavior affects receive and transmit i.e. not
     * individually.  If blocking is disabled, you must check
     * putC and getC for MICOUART_ERR_WOULD_BLOCK return value
     * which indicates that the functions would block but the UART
     * software is set to operate in non-blocking mode.
     *
     **************************************************************
     */
    unsigned int MicoUart_setBlockMode(MicoUartCtx_t *ctx, unsigned int uiBlock);


    /*
     ***************************************************************
     * Changes datawidth, parity-selection and stop-bit selection
     *
     * Argument:
     *
     * MicoUartCtx_t *ctx: context to uart
     * unsigned int dwidth: data width in bits(5, 6, 7, 8)
     * unsigned char parity_en: 0 => no pareity, 1 => parity enabled
     * unsigned char even_odd (parity): - 1 => even, 0 => odd
     * unsigned int stopbits: stop bits (1,2)
     *
     ***************************************************************
     */
    unsigned int MicoUart_dataConfig(MicoUartCtx_t *ctx,
                                     unsigned int dwidth,
                                     unsigned char parity_en,
                                     unsigned char even_odd,
                                     unsigned int stopbits);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MICOUART_HEADER_FILE_ */

