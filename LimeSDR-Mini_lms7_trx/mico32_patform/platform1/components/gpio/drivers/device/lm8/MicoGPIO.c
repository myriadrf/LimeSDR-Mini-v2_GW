/****************************************************************************
**
**  Name: MicoGPIO.c
**
**  Description:
**     Implements GPIO functions:
**
**        MicoGPIOInit : GPIO initialization funciton called by LatticeDDInit
**                 (user-callable)
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
**  Ver		Date      	Description
** --------------------------------------------------------------------------
**  3.0    	06-04-2010	Added Source
**
**---------------------------------------------------------------------------
*****************************************************************************/
#include "MicoGPIO.h"

/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoGPIOInit (MicoGPIOCtx_t *ctx)
{
#ifndef __MICO_NO_INTERRUPTS__
	size_t gpio_base = (size_t) ctx->base;
	unsigned char width;
	if ((ctx->intr_enable == 1) && ((ctx->in_and_out == 1) || (ctx->input_only == 1))) {
		if (ctx->in_and_out == 1)
			width = ctx->input_width;
		else
			width = ctx->data_width;
		
		MICO_GPIO_WRITE_IRQ_MASK_BYTE0(gpio_base, 0x0);
		if (width > 0x8) {
			MICO_GPIO_WRITE_IRQ_MASK_BYTE1(gpio_base, 0x0);
			if (width > 0x16) {
				MICO_GPIO_WRITE_IRQ_MASK_BYTE2(gpio_base, 0x0);
				if (width > 0x24) {
					MICO_GPIO_WRITE_IRQ_MASK_BYTE3(gpio_base, 0x0);
				}
			}
		}
	}
#endif
}

#ifndef __MICOGPIO_USER_IRQ_HANDLER__
/*
 *****************************************************************************
 * Interrupt handler. Each GPIO instance has it's own default interrupt handler 
 * implementation. If the developer wishes to use his own interrupt handler, 
 * he must define the __MICOGPIO_USER_IRQ_HANDLER__ preprocessor define.
 *
 * Arguments:
 *    MicoGPIOCtx_t *ctx			: GPIO context
 *
 *****************************************************************************
 */
void MicoGPIOISR (MicoGPIOCtx_t *ctx)
{
	MicoGPIOInit (ctx);
}
#endif
