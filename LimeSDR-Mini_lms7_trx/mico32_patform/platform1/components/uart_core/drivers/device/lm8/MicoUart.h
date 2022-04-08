#include <stddef.h>
#include "DDStructs.h"

/***********************************************************************
 *                                                                     *
 * UART PHYSICAL DEVICE SPECIFIC INFORMATION                           *
 *                                                                     *
 ***********************************************************************/

/* REGISTER OFFSETS FROM BASE ADDRESS */
#define MICOUART_RXTX            0x00
#define MICOUART_IER             0x01
#define MICOUART_IIR             0x02
#define MICOUART_LCR             0x03
#define MICOUART_MCR             0x04
#define MICOUART_LSR             0x05
#define MICOUART_MSR             0x06
#define MICOUART_DIVL            0x08
#define MICOUART_DIVH            0x09

/* LINE STATUS REGISTER BITS USED IN CODE */
#define MICOUART_LSR_RX_RDY_MASK 0x01
#define MICOUART_LSR_PE_MASK     0x04
#define MICOUART_LSR_FE_MASK     0x08
#define MICOUART_LSR_TX_RDY_MASK 0x20

/* INTERRUPT ENABLE REGISTER BITS USED IN THE CODE */
#define MICOUART_IER_RX_INT_MASK 0x01
#define MICOUART_IER_TX_INT_MASK 0x02


/* INTERRUPT IDENTIFICATION REGISTER BITS USED IN THE CODE */
#define MICOUART_IIR_RXRDY       0x04
#define MICOUART_IIR_TXRDY       0x02

/***********************************************************************
 *                                                                     *
 * USER FUNCTIONS                                                      *
 *                                                                     *
 ***********************************************************************/

void MicoUartInit (MicoUartCtx_t *ctx);

void MicoUart_getC (MicoUartCtx_t *ctx, char *aChar);

void MicoUart_putC (MicoUartCtx_t *ctx, char aChar);

#ifndef __MICO_NO_INTERRUPTS__
void MicoUartISR (MicoUartCtx_t *ctx);
#endif

#ifdef __MICOUART_MISC__
unsigned int MicoUart_dataConfig (MicoUartCtx_t *ctx,
				  unsigned int dwidth,
				  unsigned char parity_en,
				  unsigned char even_odd,
				  unsigned int stopbits);
#endif

/***********************************************************************
 *                                                                     *
 * USER MACROS                                                         *
 *                                                                     *
 ***********************************************************************/

#define MICO_UART_RD_RBR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_RXTX))
#define MICO_UART_WR_THR(X,Y)   (__builtin_export((char)Y, ((size_t)X+MICOUART_RXTX)))

#define MICO_UART_RD_IER(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_IER))
#define MICO_UART_WR_IER(X,Y)   (__builtin_export((char)Y, ((size_t)X+MICOUART_IER)))

#define MICO_UART_RD_IIR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_IIR))
#define MICO_UART_WR_IIR(X,Y)   (__builtin_export((char)Y, ((size_t)X+MICOUART_IIR)))

#define MICO_UART_RD_LCR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_LCR))
#define MICO_UART_WR_LCR(X,Y)   (__builtin_export((char)Y, ((size_t)X+MICOUART_LCR)))

#define MICO_UART_RD_LSR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_LSR))

#define MICO_UART_RD_MCR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_MCR))
#define MICO_UART_WR_MCR(X,Y)   (__builtin_export((char)Y, ((size_t)X+MICOUART_MCR)))

#define MICO_UART_RD_MSR(X,Y)   (Y)=(__builtin_import((size_t)X+MICOUART_MSR))

#define MICO_UART_RD_DIV(X,Y)   \
  (Y)=(((unsigned short)__builtin_import((size_t)X+MICOUART_DIVH)<<8) | (unsigned short)__builtin_import((size_t)X+MICOUART_DIVL))
#define MICO_UART_WR_DIV(X,Y)   \
  (__builtin_export((char)(Y>>8), ((size_t)X+MICOUART_DIVH)),	\
   __builtin_export((char)Y, ((size_t)X+MICOUART_DIVL)) )

/***********************************************************************
 *                                                                     *
 * ERROR CODES                                                         *
 *                                                                     *
 ***********************************************************************/

#define MICOUART_ERR_WOULD_BLOCK      1
#define MICOUART_ERR_DEVICE_ERROR     2
#define MICOUART_ERR_INVALID_ARGUMENT 3

