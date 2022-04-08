/****************************************************************************
**
**  Description:
**        Implements functions for manipulating LatticeMico SPI Flash
**
**  Disclaimer:
**   This source code is intended as a design reference which
**   illustrates how these types of functions can be implemented.  It
**   is the user's responsibility to verify their design for
**   consistency and functionality through the use of formal
**   verification methods.  Lattice Semiconductor provides no warranty
**   regarding the use or functionality of this code.
**
*****************************************************************************
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
*****************************************************************************
**  Change History (Latest changes on top)
**
**  Ver    Date        Description
** --------------------------------------------------------------------------
**  3.3    12/22/2010  Initial Version
**
*****************************************************************************/
#include "MicoSPIFlash.h"

/***********************************************************************
 *                                                                     *
 *                                                                     *
 *                                                                     *
 ***********************************************************************/
void MicoSPIFlash_Initialize (MicoSPIFlashCtx_t *ctx)
{
  
}

/*
 * Issue SPI Flash "Sector Erase" Command
 * 
 * Inputs:
 *	MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *	unsigned long anAddress: block address (function will not check if
 *                               if address is a valid SPI Flash address)
 *	unsigned int aType: Size of block to be erased
 *				1 -> Type 1 (4K)
 *				2 -> Type 2 (32K)
 *				3 -> Type 3 (64K)
 * Return Values:
 *	SPI_CONTROL_PORT_ERROR
 *	SPI_INVALID_BLOCK_ERASE_TYPE
 *	SPI_COMMAND_SUCCESS
 */
void MicoSPIFlash_BlockErase (MicoSPIFlashCtx_t *ctx, 
			      unsigned long anAddress, 
			      unsigned char aType)
{
  	size_t control_base = ctx->control_base;
  	char mask = 0x1;
  
  	/* Make sure SPI Flash is not busy */
  	do {
    	if ((mask & MICO_SPI_STATUS_READ(control_base)) == 0)
      		break;
  	} while (1);
  	
  	/* Issue SPI Flash "Sector Erase" Command */
  	switch (aType)
    {
    	case 1 : 
      		MICO_SPI_BLOCK_ERASE_TYPE1 (control_base, anAddress);
      		break;
    	case 2 : 
      		MICO_SPI_BLOCK_ERASE_TYPE2 (control_base, anAddress);
      		break;
    	case 3 : 
      		MICO_SPI_BLOCK_ERASE_TYPE3 (control_base, anAddress);
      		break;
    	default:
      		;
    }
}

/*
 * Read a "page" from SPI Flash or program a "page" to SPI Flash.
 * 
 * Inputs:
 *	MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *	unsigned long anAddress: page address (the function will
 *         align this address to a page boundary, i.e., mask off
 *         the bits to identify a byte within the page. the user
 *         will ensure that the address is within the spi flash
 *         address range as defined in the hardware platform).
 *	char *rData: pointer to byte array (the user must ensure
 *         that this array is atleast as big as a page)
 *      read_op: 1 means read. 0 means write.
 * Return Values:
 *	SPI_CONTROL_PORT_ERROR
 *	SPI_COMMAND_SUCCESS
 */
void MicoSPIFlash_PageOp (MicoSPIFlashCtx_t *ctx,
			  unsigned long anAddress,
			  char *rData,
			  char read_op)
{
  	int page_size = ctx->page_size;
  	unsigned long base_address = anAddress & ~((unsigned long)page_size-1);
  	size_t control_base = ctx->control_base;
  	char mask = 0x1;
  	
  	/* Make sure SPI Flash is not busy */
  	do {
    	if ((mask & MICO_SPI_STATUS_READ(control_base)) == 0)
      		break;
  	} while (1);
  	
  	if (read_op == 1) {
    	
    	/* Issue a SPI Flash "Page Read" Command */
    	MICO_SPI_CFG_WR_PAGE_READ_SIZE (control_base, page_size);
    	MICO_SPI_PAGE_READ (control_base, base_address);
		
		/* Fetch from Page Buffer */
	    int ii;
	    unsigned int baddress = control_base + SPI_PAGE_READ_BUFFER_OFFSET;
	    for (ii = 0; ii < page_size; ii++)
	    	*rData++ = __builtin_import((size_t)(ii+baddress));
   		     
	 } else {
    	
		/* Write to destination */
		unsigned int ii = 0;
		unsigned int baddress = control_base + SPI_PAGE_PROGRAM_BUFFER_OFFSET;
		for ( ; ii < page_size; ii++)
  			__builtin_export(*rData++, (size_t)(ii+baddress));
  		
		/* Issue a SPI Flash "Page Read" Command */
    	MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE (control_base, page_size);
    	MICO_SPI_PAGE_PROGRAM (control_base, base_address);
    		
  	}
}

