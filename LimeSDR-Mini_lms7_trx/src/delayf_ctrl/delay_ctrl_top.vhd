-- ----------------------------------------------------------------------------
-- FILE:          delay_ctrl_top.vhd
-- DESCRIPTION:   top control module for delay control for LMS7002 DIQ interface
-- DATE:          April 03, 2022
-- AUTHOR(s):     Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------

-- ----------------------------------------------------------------------------
--NOTES:
-- ----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity delay_ctrl_top is
   port (

      clk      : in std_logic;
      reset_n  : in std_logic;
      --
      delay_en          : in  std_logic;
      delay_sel         : in  std_logic_vector(1 downto 0); --0 FCLK1, 1 - TX_DIQ(not supported), 2 - FLCK2(not supported), 3 - RX_DIQ
      delay_dir         : in  std_logic;
      delay_mode        : in  std_logic;  -- 0 - manual(not supported), 1 - auto
      delay_done        : out std_logic;
      delay_error       : out std_logic;
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       : out std_logic;
      smpl_cmp_done     : in std_logic;
      smpl_cmp_error    : in std_logic;
      smpl_cmp_cnt      : out std_logic_vector(15 downto 0);
      
      delayf_loadn      : out std_logic;
      delayf_move       : out std_logic;
      delayf_direction  : out std_logic
      
   );
end delay_ctrl_top;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of delay_ctrl_top is
--declare signals,  components here

signal inst0_delayf_ctrl_en      : std_logic;
signal inst0_delayf_ctrl_ndelay  : std_logic_vector(7 downto 0);
signal inst0_delayf_ctrl_dir     : std_logic;

signal inst2_busy                : std_logic;
signal inst2_done                : std_logic;

  
begin

-- ----------------------------------------------------------------------------
-- FSM
-- ----------------------------------------------------------------------------
inst0_delay_ctrl_fsm : entity work.delay_ctrl_fsm
   port map (
      clk               => clk,
      reset_n           => reset_n,

      delay_en          => delay_en   ,
      delay_sel         => delay_sel  ,
      delay_dir         => delay_dir  ,
      delay_mode        => delay_mode ,
      delay_done        => delay_done ,
      delay_error       => delay_error,
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       => smpl_cmp_en   ,
      smpl_cmp_done     => smpl_cmp_done ,
      smpl_cmp_error    => smpl_cmp_error,
      smpl_cmp_cnt      => smpl_cmp_cnt,
      
      delayf_ctrl_en    => inst0_delayf_ctrl_en,
      delayf_ctrl_busy  => inst2_busy, 
      delayf_ctrl_done  => inst2_done,
      delayf_ctrl_ndelay=> inst0_delayf_ctrl_ndelay,
      delayf_ctrl_dir   => inst0_delayf_ctrl_dir
   );
   
   
inst2_delayf_ctrl : entity work.delayf_ctrl
   port map(
      clk                  => clk,
      reset_n              => reset_n,
      en                   => inst0_delayf_ctrl_en,
      busy                 => inst2_busy,
      done                 => inst2_done,
      ndelay               => inst0_delayf_ctrl_ndelay,
      updown               => inst0_delayf_ctrl_dir,
      --delayf ports
      delayf_loadn         => delayf_loadn,
      delayf_move          => delayf_move,
      delayf_direction     => delayf_direction
   );

  
end arch;   


