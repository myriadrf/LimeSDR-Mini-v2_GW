#ifndef __SYSTEM_CONFIG_H_
#define __SYSTEM_CONFIG_H_


#define FPGA_DEVICE_FAMILY    "ECP5U"
#define PLATFORM_NAME         "platform1"
#define USE_PLL               (0)
#define CPU_FREQUENCY         (77500000)


/* FOUND 1 CPU UNIT(S) */

/*
 * CPU Instance LM32 component configuration
 */
#define CPU_NAME "LM32"
#define CPU_EBA (0x08000000)
#define CPU_DIVIDE_ENABLED (1)
#define CPU_SIGN_EXTEND_ENABLED (1)
#define CPU_MULTIPLIER_ENABLED (1)
#define CPU_SHIFT_ENABLED (1)
#define CPU_DEBUG_ENABLED (1)
#define CPU_HW_BREAKPOINTS_ENABLED (0)
#define CPU_NUM_HW_BREAKPOINTS (0)
#define CPU_NUM_WATCHPOINTS (0)
#define CPU_ICACHE_ENABLED (1)
#define CPU_ICACHE_SETS (512)
#define CPU_ICACHE_ASSOC (1)
#define CPU_ICACHE_BYTES_PER_LINE (16)
#define CPU_DCACHE_ENABLED (1)
#define CPU_DCACHE_SETS (512)
#define CPU_DCACHE_ASSOC (1)
#define CPU_DCACHE_BYTES_PER_LINE (16)
#define CPU_DEBA (0x00000000)
#define CPU_CHARIO_IN        (1)
#define CPU_CHARIO_OUT       (1)
#define CPU_CHARIO_TYPE      "JTAG UART"

/*
 * ebr component configuration
 */
#define EBR_NAME  "ebr"
#define EBR_BASE_ADDRESS  (0x08000000)
#define EBR_SIZE  (32768)
#define EBR_IS_READABLE   (1)
#define EBR_IS_WRITABLE   (1)
#define EBR_ADDRESS_LOCK  (1)
#define EBR_DISABLE  (0)
#define EBR_WB_DAT_WIDTH  (32)
#define EBR_INIT_FILE_NAME  "none"
#define EBR_INIT_FILE_FORMAT  "hex"

/*
 * SPIFlash_ component configuration
 */
#define SPIFLASH__NAME  "SPIFlash_"
#define SPIFLASH__BASE_ADDRESS  (0x10000000)
#define SPIFLASH__SIZE  (134217728)
#define SPIFLASH__IS_READABLE   (1)
#define SPIFLASH__IS_WRITABLE   (1)
#define SPIFLASH__LATTICE_FAMILY  "XP"
#define SPIFLASH__PORT_TYPE  "Memory"
#define SPIFLASH__WB_DAT_WIDTH  (8)
#define SPIFLASH__WB_ADR_WIDTH  (32)
#define SPIFLASH__PORT_TYPE  "Memory"
#define SPIFLASH__PORT_ENABLE  (1)
#define SPIFLASH__WB_DAT_WIDTH  (8)
#define SPIFLASH__WB_ADR_WIDTH  (32)
#define SPIFLASH__PORT_CONNECTION  "S;C;S:C"
#define SPIFLASH__CLOCK_SEL  (10)
#define SPIFLASH__PAGE_SIZE  (256)
#define SPIFLASH__SECTOR_SIZE  (4096)
#define SPIFLASH__PAGE_PRG_BUF_ENA  (1)
#define SPIFLASH__PAGE_PRG_BUFFER_EBR  (0)
#define SPIFLASH__PAGE_PRG_BUFFER_DISTRIBUTED_RAM  (1)
#define SPIFLASH__PAGE_READ_BUF_ENA  (1)
#define SPIFLASH__PAGE_READ_BUFFER_EBR  (0)
#define SPIFLASH__PAGE_READ_BUFFER_DISTRIBUTED_RAM  (1)
#define SPIFLASH__SPI_READ  (3)
#define SPIFLASH__SPI_FAST_READ  (11)
#define SPIFLASH__SPI_BYTE_PRG  (2)
#define SPIFLASH__SPI_PAGE_PRG  (2)
#define SPIFLASH__SPI_BLK1_ERS  (32)
#define SPIFLASH__SPI_BLK2_ERS  (82)
#define SPIFLASH__SPI_BLK3_ERS  (216)
#define SPIFLASH__SPI_CHIP_ERS  (199)
#define SPIFLASH__SPI_WRT_ENB  (6)
#define SPIFLASH__SPI_WRT_DISB  (4)
#define SPIFLASH__SPI_READ_STAT  (5)
#define SPIFLASH__SPI_WRT_STAT  (1)
#define SPIFLASH__SPI_PWD_DOWN  (185)
#define SPIFLASH__SPI_PWD_UP  (171)
#define SPIFLASH__SPI_DEV_ID  (144)
#define SPIFLASH__ADDRESS_LOCK  (1)
#define SPIFLASH__DISABLE  (0)
#define SPIFLASH__ADDRESS_LOCK  (1)

/*
 * LED component configuration
 */
#define LED_NAME  "LED"
#define LED_BASE_ADDRESS  (0x80000800)
#define LED_SIZE  (16)
#define LED_CHARIO_IN        (0)
#define LED_CHARIO_OUT       (0)
#define LED_WB_DAT_WIDTH  (32)
#define LED_WB_ADR_WIDTH  (4)
#define LED_ADDRESS_LOCK  (1)
#define LED_DISABLE  (0)
#define LED_OUTPUT_PORTS_ONLY  (1)
#define LED_INPUT_PORTS_ONLY  (0)
#define LED_TRISTATE_PORTS  (0)
#define LED_BOTH_INPUT_AND_OUTPUT  (0)
#define LED_DATA_WIDTH  (8)
#define LED_INPUT_WIDTH  (1)
#define LED_OUTPUT_WIDTH  (1)
#define LED_IRQ_MODE  (0)
#define LED_LEVEL  (0)
#define LED_EDGE  (0)
#define LED_EITHER_EDGE_IRQ  (0)
#define LED_POSE_EDGE_IRQ  (0)
#define LED_NEGE_EDGE_IRQ  (0)

/*
 * GPIO component configuration
 */
#define GPIO_NAME  "GPIO"
#define GPIO_BASE_ADDRESS  (0x80001000)
#define GPIO_SIZE  (16)
#define GPIO_CHARIO_IN        (0)
#define GPIO_CHARIO_OUT       (0)
#define GPIO_WB_DAT_WIDTH  (32)
#define GPIO_WB_ADR_WIDTH  (4)
#define GPIO_ADDRESS_LOCK  (1)
#define GPIO_DISABLE  (0)
#define GPIO_OUTPUT_PORTS_ONLY  (0)
#define GPIO_INPUT_PORTS_ONLY  (0)
#define GPIO_TRISTATE_PORTS  (0)
#define GPIO_BOTH_INPUT_AND_OUTPUT  (1)
#define GPIO_DATA_WIDTH  (1)
#define GPIO_INPUT_WIDTH  (1)
#define GPIO_OUTPUT_WIDTH  (1)
#define GPIO_IRQ_MODE  (0)
#define GPIO_LEVEL  (0)
#define GPIO_EDGE  (0)
#define GPIO_EITHER_EDGE_IRQ  (0)
#define GPIO_POSE_EDGE_IRQ  (0)
#define GPIO_NEGE_EDGE_IRQ  (0)

/*
 * GPO component configuration
 */
#define GPO_NAME  "GPO"
#define GPO_BASE_ADDRESS  (0x80001800)
#define GPO_SIZE  (32)
#define GPO_IRQ (0)
#define GPO_CHARIO_IN        (0)
#define GPO_CHARIO_OUT       (0)
#define GPO_ADDRESS_LOCK  (1)
#define GPO_DISABLE  (0)
#define GPO_reg_08_int_val  (305441741)

/*
 * spi component configuration
 */
#define SPI_NAME  "spi"
#define SPI_BASE_ADDRESS  (0x80002000)
#define SPI_SIZE  (256)
#define SPI_IRQ (1)
#define SPI_CHARIO_IN        (0)
#define SPI_CHARIO_OUT       (0)
#define SPI_ADDRESS_LOCK  (1)
#define SPI_DISABLE  (0)
#define SPI_MASTER  (1)
#define SPI_SLAVE_NUMBER  (2)
#define SPI_CLOCK_SEL  (7)
#define SPI_CLKCNT_WIDTH  (16)
#define SPI_DELAY_TIME  (3)
#define SPI_INTERVAL_LENGTH  (2)
#define SPI_DATA_LENGTH  (32)
#define SPI_SHIFT_DIRECTION  (0)
#define SPI_CLOCK_PHASE  (0)
#define SPI_CLOCK_POLARITY  (0)

/*
 * FIFO component configuration
 */
#define FIFO_NAME  "FIFO"
#define FIFO_BASE_ADDRESS  (0x80002800)
#define FIFO_SIZE  (32)
#define FIFO_IRQ (2)
#define FIFO_CHARIO_IN        (0)
#define FIFO_CHARIO_OUT       (0)
#define FIFO_ADDRESS_LOCK  (1)
#define FIFO_DISABLE  (0)
#define FIFO_reg_16_int_val  (305441741)

/*
 * uart component configuration
 */
#define UART_NAME  "uart"
#define UART_BASE_ADDRESS  (0x80003000)
#define UART_SIZE  (16)
#define UART_IRQ (3)
#define UART_CHARIO_IN        (1)
#define UART_CHARIO_OUT       (1)
#define UART_CHARIO_TYPE      "RS-232"
#define UART_ADDRESS_LOCK  (1)
#define UART_DISABLE  (0)
#define UART_MODEM  (0)
#define UART_WB_DAT_WIDTH  (8)
#define UART_WB_ADR_WIDTH  (4)
#define UART_BAUD_RATE  (115200)
#define UART_IB_SIZE  (4)
#define UART_OB_SIZE  (4)
#define UART_BLOCK_WRITE  (1)
#define UART_BLOCK_READ  (1)
#define UART_STDOUT_SIM  (0)
#define UART_STDOUT_SIMFAST  (0)
#define UART_RXRDY_ENABLE  (0)
#define UART_TXRDY_ENABLE  (0)
#define UART_INTERRUPT_DRIVEN  (1)
#define UART_LCR_DATA_BITS  (8)
#define UART_LCR_STOP_BITS  (1)
#define UART_LCR_PARITY_ENABLE  (0)
#define UART_LCR_PARITY_ODD  (0)
#define UART_LCR_PARITY_STICK  (0)
#define UART_LCR_SET_BREAK  (0)
#define UART_FIFO  (0)

/*
 * lms_ctr_gpio component configuration
 */
#define LMS_CTR_GPIO_NAME  "lms_ctr_gpio"
#define LMS_CTR_GPIO_BASE_ADDRESS  (0x80003800)
#define LMS_CTR_GPIO_SIZE  (16)
#define LMS_CTR_GPIO_CHARIO_IN        (0)
#define LMS_CTR_GPIO_CHARIO_OUT       (0)
#define LMS_CTR_GPIO_WB_DAT_WIDTH  (32)
#define LMS_CTR_GPIO_WB_ADR_WIDTH  (4)
#define LMS_CTR_GPIO_ADDRESS_LOCK  (1)
#define LMS_CTR_GPIO_DISABLE  (0)
#define LMS_CTR_GPIO_OUTPUT_PORTS_ONLY  (1)
#define LMS_CTR_GPIO_INPUT_PORTS_ONLY  (0)
#define LMS_CTR_GPIO_TRISTATE_PORTS  (0)
#define LMS_CTR_GPIO_BOTH_INPUT_AND_OUTPUT  (0)
#define LMS_CTR_GPIO_DATA_WIDTH  (4)
#define LMS_CTR_GPIO_INPUT_WIDTH  (1)
#define LMS_CTR_GPIO_OUTPUT_WIDTH  (1)
#define LMS_CTR_GPIO_IRQ_MODE  (0)
#define LMS_CTR_GPIO_LEVEL  (0)
#define LMS_CTR_GPIO_EDGE  (0)
#define LMS_CTR_GPIO_EITHER_EDGE_IRQ  (0)
#define LMS_CTR_GPIO_POSE_EDGE_IRQ  (0)
#define LMS_CTR_GPIO_NEGE_EDGE_IRQ  (0)

/*
 * SPIFlash_USRMCLKTS_ component configuration
 */
#define SPIFLASH_USRMCLKTS__NAME  "SPIFlash_USRMCLKTS_"
#define SPIFLASH_USRMCLKTS__BASE_ADDRESS  (0x80004000)
#define SPIFLASH_USRMCLKTS__SIZE  (16)
#define SPIFLASH_USRMCLKTS__CHARIO_IN        (0)
#define SPIFLASH_USRMCLKTS__CHARIO_OUT       (0)
#define SPIFLASH_USRMCLKTS__WB_DAT_WIDTH  (32)
#define SPIFLASH_USRMCLKTS__WB_ADR_WIDTH  (4)
#define SPIFLASH_USRMCLKTS__ADDRESS_LOCK  (1)
#define SPIFLASH_USRMCLKTS__DISABLE  (0)
#define SPIFLASH_USRMCLKTS__OUTPUT_PORTS_ONLY  (1)
#define SPIFLASH_USRMCLKTS__INPUT_PORTS_ONLY  (0)
#define SPIFLASH_USRMCLKTS__TRISTATE_PORTS  (0)
#define SPIFLASH_USRMCLKTS__BOTH_INPUT_AND_OUTPUT  (0)
#define SPIFLASH_USRMCLKTS__DATA_WIDTH  (1)
#define SPIFLASH_USRMCLKTS__INPUT_WIDTH  (1)
#define SPIFLASH_USRMCLKTS__OUTPUT_WIDTH  (1)
#define SPIFLASH_USRMCLKTS__IRQ_MODE  (0)
#define SPIFLASH_USRMCLKTS__LEVEL  (0)
#define SPIFLASH_USRMCLKTS__EDGE  (0)
#define SPIFLASH_USRMCLKTS__EITHER_EDGE_IRQ  (0)
#define SPIFLASH_USRMCLKTS__POSE_EDGE_IRQ  (0)
#define SPIFLASH_USRMCLKTS__NEGE_EDGE_IRQ  (0)

/*
 * i2cm_oc_ component configuration
 */
#define I2CM_OC__NAME  "i2cm_oc_"
#define I2CM_OC__BASE_ADDRESS  (0x80004800)
#define I2CM_OC__SIZE  (128)
#define I2CM_OC__IRQ (4)
#define I2CM_OC__CHARIO_IN        (0)
#define I2CM_OC__CHARIO_OUT       (0)
#define I2CM_OC__ADDRESS_LOCK  (1)
#define I2CM_OC__DISABLE  (0)
#define I2CM_OC__SPEED  (400)

/*
 * dac_spi_ component configuration
 */
#define DAC_SPI__NAME  "dac_spi_"
#define DAC_SPI__BASE_ADDRESS  (0x80005000)
#define DAC_SPI__SIZE  (256)
#define DAC_SPI__IRQ (5)
#define DAC_SPI__CHARIO_IN        (0)
#define DAC_SPI__CHARIO_OUT       (0)
#define DAC_SPI__ADDRESS_LOCK  (1)
#define DAC_SPI__DISABLE  (0)
#define DAC_SPI__MASTER  (1)
#define DAC_SPI__SLAVE_NUMBER  (1)
#define DAC_SPI__CLOCK_SEL  (7)
#define DAC_SPI__CLKCNT_WIDTH  (16)
#define DAC_SPI__DELAY_TIME  (3)
#define DAC_SPI__INTERVAL_LENGTH  (2)
#define DAC_SPI__DATA_LENGTH  (16)
#define DAC_SPI__SHIFT_DIRECTION  (0)
#define DAC_SPI__CLOCK_PHASE  (1)
#define DAC_SPI__CLOCK_POLARITY  (0)


#endif /* __SYSTEM_CONFIG_H_ */
