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
#include <stddef.h>
#include "DDStructs.h"

/***********************************************************************
 *                                                                     *
 * SPI FLASH CONTROLLER PHYSICAL DEVICE SPECIFIC INFORMATION           *
 *                                                                     *
 ***********************************************************************/

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
 * USER MACROS                                                         *
 *                                                                     *
 ***********************************************************************/
	
/* Macro for accessing SPI Page Program Command in memory map */
#define MICO_SPI_PAGE_PROGRAM(X, Y) 						\
  (__builtin_export((char)(Y)    , (size_t)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x3)) , \
   __builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x2)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_PAGE_PROGRAM_OFFSET+0x1)) , \
   __builtin_export((char)(1)    , (size_t)(X+SPI_CMD_PAGE_PROGRAM_OFFSET)))

/* Macro for accessing SPI Page Read Command in memory map */
#define MICO_SPI_PAGE_READ(X, Y) 							\
  (__builtin_export((char)(Y)    , (size_t)(X+SPI_CMD_PAGE_READ_OFFSET+0x3)) , \
   __builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_PAGE_READ_OFFSET+0x2)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_PAGE_READ_OFFSET+0x1)) , \
   __builtin_export((char)(1)    , (size_t)(X+SPI_CMD_PAGE_READ_OFFSET)))

/* Macro for accessing SPI 4K Block Erase Command in memory map */
//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
//           the least-significant 12 bits of the address.
#define MICO_SPI_BLOCK_ERASE_TYPE1(X, Y)					\
  (__builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET+0x2)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET+0x1)) , \
   __builtin_export((char)(1)    , (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE1_OFFSET)))

/* Macro for accessing SPI 32K Block Erase Command in memory map */
//     NOTE: don't program least significant byte since SPI Flash will anyways ignore
//           the least-significant 15 bits of the address.
#define MICO_SPI_BLOCK_ERASE_TYPE2(X, Y)					\
  (__builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET+0x2)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET+0x1)) , \
   __builtin_export((char)(1)    , (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE2_OFFSET)))

/* Macro for accessing SPI 64K Block Erase Command in memory map */
//     NOTE: don't program two least significant bytes since SPI Flash will anyways
//            ignore the least-significant 16 bits of the address.
#define MICO_SPI_BLOCK_ERASE_TYPE3(X, Y)					\
  (__builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET+0x01)) , \
   __builtin_export((char)(1)    , (size_t)(X+SPI_CMD_BLOCK_ERASE_TYPE3_OFFSET)))

/* Macro for accessing SPI Chip Erase Command in memory map */
#define MICO_SPI_CHIP_ERASE(X) 								\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_CHIP_ERASE_OFFSET)))
	
/* Macro for accessing SPI Write Enable Command in memory map */
#define MICO_SPI_WRITE_ENABLE(X) 							\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_WRITE_ENABLE_OFFSET)))
	
/* Macro for accessing SPI Write Disable Command in memory map */
#define MICO_SPI_WRITE_DISABLE(X) 							\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_WRITE_DISABLE_OFFSET)))

/* Macro for accessing SPI Status Read Command in memory map */
#define MICO_SPI_STATUS_READ(X)								\
  (__builtin_import((size_t)(X+SPI_CMD_STATUS_READ_OFFSET)))

/* Macro for accessing SPI Status Write Command in memory map */
#define MICO_SPI_STATUS_WRITE(X, Y) 						\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_STATUS_WRITE_OFFSET)))

/* Macro for accessing SPI Power Down Command in memory map */
#define MICO_SPI_POWER_DOWN(X) 								\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_POWER_DOWN_OFFSET)))

/* Macro for accessing SPI Power Up Command in memory map */
#define MICO_SPI_POWER_UP(X) 								\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_POWER_UP_OFFSET)))

/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
#define MICO_SPI_MANUFACTURER_ID(X) 						\
  (__builtin_import((size_t)(X+SPI_CMD_MANUFACTURER_ID_OFFSET)))

/* Macro for accessing SPI Custom Command Lease-Signifcant Word in memory map */
#define MICO_SPI_CUSTOM_LSW(X, Y) 							\
  (__builtin_export((char)(Y>>24), (size_t)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x0)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x1)) , \
   __builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x2)) , \
   __builtin_export((char)(Y)    , (size_t)(X+SPI_CMD_CUSTOM_LSW_OFFSET+0x3)))

/* Macro for accessing SPI Custom Command Most-Significant Word in memory map */
#define MICO_SPI_CUSTOM_MSW(X, Y) 							\
  (__builtin_export((char)(Y>>24), (size_t)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x0)) , \
   __builtin_export((char)(Y>>16), (size_t)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x1)) , \
   __builtin_export((char)(Y>>8) , (size_t)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x2)) , \
   __builtin_export((char)(Y)    , (size_t)(X+SPI_CMD_CUSTOM_MSW_OFFSET+0x3)))
	
/* Macro for accessing SPI Custom Command Length in memory map */
#define MICO_SPI_CUSTOM_LENGTH(X, Y)						\
  (__builtin_export((char)(Y&0x7), (size_t)(X+SPI_CMD_CUSTOM_LENGTH_OFFSET)))

/* Macro for accessing SPI Custom Command Return Data Length in memory map */
#define MICO_SPI_CUSTOM_RETURN_LENGTH(X, Y)					\
  (__builtin_export((char)(Y&0x1), (size_t)(X+SPI_CMD_CUSTOM_RETURN_LENGTH_OFFSET)))

/* Macro for accessing SPI Custom Command Return Data in memory map */
#define MICO_SPI_CUSTOM_RETURN_DATA(X) 						\
  (__builtin_import((size_t)(X+SPI_CMD_CUSTOM_RETURN_DATA_OFFSET)))

/* Macro for accessing SPI Custom Command in memory map */
#define MICO_SPI_CUSTOM(X)									\
  (__builtin_export((char)(1), (size_t)(X+SPI_CMD_CUSTOM_OFFSET)))
  
/* Macro for accessing Read Speed configuration in memory map */
#define MICO_SPI_CFG_RD_READ_SPEED(X, Y)					\
  (Y) = (__builtin_import((size_t)(X+SPI_CFG_READ_SPEED_OFFSET)))
#define MICO_SPI_CFG_WR_READ_SPEED(X, Y)					\
  (__builtin_export((char)(Y&0x1), (size_t)(X+SPI_CFG_READ_SPEED_OFFSET)))
  
/* Macro for accessing Page Program Size configuration in memory map */
#define MICO_SPI_CFG_RD_PAGE_PROGRAM_SIZE(X, Y)				\
  (Y) = ((((unsigned int)__builtin_import((size_t)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET)))<<8) | ((unsigned int)__builtin_import((size_t)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET+0x1))))
#define MICO_SPI_CFG_WR_PAGE_PROGRAM_SIZE(X, Y)				\
  (__builtin_export((char)(Y>>8), (size_t)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET)) , \
   __builtin_export((char)(Y)   , (size_t)(X+SPI_CFG_PAGE_PROGRAM_SIZE_OFFSET+0x1)))

/* Macro for accessing Page Read Size configuration in memory map */
#define MICO_SPI_CFG_RD_PAGE_READ_SIZE(X, Y)				\
  (Y) = ((((unsigned int)__builtin_import((size_t)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET)))<<8) | ((unsigned int)__builtin_import((size_t)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET+0x1))))
#define MICO_SPI_CFG_WR_PAGE_READ_SIZE(X, Y)				\
  (__builtin_export((char)(Y>>8), (size_t)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET)) , \
   __builtin_export((char)(Y)   , (size_t)(X+SPI_CFG_PAGE_READ_SIZE_OFFSET+0x1)))

/* Macro for accessing SPI Slow Read Command in memory map */
#define MICO_SPI_CMD_CFG_RD_SLOW_READ(X, Y)					\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_SLOW_READ_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_SLOW_READ(X, Y)					\
  (__builtin_export((char)Y, (size_t)(X+SPI_CMD_CFG_SLOW_READ_OFFSET)))

/* Macro for accessing SPI Fast Read Command in memory map */
#define MICO_SPI_CMD_CFG_RD_FAST_READ(X, Y)					\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_FAST_READ_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_FAST_READ(X, Y)					\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_FAST_READ_OFFSET)))

/* Macro for accessing SPI Byte Program Command in memory map */
#define MICO_SPI_CMD_CFG_RD_BYTE_PROGRAM(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_BYTE_PROGRAM(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_BYTE_PROGRAM_OFFSET)))

/* Macro for accessing SPI Page Program Command in memory map */
#define MICO_SPI_CMD_CFG_RD_PAGE_PROGRAM(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_PAGE_PROGRAM(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_PAGE_PROGRAM_OFFSET)))

/* Macro for accessing SPI 4K Block Erase Command in memory map */
#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE1(X, Y)			\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE1(X, Y)			\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE1_OFFSET)))

/* Macro for accessing SPI 32K Block Erase Command in memory map */
#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE2(X, Y)			\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE2(X, Y)			\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE2_OFFSET)))

/* Macro for accessing SPI 64K Block Erase Command in memory map */
#define MICO_SPI_CMD_CFG_RD_BLOCK_ERASE_TYPE3(X, Y)			\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_BLOCK_ERASE_TYPE3(X, Y)			\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_BLOCK_ERASE_TYPE3_OFFSET)))

/* Macro for accessing SPI Chip Erase Command in memory map */
#define MICO_SPI_CMD_CFG_RD_CHIP_ERASE(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_CHIP_ERASE(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_CHIP_ERASE_OFFSET)))

/* Macro for accessing SPI Write Enable Command in memory map */
#define MICO_SPI_CMD_CFG_RD_WRITE_ENABLE(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_WRITE_ENABLE(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_WRITE_ENABLE_OFFSET)))

/* Macro for accessing SPI Write Disable Command in memory map */
#define MICO_SPI_CMD_CFG_RD_WRITE_DISABLE(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_WRITE_DISABLE(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_WRITE_DISABLE_OFFSET)))

/* Macro for accessing SPI Status Read Command in memory map */
#define MICO_SPI_CMD_CFG_RD_STATUS_READ(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_STATUS_READ_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_STATUS_READ(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_STATUS_READ_OFFSET)))

/* Macro for accessing SPI Status Write Command in memory map */
#define MICO_SPI_CMD_CFG_RD_STATUS_WRITE(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_STATUS_WRITE(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_STATUS_WRITE_OFFSET)))

/* Macro for accessing SPI Power Down Command in memory map */
#define MICO_SPI_CMD_CFG_RD_POWER_DOWN(X, Y)				\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_POWER_DOWN(X, Y)				\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_POWER_DOWN_OFFSET)))

/* Macro for accessing SPI Power Up Command in memory map */
#define MICO_SPI_CMD_CFG_RD_POWER_UP(X, Y)					\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_POWER_UP_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_POWER_UP(X, Y)					\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_POWER_UP_OFFSET)))

/* Macro for accessing SPI Read Manufacturer ID Command in memory map */
#define MICO_SPI_CMD_CFG_RD_MANUFACTURER_ID(X, Y)			\
  (Y) = (__builtin_import((size_t)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET)))
#define MICO_SPI_CMD_CFG_WR_MANUFACTURER_ID(X, Y)			\
  (__builtin_export((char)(Y), (size_t)(X+SPI_CMD_CFG_MANUFACTURER_ID_OFFSET)))


/***********************************************************************
 *                                                                     *
 * USER FUNCTIONS                                                      *
 *                                                                     *
 ***********************************************************************/

void MicoSPIFlash_Initialize (MicoSPIFlashCtx_t *ctx);
void MicoSPIFlash_BlockErase (MicoSPIFlashCtx_t *ctx, unsigned long anAddress, unsigned char aType);
void MicoSPIFlash_PageOp (MicoSPIFlashCtx_t *ctx, unsigned long anAddress, char *rData, char read_op);


/***********************************************************************
 *                                                                     *
 * ERROR CODES                                                         *
 *                                                                     *
 ***********************************************************************/

#define SPI_COMMAND_SUCCESS	     		(0)
#define SPI_CONTROL_PORT_ERROR	     	(-1)
#define SPI_ADDRESS_OUT_OF_RANGE     	(-2)
#define SPI_PAGE_READ_ERROR	     		(-3)
#define SPI_PAGE_PROGRAM_ERROR	     	(-4)
#define SPI_INVALID_BLOCK_ERASE_TYPE 	(-5)

