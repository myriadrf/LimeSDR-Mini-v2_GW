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
**  3.1   Apr-12-2010  Custom command address map is fixed.
**  3.2   Aug-26-2010  Macros updated to reflect the new memory map and the
**                     big-endian nature of LM32.
*****************************************************************************/
#ifndef MICOSPIFLASH_HEADER_FILE_
#define MICOSPIFLASH_HEADER_FILE_
#include "DDStructs.h"

#ifdef __cplusplus
extern "C"
{
#endif
	#define SPI_COMMAND_SUCCESS						(0)
	#define SPI_CONTROL_PORT_ERROR					(-1)
	#define SPI_ADDRESS_OUT_OF_RANGE				(-2)
	#define SPI_PAGE_READ_ERROR						(-3)
	#define SPI_PAGE_PROGRAM_ERROR					(-4)
	#define SPI_INVALID_BLOCK_ERASE_TYPE			(-5)

	#define SPI_CMD_PAGE_PROGRAM_OFFSET				(0x000)
	#define SPI_CMD_PAGE_READ_OFFSET				(0x004)
	#define SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET		(0x008)
	#define SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET		(0x00c)
	#define SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET		(0x010)
	#define SPI_CMD_CHIP_ERASE_OFFSET				(0x014)
	#define SPI_CMD_WRITE_ENABLE_OFFSET				(0x018)
	#define SPI_CMD_WRITE_DISABLE_OFFSET			(0x01c)
	#define	SPI_CMD_STATUS_READ_OFFSET				(0x020)
	#define	SPI_CMD_STATUS_WRITE_OFFSET				(0x024)
	#define SPI_CMD_POWER_DOWN_OFFSET				(0x028)
	#define SPI_CMD_POWER_UP_OFFSET					(0x02c)
	#define SPI_CMD_MANUFACTURER_ID_OFFSET			(0x030)
	#define SPI_CMD_CUSTOM_LSW_OFFSET				(0x040)
	#define SPI_CMD_CUSTOM_MSW_OFFSET				(0x044)
	#define SPI_CMD_CUSTOM_LENGTH_OFFSET			(0x048)
	#define SPI_CMD_CUSTOM_RETURN_LENGTH_OFFSET		(0x04c)
	#define SPI_CMD_CUSTOM_RETURN_DATA_OFFSET		(0x050)
	#define SPI_CMD_CUSTOM_OFFSET					(0x054)
	
	#define SPI_CFG_READ_SPEED_OFFSET				(0x100)
	#define SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET		(0x104)
	#define SPI_CFG_PAGE_READ_SIZE_OFFSET			(0x108)
	
	#define SPI_CMD_CFG_SLOW_READ_OFFSET			(0x180)
	#define SPI_CMD_CFG_FAST_READ_OFFSET			(0x184)
	#define SPI_CMD_CFG_BYTE_PROGRAM_OFFSET			(0x188)
	#define SPI_CMD_CFG_PAGE_PROGRAM_OFFSET			(0x18c)
	#define SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET	(0x190)
	#define SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET	(0x194)
	#define SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET	(0x198)
	#define SPI_CMD_CFG_CHIP_ERASE_OFFSET			(0x19c)
	#define SPI_CMD_CFG_WRITE_ENABLE_OFFSET			(0x1a0)
	#define SPI_CMD_CFG_WRITE_DISABLE_OFFSET		(0x1a4)
	#define	SPI_CMD_CFG_STATUS_READ_OFFSET			(0x1a8)
	#define	SPI_CMD_CFG_STATUS_WRITE_OFFSET			(0x1ac)
	#define SPI_CMD_CFG_POWER_DOWN_OFFSET			(0x1b0)
	#define SPI_CMD_CFG_POWER_UP_OFFSET				(0x1b4)
	#define SPI_CMD_CFG_MANUFACTURER_ID_OFFSET		(0x1b8)
	
	#define SPI_PAGE_PROGRAM_BUFFER_OFFSET			(0x200)
	#define SPI_PAGE_READ_BUFFER_OFFSET				(0x400)
	
	/***********************************************************************
 	 *                                                                     *
 	 * USER MACROS (For a 32-Bit WISHBONE Control Port					*
 	 *                                                                     *
 	 ***********************************************************************/
	
	/* Macro for accessing SPI Page Program Command in memory map */
	#define MICO_SPI_PAGE_PROGRAM(X, Y) 						\
		*((volatile unsigned int *)(X+SPI_CMD_PAGE_PROGRAM_OFFSET)) = (Y)
	
	/* Macro for accessing SPI Page Read Command in memory map */
	#define MICO_SPI_PAGE_READ(X, Y) 							\
		*((volatile unsigned int *)(X+SPI_CMD_PAGE_READ_OFFSET)) = (Y)
  	
	/* Macro for accessing SPI 4K Block Erase Command in memory map */
	//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
	//           the least-significant 12 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE1(X, Y)					\
		*((volatile unsigned int *)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET)) = (Y)
	
	/* Macro for accessing SPI 32K Block Erase Command in memory map */
	//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
	//           the least-significant 15 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE2(X, Y)					\
		*((volatile unsigned int *)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET)) = (Y)
	
	/* Macro for accessing SPI 64K Block Erase Command in memory map */
	//     NOTE: don't program two least significant bytes since SPI Flash will anyways
	//            ignore the least-significant 16 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE3(X, Y)					\
		*((volatile unsigned int *)(X+SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET)) = (Y)
	
	/* Macro for accessing SPI Chip Erase Command in memory map */
	#define MICO_SPI_CHIP_ERASE(X) 								\
		*((volatile unsigned int *)(X+SPI_CMD_CHIP_ERASE_OFFSET)) = 0x01000000
		
	/* Macro for accessing SPI Write Enable Command in memory map */
	#define MICO_SPI_WRITE_ENABLE(X) 							\
	*((volatile unsigned int *)(X+SPI_CMD_WRITE_ENABLE_OFFSET)) = 0x01000000
			
	/* Macro for accessing SPI Write Disable Command in memory map */
	#define MICO_SPI_WRITE_DISABLE(X) 							\
		*((volatile unsigned int *)(X+SPI_CMD_WRITE_DISABLE_OFFSET)) = 0x01000000
  		
	/* Macro for accessing SPI Status Read Command in memory map */
	#define MICO_SPI_STATUS_READ(X, Y)							\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_STATUS_READ_OFFSET)) >> 24)
		
	/* Macro for accessing SPI Status Write Command in memory map */
	#define MICO_SPI_STATUS_WRITE(X, Y) 						\
  		*((volatile unsigned int *)(X+SPI_CMD_STATUS_WRITE_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Power Down Command in memory map */
	#define MICO_SPI_POWER_DOWN(X) 								\
  		*((volatile unsigned int *)(X+SPI_CMD_POWER_DOWN_OFFSET)) = 0x01000000
	
	/* Macro for accessing SPI Power Up Command in memory map */
	#define MICO_SPI_POWER_UP(X) 								\
  		*((volatile unsigned int *)(X+SPI_CMD_POWER_UP_OFFSET)) = 0x01000000
	
	/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
	#define MICO_SPI_MANUFACTURER_ID(X, Y) 						\
  		(Y) = *((volatile unsigned int *)(X+SPI_CMD_MANUFACTURER_ID_OFFSET))
	
	/* Macro for accessing SPI Custom Command Lease-Signifcant Word in memory map */
	#define MICO_SPI_CUSTOM_LSW(X, Y) 							\
		*((volatile unsigned int *)(X+SPI_CMD_CUSTOM_LSW_OFFSET)) = (Y)
	
	/* Macro for accessing SPI Custom Command Most-Significant Word in memory map */
	#define MICO_SPI_CUSTOM_MSW(X, Y) 							\
		*((volatile unsigned int *)(X+SPI_CMD_CUSTOM_MSW_OFFSET)) = (Y)
		
	/* Macro for accessing SPI Custom Command Length in memory map */
	#define MICO_SPI_CUSTOM_LENGTH(X, Y)						\
	  	*((volatile unsigned int *)(X+SPI_CMD_CUSTOM_LENGTH_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Custom Command Return Data Length in memory map */
	#define MICO_SPI_CUSTOM_RETURN_LENGTH(X, Y)					\
  		*((volatile unsigned int *)(X+SPI_CMD_CUSTOM_RETURN_LENGTH_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Custom Command Return Data in memory map */
	#define MICO_SPI_CUSTOM_RETURN_DATA(X, Y) 						\
  		(Y) = *((volatile unsigned int *)(X+SPI_CMD_CUSTOM_RETURN_DATA_OFFSET))
	
	/* Macro for accessing SPI Custom Command in memory map */
	#define MICO_SPI_CUSTOM(X)									\
  		*((volatile unsigned int *)(X+SPI_CMD_CUSTOM_OFFSET)) = 0x01010101
	  
	/* Macro for accessing Read Speed configuration in memory map */
	#define MICO_SPI_CFG_RD_READ_SPEED(X, Y)					\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CFG_READ_SPEED_OFFSET)) >> 24)
	#define MICO_SPI_CFG_WR_READ_SPEED(X, Y)					\
  		*((volatile unsigned int *)(X+SPI_CFG_READ_SPEED_OFFSET)) = (Y<<24)
	  
	/* Macro for accessing Page Program Size configuration in memory map */
	#define MICO_SPI_CFG_RD_PAGE_PROGRAM_SIZE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET)) >> 16)
	#define MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET)) = (Y<<16)
	
	/* Macro for accessing Read Speed configuration in memory map */
	#define MICO_SPI_CFG_RD_PAGE_READ_SIZE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET)) >> 16)
	#define MICO_SPI_CFG_WR_PAGE_READ_SIZE(X, Y)				\
		*((volatile unsigned int *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET)) = (Y<<16)
	
	/* Macro for accessing SPI Slow Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_SLOW_READ(X, Y)					\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_SLOW_READ_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_SLOW_READ(X, Y)					\
		*((volatile unsigned int *)(X+SPI_CMD_CFG_SLOW_READ_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Fast Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_FAST_READ(X, Y)					\
	  	(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_FAST_READ_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_FAST_READ(X, Y)					\
		*((volatile unsigned int *)(X+SPI_CMD_CFG_FAST_READ_OFFSET)) = (Y<<24)
 	
	/* Macro for accessing SPI Byte Program Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BYTE_PROGRAM(X, Y)				\
    	(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_BYTE_PROGRAM(X, Y)				\
		*((volatile unsigned int *)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Page Program Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_PAGE_PROGRAM(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_PAGE_PROGRAM(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI 4K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE1(X, Y)			\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE1(X, Y)			\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI 32K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE2(X, Y)			\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE2(X, Y)			\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI 64K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE3(X, Y)			\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE3(X, Y)			\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Chip Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_CHIP_ERASE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_CHIP_ERASE(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Write Enable Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_WRITE_ENABLE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_WRITE_ENABLE(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Write Disable Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_WRITE_DISABLE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_WRITE_DISABLE(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Status Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_STATUS_READ(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_STATUS_READ_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_STATUS_READ(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_STATUS_READ_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Status Write Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_STATUS_WRITE(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_STATUS_WRITE(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Power Down Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_POWER_DOWN(X, Y)				\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_POWER_DOWN(X, Y)				\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Power Up Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_POWER_UP(X, Y)					\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_POWER_UP_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_POWER_UP(X, Y)					\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_POWER_UP_OFFSET)) = (Y<<24)
	
	/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_MANUFACTURER_ID(X, Y)			\
  		(Y) = (*((volatile unsigned int *)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET)) >> 24)
	#define MICO_SPI_CMD_CFG_WR_MANUFACTURER_ID(X, Y)			\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET)) = (Y<<24)

	/***********************************************************************
 	 *                                                                     *
 	 * USER MACROS (For a 8-Bit WISHBONE Control Port					   *
 	 *                                                                     *
 	 ***********************************************************************/

	/* Macro for accessing SPI Page Program Command in memory map */
	#define MICO_SPI_PAGE_PROGRAM_BYTEWISE(X, Y) 					\
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x3)) = (unsigned char)(Y)    ; \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x0)) = 1
	
	/* Macro for accessing SPI Page Read Command in memory map */
	#define MICO_SPI_PAGE_READ_BYTEWISE(X, Y) 						\
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_READ_OFFSET+0x3)) = (unsigned char)(Y)    ; \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_READ_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_READ_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_PAGE_READ_OFFSET+0x0)) = 1
  	
	/* Macro for accessing SPI 4K Block Erase Command in memory map */
	//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
	//           the least-significant 12 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE1_BYTEWISE(X, Y)				\
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET+0x0)) = 1
	
	/* Macro for accessing SPI 32K Block Erase Command in memory map */
	//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
	//           the least-significant 15 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE2_BYTEWISE(X, Y)				\
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET+0x0)) = 1
	
	/* Macro for accessing SPI 64K Block Erase Command in memory map */
	//     NOTE: don't program two least significant bytes since SPI Flash will anyways
	//            ignore the least-significant 16 bits of the address.
	#define MICO_SPI_BLOCK_ERASE_TYPE3_BYTEWISE(X, Y)				\
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET+0x0)) = 1
	
	/* Macro for accessing SPI Chip Erase Command in memory map */
	#define MICO_SPI_CHIP_ERASE_BYTEWISE(X) 						\
		*((volatile unsigned char *)(X+SPI_CMD_CHIP_ERASE_OFFSET)) = 1
		
	/* Macro for accessing SPI Write Enable Command in memory map */
	#define MICO_SPI_WRITE_ENABLE_BYTEWISE(X) 						\
	*((volatile unsigned char *)(X+SPI_CMD_WRITE_ENABLE_OFFSET)) = 1
			
	/* Macro for accessing SPI Write Disable Command in memory map */
	#define MICO_SPI_WRITE_DISABLE_BYTEWISE(X) 						\
		*((volatile unsigned char *)(X+SPI_CMD_WRITE_DISABLE_OFFSET)) = 1
  		
	/* Macro for accessing SPI Status Read Command in memory map */
	#define MICO_SPI_STATUS_READ_BYTEWISE(X, Y)						\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_STATUS_READ_OFFSET+0x3))
	
	/* Macro for accessing SPI Status Write Command in memory map */
	#define MICO_SPI_STATUS_WRITE_BYTEWISE(X, Y) 					\
  		*((volatile unsigned char *)(X+SPI_CMD_STATUS_WRITE_OFFSET+0x0)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Power Down Command in memory map */
	#define MICO_SPI_POWER_DOWN_BYTEWISE(X) 						\
  		*((volatile unsigned char *)(X+SPI_CMD_POWER_DOWN_OFFSET)) = 1
	
	/* Macro for accessing SPI Power Up Command in memory map */
	#define MICO_SPI_POWER_UP_BYTEWISE(X) 							\
  		*((volatile unsigned char *)(X+SPI_CMD_POWER_UP_OFFSET)) = 1
	
	/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
	#define MICO_SPI_MANUFACTURER_ID_BYTEWISE(X, Y) 				\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_MANUFACTURER_ID_OFFSET))
	
	/* Macro for accessing SPI Custom Command Lease-Signifcant Word in memory map */
	#define MICO_SPI_CUSTOM_LSW_BYTEWISE(X, Y) 						\
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x0)) = (unsigned char)(Y>>24); \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x3)) = (unsigned char)(Y)    ; \
	
	/* Macro for accessing SPI Custom Command Most-Significant Word in memory map */
	#define MICO_SPI_CUSTOM_MSW_BYTEWISE(X, Y) 						\
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x0)) = (unsigned char)(Y>>24); \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x1)) = (unsigned char)(Y>>16); \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x2)) = (unsigned char)(Y>>8) ; \
		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x3)) = (unsigned char)(Y)    ; \
		
	/* Macro for accessing SPI Custom Command Length in memory map */
	#define MICO_SPI_CUSTOM_LENGTH_BYTEWISE(X, Y)					\
	  	*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_LENGTH_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Custom Command Return Data Length in memory map */
	#define MICO_SPI_CUSTOM_RETURN_LENGTH_BYTEWISE(X, Y)			\
  		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_RETURN_LENGTH_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Custom Command Return Data in memory map */
	#define MICO_SPI_CUSTOM_RETURN_DATA_BYTEWISE(X) 				\
  		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_RETURN_DATA_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Custom Command in memory map */
	#define MICO_SPI_CUSTOM_BYTEWISE(X)								\
  		*((volatile unsigned char *)(X+SPI_CMD_CUSTOM_OFFSET)) = 1
	  
	/* Macro for accessing Read Speed configuration in memory map */
	#define MICO_SPI_CFG_RD_READ_SPEED_BYTEWISE(X, Y)				\
  		(Y) = *((volatile unsigned char *)(X+SPI_CFG_READ_SPEED_OFFSET))
	#define MICO_SPI_CFG_WR_READ_SPEED_BYTEWISE(X, Y)				\
  		*((volatile unsigned char *)(X+SPI_CFG_READ_SPEED_OFFSET)) = (unsigned char)(Y)
	  
	/* Macro for accessing Page Program Size configuration in memory map */
	#define MICO_SPI_CFG_RD_PAGE_PROGRAM_SIZE_BYTEWISE(X, Y)		\
  		(Y) = (unsigned int)(*((volatile unsigned char *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET))<<8) | (unsigned int)(*((volatile unsigned char *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET+0x1)))
	#define MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET+0x0)) = (unsigned char)(Y>>8) ; \
  		*((volatile unsigned char *)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET+0x1)) = (unsigned char)(Y)
	
	/* Macro for accessing Page Read Size configuration in memory map */
	#define MICO_SPI_CFG_RD_PAGE_READ_SIZE_BYTEWISE(X, Y)			\
  		(Y) = (unsigned int)(*((volatile unsigned char *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET))<<8) | (unsigned int)(*((volatile unsigned char *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET+0x1)))
	#define MICO_SPI_CFG_WR_PAGE_READ_SIZE_BYTEWISE(X, Y)			\
  		*((volatile unsigned char *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET+0x0)) = (unsigned char)(Y>>8) ; \
  		*((volatile unsigned char *)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET+0x1)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Slow Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_SLOW_READ_BYTEWISE(X, Y)			\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_SLOW_READ_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_SLOW_READ_BYTEWISE(X, Y)			\
		*((volatile unsigned char *)(X+SPI_CMD_CFG_SLOW_READ_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Fast Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_FAST_READ_BYTEWISE(X, Y)			\
	  	(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_FAST_READ_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_FAST_READ_BYTEWISE(X, Y)			\
		*((volatile unsigned char *)(X+SPI_CMD_CFG_FAST_READ_OFFSET)) = (unsigned char)(Y)
 	
	/* Macro for accessing SPI Byte Program Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BYTE_PROGRAM_BYTEWISE(X, Y)			\
    	(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_BYTE_PROGRAM_BYTEWISE(X, Y)			\
		*((volatile unsigned char *)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Page Program Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_PAGE_PROGRAM_BYTEWISE(X, Y)			\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_PAGE_PROGRAM_BYTEWISE(X, Y)			\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI 4K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE1_BYTEWISE(X, Y)	\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE1_BYTEWISE(X, Y)	\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI 32K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE2_BYTEWISE(X, Y)	\
  		(Y) = *((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE2_BYTEWISE(X, Y)	\
  		*((volatile unsigned int *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI 64K Block Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE3_BYTEWISE(X, Y)	\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE3_BYTEWISE(X, Y)	\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Chip Erase Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_CHIP_ERASE_BYTEWISE(X, Y)		\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_CHIP_ERASE_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Write Enable Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_WRITE_ENABLE_BYTEWISE(X, Y)		\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_WRITE_ENABLE_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Write Disable Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_WRITE_DISABLE_BYTEWISE(X, Y)	\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_WRITE_DISABLE_BYTEWISE(X, Y)	\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Status Read Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_STATUS_READ_BYTEWISE(X, Y)		\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_STATUS_READ_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_STATUS_READ_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_STATUS_READ_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Status Write Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_STATUS_WRITE_BYTEWISE(X, Y)		\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_STATUS_WRITE_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Power Down Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_POWER_DOWN_BYTEWISE(X, Y)		\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_POWER_DOWN_BYTEWISE(X, Y)		\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Power Up Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_POWER_UP_BYTEWISE(X, Y)			\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_POWER_UP_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_POWER_UP_BYTEWISE(X, Y)			\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_POWER_UP_OFFSET)) = (unsigned char)(Y)
	
	/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
	#define MICO_SPI_CMD_CFG_RD_MANUFACTURER_ID_BYTEWISE(X, Y)	\
  		(Y) = *((volatile unsigned char *)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET))
	#define MICO_SPI_CMD_CFG_WR_MANUFACTURER_ID_BYTEWISE(X, Y)	\
  		*((volatile unsigned char *)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET)) = (unsigned char)(Y)

	/**********************************************************
	 * SPI Flash Register Structure							  *
	 **********************************************************/
	
	typedef struct st_MicoSPIFlash {
		volatile unsigned int cmd;
		volatile unsigned int cmd_txdata;
		volatile unsigned int cmd_rxdata;
		volatile unsigned int cmd_length;
		volatile unsigned int cmd_tx_length;
		volatile unsigned int cmd_rx_length;
		volatile unsigned int cmd_control;
	} MicoSPIFlash_t;
	
	typedef struct st_MicoSPIFlashCmdOpcodeTable {
		volatile char slow_read;
		volatile char fast_read;
		volatile char byte_program;
		volatile char page_program;
		volatile char block_erase_type1;
		volatile char block_erase_type2;
		volatile char block_erase_type3;
		volatile char chip_erase;
		volatile char write_enable;
		volatile char write_disable;
		volatile char power_down;
		volatile char power_up;
		volatile char read_id;
	} MicoSPIFlashCmdOpcodeTable_t;
	
	/**********************************************************
	 * FUNCTIONS											  *
	 **********************************************************/
	
	void MicoSPIFlash_Initialize (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_ReadID (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_StatusRead (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_WriteEnable (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_WriteDisable (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_ChipErase (MicoSPIFlashCtx_t *ctx);
	int  MicoSPIFlash_BlockErase (MicoSPIFlashCtx_t *ctx, unsigned int anAddress, unsigned int aType);
	int  MicoSPIFlash_PageProgram (MicoSPIFlashCtx_t *ctx, unsigned int anAddress, unsigned int aLength, char *rData);
	int  MicoSPIFlash_PageRead (MicoSPIFlashCtx_t *ctx, unsigned int anAddress, unsigned int aLength, char *rData);
	int  MicoSPIFlash_AlignedPageProgram (MicoSPIFlashCtx_t *ctx, unsigned int anAddress, unsigned int aLength, unsigned int *rData);
	int  MicoSPIFlash_AlignedPageRead (MicoSPIFlashCtx_t *ctx, unsigned int anAddress, unsigned int aLength, unsigned int *rData);
	int  MicoSPIFlash_ReadSpeed (MicoSPIFlashCtx_t *ctx, unsigned int aSpeed);
	int  MicoSPIFlash_ReadCmdOpcodes (MicoSPIFlashCtx_t *ctx, MicoSPIFlashCmdOpcodeTable_t *opcode);
	int  MicoSPIFlash_WriteCmdOpcodes (MicoSPIFlashCtx_t *ctx, MicoSPIFlashCmdOpcodeTable_t *opcode);
	
	/**********************************************************
	 * MISC     											  *
	 **********************************************************/
	
	#ifndef BIT_DEFINES_DEFINED
	#define BIT_DEFINES_DEFINED
	#define ALL_BITS					(0xffffffff)
	#define BIT_31						(0x80000000)
	#define BIT_30						(0x40000000)
	#define BIT_29						(0x20000000)
	#define BIT_28						(0x10000000)
	#define BIT_27						(0x08000000)
	#define BIT_26						(0x04000000)
	#define BIT_25						(0x02000000)
	#define BIT_24						(0x01000000)
	#define BIT_23						(0x00800000)
	#define BIT_22						(0x00400000)
	#define BIT_21						(0x00200000)
	#define BIT_20						(0x00100000)
	#define BIT_19						(0x00080000)
	#define BIT_18						(0x00040000)
	#define BIT_17						(0x00020000)
	#define BIT_16						(0x00010000)
	#define BIT_15						(0x00008000)
	#define BIT_14						(0x00004000)
	#define BIT_13						(0x00002000)
	#define BIT_12						(0x00001000)
	#define BIT_11						(0x00000800)
	#define BIT_10						(0x00000400)
	#define BIT_9						(0x00000200)
	#define BIT_8						(0x00000100)
	#define BIT_7						(0x00000080)
	#define BIT_6						(0x00000040)
	#define BIT_5						(0x00000020)
	#define BIT_4						(0x00000010)
	#define BIT_3						(0x00000008)
	#define BIT_2						(0x00000004)
	#define BIT_1						(0x00000002)
	#define BIT_0						(0x00000001)
	#endif
#ifdef __cplusplus
}
#endif

#endif
