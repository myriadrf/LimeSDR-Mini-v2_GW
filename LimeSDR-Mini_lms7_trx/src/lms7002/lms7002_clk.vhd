-- ----------------------------------------------------------------------------
-- FILE:          lms7002_clk.vhd
-- DESCRIPTION:   Top module of clock controll module
-- DATE:          April 1, 2022
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
entity lms7002_clk is
   port (
      -- Clock inputs and resets 
      mclk1          : in  std_logic;
      mclk1_reset_n  : in  std_logic;
      mclk2          : in  std_logic;
      mclk2_reset_n  : in  std_logic;
      -- Forwarded clock outputs
      fclk1          : out std_logic;
      fclk2          : out std_logic;
      -- Clock for internal logic
      mclk1_int      : out std_logic;
      mclk2_int      : out std_logic;
      -- Configuration
      sel            : in  std_logic;  -- 0 - fclk1 control, 1 - fclk2 control
      cflag          : out std_logic;
      direction      : in  std_logic; 
      loadn          : in  std_logic;
      move           : in  std_logic
      
   );
end lms7002_clk;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7002_clk is
--declare signals,  components here

signal mclk1_reset : std_logic;
signal mclk2_reset : std_logic;

signal lo : std_logic;
signal hi : std_logic;

signal buf_mclk1 : std_logic;
signal buf_mclk2 : std_logic;

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
mclk1_reset <= NOT mclk1_reset_n; 
mclk2_reset <= NOT mclk2_reset_n; 
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
-- Forwarded clock fclk1
-- ----------------------------------------------------------------------------
   inst1_ddrox1: ODDRX1F
      port map (
         SCLK  => mclk1, 
         RST   => mclk1_reset, 
         D0    => lo, 
         D1    => hi, 
         Q     => inst1_q
      );
      
-- ----------------------------------------------------------------------------
-- Forwarded clock fclk2
-- ----------------------------------------------------------------------------
   inst2_ddrox1: ODDRX1F
      port map (
         SCLK  => mclk2, 
         RST   => mclk2_reset, 
         D0    => lo, 
         D1    => hi, 
         Q     => inst2_q
      );
      
   inst3_DELAYF: DELAYF
      generic map (
         DEL_VALUE=> 1, 
         DEL_MODE => "USER_DEFINED"
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
         DEL_VALUE=> 1, 
         DEL_MODE => "USER_DEFINED"
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
fclk1 <= inst3_z;
fclk2 <= inst4_z;

mclk1_int <= mclk1;
mclk2_int <= mclk2;

  
end arch;   

