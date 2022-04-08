-- ----------------------------------------------------------------------------	
-- FILE: 	lms7002_ddin.vhd
-- DESCRIPTION:	takes data from lms7002 in double data rate
-- DATE:	Mar 14, 2016
-- AUTHOR(s):	Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ECP5U;
use ECP5U.components.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lms7002_ddin is
	generic( dev_family				: string := "Cyclone IV E";
				iq_width					: integer :=12;
				invert_input_clocks	: string := "ON"
	);
	port (
      --input ports 
      clk       	: in std_logic;
      reset_n   	: in std_logic;
		rxiq		 	: in std_logic_vector(iq_width-1 downto 0);
		rxiqsel	 	: in std_logic;
		--output ports 
		data_out_h	: out std_logic_vector(iq_width downto 0);
		data_out_l	: out std_logic_vector(iq_width downto 0)
		
        );
end lms7002_ddin;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7002_ddin is
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

signal data_loadn       : std_logic;
signal data_move        : std_logic;
signal data_direction   : std_logic;
signal data_cflag       : std_logic_vector(iq_width downto 0);

attribute keep: boolean;
attribute keep of data_loadn     : signal is true;
attribute keep of data_move      : signal is true;
attribute keep of data_direction : signal is true;
attribute keep of data_cflag     : signal is true;

begin

data_loadn     <= '1';
data_move      <= '0';
data_direction <= '0';

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
         CFLAG    => data_cflag(i)
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
data_out_h <= datain_reg_h;
data_out_l <= datain_reg_l_delayed;
 
end arch;   




