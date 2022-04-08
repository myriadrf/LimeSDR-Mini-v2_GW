-- ----------------------------------------------------------------------------
-- FILE:          clk_top.vhd
-- DESCRIPTION:   Top module of clock controll module
-- DATE:          Jan 27, 2016
-- AUTHOR(s):     Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------

-- ----------------------------------------------------------------------------
--NOTES:
-- ----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ECP5U;
use ECP5U.components.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity clk_top is
   port (
      -- General 
      clk      : in  std_logic;
      reset_n  : in  std_logic;
      -- Forwarded clock outputs
      clk_fwd0 : out std_logic;
      clk_fwd1 : out std_logic;
      -- Configuration
      sel         : in  std_logic;
      cflag       : out std_logic;
      direction   : in  std_logic; 
      loadn       : in  std_logic;
      move        : in  std_logic
      
   );
end clk_top;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of clk_top is
--declare signals,  components here
signal reset : std_logic;

signal lo : std_logic;
signal hi : std_logic;

signal inst1_q : std_logic;
signal inst2_q : std_logic;

signal inst3_z          : std_logic;
signal inst3_LOADN      : std_logic;
signal inst3_MOVE       : std_logic;
signal inst3_DIRECTION  : std_logic;
signal inst3_CFLAG      : std_logic;

signal inst4_z          : std_logic;
signal inst4_LOADN      : std_logic;
signal inst4_MOVE       : std_logic;
signal inst4_DIRECTION  : std_logic;
signal inst4_CFLAG      : std_logic;


attribute keep: boolean;
attribute keep of inst3_LOADN      : signal is true;
attribute keep of inst3_MOVE       : signal is true;
attribute keep of inst3_DIRECTION  : signal is true;
attribute keep of inst3_CFLAG      : signal is true;

attribute keep of inst4_LOADN      : signal is true;
attribute keep of inst4_MOVE       : signal is true;
attribute keep of inst4_DIRECTION  : signal is true;
attribute keep of inst4_CFLAG      : signal is true;

 
begin

reset <= NOT reset_n;

-- ----------------------------------------------------------------------------
-- Constants
-- ----------------------------------------------------------------------------
   vlo_inst: VLO
   port map (Z=>lo);

   vhi_inst: VHI
   port map (Z=>hi);

-- ----------------------------------------------------------------------------
-- Control logic
-- ----------------------------------------------------------------------------
   inst3_LOADN       <= loadn       when sel = '0' else '1';
   inst3_MOVE        <= move        when sel = '0' else '0';
   inst3_DIRECTION   <= direction;

   inst4_LOADN       <= loadn       when sel = '1' else '1';
   inst4_MOVE        <= move        when sel = '1' else '0';
   inst4_DIRECTION   <= direction;
   
   cflag <= inst3_CFLAG when sel = '0' else inst4_CFLAG;

-- ----------------------------------------------------------------------------
-- Forwarded clock clk_fwd0
-- ----------------------------------------------------------------------------
   inst1_ddrox1: ODDRX1F
      port map (
         SCLK  => clk, 
         RST   => reset, 
         D0    => lo, 
         D1    => hi, 
         Q     => inst1_q
      );
      
-- ----------------------------------------------------------------------------
-- Forwarded clock clk_fwd0
-- ----------------------------------------------------------------------------
   inst2_ddrox1: ODDRX1F
      port map (
         SCLK  => clk, 
         RST   => reset, 
         D0    => lo, 
         D1    => hi, 
         Q     => inst2_q
      );
      
   inst3_DELAYF: DELAYF
      generic map (
         DEL_VALUE=>  50, 
         DEL_MODE=> "USER_DEFINED"
      )
      port map (
         A        => inst1_q, 
         LOADN    => inst3_LOADN, 
         MOVE     => inst3_MOVE, 
         DIRECTION=> inst3_DIRECTION, 
         Z        => inst3_z, 
         CFLAG    => inst3_CFLAG
      );
      
   inst4_DELAYF: DELAYF
      generic map (
         DEL_VALUE=>  1, 
         DEL_MODE=> "USER_DEFINED"
      )
      port map (
         A        => inst2_q, 
         LOADN    => inst4_LOADN, 
         MOVE     => inst4_MOVE, 
         DIRECTION=> inst4_DIRECTION, 
         Z        => inst4_z, 
         CFLAG    => inst4_CFLAG
      );
      
-- ----------------------------------------------------------------------------
-- Output ports
-- ----------------------------------------------------------------------------
clk_fwd0 <= inst3_z;
clk_fwd1 <= inst4_z;
  
end arch;   


