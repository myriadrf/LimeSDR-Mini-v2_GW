/****************************************************************************
**
**  Description:
**        Implements functions for manipulating LatticeMico32 SPI Flash
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
**  3.0   May-13-2009  Initial Version
**  3.1   Apr-12-2010  Flash sector erase function is fixed to remove mask bug
**                     that resulted in erase of first sector only regardless 
**                     of address.
**
*****************************************************************************/
#include "MicoSPIFlash.h"

/*
 * Initialize SPI Flash
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 */
void MicoSPIFlash_Initialize (MicoSPIFlashCtx_t *ctx)
{
	/* Make sure this SPI device is "Lookup-able" */
    ctx->lookupReg.name = ctx->name;
    ctx->lookupReg.deviceType = "SPIFlashDevice";
    ctx->lookupReg.priv = ctx;
    MicoRegisterDevice (&(ctx->lookupReg));
	
    return;
}




/*
 * Read SPI Flash Manufacturer ID
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 * Return Values:
 *		SPI Flash Manufacturer ID
 */
int MicoSPIFlash_ReadID (MicoSPIFlashCtx_t *ctx)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	int value;
	if (ctx->control_wbSize == 32)
		MICO_SPI_MANUFACTURER_ID (ctx->control_base, value);
	else
		MICO_SPI_MANUFACTURER_ID_BYTEWISE (ctx->control_base, value);
	
	return (0xFF & value);
}




/*
 * Read SPI Flash Status Register
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 * Return Values:
 *		SPI Flash Status Register
 */
int MicoSPIFlash_StatusRead (MicoSPIFlashCtx_t *ctx)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	int value;
	if (ctx->control_wbSize == 32)
		MICO_SPI_STATUS_READ (ctx->control_base, value);
	else
		MICO_SPI_STATUS_READ_BYTEWISE (ctx->control_base, value);
	
	return (0xFF & value);
}




/*
 * Write SPI Flash Status Register
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		int aValue: value to be written to SPI Flash Status Register
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_StatusWrite (MicoSPIFlashCtx_t *ctx, int aValue)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	if (ctx->control_wbSize == 32)
		MICO_SPI_STATUS_WRITE (ctx->control_base, (0xFF & aValue));
	else
		MICO_SPI_STATUS_WRITE_BYTEWISE (ctx->control_base, (0xFF & aValue));
		
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Issue SPI Flash "Write Enable" Command
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_WriteEnable (MicoSPIFlashCtx_t *ctx)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	if (ctx->control_wbSize == 32)
		MICO_SPI_WRITE_ENABLE (ctx->control_base);
	else
		MICO_SPI_WRITE_ENABLE_BYTEWISE (ctx->control_base);
		
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Issue SPI Flash "Write Enable" Command
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_WriteDisable (MicoSPIFlashCtx_t *ctx)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	if (ctx->control_wbSize == 32)
		MICO_SPI_WRITE_DISABLE (ctx->control_base);
	else
		MICO_SPI_WRITE_DISABLE_BYTEWISE (ctx->control_base);
		
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Adjust SPI Flash Read Speeds
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *      unsigned int aSpeed: 0 -> slow, 1 -> fast
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_ReadSpeed (MicoSPIFlashCtx_t *ctx, unsigned int aSpeed)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	if (ctx->control_wbSize == 32)
		MICO_SPI_CFG_WR_READ_SPEED (ctx->control_base, aSpeed);
	else
		MICO_SPI_CFG_WR_READ_SPEED_BYTEWISE (ctx->control_base, aSpeed);
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Issue SPI Flash "Chip Erase" Command
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_ChipErase (MicoSPIFlashCtx_t *ctx)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	if (ctx->control_wbSize == 32)
		MICO_SPI_CHIP_ERASE (ctx->control_base);
	else
		MICO_SPI_CHIP_ERASE_BYTEWISE (ctx->control_base);
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Issue SPI Flash "Sector Erase" Command
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		unsigned int anAddress: block address
 *		unsigned int aType: Size of block to be erased
 *							1 -> Type 1 (4K)
 *							2 -> Type 2 (32K)
 *							3 -> Type 3 (64K)
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_INVALID_BLOCK_ERASE_TYPE
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_BlockErase (MicoSPIFlashCtx_t *ctx, 
							unsigned int anAddress, 
							unsigned int aType)
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	unsigned int control_base = ctx->control_base;
	unsigned int memory_base  = ctx->memory_base;
	unsigned int memory_size  = ctx->memory_size;
	 
	/* Make sure the address is in the SPI Flash's range */
	if ((anAddress < memory_base)
		|| (anAddress > (memory_base + memory_size))) {
		
		return (SPI_ADDRESS_OUT_OF_RANGE);
	}
	
	/* Make sure SPI Flash is not busy */
	do {
		if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
			break;
	} while (1);
	
	/* Issue SPI Flash "Sector Erase" Command */
	if (ctx->control_wbSize == 32) {
		switch (aType)
		{
		case 1 : 
			MICO_SPI_BLOCK_ERASE_TYPE1 (control_base, (anAddress & 0xFFFFF000));
			break;
		case 2 : 
			MICO_SPI_BLOCK_ERASE_TYPE2 (control_base, (anAddress & 0xFFFF8000));
			break;
		case 3 : 
			MICO_SPI_BLOCK_ERASE_TYPE3 (control_base, (anAddress & 0xFFFF0000));
			break;
		default:
			return (SPI_INVALID_BLOCK_ERASE_TYPE);
		}
	} else {
		switch (aType)
		{
		case 1 : 
			MICO_SPI_BLOCK_ERASE_TYPE1_BYTEWISE (control_base, (anAddress & 0xFFFFF000));
			break;
		case 2 : 
			MICO_SPI_BLOCK_ERASE_TYPE2_BYTEWISE (control_base, (anAddress & 0xFFFF8000));
			break;
		case 3 : 
			MICO_SPI_BLOCK_ERASE_TYPE3_BYTEWISE (control_base, (anAddress & 0xFFFF0000));
			break;
		default:
			return (SPI_INVALID_BLOCK_ERASE_TYPE);
		}
	}	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Read a "page" from SPI Flash. This function can set up the hardware
 * to read from any location in the page. It will however truncate the
 * number of bytes read to the page boundary. That is, if the bytes to
 * be read exceed the page boundary, the function will ignore reading
 * any byte outside the page boundary.
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		unsigned int anAddress: page address
 *		unsigned int aLength: number of bytes to be read
 *		char *rData: pointer to array in which page must be returned
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 *
 * NOTE: The function expects that rData is pointing to a character 
 * array that can hold the requested bytes, else memory leaks are
 * certain!
 */
int MicoSPIFlash_PageRead (MicoSPIFlashCtx_t *ctx, 
							unsigned int anAddress,
							unsigned int aLength,
							char *rData)
{
	unsigned int control_base = ctx->control_base;
	unsigned int memory_base  = ctx->memory_base;
	unsigned int memory_size  = ctx->memory_size;
	unsigned int page_size    = ctx->page_size;
	unsigned int wb_size      = (ctx->control_port & ctx->read_buf_en) ? ctx->control_wbSize : ctx->memory_wbSize;
	
	/* Make sure the address is in the SPI Flash's range */
	if ((anAddress < memory_base)
		|| (anAddress > (memory_base + memory_size))) {
				
		return  (SPI_ADDRESS_OUT_OF_RANGE);
	}
	
	/* Make sure SPI Flash is not busy */
	do {
		if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
			break;
	} while (1);
	
	/* Make sure number of bytes does not go beyond page boundary */
	unsigned int buffer_address = anAddress & (page_size - 1);
	unsigned int max_size = page_size - buffer_address;
	unsigned int size = (aLength > max_size) ? max_size : aLength;
	
	/* Some declarations */
	unsigned int byte_count, byte_alignment, data, jj;
	unsigned int *src_word;
	unsigned char *src_byte;
	
	if (ctx->control_port && ctx->read_buf_en) {
		// Use Page Read Buffer in the Control Port (for a faster mode)
		
		/* Issue a SPI Flash "Page Read" Command */
		if (ctx->control_wbSize == 32) {
			
			MICO_SPI_CFG_WR_PAGE_READ_SIZE (control_base, size);
			MICO_SPI_PAGE_READ (control_base, anAddress);
			
		} else {
			
			MICO_SPI_CFG_WR_PAGE_READ_SIZE_BYTEWISE (control_base, size);
			MICO_SPI_PAGE_READ_BYTEWISE (control_base, anAddress);
		}
		
		/* Check if page has been read in to the Page Read Buffer */
		do {
			if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
				break;
		} while (1);
		
		/* Fetch from Page Buffer */
		if (wb_size == 32) {
			
			buffer_address = (buffer_address & (page_size - 1 - 3)) + control_base + SPI_PAGE_READ_BUFFER_OFFSET;
			src_word = (unsigned int *)buffer_address;
		} else {
			
			buffer_address = (buffer_address & (page_size - 1)) + control_base + SPI_PAGE_READ_BUFFER_OFFSET;
			src_byte = (unsigned char *)buffer_address;
		}
	} else {
		// Use Memory Port and fetch data like any non-volatile memory
		
		/* Fetch directly from memory */
		if (wb_size == 32) {
			src_word = (unsigned int *)(anAddress & 0xFFFFFFFC);
		} else {
			src_byte = (unsigned char *)anAddress;
		}
	}
	
	if (wb_size == 32) {
		
		/* Perform reads from source address. Since SPI Flash Controller 
		 * returns 4 bytes per read, extract each byte and write to byte 
		 * (char) array.
		 */
		byte_count = 0;
		byte_alignment = anAddress & 0x3;
		do {
			data = *src_word++;
			for (jj = byte_alignment; jj < 4; jj++) {
				
				switch (jj) {
					case 0:
						*rData++ = (char)(data >> 24);
						break;
					case 1:
						*rData++ = (char)(data >> 16);
						break;
					case 2:
						*rData++ = (char)(data >> 8);
						break;
					case 3:
						*rData++ = (char)data;
						break;
				}
				byte_count++;
				if (byte_count == size) {
					break;
				}
			}
			
			if (byte_count == size) 
				break;
			
			byte_alignment = 0;
			
		} while (1);
			
	} else {
			
		/* Perform reads from source address. Since SPI Flash Controller 
		 * returns 1 byte per read, extract each byte and write to byte 
		 * (char) array.
		 */
		byte_count = 0;
		do {
			*rData++ = *src_byte++;
			byte_count++;
			if (byte_count == size)
				break;
			
		} while (1);
	}
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Read a "page" from SPI Flash. This function can set up the hardware
 * to read from any location in the page. It will however truncate the
 * number of bytes read to the page boundary. That is, if the bytes to
 * be read exceed the page boundary, the function will ignore reading
 * any byte outside the page boundary.
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		unsigned int anAddress: page address
 *		unsigned int aLength: number of bytes to be read. 
 *		unsigned int *rData: pointer to array in which page must be returned
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 *
 * NOTE: The function expects that rData is pointing to a integer
 * array that can hold the requested data, else memory leaks are
 * certain!
 *
 * NOTE: The function expects that the address is word-aligned and the
 * number of bytes to be read are a multiple of 4. If these conditions
 * are not true, use MicoSPIFlash_PageRead().
 */
int MicoSPIFlash_AlignedPageRead (MicoSPIFlashCtx_t *ctx, 
									unsigned int anAddress,
									unsigned int aLength,
									unsigned int *rData)
{
	if ((aLength % 4) != 0)
		return (SPI_PAGE_READ_ERROR);
	
	if ((anAddress & 0x3) != 0)
		return (SPI_PAGE_READ_ERROR);
	
	unsigned int control_base = ctx->control_base;
	unsigned int memory_base  = ctx->memory_base;
	unsigned int memory_size  = ctx->memory_size;
	unsigned int page_size    = ctx->page_size;
	unsigned int wb_size      = (ctx->control_port & ctx->read_buf_en) ? ctx->control_wbSize : ctx->memory_wbSize;
	
	/* Make sure the address is in the SPI Flash's range */
	if ((anAddress < memory_base)
		|| (anAddress > (memory_base + memory_size))) {
				
		return  (SPI_ADDRESS_OUT_OF_RANGE);
	}
	
	/* Make sure SPI Flash is not busy */
	do {
		if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
			break;
	} while (1);
	
	/* Make sure number of bytes does not go beyond page boundary */
	unsigned int buffer_address = anAddress & (page_size - 1);
	unsigned int max_size = page_size - buffer_address;
	unsigned int size = (aLength > max_size) ? max_size : aLength;
	
	/* Some declarations */
	unsigned char byte;
	unsigned char *src_byte;
	unsigned int  byte_count, byte_alignment, word;
	unsigned int  *src_word;
	
	if (ctx->control_port && ctx->read_buf_en) {
		// Use Page Read Buffer in the Control Port (for a faster mode)
		
		/* Issue a SPI Flash "Page Read" Command */
		if (ctx->control_wbSize == 32) {
			
			MICO_SPI_CFG_WR_PAGE_READ_SIZE (control_base, size);
			MICO_SPI_PAGE_READ (control_base, anAddress);
			
		} else {
			
			MICO_SPI_CFG_WR_PAGE_READ_SIZE_BYTEWISE (control_base, size);
			MICO_SPI_PAGE_READ_BYTEWISE (control_base, anAddress);
		}
		
		/* Check if page has been read in to the Page Read Buffer */
		do {
			if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
				break;
		} while (1);
		
		/* Fetch from Page Buffer */
		buffer_address = (buffer_address & (page_size - 1)) + control_base + SPI_PAGE_READ_BUFFER_OFFSET;
		if (wb_size == 32) {
			src_word = (unsigned int *)buffer_address;
		} else {
			src_byte = (unsigned char *)buffer_address;
		}
	} else {
		// Use Memory Port and fetch data like any non-volatile memory
		
		/* Fetch directly from memory */
		if (wb_size == 32) {
			src_word = (unsigned int *)anAddress;
		} else {
			src_byte = (unsigned char *)anAddress;
		}
	}
	
	if (wb_size == 32) {
		
		/* Perform reads from source address. Since SPI Flash Controller 
		 * returns 4 bytes per read, extract each byte and write to byte 
		 * (char) array.
		 */
		byte_count = 0;
		do {
			*rData++ = *src_word++;
			byte_count = byte_count + 4;
			if (byte_count == size)
				break;
			
		} while (1);
		
	} else {
			
		/* Perform reads from source address. Since SPI Flash Controller 
		 * returns 1 byte per read, extract each byte and write to byte 
		 * (char) array.
		 */
		byte_count = 0;
		byte_alignment = 0;
		do {
			byte = *src_byte++;
			switch (byte_alignment) {
				case 0:
					word = word | ((unsigned int)byte)<<24;
					byte_alignment = 1;
					break;
				case 1:
					word = word | ((unsigned int)byte)<<16;
					byte_alignment = 2;
					break;
				case 2:
					word = word | ((unsigned int)byte)<<8;
					byte_alignment = 3;
					break;
				case 3:
					word = word | ((unsigned int)byte);
					byte_alignment = 0;
					break;
			}
			if (byte_alignment == 0)
				*rData++ = word;
			
			byte_count++;
			if (byte_count == size)
				break;
			
		} while (1);
	}
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Write a "page" to SPI Flash. This function can set up the hardware
 * to write to any location in the page. It will however truncate the
 * number of bytes that are written to the page boundary. That is, if 
 * the bytes to be written exceed the page boundary, the function will 
 * ignore writing any byte outside the page boundary. 
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		unsigned int anAddress: page address
 *      unsigned int aLength: number of bytes to be written
 *		char *rData: pointer to array which contains bytes to be written
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_ADDRESS_OUT_OF_RANGE
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_PageProgram (MicoSPIFlashCtx_t *ctx, 
							  unsigned int anAddress,
							  unsigned int aLength,
							  char *rData)
{
	unsigned int control_base = ctx->control_base;
	unsigned int memory_base  = ctx->memory_base;
	unsigned int memory_size  = ctx->memory_size;
	unsigned int page_size    = ctx->page_size;
	unsigned int wb_size      = (ctx->control_port & ctx->read_buf_en) ? ctx->control_wbSize : ctx->memory_wbSize;

	/* Make sure the address is in the SPI Flash's range */
	if ((anAddress < memory_base)
		|| (anAddress > (memory_base + memory_size))) {
		
		return (SPI_ADDRESS_OUT_OF_RANGE);
	}
	
	/* Make sure SPI Flash is not busy */
	do {
		if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
			break;
	} while (1);
	
	/* Make sure number of bytes does not go beyond page boundary */
	unsigned int buffer_address = anAddress & (page_size - 1);
	unsigned int max_size = page_size - buffer_address;
	unsigned int size = (aLength > max_size) ? max_size : aLength;
	
	/* Some declarations */
	unsigned int byte_count, byte_alignment, current_int, ii;
	unsigned int *dst_word;
	unsigned char *dst_byte; 
	
	/* Compute destination address */
	if (ctx->control_port && ctx->program_buf_en) {
		// Use Page Program Buffer in the Control Port (for a faster mode)
		
		if (wb_size == 32) {
			dst_word = (unsigned int *)(control_base + SPI_PAGE_PROGRAM_BUFFER_OFFSET + (anAddress & (page_size - 1 - 3)));
		} else {
			dst_byte = (unsigned char *)(control_base + SPI_PAGE_PROGRAM_BUFFER_OFFSET + (anAddress & (page_size - 1)));
		}
	} else {
		// Use Memory Port and write data like any other non-volatile memory (for a slower write)
		
		if (wb_size == 32) {
			dst_word = (unsigned int *)(anAddress & 0xFFFFFFFC);
		} else {
			dst_byte = (unsigned char *)anAddress;
		}
	}
	
	if (wb_size == 32) {
		
		/* Before writing to destination, club 4 bytes (chars) together to 
		 * make an integer. This is because the SPI Flash Controller only
		 * accepts 4-byte data.
		 */
		byte_count = 0;
		byte_alignment = anAddress & 0x3;
		do {
			current_int = 0;
			for (ii = byte_alignment; ii < 4; ii++) {
				char current_byte = *rData++;
				switch (ii) {
					case 0:
						current_int = current_int | (((unsigned int)current_byte) << 24);
						break;
					case 1:
						current_int = current_int | (((unsigned int)current_byte) << 16);
						break;
					case 2:
						current_int = current_int | (((unsigned int)current_byte) << 8);
						break;
					case 3:
						current_int = current_int | (unsigned int)current_byte;
						break;
				}
				byte_count++;
				if (byte_count == size)
					break;
			}
			*dst_word++ = current_int;
			
			if (byte_count == size)
				break;
			
			byte_alignment = 0;
			
		} while (1);
		
	} else {
		
		/* Write each byte */
		byte_count = 0;
		do {
			*dst_byte++ = *rData++;
			byte_count++;
			if (byte_count == size)
				break;
			
		} while (1);
		
	}
	
	if (ctx->control_port && ctx->program_buf_en) {
		
		if (wb_size == 32) {
			
			/* Issue a SPI Flash "Page Program" Command */
			MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE(control_base, byte_count);
			MICO_SPI_PAGE_PROGRAM(control_base, anAddress);
		} else {
			
			/* Issue a SPI Flash "Page Program" Command */
			MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE_BYTEWISE (control_base, byte_count);
			MICO_SPI_PAGE_PROGRAM_BYTEWISE (control_base, anAddress);
		}
		
		/* Check if page has been written to SPI Flash */
		do {
			if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
				break;
		} while (1);
	}
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Write a "page" to SPI Flash. This function can set up the hardware
 * to write to any location in the page. It will however truncate the
 * number of bytes that are written to the page boundary. That is, if 
 * the bytes to be written exceed the page boundary, the function will 
 * ignore writing any byte outside the page boundary. 
 * 
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		unsigned int anAddress: page address
 *      unsigned int aLength: number of bytes to be written
 *		unsigned int *rData: pointer to array which contains bytes to be written
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_PAGE_PROGRAM_ERROR
 *		SPI_COMMAND_SUCCESS
 * 
 * NOTE: The function expects that the number of bytes to be written 
 * to SPI Flash is a multiple of 4 and the address to be word-aligned.
 * If either condition is not met, use MicoSPIFlash_PageProgram()
 * function.
 */
int MicoSPIFlash_AlignedPageProgram (MicoSPIFlashCtx_t *ctx, 
							  unsigned int anAddress,
							  unsigned int aLength,
							  unsigned int *rData)
{
	if ((aLength % 4) != 0)
		return (SPI_PAGE_PROGRAM_ERROR);
	
	if ((anAddress & 0x3) != 0)
		return (SPI_PAGE_PROGRAM_ERROR);
	
	unsigned int control_base = ctx->control_base;
	unsigned int memory_base  = ctx->memory_base;
	unsigned int memory_size  = ctx->memory_size;
	unsigned int page_size    = ctx->page_size;
	unsigned int wb_size      = (ctx->control_port & ctx->read_buf_en) ? ctx->control_wbSize : ctx->memory_wbSize;

	/* Make sure the address is in the SPI Flash's range */
	if ((anAddress < memory_base)
		|| (anAddress > (memory_base + memory_size))) {
		
		return (SPI_ADDRESS_OUT_OF_RANGE);
	}
	
	/* Make sure SPI Flash is not busy */
	do {
		if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
			break;
	} while (1);
	
	/* Make sure number of bytes does not go beyond page boundary */
	unsigned int buffer_address = anAddress & (page_size - 1);
	unsigned int max_size = page_size - buffer_address;
	unsigned int size = (aLength > max_size) ? max_size : aLength;
	
	/* Some declarations */
	unsigned int byte_count, word;
	unsigned int *dst_word;
	unsigned char *dst_byte; 
	
	/* Compute destination address */
	if (ctx->control_port && ctx->program_buf_en) {
		// Use Page Program Buffer in the Control Port (for a faster mode)
		
		if (wb_size == 32) {
			dst_word = (unsigned int *)(control_base + SPI_PAGE_PROGRAM_BUFFER_OFFSET + (anAddress & (page_size - 1)));
		} else {
			dst_byte = (unsigned char *)(control_base + SPI_PAGE_PROGRAM_BUFFER_OFFSET + (anAddress & (page_size - 1)));
		}
	} else {
		// Use Memory Port and write data like any other non-volatile memory (for a slower write)
		
		if (wb_size == 32) {
			dst_word = (unsigned int *)anAddress;
		} else {
			dst_byte = (unsigned char *)anAddress;
		}
	}
	
	if (wb_size == 32) {
		
		/* Before writing to destination, club 4 bytes (chars) together to 
		 * make an integer. This is because the SPI Flash Controller only
		 * accepts 4-byte data.
		 */
		byte_count = 0;
		do {
			*dst_word++ = *rData++;
			byte_count = byte_count + 4;
			if (byte_count == size)
				break;
			
		} while (1);
		
	} else {
		
		/* Write each byte */
		byte_count = 0;
		do {
			switch (byte_count % 4) {
				case 0:
					word = *rData++;
					*dst_byte++ = (unsigned char)(word >> 24);
					break;
				case 1:
					*dst_byte++ = (unsigned char)(word >> 16);
					break;
				case 2:
					*dst_byte++ = (unsigned char)(word >> 8);
					break;
				case 3:
					*dst_byte++ = (unsigned char)(word);
					break;
			}
			byte_count++;
			if (byte_count == size)
				break;
			
		} while (1);
	}
	
	if (ctx->control_port && ctx->program_buf_en) {
		
		if (wb_size == 32) {
			
			/* Issue a SPI Flash "Page Program" Command */
			MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE(control_base, byte_count);
			MICO_SPI_PAGE_PROGRAM(control_base, anAddress);
		} else {
			
			/* Issue a SPI Flash "Page Program" Command */
			MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE_BYTEWISE (control_base, byte_count);
			MICO_SPI_PAGE_PROGRAM_BYTEWISE (control_base, anAddress);
		}
		
		/* Check if page has been written to SPI Flash */
		do {
			if ((BIT_0 & MicoSPIFlash_StatusRead (ctx)) == 0)
				break;
		} while (1);
	}
	
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Read SPI Flash Command Opcodes
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		MicoSPIFlashCmdOpcodeTable_t *opcode: command structure
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_ReadCmdOpcodes (MicoSPIFlashCtx_t *ctx,
								 MicoSPIFlashCmdOpcodeTable_t *opcode) 
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	unsigned int control_base = ctx->control_base;
	if (ctx->control_wbSize == 32) {
		MICO_SPI_CMD_CFG_RD_SLOW_READ (control_base, opcode->slow_read);
		MICO_SPI_CMD_CFG_RD_FAST_READ (control_base, opcode->fast_read);
		MICO_SPI_CMD_CFG_RD_BYTE_PROGRAM (control_base, opcode->byte_program);
		MICO_SPI_CMD_CFG_RD_PAGE_PROGRAM (control_base, opcode->page_program);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE1 (control_base, opcode->block_erase_type1);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE2 (control_base, opcode->block_erase_type2);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE3 (control_base, opcode->block_erase_type3);
		MICO_SPI_CMD_CFG_RD_CHIP_ERASE (control_base, opcode->chip_erase);
		MICO_SPI_CMD_CFG_RD_WRITE_ENABLE (control_base, opcode->write_enable);
		MICO_SPI_CMD_CFG_RD_WRITE_DISABLE (control_base, opcode->write_disable);
		MICO_SPI_CMD_CFG_RD_POWER_DOWN (control_base, opcode->power_down);
		MICO_SPI_CMD_CFG_RD_POWER_UP (control_base, opcode->power_up);
		MICO_SPI_CMD_CFG_RD_MANUFACTURER_ID (control_base, opcode->read_id);
	} else {
		MICO_SPI_CMD_CFG_RD_SLOW_READ_BYTEWISE (control_base, opcode->slow_read);
		MICO_SPI_CMD_CFG_RD_FAST_READ_BYTEWISE (control_base, opcode->fast_read);
		MICO_SPI_CMD_CFG_RD_BYTE_PROGRAM_BYTEWISE (control_base, opcode->byte_program);
		MICO_SPI_CMD_CFG_RD_PAGE_PROGRAM_BYTEWISE (control_base, opcode->page_program);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE1_BYTEWISE (control_base, opcode->block_erase_type1);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE2_BYTEWISE (control_base, opcode->block_erase_type2);
		MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE3_BYTEWISE (control_base, opcode->block_erase_type3);
		MICO_SPI_CMD_CFG_RD_CHIP_ERASE_BYTEWISE (control_base, opcode->chip_erase);
		MICO_SPI_CMD_CFG_RD_WRITE_ENABLE_BYTEWISE (control_base, opcode->write_enable);
		MICO_SPI_CMD_CFG_RD_WRITE_DISABLE_BYTEWISE (control_base, opcode->write_disable);
		MICO_SPI_CMD_CFG_RD_POWER_DOWN_BYTEWISE (control_base, opcode->power_down);
		MICO_SPI_CMD_CFG_RD_POWER_UP_BYTEWISE (control_base, opcode->power_up);
		MICO_SPI_CMD_CFG_RD_MANUFACTURER_ID_BYTEWISE (control_base, opcode->read_id);
	}
		
	return (SPI_COMMAND_SUCCESS);
}




/*
 * Write SPI Flash Command Opcodes
 * Inputs:
 *		MicoSPIFlashCtx_t *ctx: pointer to valid ctx
 *		MicoSPIFlashCmdOpcodeTable_t *opcode: command structure
 * Return Values:
 *		SPI_CONTROL_PORT_ERROR
 *		SPI_COMMAND_SUCCESS
 */
int MicoSPIFlash_WriteCmdOpcodes (MicoSPIFlashCtx_t *ctx,
								  MicoSPIFlashCmdOpcodeTable_t *opcode) 
{
	if (ctx->control_port == 0)
		return (SPI_CONTROL_PORT_ERROR);
	
	unsigned int control_base = ctx->control_base;
	if (ctx->control_wbSize == 32) {
		MICO_SPI_CMD_CFG_WR_SLOW_READ (control_base, opcode->slow_read);
		MICO_SPI_CMD_CFG_WR_FAST_READ (control_base, opcode->fast_read);
		MICO_SPI_CMD_CFG_WR_BYTE_PROGRAM (control_base, opcode->byte_program);
		MICO_SPI_CMD_CFG_WR_PAGE_PROGRAM (control_base, opcode->page_program);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE1 (control_base, opcode->block_erase_type1);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE2 (control_base, opcode->block_erase_type2);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE3 (control_base, opcode->block_erase_type3);
		MICO_SPI_CMD_CFG_WR_CHIP_ERASE (control_base, opcode->chip_erase);
		MICO_SPI_CMD_CFG_WR_WRITE_ENABLE (control_base, opcode->write_enable);
		MICO_SPI_CMD_CFG_WR_WRITE_DISABLE (control_base, opcode->write_disable);
		MICO_SPI_CMD_CFG_WR_POWER_DOWN (control_base, opcode->power_down);
		MICO_SPI_CMD_CFG_WR_POWER_UP (control_base, opcode->power_up);
		MICO_SPI_CMD_CFG_WR_MANUFACTURER_ID (control_base, opcode->read_id);
	} else {
		MICO_SPI_CMD_CFG_WR_SLOW_READ_BYTEWISE (control_base, opcode->slow_read);
		MICO_SPI_CMD_CFG_WR_FAST_READ_BYTEWISE (control_base, opcode->fast_read);
		MICO_SPI_CMD_CFG_WR_BYTE_PROGRAM_BYTEWISE (control_base, opcode->byte_program);
		MICO_SPI_CMD_CFG_WR_PAGE_PROGRAM_BYTEWISE (control_base, opcode->page_program);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE1_BYTEWISE (control_base, opcode->block_erase_type1);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE2_BYTEWISE (control_base, opcode->block_erase_type2);
		MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE3_BYTEWISE (control_base, opcode->block_erase_type3);
		MICO_SPI_CMD_CFG_WR_CHIP_ERASE_BYTEWISE (control_base, opcode->chip_erase);
		MICO_SPI_CMD_CFG_WR_WRITE_ENABLE_BYTEWISE (control_base, opcode->write_enable);
		MICO_SPI_CMD_CFG_WR_WRITE_DISABLE_BYTEWISE (control_base, opcode->write_disable);
		MICO_SPI_CMD_CFG_WR_POWER_DOWN_BYTEWISE (control_base, opcode->power_down);
		MICO_SPI_CMD_CFG_WR_POWER_UP_BYTEWISE (control_base, opcode->power_up);
		MICO_SPI_CMD_CFG_WR_MANUFACTURER_ID_BYTEWISE (control_base, opcode->read_id);
	}
		
	return (SPI_COMMAND_SUCCESS);
}
