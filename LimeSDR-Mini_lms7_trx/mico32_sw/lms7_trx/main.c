/**************************************************************
 * This example exercises LEDs on LatticeMico32 Development   *
 * board.                                                     *
 *                                                            *
 *                                                            *
 *                                                            *
 *------------------------------------------------------------*
 * PREREQUISITES:                                             *
 *                                                            *                                                                                                    
 * - GPIO with 8-bit output named LED connected to the        *
 *   board's LED pins.                                        *
 **************************************************************/
#include "DDStructs.h"
#include "LookupServices.h"
#include "stdint.h"
//#include <stdio.h>
#include "MicoUtils.h"
#include "system_conf.h"
#include "MicoGPIO.h"
#include "MicoSPIService.h"
#include "MicoSPIFlash.h"
#include "OpenCoresI2CMaster.h"

#include "LMS64C_protocol.h"
#include "LimeSDR_MINI_brd_v1r0.h"

#define sbi(p,n) ((p) |= (1UL << (n)))
#define cbi(p,n) ((p) &= ~(1 << (n)))

//get info
//#define FW_VER				1 //Initial version
//#define FW_VER				2 //FLASH programming added
//#define FW_VER				3 //Temperature and Si5351C control added
//#define FW_VER				4 //LM75 configured to control fan; I2C speed increased up to 400kHz; ADF/DAC control implementation.
//#define FW_VER				5 //EEPROM and FLASH R/W functionality added
//#define FW_VER				6 // DAC value read from EEPROM memory
//#define FW_VER				7 // DAC value read from FLASH memory
//#define FW_VER				8 // Added FLASH write command protect when write count is 0
//#define FW_VER				9 // Temporary fix for LM75 configuration
#define FW_VER			   10 // Fix for LM75 temperature reading with 0.5 precision


#define SPI_LMS7002_SELECT 0x01
#define SPI_FPGA_SELECT 0x02

#define DAC_VAL_ADDR  			0x0010		// Address in EEPROM memory where TCXO DAC value is stored
#define DAC_VAL_ADDR_IN_FLASH  	0x00FF0000	// Address in FLASH memory where TCXO DAC value is stored
#define DAC_DEFF_VAL			566			// Default TCXO DAC value loaded when EEPROM is empty

#define FLASH_USRSEC_START_ADDR	0x00400000  // Start address for user space in FLASH memory


const char *LED_GPIO_INSTANCE = "LED";
const char *GPIO_GPIO_INSTANCE = "GPIO";
const char *SPI_INSTANCE = "spi";
const char *SPI_DAC_INSTANCE = "dac_spi_";
const char *GPIO_LMS_CTR_GPIO_INSTANCE = "lms_ctr_gpio";
const char *SPI_FLASH_INSTANCE = "SPIFlash_";
const char *SPI_FLASH_USRMCLKTS_INSTANCE = "SPIFlash_USRMCLKTS_";
const char *I2C_OC_INSTANCE = "i2cm_oc_";
const unsigned int uiBlink = 1;

uint8_t test, block, cmd_errors, glEp0Buffer_Rx[64], glEp0Buffer_Tx[64];
tLMS_Ctrl_Packet *LMS_Ctrl_Packet_Tx = (tLMS_Ctrl_Packet*)glEp0Buffer_Tx;
tLMS_Ctrl_Packet *LMS_Ctrl_Packet_Rx = (tLMS_Ctrl_Packet*)glEp0Buffer_Rx;

int flash_page = 0, flash_page_data_cnt = 0, flash_data_cnt_free = 0, flash_data_counter_to_copy = 0;
//FPGA conf
unsigned long int last_portion, current_portion, fpga_data;
unsigned char data_cnt;
unsigned char sc_brdg_data[4];
unsigned char flash_page_data[FLASH_PAGE_SIZE];
tBoard_Config_FPGA *Board_Config_FPGA = (tBoard_Config_FPGA*) flash_page_data;
unsigned long int fpga_byte;

// Used for MAX10 Flash programming
uint32_t CFM0StartAddress = 0x000000;
uint32_t CFM0EndAddress   = 0x13FFFF;
uint32_t address = 0x0;
uint32_t byte = 0;
uint32_t byte1;
uint32_t word = 0x0;
uint8_t state, Flash = 0x0;
char spiflash_wdata[4];

int boot_img_en = 0;

uint16_t dac_val = 720;
unsigned char dac_data[2];

signed short int converted_val = 300;


unsigned int lat_wishbone_spi_command(MicoSPICtx_t *pMaster, int slave_address, unsigned int write_data, uint8_t *rd_data)
{
	//SPI
	unsigned int read_data = 0x0;
	unsigned int slave_address_int;
	uint32_t* dest = (uint32_t*)rd_data;
	/* Enable slave by writing to master */
	MicoSPISetSlaveEnable(pMaster, slave_address);
	/* Check slave enable status. */
	MicoSPIGetSlaveEnable(pMaster, &slave_address_int);
	if(slave_address != slave_address_int){
		//printf("failed to select internal slave! fatal error\n");
		while(1);
	}

	/* write data targetted for slave */
	MicoSPITxData(pMaster, write_data,1);

	/* wait for master's transmission to complete */
	while(1){
		volatile unsigned int iTimeout;
		unsigned int iValue;
		volatile unsigned int *pStatus;
		do{
			if(MicoSPIIsTxDone(pMaster)!=0)
				break;
		}while(1);

		/* read data if there's data to read */
		iTimeout = 0;
		do{
			if(MicoSPIRxData(pMaster, &read_data, 0) == 0)
				break;
			if(iTimeout >= 10){
				pStatus = (volatile unsigned int *)(pMaster->base + 8);
				iValue = *pStatus;
				pStatus = (volatile unsigned int *)(pMaster->base + 0);
				iValue = *pStatus;
				//printf("master failed to rx data within a second %d\n", iTimeout);
				while(1);
			}
			iTimeout++;
			MicoSleepMilliSecs(100);
		}while(1);
		break;
	}
	*dest = read_data;
	return read_data;

}

unsigned int IORD(uint32_t BASE, uint32_t OFFSET)
{
	uint32_t *word_aligned_address = (uint32_t*) (BASE + OFFSET * 4);
	uint32_t val = 0;

	val = *word_aligned_address;
	return val;
}

void IOWR(unsigned int BASE, unsigned int OFFSET, unsigned int DATA)
{
	unsigned int *reg = BASE + OFFSET * 4;
	*reg = DATA;
}

int FIFO_loopback_test(int base_addr)
{
    unsigned int fifo_val =0;
    int fifo_wrcnt = 0;
    int fifo_wr_data =0;
    int fifo_rd_cnt =0;
    int fifo_wr_data_array[4];
    int fifo_rd_data_array[4];

	//FIFO testing
	fifo_wrcnt = 0;
	fifo_wr_data = 1;

	//Reset FIFO
	//printf("FIFO reset \n\n");
	IOWR(base_addr, 3, 0x1);
	IOWR(base_addr, 3, 0x0);
	MicoSleepMilliSecs(100);

	while ((IORD(base_addr, 2) & 0x2) != 0x2)
	{
		IOWR(base_addr, 0, fifo_wr_data);
		//printf("FIFO Write: %#x \n", fifo_wr_data);
		fifo_wr_data_array[fifo_wrcnt]=fifo_wr_data;
		fifo_wrcnt++;
		fifo_wr_data++;
		//MicoSleepMilliSecs(100);

	}
	//printf("FIFO Write counter: %d \n", fifo_wrcnt);

	fifo_rd_cnt=0;
	while (!(IORD(base_addr, 2) & 0x1))
	{
		fifo_val = IORD(base_addr, 1);
		fifo_rd_data_array[fifo_rd_cnt] = fifo_val;
		//MicoSleepMilliSecs(100);
		//printf("FIFO Read: %#x \n", fifo_val);
		fifo_rd_cnt++;
	}
	//printf("FIFO Read counter: %d \n", fifo_rd_cnt);

	int i = 0;
	if (fifo_wrcnt == fifo_rd_cnt){
		//printf("FIFO WR/RD counters match \n");
		for (i =0; i < fifo_wrcnt; i++){
			if (fifo_wr_data_array[i] != fifo_rd_data_array[i]){
				//printf("FIFO WR/RD arrays does not match \n");
				return(0);
			}
		}
	}
	else {
		//printf("FIFO WR/RD counters does not match \n");
		return(0);
	}

	return(1);
}

/**
 * Gets 64 bytes packet from FIFO.
 */

void getFifoData(uint8_t *buf, uint8_t k)
{
	uint8_t cnt = 0;
	uint32_t* dest = (uint32_t*)buf;
	uint32_t fifo_val = 0;

	for(cnt=0; cnt<k/sizeof(uint32_t); ++cnt)
	{
		fifo_val = IORD(FIFO_BASE_ADDRESS, 1);
		dest[cnt] = ((fifo_val & 0x000000FF) <<24) | ((fifo_val & 0x0000FF00) <<8) | ((fifo_val & 0x00FF0000) >>8) | ((fifo_val & 0xFF000000) >>24);	// Read Data from FIFO
		//dest[cnt] = IORD(FIFO_BASE_ADDRESS, 1);
	};
}

/**	This function checks if all blocks could fit in data field.
*	If blocks will not fit, function returns TRUE. */
unsigned char Check_many_blocks (unsigned char block_size)
{
	if (LMS_Ctrl_Packet_Rx->Header.Data_blocks > (sizeof(LMS_Ctrl_Packet_Tx->Data_field)/block_size))
	{
		LMS_Ctrl_Packet_Tx->Header.Status = STATUS_BLOCKS_ERROR_CMD;
		return 1;
	}
	else return 0;
	return 1;
}

void spiflash_wr_command(MicoSPIFlashCtx_t *ctx, unsigned int MSW, unsigned int LSW, unsigned int LENGTH)
{
	MICO_SPI_CUSTOM_MSW_BYTEWISE(ctx->control_base, MSW); 		//Most significant word
	MICO_SPI_CUSTOM_LSW_BYTEWISE(ctx->control_base, LSW); 		//Most significant word
	MICO_SPI_CUSTOM_LENGTH_BYTEWISE(ctx->control_base, LENGTH); 	// Write length
	MICO_SPI_CUSTOM_RETURN_LENGTH_BYTEWISE(ctx->control_base, 0x0);
	//Execute command
	MICO_SPI_CUSTOM_BYTEWISE(ctx->control_base);
}

void spiflash_rd_command(MicoSPIFlashCtx_t *ctx, unsigned int MSW, unsigned int LSW,
						unsigned int LENGTH, unsigned int RETURN_LENGTH, unsigned int *RETURN_DATA)
{
	unsigned int value;
	MICO_SPI_CUSTOM_MSW_BYTEWISE(ctx->control_base, MSW); 		//Most significant word
	MICO_SPI_CUSTOM_LSW_BYTEWISE(ctx->control_base, LSW); 		//Most significant word
	MICO_SPI_CUSTOM_LENGTH_BYTEWISE(ctx->control_base, LENGTH); 	// Write length
	MICO_SPI_CUSTOM_RETURN_LENGTH_BYTEWISE(ctx->control_base, RETURN_LENGTH);
	//Execute command
	MICO_SPI_CUSTOM_BYTEWISE(ctx->control_base);
	//Read received data from register
	MICO_SPI_CUSTOM_RETURN_DATA(ctx->control_base, value);
	*RETURN_DATA = value;
}

void spiflash_erase_primary(MicoSPIFlashCtx_t *ctx)
{
	int flash_op_status;
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00000000, 3);
	/*
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00010000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00020000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00030000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00040000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00050000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00060000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00070000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00080000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00090000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000A0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000B0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000C0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000D0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000E0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x000F0000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00100000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00110000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00120000, 3);
	flash_op_status = MicoSPIFlash_BlockErase(ctx, ctx->memory_base + 0x00130000, 3);
	*/
}

/**
 * Configures LM75
 */
void Configure_LM75(OpenCoresI2CMasterCtx_t *ctx)
{
	int spirez;
	unsigned char wdata[4];
	unsigned char rdata[4];

	// OS polarity configuration
	wdata[0]=0x01; // Pointer = configuration register
	wdata[1]=0x04; //Configuration value: OS polarity = 1, Comparator/int = 0, Shutdown = 0
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 2, wdata);
	OpenCoresI2CMasterStop(ctx);


	// Read  back OS polarity configuration
	wdata[0]=0x01; // Pointer = Configuration register
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 1, wdata);

	spirez = OpenCoresI2CMasterRead(ctx, LM75_I2C_ADDR, 1, rdata);
	OpenCoresI2CMasterStop(ctx);
	MicoSleepMilliSecs(100);



	// THYST configuration
	wdata[0]=0x02;	// Pointer = THYST register
	wdata[1]=0x2D;	// Set THYST H (45)
	wdata[2]=0;		// Set THYST L
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 3, wdata);
	OpenCoresI2CMasterStop(ctx);


	// Read  back THYST configuration
	wdata[0]=0x02; // Pointer = THYST
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 1, wdata);

	spirez = OpenCoresI2CMasterRead(ctx, LM75_I2C_ADDR, 2, rdata);
	OpenCoresI2CMasterStop(ctx);
	MicoSleepMilliSecs(100);

	// TOS configuration
	wdata[0]=0x03;	// Pointer = TOS register
	wdata[1]=0x37;	// Set TOS H (55)
	wdata[2]=0;		// Set TOS L
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 3, wdata);
	OpenCoresI2CMasterStop(ctx);


	// Read back TOS configuration
	wdata[0]=0x03; // Pointer = TOS
	spirez = OpenCoresI2CMasterWrite(ctx, LM75_I2C_ADDR, 1, wdata);

	spirez = OpenCoresI2CMasterRead(ctx, LM75_I2C_ADDR, 2, rdata);
	OpenCoresI2CMasterStop(ctx);
	MicoSleepMilliSecs(100);
}

//
void testEEPROM(OpenCoresI2CMasterCtx_t *ctx)
{
	int spirez;
	uint8_t converted_val;
	unsigned char wdata[4];
	unsigned char rdata[4]= {0xFF, 0xFF, 0xFF, 0xFF};

	/*
	//EEPROM Test, RD from 0x0000
	spirez = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x00, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x00, 0);
	*/

	//EEPROM Test, RD from 0x0000
	wdata[0]=0x00;
	wdata[1]=0x00;
	spirez = OpenCoresI2CMasterWrite(ctx, EEPROM_I2C_ADDR, 2, wdata);

	/*
	spirez = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 1);
	converted_val = I2C_read(I2C_OPENCORES_0_BASE, 1);
	*/
	spirez = OpenCoresI2CMasterRead(ctx, EEPROM_I2C_ADDR, 1, rdata);
	OpenCoresI2CMasterStop(ctx);

	/*
	//WR
	spirez = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x00, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x01, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x5A, 1);
	*/

	//WR
	wdata[0]=0x00;
	wdata[1]=0x01;
	wdata[2]=0x5A;
	spirez = OpenCoresI2CMasterWrite(ctx, EEPROM_I2C_ADDR, 3, wdata);
	OpenCoresI2CMasterStop(ctx);

	/*
	//EEPROM Test, RD from 0x0001
	spirez = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x00, 0);
	spirez = I2C_write(I2C_OPENCORES_0_BASE, 0x01, 0);

	spirez = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 1);
	converted_val = I2C_read(I2C_OPENCORES_0_BASE, 1);
	*/

	//EEPROM Test, RD from 0x0001
	wdata[0]=0x00;
	wdata[1]=0x01;
	spirez = OpenCoresI2CMasterWrite(ctx, EEPROM_I2C_ADDR, 2, wdata);

	spirez = OpenCoresI2CMasterRead(ctx, EEPROM_I2C_ADDR, 1, rdata);
	OpenCoresI2CMasterStop(ctx);
	converted_val = rdata[0];

}



uint16_t rd_dac_val(OpenCoresI2CMasterCtx_t *ctx, uint16_t addr)
{
	//uint8_t i2c_error;
	int i2c_error;
	uint8_t addr_lsb = (uint8_t) addr & 0x00FF;
	uint8_t addr_msb = (uint8_t) (addr & 0xFF00) >> 8;
	uint8_t eeprom_rd_val_0;
	uint8_t eeprom_rd_val_1;
	uint16_t rez;
	unsigned char wdata[4];
	unsigned char rdata[4]={0xFF, 0xFF, 0xFF, 0xFF};

	/*
	i2c_error = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 0);
	i2c_error = I2C_write(I2C_OPENCORES_0_BASE, addr_msb, 0);
	i2c_error = I2C_write(I2C_OPENCORES_0_BASE, addr_lsb, 0);
	i2c_error = I2C_start(I2C_OPENCORES_0_BASE, EEPROM_I2C_ADDR, 1);
	eeprom_rd_val_0 = I2C_read(I2C_OPENCORES_0_BASE, 0);
	eeprom_rd_val_1 = I2C_read(I2C_OPENCORES_0_BASE, 1);
	*/

	wdata[0]=addr_msb;
	wdata[1]=addr_lsb;
	i2c_error = OpenCoresI2CMasterWrite(ctx, EEPROM_I2C_ADDR, 2, wdata);
	i2c_error = OpenCoresI2CMasterRead(ctx, EEPROM_I2C_ADDR, 2, rdata);
	OpenCoresI2CMasterStop(ctx);
	eeprom_rd_val_0 = rdata[0];
	eeprom_rd_val_1 = rdata[1];

	rez = ((uint16_t)eeprom_rd_val_1 << 8) | eeprom_rd_val_0;
	return rez;
}


int main(void)
{
    uint32_t* dest = (uint32_t*)glEp0Buffer_Tx;
    volatile int spirez;
    int i2crez;
    int k;
    uint16_t * ptr_spi_wrdata;
    uint32_t * p_spi_wrdata32;
    char cnt = 0;

    unsigned char test_val;
    unsigned char value;

    char wdata[256];
    char rdata[256];

    char i2c_wdata[64];
    char i2c_rdata[64];
    uint16_t eeprom_dac_val;
    uint16_t flash_dac_val;

    //Flash testing
    unsigned int FPGA_CFG_FLASH_ID;
    unsigned int rd_status_1;
    unsigned int rd_status_2;
    unsigned int rd_status_3;
    unsigned int rd_data;
    int flash_op_status;
    int read_status;
    int read_data;

	unsigned char iValue = 0x1;
	unsigned int gpio_val = 0x0;
	unsigned int gpo_val = 0x0;
	unsigned int gpio_rd_val = 0x0;
	unsigned int gpio_rd_val2 = 0x0;
	static unsigned int spi_read_val = 0x0;
	unsigned int spi_read_val_p = 0x0;
	unsigned char iShiftLeft = 1;
	uint32_t dest_byte_reordered = 0;
	unsigned int dac_spi_wrdata = 0;

	/*
	* Names of the SPI master and slave, as
	* defined in MSB
	*/
	const char *const SPIM_INSTANCE_NAME = "spim_";

    /* Fetch GPIO instance named "LED" */
	MicoGPIOCtx_t *leds = (MicoGPIOCtx_t *)MicoGetDevice(LED_GPIO_INSTANCE);
    if (leds == 0) {
        //printf("failed to find GPIO instance named LED\r\n");
        return(0);
    }

    /* Fetch GPIO instance named "GPIO" */
	MicoGPIOCtx_t *gpio = (MicoGPIOCtx_t *)MicoGetDevice(GPIO_GPIO_INSTANCE);
    if (gpio == 0) {
        //printf("failed to find GPIO instance named GPIO\r\n");
        return(0);
    }

    /* Fetch GPIO instance named "lms_ctr_gpio" */
	MicoGPIOCtx_t *lms_ctr_gpio = (MicoGPIOCtx_t *)MicoGetDevice(GPIO_LMS_CTR_GPIO_INSTANCE);
    if (lms_ctr_gpio == 0) {
        //printf("failed to find GPIO instance named GPIO\r\n");
        return(0);
    }

	MicoGPIOCtx_t *spiflash_usrmclkts = (MicoGPIOCtx_t *)MicoGetDevice(SPI_FLASH_USRMCLKTS_INSTANCE);
    if (spiflash_usrmclkts == 0) {
        //printf("failed to find GPIO instance named GPIO\r\n");
        return(0);
    }

    // Fetch the context for SPI flash controller named "SPIFlash" from DDStructs.h
    MicoSPIFlashCtx_t *spiflash = (MicoSPIFlashCtx_t *)MicoGetDevice(SPI_FLASH_INSTANCE);
    if (spiflash == 0) {
    	return(0);
    }

    // Fetch the context for I2C master controller named "i2cm_oc_" from DDStructs.h
    OpenCoresI2CMasterCtx_t *i2c_master = (OpenCoresI2CMasterCtx_t *)MicoGetDevice(I2C_OC_INSTANCE);
    if (i2c_master == 0) {
    	return(0);
    }

    // Fetch the context for DAC SPI controller named "dac_spi_" from DDStructs.h
    MicoSPICtx_t *dac_spi = (MicoSPICtx_t *)MicoGetDevice(SPI_DAC_INSTANCE);
    if (dac_spi == 0) {
    	return(0);
    }

    MICO_GPIO_WRITE_DATA(spiflash_usrmclkts,0x0);

    unsigned int *reg = GPIO_BASE_ADDRESS;
    unsigned int *gpo_reg = GPO_BASE_ADDRESS;
    unsigned int *gpo_reg_04 = GPO_BASE_ADDRESS + 4;
    unsigned int *gpo_reg_08 = GPO_BASE_ADDRESS + 8;

	
    /* if we're not to blink, return immediately */
    if (uiBlink == 0)
        return(0);
    MICO_GPIO_WRITE_DATA(gpio,0xFFFFFFFF);

    *((volatile unsigned char *)(leds->base)) = 0xFF;



    //SPI test
    int runs = 0;
    int slave_address = 0x01;
    int master_txdata = 0x0;
    MicoSPICtx_t *pMaster;
    /* Fetch pointers to master/slave SPI dev-ctx instances */
    pMaster = (MicoSPICtx_t *)MicoGetDevice(SPI_INSTANCE);
    //TODO: Remove after testing
    int spi_rdwr = 0;
    unsigned int spi_wrval = 0;

    /* Make sure pointers are valid */
    if(pMaster == 0){
    	//printf("Cannot use SPI Master as ctx is unidentified\n");
    return(0);
    }

    // RESET FIFO once on power-up
	IOWR(FIFO_BASE_ADDRESS, 3, 0x1);
	IOWR(FIFO_BASE_ADDRESS, 3, 0x0);

    //Reset LMS7
	MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0x0);
	MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0xFFFFFFFF);

	//testEEPROM(i2c_master);

	// Read TCXO DAC value from EEPROM memory
	/*
	eeprom_dac_val = rd_dac_val(i2c_master, DAC_VAL_ADDR);
	if (eeprom_dac_val == 0xFFFF){
		dac_val = DAC_DEFF_VAL; //default DAC value
	}
	else {
		dac_val = (uint16_t) eeprom_dac_val;
	}
	*/

	if(MicoSPIFlash_PageRead(spiflash, spiflash->memory_base+DAC_VAL_ADDR_IN_FLASH, 0x2, rdata)!= 0) {
		dac_val = DAC_DEFF_VAL;
	}
	else {
		if (rdata[0]==0xFF & rdata[1]==0xFF) {
			dac_val = DAC_DEFF_VAL;
		}
		else {
			dac_val = ((uint16_t)rdata[1])<<8 | ((uint16_t)rdata[0]);
		}
	}

	spirez = MicoSPISetSlaveEnable(dac_spi, 1);
    // Write initial data to the 10bit DAC
	dac_data[0] = (unsigned char) ((dac_val & 0x03F0) >> 4); //POWER-DOWN MODE = NORMAL OPERATION (MSB bits =00) + MSB data
	dac_data[1] = (unsigned char) ((dac_val & 0x000F) << 4); //LSB data

	dac_spi_wrdata = ((unsigned int) dac_data[0]<<8)| ((unsigned int) dac_data[1]) ;
	spirez= MicoSPISetSlaveEnable(dac_spi, 0x01);
	spirez= MicoSPITxData(dac_spi, dac_spi_wrdata, 0);

	gpo_val = 0x00000000;
	*gpo_reg = gpo_val;

	gpo_val = 0x00000001;
	*gpo_reg = gpo_val;

	gpo_val = 0x00000000;
	*gpo_reg = gpo_val;

	Configure_LM75(i2c_master);


	while(1) {

		spirez = IORD(FIFO_BASE_ADDRESS, 2);	// Read FIFO Status

		if(!(spirez & 0x01))
		{
			gpo_val = 0x00000001;
			*gpo_reg = gpo_val;
			//Read packet from the FIFO
			getFifoData(glEp0Buffer_Rx, 64);

			memset(glEp0Buffer_Tx, 0, sizeof(glEp0Buffer_Tx)); //fill whole tx buffer with zeros
			cmd_errors = 0;

			LMS_Ctrl_Packet_Tx->Header.Command = LMS_Ctrl_Packet_Rx->Header.Command;
			LMS_Ctrl_Packet_Tx->Header.Data_blocks = LMS_Ctrl_Packet_Rx->Header.Data_blocks;
			LMS_Ctrl_Packet_Tx->Header.Periph_ID = LMS_Ctrl_Packet_Rx->Header.Periph_ID;
			LMS_Ctrl_Packet_Tx->Header.Status = STATUS_BUSY_CMD;

			switch (LMS_Ctrl_Packet_Rx->Header.Command) {

 			case CMD_GPIO_DIR_WR:
 				//if(Check_many_blocks (2)) break;

				//write reg addr
				sc_brdg_data[0] = 0x80;		// Write command & BOARD_GPIO_DIR register address MSB
				sc_brdg_data[1] = 0xC4;		// BOARD_GPIO_DIR register address LSB
				sc_brdg_data[2] = LMS_Ctrl_Packet_Rx->Data_field[1];	// Data fields swapped, while MSB in the data packet is in the
				sc_brdg_data[3] = LMS_Ctrl_Packet_Rx->Data_field[0];	// leftmost byte
				//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 4, sc_brdg_data, 0, NULL, 0);
				p_spi_wrdata32 = (uint32_t*) &sc_brdg_data;
				spi_read_val = lat_wishbone_spi_command(pMaster, SPI_FPGA_SELECT, *p_spi_wrdata32, 0);

 				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
 			break;


 			case CMD_GPIO_DIR_RD:
 				//if(Check_many_blocks (2)) break;

 				//write reg addr
 				sc_brdg_data[0] = 0x00;		// Read command & BOARD_GPIO_DIR register address MSB
 				sc_brdg_data[1] = 0xC4;		// BOARD_GPIO_DIR register address LSB
 				//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 2, sc_brdg_data, 2, &sc_brdg_data[2], 0);

 				LMS_Ctrl_Packet_Tx->Data_field[0] = sc_brdg_data[3];	// Data fields swapped, while MSB in the data packet is in the
 				LMS_Ctrl_Packet_Tx->Data_field[1] = sc_brdg_data[2];	// leftmost byte

 				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
 				break;



 			case CMD_GPIO_WR:
 				//if(Check_many_blocks (2)) break;

 				//write reg addr
 				sc_brdg_data[0] = 0x80;		// Write command & BOARD_GPIO_VAL register address MSB
 				sc_brdg_data[1] = 0xC6;		// BOARD_GPIO_VAL register address LSB
 				sc_brdg_data[2] = LMS_Ctrl_Packet_Rx->Data_field[1];	// Data fields swapped, while MSB in the data packet is in the
 				sc_brdg_data[3] = LMS_Ctrl_Packet_Rx->Data_field[0];	// leftmost byte
 				//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 4, sc_brdg_data, 0, NULL, 0);

 				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
 				break;

 			case CMD_GPIO_RD:
 				//if(Check_many_blocks (2)) break;

 				//write reg addr
 				sc_brdg_data[0] = 0x00;		// Read command & BOARD_GPIO_RD register address MSB
 				sc_brdg_data[1] = 0xC2;		// BOARD_GPIO_RD register address LSB
 				//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 2, sc_brdg_data, 2, &sc_brdg_data[2], 0);

 				LMS_Ctrl_Packet_Tx->Data_field[0] = sc_brdg_data[3];	// Data fields swapped, while MSB in the data packet is in the
 				LMS_Ctrl_Packet_Tx->Data_field[1] = sc_brdg_data[2];	// leftmost byte

 				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
 				break;

			case CMD_GET_INFO:

				LMS_Ctrl_Packet_Tx->Data_field[0] = FW_VER;
				LMS_Ctrl_Packet_Tx->Data_field[1] = DEV_TYPE;
				LMS_Ctrl_Packet_Tx->Data_field[2] = LMS_PROTOCOL_VER;
				LMS_Ctrl_Packet_Tx->Data_field[3] = HW_VER;
				LMS_Ctrl_Packet_Tx->Data_field[4] = EXP_BOARD;

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_LMS_RST:

				switch (LMS_Ctrl_Packet_Rx->Data_field[0])
				{
				case LMS_RST_DEACTIVATE:
					MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0xFFFFFFFF);
					break;

				case LMS_RST_ACTIVATE:
					MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0x0);
					break;

				case LMS_RST_PULSE:
					MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0x0);
					asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
					asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
					MICO_GPIO_WRITE_DATA(lms_ctr_gpio,0xFFFFFFFF);
					break;

				default:
					cmd_errors++;
					break;
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_LMS7002_WR:
				if (Check_many_blocks(4))
					break;

				for (block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++) {
					//write reg addr
					sbi(LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)], 7); //set write bit
					//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_LMS7002M, 4, &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)], 0, NULL, 0);
					p_spi_wrdata32 = (uint32_t*) &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)];
					spi_read_val = lat_wishbone_spi_command(pMaster, SPI_LMS7002_SELECT, *p_spi_wrdata32, 0);
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_LMS7002_RD:
				if (Check_many_blocks(4))
					break;

				for (block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++) {

					//write reg addr
					cbi(LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)], 7); //clear write bit
					//spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_LMS7002M, 2, &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)], 2, &LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)], 0);
					ptr_spi_wrdata = (uint16_t*) &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)];
					spi_read_val = lat_wishbone_spi_command(pMaster, SPI_LMS7002_SELECT, ((uint32_t) *ptr_spi_wrdata) << 16, &LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)]);
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_BRDSPI16_WR:
				if(Check_many_blocks (4)) break;

				for(block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++)
				{
					sbi(LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)], 7); //set write bit
					//TODO: spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 4, &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)], 0, NULL, 0);
					p_spi_wrdata32 = (uint32_t*) &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)];
					spi_read_val = lat_wishbone_spi_command(pMaster, SPI_FPGA_SELECT, *p_spi_wrdata32, 0);
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_BRDSPI16_RD:
				if(Check_many_blocks (4)) break;

				for(block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++)
				{
					cbi(LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)], 7);  //clear write bit
					//TODO: spirez = alt_avalon_spi_command(FPGA_SPI_BASE, SPI_NR_FPGA, 2, &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)], 2, &LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)], 0);
					ptr_spi_wrdata = (uint16_t*) &LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 2)];
					spi_read_val = lat_wishbone_spi_command(pMaster, SPI_FPGA_SELECT, ((uint32_t) *ptr_spi_wrdata) << 16, &LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)]);
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
				break;

			case CMD_MEMORY_WR:
				current_portion = (LMS_Ctrl_Packet_Rx->Data_field[1] << 24) | (LMS_Ctrl_Packet_Rx->Data_field[2] << 16) | (LMS_Ctrl_Packet_Rx->Data_field[3] << 8) | (LMS_Ctrl_Packet_Rx->Data_field[4]);
				data_cnt = LMS_Ctrl_Packet_Rx->Data_field[5];

				if((LMS_Ctrl_Packet_Rx->Data_field[10] == 0) && (LMS_Ctrl_Packet_Rx->Data_field[11] == 3)) //TARGET = 3 (EEPROM)
				{
					if(LMS_Ctrl_Packet_Rx->Data_field[0] == 0) //write data to EEPROM #1
					{
						i2c_wdata[0]= LMS_Ctrl_Packet_Rx->Data_field[8];
						i2c_wdata[1]= LMS_Ctrl_Packet_Rx->Data_field[9];

						for(k=0; k<data_cnt; k++)
						{
							i2c_wdata[k+2]= LMS_Ctrl_Packet_Rx->Data_field[24+k];
						}
						i2crez = OpenCoresI2CMasterWrite(i2c_master, EEPROM_I2C_ADDR, data_cnt+2, i2c_wdata);
						OpenCoresI2CMasterStop(i2c_master);
						MicoSleepMilliSecs(5);

						if(i2crez) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
						else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
					}
					else
						LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
				}
				else
				{

					if((LMS_Ctrl_Packet_Rx->Data_field[10] == 0) && (LMS_Ctrl_Packet_Rx->Data_field[11] == 1)) // TARGET = 1 (FX3)
					{
						switch (LMS_Ctrl_Packet_Rx->Data_field[0]) //PROG_MODE
						{

							case 2: //PROG_MODE = 2 (write FW to flash). Note please, that this command is used just to program XO DAC value

								flash_page = (LMS_Ctrl_Packet_Rx->Data_field[6] << 24) | (LMS_Ctrl_Packet_Rx->Data_field[7] << 16) | (LMS_Ctrl_Packet_Rx->Data_field[8] << 8) | (LMS_Ctrl_Packet_Rx->Data_field[9]);

								if (flash_page >= FLASH_USRSEC_START_ADDR) {
									if (flash_page % FLASH_BLOCK_SIZE == 0 && data_cnt > 0) {
										flash_op_status = MicoSPIFlash_BlockErase(spiflash, spiflash->memory_base+flash_page, 3);
									}

									for (k=0; k<data_cnt; k++) {
										wdata[k] = LMS_Ctrl_Packet_Rx->Data_field[24+k];
									}

									if (data_cnt > 0) {
										if(MicoSPIFlash_PageProgram(spiflash, spiflash->memory_base+flash_page, (unsigned int)data_cnt, wdata)!= 0) cmd_errors++;
									}
									if(cmd_errors) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
									else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;

									break;
								}
								else {
									LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
									break;
								}


							default:
								LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
								break;
						}
					}
					else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
				}

			break;

			case CMD_MEMORY_RD:
				current_portion = (LMS_Ctrl_Packet_Rx->Data_field[1] << 24) | (LMS_Ctrl_Packet_Rx->Data_field[2] << 16) | (LMS_Ctrl_Packet_Rx->Data_field[3] << 8) | (LMS_Ctrl_Packet_Rx->Data_field[4]);
				data_cnt = LMS_Ctrl_Packet_Rx->Data_field[5];

				if((LMS_Ctrl_Packet_Rx->Data_field[10] == 0) && (LMS_Ctrl_Packet_Rx->Data_field[11] == 3)) //TARGET = 3 (EEPROM)
				{
					if(LMS_Ctrl_Packet_Rx->Data_field[0] == 0) //read data from EEPROM #1
					{
						i2c_wdata[0]= LMS_Ctrl_Packet_Rx->Data_field[8];
						i2c_wdata[1]= LMS_Ctrl_Packet_Rx->Data_field[9];
						i2crez = OpenCoresI2CMasterWrite(i2c_master, EEPROM_I2C_ADDR, 2, i2c_wdata);

						i2crez += OpenCoresI2CMasterRead(i2c_master, EEPROM_I2C_ADDR, (unsigned int) data_cnt, i2c_rdata);
						OpenCoresI2CMasterStop(i2c_master);

						for(k=0; k<data_cnt; k++)
						{
							LMS_Ctrl_Packet_Tx->Data_field[24+k] = i2c_rdata[k];

						}

						if(i2crez) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
						else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
					}
					else
						LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
				}
				else
				{
					if((LMS_Ctrl_Packet_Rx->Data_field[10] == 0) && (LMS_Ctrl_Packet_Rx->Data_field[11] == 1)) // TARGET = 1 (FX3)
					{
						flash_page  = LMS_Ctrl_Packet_Rx->Data_field[6] << 24;
						flash_page |= LMS_Ctrl_Packet_Rx->Data_field[7] << 16;
						flash_page |= LMS_Ctrl_Packet_Rx->Data_field[8] << 8;
						flash_page |= LMS_Ctrl_Packet_Rx->Data_field[9];
						//flash_page = flash_page / FLASH_PAGE_SIZE;

						//if( FlashSpiTransfer(FLASH_SPI_BASE, SPI_NR_FLASH, flash_page, FLASH_PAGE_SIZE, flash_page_data, CyTrue) != CY_U3P_SUCCESS)  cmd_errors++;//write to flash
						//TODO:if( FlashSpiRead(FLASH_SPI_BASE, SPI_NR_FLASH, flash_page, FLASH_PAGE_SIZE, flash_page_data) != CY_U3P_SUCCESS)  cmd_errors++;//write to flash

						if(MicoSPIFlash_PageRead(spiflash, spiflash->memory_base+flash_page, (unsigned int)data_cnt, rdata)!= 0) cmd_errors++;

						for(k=0; k<data_cnt; k++)
						{
							LMS_Ctrl_Packet_Tx->Data_field[24+k] = rdata[k];
						}

						if(cmd_errors) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
						else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
					}
					else
						LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
				}

			break;


			case CMD_ANALOG_VAL_RD:

				for(block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++)
				{

					//signed short int converted_val = 300;

					switch (LMS_Ctrl_Packet_Rx->Data_field[0 + (block)])//ch
					{
					case 0://dac val

						LMS_Ctrl_Packet_Tx->Data_field[0 + (block * 4)] = LMS_Ctrl_Packet_Rx->Data_field[block]; //ch
						LMS_Ctrl_Packet_Tx->Data_field[1 + (block * 4)] = 0x00; //RAW //unit, power

						LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)] = (dac_val >> 8) & 0xFF; //unsigned val, MSB byte
						LMS_Ctrl_Packet_Tx->Data_field[3 + (block * 4)] = dac_val & 0xFF; //unsigned val, LSB byte
						break;

					case 1: //temperature

						i2c_wdata[0]=0x00; // Pointer = temperature register
						spirez = OpenCoresI2CMasterWrite(i2c_master, LM75_I2C_ADDR, 1, i2c_wdata);

						// Read temperature and recalculate
						spirez = OpenCoresI2CMasterRead(i2c_master, LM75_I2C_ADDR, 2, i2c_rdata);
						OpenCoresI2CMasterStop(i2c_master);

						converted_val = (signed short int)i2c_rdata[0];
						converted_val = converted_val << 8;
						converted_val = 10 * (converted_val >> 8);
						spirez = i2c_rdata[1];
						if(spirez & 0x80) converted_val = converted_val + 5;

						LMS_Ctrl_Packet_Tx->Data_field[0 + (block * 4)] = LMS_Ctrl_Packet_Rx->Data_field[block]; //ch
						LMS_Ctrl_Packet_Tx->Data_field[1 + (block * 4)] = 0x50; //mC //unit, power

						LMS_Ctrl_Packet_Tx->Data_field[2 + (block * 4)] = (converted_val >> 8); //signed val, MSB byte
						LMS_Ctrl_Packet_Tx->Data_field[3 + (block * 4)] = converted_val; //signed val, LSB byte

						break;

					default:
						cmd_errors++;
						break;
					}
				}

				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;

				break;

			case CMD_ANALOG_VAL_WR:
				if(Check_many_blocks (4)) break;

				for(block = 0; block < LMS_Ctrl_Packet_Rx->Header.Data_blocks; block++)
				{
					switch (LMS_Ctrl_Packet_Rx->Data_field[0 + (block * 4)]) //do something according to channel
					{
					case 0:
						if (LMS_Ctrl_Packet_Rx->Data_field[1 + (block * 4)] == 0) //RAW units?
						{
							dac_val = (LMS_Ctrl_Packet_Rx->Data_field[2 + (block * 4)] << 8 ) + LMS_Ctrl_Packet_Rx->Data_field[3 + (block * 4)];
						    // Write data to the 10bit DAC
							dac_data[0] = (unsigned char) ((dac_val & 0x03F0) >> 4); //POWER-DOWN MODE = NORMAL OPERATION (MSB bits =00) + MSB data
							dac_data[1] = (unsigned char) ((dac_val & 0x000F) << 4); //LSB data

							dac_spi_wrdata = ((unsigned int) dac_data[0]<<8)| ((unsigned int) dac_data[1]) ;
							spirez= MicoSPISetSlaveEnable(dac_spi, 0x01);
							spirez= MicoSPITxData(dac_spi, dac_spi_wrdata, 0);

						}
						else cmd_errors++;

						break;

					default:
						cmd_errors++;
						break;
					}
				}


				if(cmd_errors) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
				else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;

				break;

			case CMD_ALTERA_FPGA_GW_WR: //FPGA passive serial

				current_portion = (LMS_Ctrl_Packet_Rx->Data_field[1] << 24) | (LMS_Ctrl_Packet_Rx->Data_field[2] << 16) | (LMS_Ctrl_Packet_Rx->Data_field[3] << 8) | (LMS_Ctrl_Packet_Rx->Data_field[4]);
				data_cnt = LMS_Ctrl_Packet_Rx->Data_field[5];

				switch(LMS_Ctrl_Packet_Rx->Data_field[0])//prog_mode
				{
				/*
					Programming mode:

					0 - Bitstream to FPGA
					1 - Bitstream to Flash
					2 - Bitstream from Flash
				 */

				case 0://Bitstream to FPGA from PC
					/*
						if ( Configure_FPGA (&LMS_Ctrl_Packet_Rx->Data_field[24], current_portion, data_cnt) ) LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
						else LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
					 */
					LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;

					break;

				case 1: //write data to Flash from PC

					current_portion = (LMS_Ctrl_Packet_Rx->Data_field[1] << 24) | (LMS_Ctrl_Packet_Rx->Data_field[2] << 16) | (LMS_Ctrl_Packet_Rx->Data_field[3] << 8) | (LMS_Ctrl_Packet_Rx->Data_field[4]);
					data_cnt = LMS_Ctrl_Packet_Rx->Data_field[5];

					if (current_portion == 0) state = 10;
					if (data_cnt        == 0)
					{
						state = 30;
					}
					Flash = 1;

					while(Flash)
					{
						switch (state)
						{
						//Init
						case 10:
							//Set Flash memory addresses
							CFM0StartAddress = 0x000000;
							CFM0EndAddress   = 0x13FFFF;

							address = CFM0StartAddress;

							//spiflash_erase_primary(spiflash);
							// Erase First 64KB block, other blocks are erased later
							flash_op_status = MicoSPIFlash_BlockErase(spiflash, spiflash->memory_base + 0x00000000, 3);

							state = 11;
							Flash = 1;

						case 11:
							//Start erase CFM0
							if ((0x03 & MicoSPIFlash_StatusRead (spiflash)) == 0)
							{
								//printf("CFM0 Erased\n");
								//printf("Enter Programming file.\n");
								state = 20;
								Flash = 1;
							}
							if((0x01 & MicoSPIFlash_StatusRead (spiflash)) == 0x01)
							{
								//printf("Erasing CFM0\n");
								state = 11;
								Flash = 1;
							}
							if((0x02 & MicoSPIFlash_StatusRead (spiflash)) == 0x02)
							{
								//printf("Erase CFM0 Failed\n");
								state = 0;
							}

							break;

							//Program
						case 20:
							for(byte = 24; byte <= 52; byte += 4)
							{
								//Take word
								wdata[0] = LMS_Ctrl_Packet_Rx->Data_field[byte+0];
								wdata[1] = LMS_Ctrl_Packet_Rx->Data_field[byte+1];
								wdata[2] = LMS_Ctrl_Packet_Rx->Data_field[byte+2];
								wdata[3] = LMS_Ctrl_Packet_Rx->Data_field[byte+3];

								//Command to write into On-Chip Flash IP
								if(address <= CFM0EndAddress)
								{
									// Erase Block if we reach starting address of 64KB block
									if (address % FLASH_BLOCK_SIZE == 0) {
										flash_op_status = MicoSPIFlash_BlockErase(spiflash, spiflash->memory_base+address, 3);
									}

									//IOWR_32DIRECT(ONCHIP_FLASH_0_DATA_BASE, address, word);
									flash_op_status = MicoSPIFlash_AlignedPageProgram(spiflash, spiflash->memory_base+address, 0x4, wdata);

									address += 4;

									
									while((0x01 & MicoSPIFlash_StatusRead (spiflash)) == 0x01)
									{
										//printf("Writing CFM0(%d)\n", address);
									}
									//TODO: Do we need this?
									if((0x02 & MicoSPIFlash_StatusRead (spiflash)) == 0x02)
									{
										//printf("Write to %d failed\n", address);
										state = 0;
										address = 700000;
									}
                           /*
									if((IORD(ONCHIP_FLASH_0_CSR_BASE, 0) & 0x0b) == 0x08)
									{
									};
                           */
									
								}
								else
								{
									LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
								};
							};

							state = 20;
							Flash = 0;
							LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;

							break;

							//Finish
						case 30:
							//Re-protect the sector
							//IOWR(ONCHIP_FLASH_0_CSR_BASE, 1, 0xffffffff);

							state = 0;
							Flash = 0;

							LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;

							break;

						default:
							LMS_Ctrl_Packet_Tx->Header.Status = STATUS_ERROR_CMD;
							state = 0;
							Flash = 0;
						};
					};

					break;

				case 2: //configure FPGA from flash

					//enable boot to factory image, booting is executed after response to command is sent
					boot_img_en = 1;

					LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;



					break;

				default:
					LMS_Ctrl_Packet_Tx->Header.Status = STATUS_COMPLETED_CMD;
					break;

				}

				break;


			default:
				/* This is unknown request. */
				LMS_Ctrl_Packet_Tx->Header.Status = STATUS_UNKNOWN_CMD;
				break;
			}

			//Send response to the command
			for(cnt=0; cnt<64/sizeof(uint32_t); ++cnt)
			{
				dest_byte_reordered = ((dest[cnt] & 0x000000FF) <<24) | ((dest[cnt] & 0x0000FF00) <<8) | ((dest[cnt] & 0x00FF0000) >>8) | ((dest[cnt] & 0xFF000000) >>24);
				IOWR(FIFO_BASE_ADDRESS, 0, dest_byte_reordered);
			};

			gpo_val = 0x0;
			*gpo_reg = gpo_val;
		}

		unsigned int gpo_reg_04_val = *gpo_reg_04;
		unsigned int gpo_reg_08_val = *gpo_reg_08;

		if (iShiftLeft == 1){
			if (iValue == 0x8) {
				iShiftLeft = 0;
				iValue = 0x4;
			} else {
				iValue = iValue << 1;
			}
		} else {
			iValue = iValue >> 1;
			if (iValue == 0) {
				iValue = 0x02;
				iShiftLeft = 1;
			}
		}


	}
	
    /* all done */
	return(0);
}

