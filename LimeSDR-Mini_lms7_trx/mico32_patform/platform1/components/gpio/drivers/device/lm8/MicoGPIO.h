/****************************************************************************
**
**  Name: MicoGPIO.h 
**
**  Description:
**       Declares GPIO register structure and macros/functions for 
**       manipulating GPIO
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
**  3.0    	06-04-2010	Added Header
**
**---------------------------------------------------------------------------
*****************************************************************************/
#include "DDStructs.h"

/***********************************************************************
 *                                                                     *
 * SPI FLASH CONTROLLER PHYSICAL DEVICE SPECIFIC INFORMATION           *
 *                                                                     *
 ***********************************************************************/
#define GPIO_DATA_OFFSET			(0x0)
#define GPIO_TRISTATE_OFFSET        (0x4)
#define GPIO_IRQ_MASK_OFFSET		(0x8)
#define GPIO_EDGE_CAPTURE_OFFSET	(0xC)

/***********************************************************************
 *                                                                     *
 * USER MACROS                                                         *
 *                                                                     *
 ***********************************************************************/

/* Macros for accessing each byte of the Data Register */
#define MICO_GPIO_READ_DATA_BYTE0(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_DATA_OFFSET+0)))

#define MICO_GPIO_READ_DATA_BYTE1(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_DATA_OFFSET+1)))

#define MICO_GPIO_READ_DATA_BYTE2(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_DATA_OFFSET+2)))

#define MICO_GPIO_READ_DATA_BYTE3(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_DATA_OFFSET+3)))

/* Macros for writing each byte of the Data Register */
#define MICO_GPIO_WRITE_DATA_BYTE0(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_DATA_OFFSET+0)))

#define MICO_GPIO_WRITE_DATA_BYTE1(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_DATA_OFFSET+1)))

#define MICO_GPIO_WRITE_DATA_BYTE2(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_DATA_OFFSET+2)))

#define MICO_GPIO_WRITE_DATA_BYTE3(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_DATA_OFFSET+3)))

/* Macros for accessing each byte of the Tristate Register */
#define MICO_GPIO_READ_TRISTATE_BYTE0(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_TRISTATE_OFFSET+0)))

#define MICO_GPIO_READ_TRISTATE_BYTE1(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_TRISTATE_OFFSET+1)))

#define MICO_GPIO_READ_TRISTATE_BYTE2(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_TRISTATE_OFFSET+2)))

#define MICO_GPIO_READ_TRISTATE_BYTE3(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_TRISTATE_OFFSET+3)))

/* Macros for writing each byte of the Tristate Register */
#define MICO_GPIO_WRITE_TRISTATE_BYTE0(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_TRISTATE_OFFSET+0)))
	
#define MICO_GPIO_WRITE_TRISTATE_BYTE1(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_TRISTATE_OFFSET+1)))
	
#define MICO_GPIO_WRITE_TRISTATE_BYTE2(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_TRISTATE_OFFSET+2)))
	
#define MICO_GPIO_WRITE_TRISTATE_BYTE3(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_TRISTATE_OFFSET+3)))
	
/* Macros for accessing each byte of the IRQ Mask Register */
#define MICO_GPIO_READ_IRQ_MASK_BYTE0(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_IRQ_MASK_OFFSET+0)))

#define MICO_GPIO_READ_IRQ_MASK_BYTE1(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_IRQ_MASK_OFFSET+1)))

#define MICO_GPIO_READ_IRQ_MASK_BYTE2(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_IRQ_MASK_OFFSET+2)))

#define MICO_GPIO_READ_IRQ_MASK_BYTE3(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_IRQ_MASK_OFFSET+3)))

/* Macros for writing each byte of the IRQ Mask Register */
#define MICO_GPIO_WRITE_IRQ_MASK_BYTE0(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_IRQ_MASK_OFFSET+0)))
	
#define MICO_GPIO_WRITE_IRQ_MASK_BYTE1(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_IRQ_MASK_OFFSET+1)))
	
#define MICO_GPIO_WRITE_IRQ_MASK_BYTE2(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_IRQ_MASK_OFFSET+2)))
	
#define MICO_GPIO_WRITE_IRQ_MASK_BYTE3(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_IRQ_MASK_OFFSET+3)))
	
/* Macros for accessing each byte of the Edge Capture Register */
#define MICO_GPIO_READ_EDGE_CAPTURE_BYTE0(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+0)))

#define MICO_GPIO_READ_EDGE_CAPTURE_BYTE1(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+1)))

#define MICO_GPIO_READ_EDGE_CAPTURE_BYTE2(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+2)))

#define MICO_GPIO_READ_EDGE_CAPTURE_BYTE3(X, Y) \
	(Y) = (__builtin_import((size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+3)))

/* Macros for writing each byte of the Edge Capture Register */
#define MICO_GPIO_WRITE_EDGE_CAPTURE_BYTE0(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+0)))
	
#define MICO_GPIO_WRITE_EDGE_CAPTURE_BYTE1(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+1)))
	
#define MICO_GPIO_WRITE_EDGE_CAPTURE_BYTE2(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+2)))
	
#define MICO_GPIO_WRITE_EDGE_CAPTURE_BYTE3(X, Y) \
	(__builtin_export((char)(Y), (size_t)(X+GPIO_EDGE_CAPTURE_OFFSET+3)))
	
/***********************************************************************
 *                                                                     *
 * USER FUNCTIONS                                                      *
 *                                                                     *
 ***********************************************************************/
void MicoGPIOInit(MicoGPIOCtx_t *ctx);
#ifndef __MICOGPIO_USER_IRQ_HANDLER__
void MicoGPIOISR (MicoGPIOCtx_t *ctx);
#endif
