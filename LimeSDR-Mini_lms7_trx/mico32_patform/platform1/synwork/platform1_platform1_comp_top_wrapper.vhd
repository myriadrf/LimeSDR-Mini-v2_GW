--
-- Synopsys
-- Vhdl wrapper for top level design, written on Tue Feb 15 20:16:19 2022
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library work;
use work.fpgacfg_pkg.all;

entity wrapper_for_top is
   port (
      LMK_CLK : in std_logic;
      FT_CLK : in std_logic;
      FT_BE : in std_logic_vector(3 downto 0);
      FT_D : in std_logic_vector(31 downto 0);
      FT_RXFn : in std_logic;
      FT_TXEn : in std_logic;
      FT_WRn : out std_logic;
      FPGA_SPI_SCLK : out std_logic;
      FPGA_SPI_MOSI : out std_logic;
      FPGA_SPI_MISO : in std_logic;
      FPGA_SPI_LMS_SS : out std_logic;
      FPGA_LED1_R : out std_logic;
      FPGA_LED1_G : out std_logic;
      HW_VER : in std_logic_vector(3 downto 0);
      BOM_VER : in std_logic_vector(2 downto 0)
   );
end wrapper_for_top;

architecture top_arch of wrapper_for_top is

component top
 port (
   LMK_CLK : in std_logic;
   FT_CLK : in std_logic;
   FT_BE : inout std_logic_vector (3 downto 0);
   FT_D : inout std_logic_vector (31 downto 0);
   FT_RXFn : in std_logic;
   FT_TXEn : in std_logic;
   FT_WRn : out std_logic;
   FPGA_SPI_SCLK : out std_logic;
   FPGA_SPI_MOSI : out std_logic;
   FPGA_SPI_MISO : in std_logic;
   FPGA_SPI_LMS_SS : out std_logic;
   FPGA_LED1_R : out std_logic;
   FPGA_LED1_G : out std_logic;
   HW_VER : in std_logic_vector (3 downto 0);
   BOM_VER : in std_logic_vector (2 downto 0)
 );
end component;

signal tmp_LMK_CLK : std_logic;
signal tmp_FT_CLK : std_logic;
signal tmp_FT_BE : std_logic_vector (3 downto 0);
signal tmp_FT_D : std_logic_vector (31 downto 0);
signal tmp_FT_RXFn : std_logic;
signal tmp_FT_TXEn : std_logic;
signal tmp_FT_WRn : std_logic;
signal tmp_FPGA_SPI_SCLK : std_logic;
signal tmp_FPGA_SPI_MOSI : std_logic;
signal tmp_FPGA_SPI_MISO : std_logic;
signal tmp_FPGA_SPI_LMS_SS : std_logic;
signal tmp_FPGA_LED1_R : std_logic;
signal tmp_FPGA_LED1_G : std_logic;
signal tmp_HW_VER : std_logic_vector (3 downto 0);
signal tmp_BOM_VER : std_logic_vector (2 downto 0);

begin

tmp_LMK_CLK <= LMK_CLK;

tmp_FT_CLK <= FT_CLK;

tmp_FT_BE <= FT_BE;

tmp_FT_D <= FT_D;

tmp_FT_RXFn <= FT_RXFn;

tmp_FT_TXEn <= FT_TXEn;

FT_WRn <= tmp_FT_WRn;

FPGA_SPI_SCLK <= tmp_FPGA_SPI_SCLK;

FPGA_SPI_MOSI <= tmp_FPGA_SPI_MOSI;

tmp_FPGA_SPI_MISO <= FPGA_SPI_MISO;

FPGA_SPI_LMS_SS <= tmp_FPGA_SPI_LMS_SS;

FPGA_LED1_R <= tmp_FPGA_LED1_R;

FPGA_LED1_G <= tmp_FPGA_LED1_G;

tmp_HW_VER <= HW_VER;

tmp_BOM_VER <= BOM_VER;



u1:   top port map (
		LMK_CLK => tmp_LMK_CLK,
		FT_CLK => tmp_FT_CLK,
		FT_BE => tmp_FT_BE,
		FT_D => tmp_FT_D,
		FT_RXFn => tmp_FT_RXFn,
		FT_TXEn => tmp_FT_TXEn,
		FT_WRn => tmp_FT_WRn,
		FPGA_SPI_SCLK => tmp_FPGA_SPI_SCLK,
		FPGA_SPI_MOSI => tmp_FPGA_SPI_MOSI,
		FPGA_SPI_MISO => tmp_FPGA_SPI_MISO,
		FPGA_SPI_LMS_SS => tmp_FPGA_SPI_LMS_SS,
		FPGA_LED1_R => tmp_FPGA_LED1_R,
		FPGA_LED1_G => tmp_FPGA_LED1_G,
		HW_VER => tmp_HW_VER,
		BOM_VER => tmp_BOM_VER
       );
end top_arch;
