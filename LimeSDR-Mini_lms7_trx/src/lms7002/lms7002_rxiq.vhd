-- ----------------------------------------------------------------------------	
-- FILE: 	lms7002_ddin.vhd
-- DESCRIPTION:	takes data from lms7002 in double data rate
-- DATE:	April 01, 2022
-- AUTHOR(s):	Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------
-- ----------------------------------------------------------------------------
-- Notes: Lattice version
-- ----------------------------------------------------------------------------
	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ECP5U;
use ECP5U.components.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lms7002_rxiq is
	generic(
      iq_width       : integer :=12
	);
	port (
      --input ports 
      clk       	   : in  std_logic;
      reset_n   	   : in  std_logic;
		rxiq		 	   : in  std_logic_vector(iq_width-1 downto 0);
		rxiqsel	 	   : in  std_logic;
      --Delay control
      data_loadn     : in  std_logic;
      data_move      : in  std_logic;
      data_direction : in  std_logic;
      data_cflag     : out std_logic;
		--output ports to internal logic
		rx_diq2_h	   : out std_logic_vector(iq_width downto 0);
		rx_diq2_l	   : out std_logic_vector(iq_width downto 0)	
   );
end lms7002_rxiq;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7002_rxiq is
--declare signals,  components here
signal aclr 		   : std_logic;
signal datain		   : std_logic_vector(iq_width downto 0);
signal buf_datain	   : std_logic_vector(iq_width downto 0);
signal delay_datain	: std_logic_vector(iq_width downto 0);
signal datain_h 	   : std_logic_vector(iq_width downto 0);
signal datain_l 	   : std_logic_vector(iq_width downto 0);
signal datain_reg_h 	: std_logic_vector(iq_width downto 0);
signal datain_reg_l 	: std_logic_vector(iq_width downto 0);
signal datain_reg_l_delayed 	: std_logic_vector(iq_width downto 0);
signal gen_delay_cflag  : std_logic_vector(iq_width downto 0);

begin

datain<=rxiqsel & rxiq;

aclr<=not reset_n;

-- ----------------------------------------------------------------------------
-- Input buffers
-- ----------------------------------------------------------------------------
gen_ibuf : for i in 0 to iq_width generate
   inst_IB : IB
      port map (I=>datain(i), O=>buf_datain(i));
end generate gen_ibuf;

-- ----------------------------------------------------------------------------
-- Delay components
-- ----------------------------------------------------------------------------
gen_delay : for i in 0 to iq_width generate 
   inst_DELAYF : DELAYF
      generic map (DEL_VALUE=>  1, DEL_MODE=> "USER_DEFINED")
      port map (
         A        => buf_datain(i), 
         LOADN    => data_loadn, 
         MOVE     => data_move, 
         DIRECTION=> data_direction, 
         Z        => delay_datain(i), 
         CFLAG    => gen_delay_cflag(i)
      );
end generate gen_delay;

-- ----------------------------------------------------------------------------
-- IDDR components
-- ----------------------------------------------------------------------------
gen_iddr: for i in 0 to iq_width generate 
   inst_IDDRX1F: IDDRX1F
      port map (
         D     => delay_datain(i), 
         SCLK  => clk, 
         RST   => aclr, 
         Q0    => datain_h(i), 
         Q1    => datain_l(i)
      );
end generate gen_iddr;            

-- ----------------------------------------------------------------------------
-- Internal registers
-- ----------------------------------------------------------------------------	 
	-- Resync both on rising edge
   process(aclr, clk)
    begin
      if aclr='1' then
			datain_reg_h 	<= (others=>'0');
			datain_reg_l 	<= (others=>'0');
			datain_reg_l_delayed <= (others=>'0');
      elsif rising_edge(clk) then
			datain_reg_h 	<= datain_h;
			datain_reg_l   <= datain_l;
			-- We need to delay data captured on falling edge, in order to allign samples
			datain_reg_l_delayed <= datain_reg_l;
      end if;
    end process;
	 
-- ----------------------------------------------------------------------------
-- Output
-- ---------------------------------------------------------------------------- 
rx_diq2_h <= datain_reg_h;
rx_diq2_l <= datain_reg_l_delayed;

data_cflag <= OR gen_delay_cflag; -- OR all vector bits
 
end arch;   




