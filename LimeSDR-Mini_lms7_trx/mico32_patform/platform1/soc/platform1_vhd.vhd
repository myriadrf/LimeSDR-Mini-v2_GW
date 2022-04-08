library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
entity platform1_vhd is
port(
clk_i   : in std_logic
; reset_n : in std_logic
; SPIFlash_CEJ : out std_logic
; SPIFlash_SCK : out std_logic
; SPIFlash_SI : out std_logic
; SPIFlash_SO : in std_logic
; SPIFlash_WPJ : out std_logic
; LEDPIO_OUT : out std_logic_vector(7 downto 0)
; GPIOPIO_BOTH_IN : in std_logic_vector(0 downto 0)
; GPIOPIO_BOTH_OUT : out std_logic_vector(0 downto 0)
; GPOout_pins : out std_logic_vector(7 downto 0)
; spiMISO_MASTER : in std_logic
; spiMOSI_MASTER : out std_logic
; spiSS_N_MASTER : out std_logic_vector(1 downto 0)
; spiSCLK_MASTER : out std_logic
; FIFOout_pins : out std_logic_vector(7 downto 0)
; FIFOof_d : out std_logic_vector(31 downto 0)
; FIFOof_wr : out std_logic
; FIFOof_wrfull : in std_logic
; FIFOif_d : in std_logic_vector(31 downto 0)
; FIFOif_rd : out std_logic
; FIFOif_rdempty : in std_logic
; FIFOfifo_rst : out std_logic
; uartSIN : in std_logic
; uartSOUT : out std_logic
; lms_ctr_gpioPIO_OUT : out std_logic_vector(3 downto 0)
; SPIFlash_USRMCLKTS_PIO_OUT : out std_logic_vector(0 downto 0)
; i2cm_oc_SDA : inout std_logic
; i2cm_oc_SCL : inout std_logic
; dac_spi_MISO_MASTER : in std_logic
; dac_spi_MOSI_MASTER : out std_logic
; dac_spi_SS_N_MASTER : out std_logic_vector(0 downto 0)
; dac_spi_SCLK_MASTER : out std_logic
);
end platform1_vhd;

architecture platform1_vhd_a of platform1_vhd is

component platform1
   port(
      clk_i   : in std_logic
      ; reset_n : in std_logic
      ; SPIFlash_CEJ : out std_logic
      ; SPIFlash_SCK : out std_logic
      ; SPIFlash_SI : out std_logic
      ; SPIFlash_SO : in std_logic
      ; SPIFlash_WPJ : out std_logic
      ; LEDPIO_OUT : out std_logic_vector(7 downto 0)
      ; GPIOPIO_BOTH_IN : in std_logic_vector(0 downto 0)
      ; GPIOPIO_BOTH_OUT : out std_logic_vector(0 downto 0)
      ; GPOout_pins : out std_logic_vector(7 downto 0)
      ; spiMISO_MASTER : in std_logic
      ; spiMOSI_MASTER : out std_logic
      ; spiSS_N_MASTER : out std_logic_vector(1 downto 0)
      ; spiSCLK_MASTER : out std_logic
      ; FIFOout_pins : out std_logic_vector(7 downto 0)
      ; FIFOof_d : out std_logic_vector(31 downto 0)
      ; FIFOof_wr : out std_logic
      ; FIFOof_wrfull : in std_logic
      ; FIFOif_d : in std_logic_vector(31 downto 0)
      ; FIFOif_rd : out std_logic
      ; FIFOif_rdempty : in std_logic
      ; FIFOfifo_rst : out std_logic
      ; uartSIN : in std_logic
      ; uartSOUT : out std_logic
      ; lms_ctr_gpioPIO_OUT : out std_logic_vector(3 downto 0)
      ; SPIFlash_USRMCLKTS_PIO_OUT : out std_logic_vector(0 downto 0)
      ; i2cm_oc_SDA : inout std_logic
      ; i2cm_oc_SCL : inout std_logic
      ; dac_spi_MISO_MASTER : in std_logic
      ; dac_spi_MOSI_MASTER : out std_logic
      ; dac_spi_SS_N_MASTER : out std_logic_vector(0 downto 0)
      ; dac_spi_SCLK_MASTER : out std_logic
      );
   end component;

   attribute black_box_pad_pin : string;
   attribute black_box_pad_pin of platform1 : component is "i2cm_oc_SDA,i2cm_oc_SCL";
begin

lm32_inst : platform1
port map (
   clk_i  => clk_i
   ,reset_n  => reset_n
   ,SPIFlash_CEJ  => SPIFlash_CEJ
   ,SPIFlash_SCK  => SPIFlash_SCK
   ,SPIFlash_SI  => SPIFlash_SI
   ,SPIFlash_SO  => SPIFlash_SO
   ,SPIFlash_WPJ  => SPIFlash_WPJ
   ,LEDPIO_OUT  => LEDPIO_OUT
   ,GPIOPIO_BOTH_IN  => GPIOPIO_BOTH_IN
   ,GPIOPIO_BOTH_OUT  => GPIOPIO_BOTH_OUT
   ,GPOout_pins  => GPOout_pins
   ,spiMISO_MASTER  => spiMISO_MASTER
   ,spiMOSI_MASTER  => spiMOSI_MASTER
   ,spiSS_N_MASTER  => spiSS_N_MASTER
   ,spiSCLK_MASTER  => spiSCLK_MASTER
   ,FIFOout_pins  => FIFOout_pins
   ,FIFOof_d  => FIFOof_d
   ,FIFOof_wr  => FIFOof_wr
   ,FIFOof_wrfull  => FIFOof_wrfull
   ,FIFOif_d  => FIFOif_d
   ,FIFOif_rd  => FIFOif_rd
   ,FIFOif_rdempty  => FIFOif_rdempty
   ,FIFOfifo_rst  => FIFOfifo_rst
   ,uartSIN  => uartSIN
   ,uartSOUT  => uartSOUT
   ,lms_ctr_gpioPIO_OUT  => lms_ctr_gpioPIO_OUT
   ,SPIFlash_USRMCLKTS_PIO_OUT  => SPIFlash_USRMCLKTS_PIO_OUT
   ,i2cm_oc_SDA  => i2cm_oc_SDA
   ,i2cm_oc_SCL  => i2cm_oc_SCL
   ,dac_spi_MISO_MASTER  => dac_spi_MISO_MASTER
   ,dac_spi_MOSI_MASTER  => dac_spi_MOSI_MASTER
   ,dac_spi_SS_N_MASTER  => dac_spi_SS_N_MASTER
   ,dac_spi_SCLK_MASTER  => dac_spi_SCLK_MASTER
   );

end platform1_vhd_a;

