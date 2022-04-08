`include "platform1.v"


module platform1_top(
	input clk_i,
   input reset_n,
   output [7:0] LEDPIO_OUT,
	input GPIOPIO_BOTH_IN,
	output GPIOPIO_BOTH_OUT,
	output [7:0] GPOout_pins,
	input spiMISO_MASTER,
	output spiMOSI_MASTER,
	output [1:0] spiSS_N_MASTER,
	output spiSCLK_MASTER,
	output [7:0] FIFOout_pins,
	output [31:0] FIFOof_d,
	output FIFOof_wr,
	input FIFOof_wrfull,
	input [31:0] FIFOif_d,
	output FIFOif_rd,
	input FIFOif_rdempty,
	output FIFOfifo_rst,
	input uartSIN, 
	output uartSOUT,
   output [3:0] lms_ctr_gpioPIO_OUT,
	output SPIFlash_CEJ,
	output SPIFlash_SCK,
	output SPIFlash_SI,
   input SPIFlash_SO,
	output SPIFlash_WPJ,
	output SPIFlash_USRMCLKTS_PIO_OUT,
	inout i2cm_oc_SDA,
	inout i2cm_oc_SCL,
	input dac_spi_MISO_MASTER,
	output dac_spi_MOSI_MASTER,
	output [0:0] dac_spi_SS_N_MASTER,
	output dac_spi_SCLK_MASTER
);
// LatticeECP3 internal oscillator generates platform clock
//wire clk_i;
//OSCF OSCinst0 (.OSC(clk_i));
//defparam OSCinst0.NOM_FREQ = "26" ;
   
platform1 platform1_u(
   .clk_i (clk_i),
   .reset_n (reset_n),
   .LEDPIO_OUT (LEDPIO_OUT),
	.GPIOPIO_BOTH_IN (GPIOPIO_BOTH_IN),
	.GPIOPIO_BOTH_OUT (GPIOPIO_BOTH_OUT),
	.GPOout_pins (GPOout_pins),
	.spiMISO_MASTER(spiMISO_MASTER),
	.spiMOSI_MASTER(spiMOSI_MASTER),
	.spiSS_N_MASTER(spiSS_N_MASTER),
	.spiSCLK_MASTER(spiSCLK_MASTER),
	.FIFOout_pins(FIFOout_pins),
	.FIFOof_d(FIFOof_d),
	.FIFOof_wr(FIFOof_wr),
	.FIFOof_wrfull(FIFOof_wrfull),
	.FIFOif_d(FIFOif_d),
	.FIFOif_rd(FIFOif_rd),
	.FIFOif_rdempty(FIFOif_rdempty),
   .FIFOfifo_rst(FIFOfifo_rst),
	.uartSIN(uartSIN),
	.uartSOUT(uartSOUT),
   .lms_ctr_gpioPIO_OUT(lms_ctr_gpioPIO_OUT),
	.SPIFlash_CEJ(SPIFlash_CEJ),
	.SPIFlash_SCK(SPIFlash_SCK),
	.SPIFlash_SI(SPIFlash_SI),
   .SPIFlash_SO(SPIFlash_SO),
	.SPIFlash_WPJ(SPIFlash_WPJ),
	.SPIFlash_USRMCLKTS_PIO_OUT(SPIFlash_USRMCLKTS_PIO_OUT),
	.i2cm_oc_SDA(i2cm_oc_SDA),
	.i2cm_oc_SCL(i2cm_oc_SCL),
	.dac_spi_MISO_MASTER(dac_spi_MISO_MASTER),
	.dac_spi_MOSI_MASTER(dac_spi_MOSI_MASTER),
	.dac_spi_SS_N_MASTER(dac_spi_SS_N_MASTER),
	.dac_spi_SCLK_MASTER(dac_spi_SCLK_MASTER)
	
	
);
endmodule