#include <stddef.h>
#include "MicoUart.h"
#include "MicoInterrupts.h"

/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUartInit(MicoUartCtx_t *ctx)
{
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
	size_t uart_address = ctx->base;
  
	// Use interrupt-mode if interrupts are available
	if (ctx->intrAvail) {
	    // Initialize buffer-related parameters
		ctx->rxDataBytes = 0;
		ctx->rxReadLoc  = 0;
		ctx->rxWriteLoc = 0;
		ctx->txDataBytes = 0;
		ctx->txReadLoc  = 0;
		ctx->txWriteLoc = 0;
		
		// Don't enable transmit interrupt (write-function does that)
	    MICO_UART_WR_IER (uart_address, MICOUART_IER_RX_INT_MASK);
	}
	  
#endif
#endif
}

/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUart_getC (MicoUartCtx_t *ctx, char *aChar)
{
  	char iValue;
  	size_t uart_address = ctx->base;
  
  	do {
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
    
    	if (ctx->intrAvail == 1) {
      
      		if (ctx->rxDataBytes > 0) {
				// At least one byte is available to be read
				*aChar = ctx->rxBuffer[ctx->rxReadLoc];
				ctx->rxReadLoc++;
				if (ctx->rxReadLoc >= ctx->rxBufferSize)
	  				ctx->rxReadLoc = 0;
	  			MicoDisableInterrupt(ctx->intrLevel);
				ctx->rxDataBytes--;
				MICO_UART_RD_IER (uart_address, iValue);
				MICO_UART_WR_IER (uart_address, (iValue | MICOUART_IER_RX_INT_MASK));
				MicoEnableInterrupt(ctx->intrLevel);
				return;
      		}
    	} else 
      
#endif
#endif
		{
      		MICO_UART_RD_LSR (uart_address, iValue);
			      
      		if (iValue & MICOUART_LSR_RX_RDY_MASK) {
				MICO_UART_RD_RBR (uart_address, iValue);
				*aChar = iValue;
				return;
      		}
		}
    
#ifdef __MICOUART_BLOCKING__
    	if (ctx->blockingRx == 0)
      		return;
#endif
  	} while (1);
}

/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUart_putC (MicoUartCtx_t *ctx, char aChar)
{
  	char iValue;
  	size_t uart_address = ctx->base;
  
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
  
  	if (ctx->intrAvail == 1) {
    	// Wait if TX-Buffer is full
    	while (ctx->txDataBytes == ctx->txBufferSize) {
      
#ifdef __MICOUART_BLOCKING__
      		// Return immediately if non-blocking TX
      		if (ctx->blockingTx == 0)
				return;
#endif
    	}
		    
    	// Stuff data in TX-Buffer
    	ctx->txBuffer[ctx->txWriteLoc] = aChar;
		ctx->txWriteLoc++;
		if (ctx->txWriteLoc >= ctx->txBufferSize)
  			ctx->txWriteLoc = 0;
		    
		// Increment tx-databytes and enable transmit interrupt
		MicoDisableInterrupt(ctx->intrLevel);
		ctx->txDataBytes++;
		MICO_UART_RD_IER (uart_address, iValue);
		MICO_UART_WR_IER (uart_address, (iValue | MICOUART_IER_TX_INT_MASK));
		MicoEnableInterrupt(ctx->intrLevel);
		return;
  	} else

#endif
#endif
	{
    	do {
      		MICO_UART_RD_LSR (uart_address, iValue);
			      
      		if (iValue & MICOUART_LSR_TX_RDY_MASK) {
				MICO_UART_WR_THR (uart_address, aChar);
				return;
      		}
      
#ifdef __MICOUART_BLOCKING__
	      	// Return immediately if non-blocking TX
      		if (ctx->blockingTx == 0)
				return;
#endif
    	} while (1);
	}
}

#ifndef __MICO_NO_INTERRUPTS__
/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUartISR (MicoUartCtx_t *ctx)
{
#ifdef __MICOUART_INTERRUPT__
  	char iir, ier, lsr, data;
  	unsigned int count;
  	size_t uart_address = ctx->base;
  	
  	// Interrogate interrupt cause
  	MICO_UART_RD_IIR (uart_address, iir);
  	MICO_UART_RD_IER (uart_address, ier);
  	
  	// Check if interrupt is due to RX Data
  	switch (iir) {
  		case MICOUART_IIR_RXRDY:
    		// Read LSR to check for errors associated with current word. 
    		MICO_UART_RD_LSR (uart_address, lsr);
    		
    		// Read the data into the buffer
    		MICO_UART_RD_RBR (uart_address, data);
    		ctx->rxBuffer[ctx->rxWriteLoc] = data;
    		
    		// Check for error(s) with the current word
    		if ((lsr & (MICOUART_LSR_PE_MASK | MICOUART_LSR_FE_MASK)) == 0) { // No error detected.
    		  
      			// Adjust RX buffer pointers
      			ctx->rxWriteLoc++;
      			if (ctx->rxWriteLoc >= ctx->rxBufferSize) {
					ctx->rxWriteLoc = 0;
      			}
      			ctx->rxDataBytes++;
      			
      			// If buffer is full, disable interrupts
      			if (ctx->rxDataBytes == ctx->rxBufferSize) {
      				MICO_UART_RD_IER (uart_address, data);
					data &= ~MICOUART_IER_RX_INT_MASK; 
					MICO_UART_WR_IER (uart_address, data);
      			}
    		}
    		break;
    
  		case MICOUART_IIR_TXRDY:
    		if (ctx->fifoenable == 1) {
      			// Is there any data to transmit?
      			count = (ctx->txDataBytes > 16) ? 16 : ctx->txDataBytes;
    		}else {
      			count = 1;
    		}
    		
    		while (count != 0) {				
      			if (ctx->txDataBytes == 1) {
					// Disable TX interrupts as this is the last data to send
					MICO_UART_RD_IER (uart_address, data); 
					data &= ~MICOUART_IER_TX_INT_MASK;
					MICO_UART_WR_IER (uart_address, data);
      			}
      			MICO_UART_WR_THR (uart_address, ctx->txBuffer[ctx->txReadLoc]);
      			ctx->txReadLoc++;
      			if (ctx->txReadLoc >= ctx->txBufferSize) {
					ctx->txReadLoc = 0;
      			}
      			--ctx->txDataBytes;
      			--count;
      
      			if (ctx->txDataBytes == 0)
					break;
    		}
    		break;
    
  		default:
    		break;
  	}
  	return;
#endif
}
#endif

#ifdef __MICOUART_BLOCKING__
/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUart_setBlockMode (MicoUartCtx_t *ctx, char uiBlock)
{
  
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
  	// Disable UART interrupt line in LM8
  	MicoDisableInterrupt(ctx->intrLevel);
#endif
#endif
  
  	ctx->blockingTx = uiBlock;
  	ctx->blockingRx = uiBlock;
  
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
  	// Enable UART interrupt line in LM8
  	MicoEnableInterrupt(ctx->intrLevel);
#endif
#endif
}
#endif

#ifdef __MICOUART_MISC__
/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoUart_setRate (MicoUartCtx_t *ctx, unsigned long baudrate)
{
  	unsigned short divisor;
  	size_t uart_address = ctx->base;
  	
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
  	// Disable UART interrupt line in LM8
  	MicoDisableInterrupt(ctx->intrLevel);
#endif
#endif
  	
  	// Calculate clock-divisor from baudrate and program UART.
  	divisor = (MICO8_CPU_CLOCK_MHZ)/baudrate;
  	MICO_UART_WR_DIV (uart_address, divisor);
  	//__builtin_export ((char)(divisor>>8), uart_address+MICOUART_DIVH);
  	//__builtin_export ((char)divisor, uart_address+MICOUART_DIVL);
  	
#ifndef __MICO_NO_INTERRUPTS__
#ifdef __MICOUART_INTERRUPT__
  	// Enable UART interrupt line in LM8
  	MicoEnableInterrupt(ctx->intrLevel);
#endif
#endif
  	
  	return;
}
#endif

