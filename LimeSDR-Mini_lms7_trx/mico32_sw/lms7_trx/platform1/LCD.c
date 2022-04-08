/****************************************************************************
**
**  Name: LCD.c
**
**  Description:
**       Implements functions for manipulating a 
**       dot-matrix LCD
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

#include "LCD.h"
#include "MicoUtils.h"

void LCD_WriteData(volatile unsigned int *pAddress, unsigned int data)
{
	unsigned int iData = data;
	
	/* first output the data to write */
	*pAddress = iData;
	
	/* strobe enable while maintaining data */
	iData |= 0x00040000;
	*pAddress = iData;
	
	/* remove strobe while maintaining data */
	iData &= ~0x00040000;
	*pAddress = iData;
	
	/* all done */
	return;
}


void LCD_DisplayOnOff(MicoGPIOCtx_t *ctx, unsigned int bOn)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;

	if (bOn == 0)	/* turn display off */
		value = 0x8;
	else			/* turn display on */
		value = 0xc;

	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}


void LCD_CursorOnOff(MicoGPIOCtx_t *ctx, unsigned int bOn)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	/* cursor on-off control is valid only if the display is turned on */
	unsigned int iValue;
	
	if (bOn == 0)
		iValue = 0x0c;
	else
		iValue = 0x0e;

	LCD_WriteData(pAddress, (iValue<<24));
	MicoSleepMilliSecs(10);

}


void LCD_BlinkOnOff(MicoGPIOCtx_t *ctx, unsigned int bOn)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	/* character-blink on/off is valid only if the display is turned on */
	unsigned int iValue;

	if (bOn == 0)
		iValue = 0xc;
	else
		iValue = 0xd;

	LCD_WriteData(pAddress, (iValue<<24));
	MicoSleepMilliSecs(10);
}


/* clears LCD display */
void LCD_ClearDisplay(MicoGPIOCtx_t *ctx)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	LCD_WriteData(pAddress, (0x01<<24));
	MicoSleepMilliSecs(10);
}


/* sets LCD function (#lines for the display, with 8-bit interface */
void LCD_SetFunction(MicoGPIOCtx_t *ctx, unsigned int iNumLines)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;

	/* always sets an 8-bit interface */
	if (iNumLines == 2)
		value = 0x38;
	else
		value = 0x00;

	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}

/* sets cursor-move mode */
void LCD_SetCursorMoveMode(MicoGPIOCtx_t *ctx, unsigned int bIncrement)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;
	if (bIncrement != 0)
		value = 0x6;
	else
		value = 0x4;
	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}


/* shifts display to left (!= 0) or right (== 0) */
void LCD_ShiftDisplay(MicoGPIOCtx_t *ctx, unsigned int bLeft)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;
	if (bLeft == 0)	/* shift to right */
		value = 0x18;
	else 			/* shift to left */
		value = 0x1c;
	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}


/* shifts cursor to left (!= 0) or right (== 0) */
void LCD_ShiftCursor(MicoGPIOCtx_t *ctx, unsigned int bLeft)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;
	if (bLeft == 0)	/* shift to right */
		value = 0x10;
	else			/* shift to left */
		value = 0x14;
	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}


void LCD_WriteChar(MicoGPIOCtx_t *ctx, unsigned char character)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value = (unsigned int)character;
	value = (value<<24) | 0x00020000;
	LCD_WriteData(pAddress, value);
	MicoSleepMilliSecs(10);
}


void LCD_SetCursorPos(MicoGPIOCtx_t *ctx, unsigned int iLine, unsigned int iCol)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	unsigned int value;
	if (iLine == 0)	/* first line */
		value = 0x80;
	else			/* second line */
		value = 0xC0;

	value += iCol;

	LCD_WriteData(pAddress, (value<<24));
	MicoSleepMilliSecs(10);
}


void LCD_Init(MicoGPIOCtx_t *ctx, unsigned int iLines)
{
	volatile unsigned int *pAddress = (volatile unsigned int *)(ctx->base);
	/* - wait for power-stabilization */
	MicoSleepMilliSecs(40);

	*pAddress = 0x0;

	/* - set data-width (8 bits) and lines in display (2) */
	LCD_SetFunction(ctx, iLines);
	LCD_SetFunction(ctx, iLines);
	LCD_SetFunction(ctx, iLines);


	/* turn off the display */
	LCD_DisplayOnOff(ctx, 0);

	/* - clear display */
	LCD_ClearDisplay(ctx);
			
	/* turn-on the display */
	LCD_DisplayOnOff(ctx, 1);

	/* turn on the blinking of the cursor-position */
	LCD_BlinkOnOff(ctx, 0);

	/* set cursor move-mode to increment */
	LCD_SetCursorMoveMode(ctx, 1);

	/* all done */
	return;
}

