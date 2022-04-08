-- ----------------------------------------------------------------------------	
-- FILE: 	lms7002_ddout.vhd
-- DESCRIPTION:	takes data in SDR and ouputs double data rate
-- DATE:	Mar 14, 2016
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
entity lms7002_txiq is
   generic(
      iq_width       : integer :=12
   );
   port (
      --
      clk            : in  std_logic;
      reset_n        : in  std_logic;
      txiq           : out std_logic_vector(iq_width-1 downto 0);
      txiqsel        : out std_logic;
      --Delay control
      data_loadn     : in  std_logic;
      data_move      : in  std_logic;
      data_direction : in  std_logic;
      data_cflag     : out std_logic;
		-- from internal logic
      tx_diq1_h      : in  std_logic_vector(iq_width downto 0);
      tx_diq1_l      : in  std_logic_vector(iq_width downto 0)
   );
end lms7002_txiq;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7002_txiq is
--declare signals,  components here

signal reset         : std_logic;
signal oddr_q        : std_logic_vector(iq_width downto 0);
signal delay_z       : std_logic_vector(iq_width downto 0);
signal delay_cflag   : std_logic_vector(iq_width downto 0);


begin

reset <= NOT reset_n;

-- ----------------------------------------------------------------------------
-- ODDR components
-- ----------------------------------------------------------------------------
   gen_oddr: for i in 0 to iq_width generate 
   inst_ODDRX1F: ODDRX1F
      port map (
         SCLK  =>clk, 
         RST   =>reset, 
         D0    =>tx_diq1_h(i), 
         D1    =>tx_diq1_l(i), 
         Q     =>oddr_q(i)
      );      
   end generate gen_oddr;

-- ----------------------------------------------------------------------------
-- Delay components
-- ----------------------------------------------------------------------------
   gen_delay : for i in 0 to iq_width generate 
      inst_DELAYF : DELAYF
         generic map (DEL_VALUE=>  1, DEL_MODE=> "USER_DEFINED")
         port map (
            A        => oddr_q(i), 
            LOADN    => data_loadn, 
            MOVE     => data_move, 
            DIRECTION=> data_direction, 
            Z        => delay_z(i), 
            CFLAG    => delay_cflag(i)
         );
   end generate gen_delay;

-- ----------------------------------------------------------------------------
-- Connect outputs
-- ----------------------------------------------------------------------------
   txiq        <= delay_z(iq_width-1 downto 0);
   txiqsel     <= delay_z(iq_width);
   
   data_cflag  <= OR delay_cflag;  -- OR whole vector
   
   


	
end arch;   




