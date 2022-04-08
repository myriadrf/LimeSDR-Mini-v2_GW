/****************************************************************************
**
**  Name: LCD.h
**
**  Description:
**        Declares user-callable functions for manipulating
**        dot-matrix LCD displays
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

#ifndef LCD_H_
#define LCD_H_

#include "DDStructs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* for direct read/write to the LCD */
void LCD_WriteData(volatile unsigned int * pAddress, unsigned int data);


/* turns display on/off */
void LCD_DisplayOnOff(MicoGPIOCtx_t *pLCD, unsigned int bOn);


/* turns cursor on/off */
void LCD_CursorOnOff(MicoGPIOCtx_t *pLCD, unsigned int bOn);


/* turns blinking on/off */
void LCD_BlinkOnOff(MicoGPIOCtx_t *pLCD, unsigned int bOn);


/* clears display */
void LCD_ClearDisplay(MicoGPIOCtx_t *pLCD);


/* sets function */
void LCD_SetFunction(MicoGPIOCtx_t *pLCD, unsigned int iNumLines);


/* sets cursor move mode */
void LCD_SetCursorMoveMode(MicoGPIOCtx_t *pLCD, unsigned int bIncrement);


/* shifts display left or right by 1 */
void LCD_ShiftDisplay(MicoGPIOCtx_t *pLCD, unsigned int bLeft);


/* shifts cursor left or right by 1 */
void LCD_ShiftCursor(MicoGPIOCtx_t *pLCD, unsigned int bLeft);


/* writes a character to the display */
void LCD_WriteChar(MicoGPIOCtx_t *pLCD, unsigned char character);


/* sets write-position (i.e. cursor position) */
void LCD_SetCursorPos(MicoGPIOCtx_t *pLCD, unsigned int iLine, unsigned int iCol);


/* Initializes the LCD */
void LCD_Init(MicoGPIOCtx_t *pLCD, unsigned int iLines);


/* writes a line to the LCD at the cursor position */
void LCD_WriteLine(MicoGPIOCtx_t *pLCD, const char *pLine);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*LCD_H_*/

