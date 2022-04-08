`define LATTICE_FAMILY "ECP5U"
`define LATTICE_FAMILY_ECP5U
`define LATTICE_DEVICE "LFE5U-85F"
`ifndef SYSTEM_CONF
`define SYSTEM_CONF
`timescale 1ns / 100 ps
`ifndef SIMULATION
   `define CharIODevice
`endif
`ifndef SIMULATION
   `define DEBUG_ROM
`endif
`ifndef SIMULATION
   `define CFG_DEBUG_ENABLED
`endif
`define CFG_EBA_RESET 32'h8000000
`define CFG_DEBA_RESET 32'h0
`define CFG_DISTRAM_POSEDGE_REGISTER_FILE
`define MULT_ENABLE
`define CFG_PL_MULTIPLY_ENABLED
`define SHIFT_ENABLE
`define CFG_PL_BARREL_SHIFT_ENABLED
`define CFG_MC_DIVIDE_ENABLED
`define CFG_SIGN_EXTEND_ENABLED
`ifndef SIMULATION
   `define CFG_WATCHPOINTS 32'h0
`endif
`ifndef SIMULATION
   `define CFG_JTAG_ENABLED
`endif
`ifndef SIMULATION
   `define CFG_JTAG_UART_ENABLED
`endif
`define CFG_ICACHE_ENABLED
`define CFG_ICACHE_BASE_ADDRESS 32'h0
`define CFG_ICACHE_LIMIT 32'h17ffffff
`define CFG_ICACHE_SETS 512
`define CFG_ICACHE_ASSOCIATIVITY 1
`define CFG_ICACHE_BYTES_PER_LINE 16
`define CFG_ICACHE_AUTO
`define CFG_DCACHE_ENABLED
`define CFG_DCACHE_BASE_ADDRESS 32'h0
`define CFG_DCACHE_LIMIT 32'h17ffffff
`define CFG_DCACHE_SETS 512
`define CFG_DCACHE_ASSOCIATIVITY 1
`define CFG_DCACHE_BYTES_PER_LINE 16
`define CFG_DCACHE_AUTO
`define INCLUDE_LM32
`define ADDRESS_LOCK
`define LM32_I_PC_WIDTH 28
`define ADDRESS_LOCK
`define ebrEBR_WB_DAT_WIDTH 32
`define ebrINIT_FILE_NAME "none"
`define ebrINIT_FILE_FORMAT "hex"
`define SPIFlash_LATTICE_FAMILY "ECP5U"
`define S_PORT_TYPE "Memory"
`define SPIFlash_S_WB_DAT_WIDTH 8
`define SPIFlash_S_WB_ADR_WIDTH 32
`define C_PORT_TYPE "IO"
`define SPIFlash_C_PORT_ENABLE
`define SPIFlash_C_WB_DAT_WIDTH 8
`define SPIFlash_C_WB_ADR_WIDTH 11
`define PORT_CONNECTION "S;C;S:C"
`define SPIFlash_CLOCK_SEL 32'ha
`define PAGE_SIZE 32'h100
`define SECTOR_SIZE 32'h1000
`define SPIFlash_PAGE_PRG_BUF_ENA
`define SPIFlash_PAGE_PRG_BUFFER_DISTRIBUTED_RAM
`define SPIFlash_PAGE_READ_BUF_ENA
`define SPIFlash_PAGE_READ_BUFFER_DISTRIBUTED_RAM
`define SPIFlash_SPI_READ 32'h3
`define SPIFlash_SPI_FAST_READ 32'hb
`define SPIFlash_SPI_BYTE_PRG 32'h2
`define SPIFlash_SPI_PAGE_PRG 32'h2
`define SPIFlash_SPI_BLK1_ERS 32'h20
`define SPIFlash_SPI_BLK2_ERS 32'h52
`define SPIFlash_SPI_BLK3_ERS 32'hd8
`define SPIFlash_SPI_CHIP_ERS 32'hc7
`define SPIFlash_SPI_WRT_ENB 32'h6
`define SPIFlash_SPI_WRT_DISB 32'h4
`define SPIFlash_SPI_READ_STAT 32'h5
`define SPIFlash_SPI_WRT_STAT 32'h1
`define SPIFlash_SPI_PWD_DOWN 32'hb9
`define SPIFlash_SPI_PWD_UP 32'hab
`define SPIFlash_SPI_DEV_ID 32'h90
`define S_ADDRESS_LOCK
`define C_ADDRESS_LOCK
`define LEDGPIO_WB_DAT_WIDTH 32
`define LEDGPIO_WB_ADR_WIDTH 4
`define ADDRESS_LOCK
`define LEDOUTPUT_PORTS_ONLY
`define LEDDATA_WIDTH 32'h8
`define GPIOGPIO_WB_DAT_WIDTH 32
`define GPIOGPIO_WB_ADR_WIDTH 4
`define ADDRESS_LOCK
`define GPIOBOTH_INPUT_AND_OUTPUT
`define GPIODATA_WIDTH 32'h1
`define GPIOINPUT_WIDTH 32'h1
`define GPIOOUTPUT_WIDTH 32'h1
`define ADDRESS_LOCK
`define GPOreg_08_int_val 32'h1234abcd
`define GPOCLK_MHZ 0
`define ADDRESS_LOCK
`define spiMASTER
`define spiSLAVE_NUMBER 32'h2
`define spiCLOCK_SEL 7
`define spiCLKCNT_WIDTH 16
`define DELAY_TIME 3
`define spiINTERVAL_LENGTH 2
`define spiDATA_LENGTH 32
`define spiSHIFT_DIRECTION 0
`define spiCLOCK_PHASE 0
`define spiCLOCK_POLARITY 0
`define ADDRESS_LOCK
`define FIFOreg_16_int_val 32'h1234abcd
`define ADDRESS_LOCK
`define uartUART_WB_DAT_WIDTH 8
`define uartUART_WB_ADR_WIDTH 4
`define uartCLK_IN_MHZ 0
`define uartBAUD_RATE 115200
`define IB_SIZE 32'h4
`define OB_SIZE 32'h4
`define BLOCK_WRITE
`define BLOCK_READ
`define INTERRUPT_DRIVEN
`define CharIODevice
`define uartLCR_DATA_BITS 8
`define uartLCR_STOP_BITS 1
`define lms_ctr_gpioGPIO_WB_DAT_WIDTH 32
`define lms_ctr_gpioGPIO_WB_ADR_WIDTH 4
`define ADDRESS_LOCK
`define lms_ctr_gpioOUTPUT_PORTS_ONLY
`define lms_ctr_gpioDATA_WIDTH 32'h4
`define SPIFlash_USRMCLKTS_GPIO_WB_DAT_WIDTH 32
`define SPIFlash_USRMCLKTS_GPIO_WB_ADR_WIDTH 4
`define ADDRESS_LOCK
`define SPIFlash_USRMCLKTS_OUTPUT_PORTS_ONLY
`define SPIFlash_USRMCLKTS_DATA_WIDTH 32'h1
`define ADDRESS_LOCK
`define i2cm_oc_SPEED 400
`define i2cm_oc_SYSCLK 0
`define ADDRESS_LOCK
`define dac_spi_MASTER
`define dac_spi_SLAVE_NUMBER 32'h1
`define dac_spi_CLOCK_SEL 7
`define dac_spi_CLKCNT_WIDTH 16
`define DELAY_TIME 3
`define dac_spi_INTERVAL_LENGTH 2
`define dac_spi_DATA_LENGTH 16
`define dac_spi_SHIFT_DIRECTION 0
`define dac_spi_CLOCK_PHASE 0
`define dac_spi_CLOCK_POLARITY 1
`endif // SYSTEM_CONF
