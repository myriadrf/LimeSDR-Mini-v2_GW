-- ----------------------------------------------------------------------------
-- FILE:          lms7_trx_top.vhd
-- DESCRIPTION:   Top level file for LimeSDR-Mini board
-- DATE:          10:06 AM Friday, May 11, 2018
-- AUTHOR(s):     Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------

-- ----------------------------------------------------------------------------
--NOTES:
-- ----------------------------------------------------------------------------
-- altera vhdl_input_version vhdl_2008
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.fpgacfg_pkg.all;
use work.pllcfg_pkg.all;
use work.tstcfg_pkg.all;
use work.periphcfg_pkg.all;
use work.FIFO_PACK.all;

library ECP5U;
use ECP5U.components.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lms7_trx_top is
   generic(
      -- General parameters
      BOARD                   : string := "LimeSDR-Mini";
      DEV_FAMILY              : string := "MAX 10";
      -- LMS7002 related 
      LMS_DIQ_WIDTH           : integer := 12;
      -- FTDI (USB3) related
      FTDI_DQ_WIDTH           : integer := 32;     -- FTDI Data bus size      
      CTRL0_FPGA_RX_SIZE      : integer := 1024;   -- Control PC->FPGA, FIFO size in bytes.
      CTRL0_FPGA_RX_RWIDTH    : integer := 32;     -- Control PC->FPGA, FIFO rd width.
      CTRL0_FPGA_TX_SIZE      : integer := 1024;   -- Control FPGA->PC, FIFO size in bytes
      CTRL0_FPGA_TX_WWIDTH    : integer := 32;     -- Control FPGA->PC, FIFO wr width
      STRM0_FPGA_RX_SIZE      : integer := 4096;   -- Stream PC->FPGA, FIFO size in bytes
      STRM0_FPGA_RX_RWIDTH    : integer := 128;    -- Stream PC->FPGA, rd width
      STRM0_FPGA_TX_SIZE      : integer := 16384;  -- Stream FPGA->PC, FIFO size in bytes
      STRM0_FPGA_TX_WWIDTH    : integer := 64;     -- Stream FPGA->PC, wr width
      -- 
      TX_N_BUFF               : integer := 4;      -- N 4KB buffers in TX interface (2 OR 4)
      TX_PCT_SIZE             : integer := 4096;   -- TX packet size in bytes
      TX_IN_PCT_HDR_SIZE      : integer := 16;
      -- Internal configuration memory 
      FPGACFG_START_ADDR      : integer := 0;
      PLLCFG_START_ADDR       : integer := 32;
      TSTCFG_START_ADDR       : integer := 96;
      PERIPHCFG_START_ADDR    : integer := 192
   );
   port (
      -- ----------------------------------------------------------------------------
      -- External GND pin for reset
      --EXT_GND           : in     std_logic;
      -- ----------------------------------------------------------------------------
      -- Clock sources
         -- Reference clock, coming from LMK clock buffer.
      LMK_CLK           : in     std_logic;
      -- ----------------------------------------------------------------------------
      -- LMS7002 Digital
         -- PORT1
      LMS_MCLK1         : in     std_logic;
      LMS_FCLK1         : out    std_logic;
      LMS_TXNRX1        : out    std_logic;
      LMS_ENABLE_IQSEL1 : out    std_logic;
      LMS_DIQ1_D        : out    std_logic_vector(LMS_DIQ_WIDTH-1 downto 0);
         -- PORT2
      LMS_MCLK2         : in     std_logic;
      LMS_FCLK2         : out    std_logic;
      LMS_TXNRX2_or_CLK_SEL : out    std_logic; --In v2.3 board version this pin is changed to CLK_SEL
      LMS_ENABLE_IQSEL2 : in     std_logic;
      LMS_DIQ2_D        : in     std_logic_vector(LMS_DIQ_WIDTH-1 downto 0);
         --MISC
      LMS_RESET         : out    std_logic := '1';
      LMS_TXEN          : out    std_logic;
      LMS_RXEN          : out    std_logic;
      LMS_CORE_LDO_EN   : out    std_logic;
      -- ----------------------------------------------------------------------------
      -- FTDI (USB3)
         -- Clock source
      FT_CLK            : in     std_logic;
         -- DATA
      FT_BE             : inout  std_logic_vector(FTDI_DQ_WIDTH/8-1 downto 0);
      FT_D              : inout  std_logic_vector(FTDI_DQ_WIDTH-1 downto 0);
         -- Control, flags
      FT_RXFn           : in     std_logic;
      FT_TXEn           : in     std_logic;
      FT_WRn            : out    std_logic; 
      FT_RESETn         : out    std_logic;
	  FT_WAKEUPn        : out    std_logic;
      -- ----------------------------------------------------------------------------
      -- External communication interfaces
         -- FPGA_SPI
      FPGA_SPI_SCLK     : out    std_logic;
      FPGA_SPI_MOSI     : out    std_logic;
      FPGA_SPI_MISO     : in     std_logic;      
      FPGA_SPI_LMS_SS   : out    std_logic;
      FPGA_SPI_DAC_SS   : out    std_logic;
         -- FPGA_CFG
      FPGA_CFG_SPI_MISO : in     std_logic;
      FPGA_CFG_SPI_MOSI : out    std_logic;
      --FPGA_CFG_SPI_SCLK : out    std_logic; -- SCLK pin can be accessed only trough USRMCLK component
      FPGA_CFG_SPI_SS_N : out    std_logic;
         -- FPGA I2C
      FPGA_I2C_SCL      : inout  std_logic;
      FPGA_I2C_SDA      : inout  std_logic;
      -- ----------------------------------------------------------------------------
      -- General periphery
         -- LEDs          
      FPGA_LED1_R        : out    std_logic;
      FPGA_LED1_G        : out    std_logic;
         -- GPIO 
      FPGA_GPIO         : inout  std_logic_vector(7 downto 0);
      FPGA_EGPIO        : inout  std_logic_vector(1 downto 0);
         -- Temperature sensor
      LM75_OS           : in     std_logic;
         -- Fan control 
      FAN_CTRL          : out    std_logic;
         -- RF loop back control 
      RFSW_RX_V1        : out    std_logic;
      RFSW_RX_V2        : out    std_logic;
      RFSW_TX_V1        : out    std_logic;
      RFSW_TX_V2        : out    std_logic;
      TX_LB_AT          : out    std_logic;
      TX_LB_SH          : out    std_logic;
         -- Bill Of material and hardware version 
      BOM_VER           : in     std_logic_vector(2 downto 0);
      HW_VER            : in     std_logic_vector(3 downto 0)  --PULL UP has to be enabled for HW_VER

   );
end lms7_trx_top;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7_trx_top is
--declare signals,  components here
signal ext_rst                   : std_logic;
signal por_rst_vect              : std_logic_vector(3 downto 0);
signal por_rst_n                 : std_logic;

signal reset_n                   : std_logic; 

--inst0 (NIOS CPU instance)
signal inst0_exfifo_if_rd        : std_logic;
signal inst0_exfifo_of_d         : std_logic_vector(FTDI_DQ_WIDTH-1 downto 0);
signal inst0_exfifo_of_wr        : std_logic;
signal inst0_exfifo_of_rst       : std_logic;
signal inst0_gpo                 : std_logic_vector(7 downto 0);
signal inst0_gpo_reg             : std_logic_vector(7 downto 0);
signal inst0_lms_ctr_gpio        : std_logic_vector(3 downto 0);
signal inst0_spi_0_MISO          : std_logic;
signal inst0_spi_0_MOSI          : std_logic;
signal inst0_spi_0_SCLK          : std_logic;
signal inst0_spi_0_SS_n          : std_logic_vector(4 downto 0);
signal inst0_spi_1_MISO          : std_logic;
signal inst0_spi_1_MOSI          : std_logic;
signal inst0_spi_1_SCLK          : std_logic;
signal inst0_spi_1_SS_n          : std_logic_vector(1 downto 0);
signal inst0_from_fpgacfg        : t_FROM_FPGACFG;
signal inst0_to_fpgacfg          : t_TO_FPGACFG;
signal inst0_from_pllcfg         : t_FROM_PLLCFG;
signal inst0_to_pllcfg           : t_TO_PLLCFG;
signal inst0_from_tstcfg         : t_FROM_TSTCFG;
signal inst0_to_tstcfg           : t_TO_TSTCFG;
signal inst0_from_periphcfg      : t_FROM_PERIPHCFG;
signal inst0_to_periphcfg        : t_TO_PERIPHCFG;
signal cpu_alive_cnt             : std_logic_vector(15 downto 0);
signal inst0_fpga_cfg_usrmclkts  : std_logic_vector(0 downto 0);
signal inst0_fpga_cfg_spi_MOSI   : std_logic;
signal inst0_fpga_cfg_spi_SCLK   : std_logic;
signal inst0_fpga_cfg_spi_SS_n   : std_logic;

signal u1_USRMCLKI               : std_logic;




--inst1 (pll_top instance)
signal inst1_pll_c0              : std_logic;
signal inst1_pll_c1              : std_logic;
signal inst1_pll_c3              : std_logic;
signal inst1_pll_locked          : std_logic;
signal inst1_pll_smpl_cmp_en     : std_logic;
signal inst1_pll_smpl_cmp_cnt    : std_logic_vector(15 downto 0);

--inst2
constant C_EP02_RDUSEDW_WIDTH    : integer := FIFO_WORDS_TO_Nbits(CTRL0_FPGA_RX_SIZE/(CTRL0_FPGA_RX_RWIDTH/8),true);
constant C_EP82_WRUSEDW_WIDTH    : integer := FIFO_WORDS_TO_Nbits(CTRL0_FPGA_TX_SIZE/(CTRL0_FPGA_TX_WWIDTH/8),true);
constant C_EP03_RDUSEDW_WIDTH    : integer := FIFO_WORDS_TO_Nbits(STRM0_FPGA_RX_SIZE/(STRM0_FPGA_RX_RWIDTH/8),true);
constant C_EP83_WRUSEDW_WIDTH    : integer := FIFO_WORDS_TO_Nbits(STRM0_FPGA_TX_SIZE/(STRM0_FPGA_TX_WWIDTH/8),true);
signal inst2_ext_buff_data       : std_logic_vector(FTDI_DQ_WIDTH-1 downto 0);
signal inst2_ext_buff_wr         : std_logic;
signal inst2_EP82_wfull          : std_logic;
signal inst2_EP82_wrusedw        : std_logic_vector(C_EP82_WRUSEDW_WIDTH-1 downto 0);
signal inst2_EP03_active         : std_logic;
signal inst2_EP03_rdata          : std_logic_vector(STRM0_FPGA_RX_RWIDTH-1 downto 0);
signal inst2_EP03_rempty         : std_logic;
signal inst2_EP03_rdusedw        : std_logic_vector(C_EP03_RDUSEDW_WIDTH-1 downto 0);
signal inst2_EP83_active         : std_logic;
signal inst2_EP83_wfull          : std_logic;
signal inst2_EP83_wrusedw        : std_logic_vector(C_EP83_WRUSEDW_WIDTH-1 downto 0);
signal inst2_GPIF_busy           : std_logic;
signal inst2_faddr               : std_logic_vector(4 downto 0);
signal inst2_EP02_rdata          : std_logic_vector(CTRL0_FPGA_RX_RWIDTH-1 downto 0);
signal inst2_EP02_rempty         : std_logic;
signal inst2_EP02_rdusedw        : std_logic_vector(C_EP02_RDUSEDW_WIDTH-1 downto 0);

--inst4
constant C_INST4_GPIO_N          : integer := FPGA_GPIO'length + FPGA_EGPIO'length;
signal inst4_gpio                : std_logic_vector(C_INST4_GPIO_N-1 downto 0);


--inst5
signal inst5_busy : std_logic;

--inst6
signal inst6_tx_pct_loss_flg        : std_logic;
signal inst6_tx_txant_en            : std_logic;
signal inst6_tx_in_pct_full         : std_logic;
signal inst6_rx_pct_fifo_wrreq      : std_logic;
signal inst6_rx_pct_fifo_wdata      : std_logic_vector(63 downto 0);
signal inst6_rx_smpl_cmp_done       : std_logic;
signal inst6_rx_smpl_cmp_err        : std_logic;
signal inst6_to_tstcfg_from_rxtx    : t_TO_TSTCFG_FROM_RXTX;
signal inst6_rx_pct_fifo_aclrn_req  : std_logic;
signal inst6_tx_in_pct_rdreq        : std_logic;
signal inst6_tx_in_pct_reset_n_req  : std_logic;
signal inst6_wfm_in_pct_reset_n_req : std_logic;
signal inst6_wfm_in_pct_rdreq       : std_logic;
signal inst6_wfm_phy_clk            : std_logic;


--lms inst
signal lms_tx_clk                   : std_logic;
signal lms_rx_clk                   : std_logic;
signal lms_rx_diq2_h                : std_logic_vector(12 downto 0);
signal lms_rx_diq2_l                : std_logic_vector(12 downto 0);

signal lms_tx_diq1_h                : std_logic_vector(12 downto 0);
signal lms_tx_diq1_l                : std_logic_vector(12 downto 0);               

signal lms_delay_en        : std_logic; 
signal lms_delay_sel       : std_logic_vector(1 downto 0);  
signal lms_delay_dir       : std_logic;
signal lms_delay_mode      : std_logic;
signal lms_delay_done      : std_logic;
signal lms_delay_error     : std_logic;
signal lms_smpl_cmp_en     : std_logic;
signal lms_smpl_cmp_done   : std_logic;
signal lms_smpl_cmp_error  : std_logic;

signal mico32_busy         : std_logic;

signal osc_clk             : std_logic;

attribute keep: boolean;
attribute keep of por_rst_vect: signal is true;


-- Component for FPGA configuration flash access
COMPONENT USRMCLK
PORT(
   USRMCLKI    : IN STD_ULOGIC;
   USRMCLKTS   : IN STD_ULOGIC
);
END COMPONENT;

attribute syn_noprune: boolean ;
attribute syn_noprune of USRMCLK: component is true;

component lms7002_top
   port (
      -- Free running clock and reset 
      clk               : in  std_logic;
      reset_n           : in  std_logic;
      -- TX DIQ   
      MCLK1             : in  std_logic;
      FLCK1             : out std_logic;
      ENABLE_IQSEL1     : out std_logic;
      DIQ1_D            : out std_logic_vector(11 downto 0);
      -- RX DIQ   
      MCLK2             : in  std_logic;
      FLCK2             : out std_logic;
      ENABLE_IQSEL2     : in  std_logic;
      DIQ2_D            : in  std_logic_vector(11 downto 0);
      -- Internal logic
      -- tx 
      tx_clk            : out std_logic;
      tx_diq1_h         : in  std_logic_vector(12 downto 0);
      tx_diq1_l         : in  std_logic_vector(12 downto 0);
      -- rx 
      rx_clk            : out std_logic;
      rx_diq2_h         : out std_logic_vector(12 downto 0);
      rx_diq2_l         : out std_logic_vector(12 downto 0);
      -- delay control
      delay_en          : in  std_logic;
      delay_sel         : in  std_logic_vector(1 downto 0); --0 FCLK1, 1 - TX_DIQ(not supported), 2 - FLCK2(not supported), 3 - RX_DIQ
      delay_dir         : in  std_logic;
      delay_mode        : in  std_logic;  -- 0 - manual, 1 - auto
      delay_done        : out std_logic;
      delay_error       : out std_logic;
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       : out std_logic;
      smpl_cmp_done     : in std_logic;
      smpl_cmp_error    : in std_logic;
      smpl_cmp_cnt      : out std_logic_vector(15 downto 0)     
   );
end component;


COMPONENT OSCG
--synthesis translate_off
   GENERIC (
      DIV: integer := 128
   );
--synthesis translate_on
   PORT (
      OSC : OUT std_logic
   );
END COMPONENT;

attribute DIV : integer;
attribute DIV of OSCinst0 : label is 4;


begin

-- ----------------------------------------------------------------------------
-- Internal Oscilator
-- ----------------------------------------------------------------------------
-- DIV values: 2(~155MHz) - 128(~2.4MHz)
OSCInst0: OSCG
--synthesis translate_off
   GENERIC MAP (DIV => 4)
--synthesis translate_on
   PORT MAP (OSC => osc_clk);

-- ----------------------------------------------------------------------------
-- Reset logic
-- ----------------------------------------------------------------------------
   -- HW_VER(3) is connected to GND
   ext_rst  <= HW_VER(3);
   
   process(ext_rst, osc_clk)
   begin 
      if ext_rst = '1' then 
         por_rst_vect <= (others=>'0');
      elsif rising_edge(osc_clk) then 
         por_rst_vect <= por_rst_vect(2 downto 0) & '1';
      end if;
   end process;
      
   reset_n <= '1' when por_rst_vect = "1111" else '0';
  
   -- Reset for all logic. 
   --reset_n <= por_rst_n;  

-- ----------------------------------------------------------------------------
-- CPU (Mico32) instance.
-- CPU is responsible for communication interfaces and control logic
-- ----------------------------------------------------------------------------   
   inst0_cpu : entity work.cpu
   generic map (
      FPGACFG_START_ADDR   => FPGACFG_START_ADDR,
      PLLCFG_START_ADDR    => PLLCFG_START_ADDR,
      TSTCFG_START_ADDR    => TSTCFG_START_ADDR,
      PERIPHCFG_START_ADDR => PERIPHCFG_START_ADDR
   )
   port map(
      clk                        => osc_clk,
      reset_n                    => reset_n,
      -- Control data FIFO
      exfifo_if_d                => inst2_EP02_rdata,
      exfifo_if_rd               => inst0_exfifo_if_rd, 
      exfifo_if_rdempty          => inst2_EP02_rempty,
      exfifo_of_d                => inst0_exfifo_of_d, 
      exfifo_of_wr               => inst0_exfifo_of_wr, 
      exfifo_of_wrfull           => inst2_EP82_wfull,
      exfifo_of_rst              => inst0_exfifo_of_rst, 
      -- SPI 0 
      spi_0_MISO                 => FPGA_SPI_MISO,
      spi_0_MOSI                 => inst0_spi_0_MOSI,
      spi_0_SCLK                 => inst0_spi_0_SCLK,
      spi_0_SS_n                 => inst0_spi_0_SS_n,
      -- SPI 1
      spi_1_MISO                 => '0',--FPGA_QSPI_IO1,
      spi_1_MOSI                 => inst0_spi_1_MOSI,
      spi_1_SCLK                 => inst0_spi_1_SCLK,
      spi_1_SS_n                 => inst0_spi_1_SS_n,
         -- FPGA_CFG_SPI
      fpga_cfg_spi_MISO          => FPGA_CFG_SPI_MISO, 
      fpga_cfg_spi_MOSI          => inst0_fpga_cfg_spi_MOSI,
      fpga_cfg_spi_SCLK          => inst0_fpga_cfg_spi_SCLK,
      fpga_cfg_spi_SS_n          => inst0_fpga_cfg_spi_SS_n,
      fpga_cfg_usrmclkts         => inst0_fpga_cfg_usrmclkts,
      -- I2C
      i2c_scl                    => FPGA_I2C_SCL,
      i2c_sda                    => FPGA_I2C_SDA,
      -- Genral purpose I/O
      gpi                        => (others=>'0'),
      gpo                        => inst0_gpo, 
      -- LMS7002 control 
      lms_ctr_gpio               => inst0_lms_ctr_gpio,
      -- Configuration registers
      from_fpgacfg               => inst0_from_fpgacfg,
      to_fpgacfg                 => inst0_to_fpgacfg,
      from_pllcfg                => inst0_from_pllcfg,
      to_pllcfg                  => inst0_to_pllcfg,
      from_tstcfg                => inst0_from_tstcfg,
      to_tstcfg                  => inst0_to_tstcfg,
      to_tstcfg_from_rxtx        => inst6_to_tstcfg_from_rxtx,
      from_periphcfg             => inst0_from_periphcfg,
      to_periphcfg               => inst0_to_periphcfg
   );
   
   -- Module to access FPGA_CFG_SPI_SCLK pin
   u1: USRMCLK port map (
      USRMCLKI    => u1_USRMCLKI,
      USRMCLKTS   => inst0_fpga_cfg_usrmclkts(0)
   );
   
   u1_USRMCLKI <= inst0_fpga_cfg_spi_SCLK when inst0_fpga_cfg_spi_SS_n ='0' else '0';
   
   inst0_to_fpgacfg.HW_VER    <= HW_VER;
   inst0_to_fpgacfg.BOM_VER   <= '0' & BOM_VER; 
   inst0_to_fpgacfg.PWR_SRC   <= '0';
   
   --CPU alive status
   busy_delay_inst : entity work.busy_delay
   generic map(
      clock_period   => 25,  -- input clock period in ns
      delay_time     => 100  -- delay time in ms
   )
   port map(
      clk      => osc_clk,
      reset_n  => reset_n,
      busy_in  => inst0_gpo(0),
      busy_out => mico32_busy
   );
   
   process(osc_clk, reset_n)
   begin 
      if reset_n = '0' then 
         cpu_alive_cnt <= (others=>'0');
         inst0_gpo_reg <= (others=>'0');
      elsif rising_edge(osc_clk) then 
         inst0_gpo_reg <= inst0_gpo;
         --Increment on rising edge
         if inst0_gpo_reg(0) = '0' AND inst0_gpo(0) = '1' then 
            cpu_alive_cnt <= std_logic_vector(unsigned(cpu_alive_cnt) +1);
         else 
            cpu_alive_cnt <= cpu_alive_cnt;
         end if;
      end if;
   end process;
   
-- ----------------------------------------------------------------------------
-- pll_top instance.
-- Clock source for LMS7002 RX and TX logic
-- ----------------------------------------------------------------------------   
   
   inst1_pll_locked        <= reset_n;
   inst1_pll_smpl_cmp_cnt  <= (others=>'1');
   
   --TODO: remove pll config bypass logic
   inst0_to_pllcfg.pllcfg_busy    <= '0';
   inst0_to_pllcfg.pllcfg_done    <= '1';
   inst0_to_pllcfg.pll_lock       <= (others=>'0');
   
   lms_inst : lms7002_top
   port map (
      -- Free running clock and reset 
      clk            => lms_rx_clk,
      reset_n        => reset_n,
      -- TX DIQ
      MCLK1          => LMS_MCLK1, 
      FLCK1          => LMS_FCLK1, 
      ENABLE_IQSEL1  => LMS_ENABLE_IQSEL1, 
      DIQ1_D         => LMS_DIQ1_D, 
      -- RX DIQ
      MCLK2          => LMS_MCLK2, 
      FLCK2          => LMS_FCLK2, 
      ENABLE_IQSEL2  => LMS_ENABLE_IQSEL2,
      DIQ2_D         => LMS_DIQ2_D, 
      -- Internal logic
      -- tx 
      tx_clk         => lms_tx_clk, 
      tx_diq1_h      => lms_tx_diq1_h,
      tx_diq1_l      => lms_tx_diq1_l,
      -- rx
      rx_clk         => lms_rx_clk,
      rx_diq2_h      => lms_rx_diq2_h,
      rx_diq2_l      => lms_rx_diq2_l,
            -- delay control
      delay_en       => lms_delay_en, 
      delay_sel      => lms_delay_sel, --0 FCLK1, 1 - TX_DIQ(not supported), 2 - FLCK2(not supported), 3 - RX_DIQ
      delay_dir      => lms_delay_dir, 
      delay_mode     => lms_delay_mode,  -- 0 - manual, 1 - auto
      delay_done     => lms_delay_done,
      delay_error    => lms_delay_error,
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en    => lms_smpl_cmp_en,
      smpl_cmp_done  => lms_smpl_cmp_done,
      smpl_cmp_error => lms_smpl_cmp_error,
      smpl_cmp_cnt   => open -- inst1_pll_smpl_cmp_cnt
   );
   
   lms_delay_en         <= inst0_from_pllcfg.phcfg_start;
   lms_delay_sel        <= "00" when inst0_from_pllcfg.cnt_ind= "00011" else 
                           "11";
   lms_delay_dir        <= inst0_from_pllcfg.phcfg_updn;
   lms_delay_mode       <= inst0_from_pllcfg.phcfg_mode;
   lms_smpl_cmp_done    <= inst6_rx_smpl_cmp_done;
   lms_smpl_cmp_error   <= inst6_rx_smpl_cmp_err;
   
   
   inst0_to_pllcfg.phcfg_done    <= lms_delay_done;
   inst0_to_pllcfg.phcfg_error   <= lms_delay_error; 
   

   
   
-- ----------------------------------------------------------------------------
-- FT601_top instance.
-- USB3 interface 
-- ----------------------------------------------------------------------------
   inst2_FT601_top : entity work.FT601_top
   generic map(
      FT_data_width        => FTDI_DQ_WIDTH,
      FT_be_width          => FTDI_DQ_WIDTH/8,
      EP02_rdusedw_width   => C_EP02_RDUSEDW_WIDTH, 
      EP02_rwidth          => CTRL0_FPGA_RX_RWIDTH,
      EP82_wrusedw_width   => C_EP82_WRUSEDW_WIDTH,
      EP82_wwidth          => CTRL0_FPGA_TX_WWIDTH,
      EP82_wsize           => 64,  --packet size in bytes, has to be multiple of 4 bytes
      EP03_rdusedw_width   => C_EP03_RDUSEDW_WIDTH,    
      EP03_rwidth          => STRM0_FPGA_RX_RWIDTH,
      EP83_wrusedw_width   => C_EP83_WRUSEDW_WIDTH,
      EP83_wwidth          => STRM0_FPGA_TX_WWIDTH,
      EP83_wsize           => 2048 --packet size in bytes, has to be multiple of 4 bytes	
   )
   port map(
      --input ports 
      clk            => FT_CLK,   --FTDI CLK
      reset_n        => reset_n,
      --FTDI external ports
      FT_wr_n        => FT_WRn,
      FT_rxf_n       => FT_RXFn,
      FT_data        => FT_D,
      FT_be          => FT_BE,
      FT_txe_n       => FT_TXEn,
      FT_RESETn      => FT_RESETn,
      --controll endpoint fifo PC->FPGA 
      EP02_rdclk     => osc_clk, 
      EP02_rd        => inst0_exfifo_if_rd,
      EP02_rdata     => inst2_EP02_rdata,
      EP02_rempty    => inst2_EP02_rempty,
      --controll endpoint fifo FPGA->PC
      EP82_wclk      => osc_clk,
      EP82_aclrn     => not inst0_exfifo_of_rst,
      EP82_wr        => inst0_exfifo_of_wr,
      EP82_wdata     => inst0_exfifo_of_d,
      EP82_wfull     => inst2_EP82_wfull,
      --stream endpoint fifo PC->FPGA
      EP03_active    => inst2_EP03_active,
      EP03_aclrn     => inst0_from_fpgacfg.rx_en,
      EP03_rdclk     => lms_tx_clk, --inst1_pll_c1,
      EP03_rd        => inst6_tx_in_pct_rdreq,
      EP03_rdata     => inst2_EP03_rdata,
      EP03_rempty    => inst2_EP03_rempty,
      EP03_rusedw    => inst2_EP03_rdusedw,
      --stream endpoint fifo FPGA->PC
      EP83_active    => inst2_EP83_active,
      EP83_wclk      => lms_rx_clk, --inst1_pll_c3, 
      EP83_aclrn     => inst6_rx_pct_fifo_aclrn_req,
      EP83_wr        => inst6_rx_pct_fifo_wrreq,
      EP83_wdata     => inst6_rx_pct_fifo_wdata,
      EP83_wfull     => inst2_EP83_wfull,
      EP83_wrusedw   => inst2_EP83_wrusedw
   );

-- ----------------------------------------------------------------------------
-- tst_top instance.
-- Clock test logic
-- ----------------------------------------------------------------------------
   inst3_tst_top : entity work.tst_top
   port map(
      --input ports 
      FX3_clk           => FT_CLK,
      reset_n           => reset_n,    
      Si5351C_clk_0     => '0',
      Si5351C_clk_1     => '0',
      Si5351C_clk_2     => '0',
      Si5351C_clk_3     => '0',
      Si5351C_clk_5     => '0',
      Si5351C_clk_6     => '0',
      Si5351C_clk_7     => '0',
      LMK_CLK           => LMK_CLK,
      ADF_MUXOUT        => '0',    
   
      -- To configuration memory
      to_tstcfg         => inst0_to_tstcfg,
      from_tstcfg       => inst0_from_tstcfg
   );
   
-- ----------------------------------------------------------------------------
-- general_periph_top instance.
-- Control module for external periphery
-- ----------------------------------------------------------------------------
   inst4_general_periph_top : entity work.general_periph_top
   generic map(
      DEV_FAMILY  => DEV_FAMILY,
      N_GPIO      => C_INST4_GPIO_N
   )
   port map(
      -- General ports
      clk                  => osc_clk,
      reset_n              => reset_n,
      -- configuration memory
      to_periphcfg         => inst0_to_periphcfg,
      from_periphcfg       => inst0_from_periphcfg,     
      -- Dual colour LEDs
      -- LED1 (Clock and PLL lock status)
      led1_mico32_busy     => mico32_busy,
      led1_ctrl            => inst0_from_fpgacfg.FPGA_LED1_CTRL,
      led1_g               => FPGA_LED1_G,
      led1_r               => FPGA_LED1_R,      
      --LED2 (TCXO control status)
      led2_clk             => '0',
      led2_adf_muxout      => '0',
      led2_dac_ss          => '0',
      led2_adf_ss          => '0',
      led2_ctrl            => inst0_from_fpgacfg.FPGA_LED2_CTRL,
      led2_g               => open,
      led2_r               => open,     
      --LED3 (FX3 and NIOS CPU busy)
      led3_g_in            => '0',
      led3_r_in            => '0',
      led3_ctrl            => inst0_from_fpgacfg.FX3_LED_CTRL,
      led3_hw_ver          => HW_VER,
      led3_g               => open,
      led3_r               => open,     
      --GPIO
      gpio_dir             => (others=>'1'),
      gpio_out_val(9 downto 8)   => "00",
      gpio_out_val(7)            => NOT inst2_EP03_active,  -- Shared with FPGA_LED3_R
      gpio_out_val(6)            => '1',                    -- Shared with FPGA_LED3_G
      gpio_out_val(5)            => '1',                    -- Shared with FPGA_LED2_R
      gpio_out_val(4)            => NOT inst2_EP83_active,  -- Shared with FPGA_LED2_G
      gpio_out_val(3 downto 0)   => "0000",
      gpio_rd_val          => open,
      gpio(7 downto 0)     => FPGA_GPIO,
      gpio(9 downto 8)     => FPGA_EGPIO,
      --Fan control
      fan_sens_in          => LM75_OS,
      fan_ctrl_out         => FAN_CTRL
   );
   
 ----------------------------------------------------------------------------
 -- rxtx_top instance.
 -- Receive and transmit interface for LMS7002
 ----------------------------------------------------------------------------
  inst6_rxtx_top : entity work.rxtx_top
  generic map(
     DEV_FAMILY              => DEV_FAMILY,
     -- TX parameters
     TX_IQ_WIDTH             => LMS_DIQ_WIDTH,
     TX_N_BUFF               => TX_N_BUFF,              -- 2,4 valid values
     TX_IN_PCT_SIZE          => TX_PCT_SIZE,
     TX_IN_PCT_HDR_SIZE      => TX_IN_PCT_HDR_SIZE,
     TX_IN_PCT_DATA_W        => STRM0_FPGA_RX_RWIDTH,      -- 
     TX_IN_PCT_RDUSEDW_W     => C_EP03_RDUSEDW_WIDTH,
     
     -- RX parameters
     RX_IQ_WIDTH             => LMS_DIQ_WIDTH,
     RX_INVERT_INPUT_CLOCKS  => "ON",
     RX_PCT_BUFF_WRUSEDW_W   => C_EP83_WRUSEDW_WIDTH --bus width in bits 
     
  )
  port map(                                            
     from_fpgacfg            => inst0_from_fpgacfg,
     to_tstcfg_from_rxtx     => inst6_to_tstcfg_from_rxtx,
     from_tstcfg             => inst0_from_tstcfg,
     
     -- TX module signals
     tx_clk                  => lms_tx_clk, --inst1_pll_c1,
     tx_clkout               => open, --LMS_FCLK1,
     tx_clk_reset_n          => inst1_pll_locked,     
     tx_pct_loss_flg         => inst6_tx_pct_loss_flg,
     tx_txant_en             => inst6_tx_txant_en,  
     --Tx interface data 
     tx_diq1_h               => lms_tx_diq1_h, --LMS_DIQ1_D,
     tx_diq1_l               => lms_tx_diq1_l, --LMS_ENABLE_IQSEL1,
     --fifo ports
     tx_in_pct_rdreq         => inst6_tx_in_pct_rdreq,
     tx_in_pct_data          => inst2_EP03_rdata,
     tx_in_pct_rdempty       => inst2_EP03_rempty,
     tx_in_pct_rdusedw       => inst2_EP03_rdusedw,
     
     -- RX path
     rx_clk                  => lms_rx_clk, --inst1_pll_c3,
     rx_clk_reset_n          => inst1_pll_locked,
     --Rx interface data 
     rx_diq2_h               => lms_rx_diq2_h,
     rx_diq2_l               => lms_rx_diq2_l,
     --Packet fifo ports
     rx_pct_fifo_aclrn_req   => inst6_rx_pct_fifo_aclrn_req,
     rx_pct_fifo_wusedw      => inst2_EP83_wrusedw,
     rx_pct_fifo_wrreq       => inst6_rx_pct_fifo_wrreq,
     rx_pct_fifo_wdata       => inst6_rx_pct_fifo_wdata,
     --sample compare
     rx_smpl_cmp_start       => lms_smpl_cmp_en,
     rx_smpl_cmp_length      => inst0_from_pllcfg.auto_phcfg_smpls, --inst1_pll_smpl_cmp_cnt,
     rx_smpl_cmp_done        => inst6_rx_smpl_cmp_done,
     rx_smpl_cmp_err         => inst6_rx_smpl_cmp_err     
  );
   
-- ----------------------------------------------------------------------------
-- Output ports
-- ----------------------------------------------------------------------------

   FPGA_SPI_MOSI     <= inst0_spi_0_MOSI;
   FPGA_SPI_SCLK     <= inst0_spi_0_SCLK;
   FPGA_SPI_LMS_SS   <= inst0_spi_0_SS_n(0);
   FPGA_SPI_DAC_SS   <= inst0_spi_0_SS_n(2);
    
   FPGA_CFG_SPI_MOSI <= inst0_fpga_cfg_spi_MOSI;
   FPGA_CFG_SPI_SS_N <= inst0_fpga_cfg_spi_SS_n;
   
   LMS_RESET         <= inst0_from_fpgacfg.LMS1_RESET AND inst0_lms_ctr_gpio(0);
   LMS_TXEN          <= inst0_from_fpgacfg.LMS1_TXEN;
   LMS_RXEN          <= inst0_from_fpgacfg.LMS1_RXEN;
   LMS_CORE_LDO_EN   <= inst0_from_fpgacfg.LMS1_CORE_LDO_EN;
   LMS_TXNRX1        <= inst0_from_fpgacfg.LMS1_TXNRX1;
   
   --In HW versions before v2.3 this pin is LMS_TXNRX2. After v2.3 - Clock select for LMK clock buffer (CLK_SEL) 
   LMS_TXNRX2_or_CLK_SEL <=   inst0_from_periphcfg.PERIPH_OUTPUT_VAL_1(0) when unsigned(HW_VER) > 5 else 
                              inst0_from_fpgacfg.LMS1_TXNRX2;
   
   RFSW_RX_V1        <= inst0_from_fpgacfg.GPIO(8);
   RFSW_RX_V2        <= inst0_from_fpgacfg.GPIO(9);
   RFSW_TX_V1        <= inst0_from_fpgacfg.GPIO(12);
   RFSW_TX_V2        <= inst0_from_fpgacfg.GPIO(13);
   TX_LB_AT          <= inst0_from_fpgacfg.GPIO(1);
   TX_LB_SH          <= inst0_from_fpgacfg.GPIO(2);
   
   FT_WAKEUPn        <= '1';
   
   
end arch;   