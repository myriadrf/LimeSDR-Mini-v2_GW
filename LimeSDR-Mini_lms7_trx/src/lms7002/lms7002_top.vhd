-- ----------------------------------------------------------------------------
-- FILE:          lms7002_top.vhd
-- DESCRIPTION:   Top module for LMS7002M IC
-- DATE:          April 01, 2022
-- AUTHOR(s):     Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------

-- ----------------------------------------------------------------------------
-- NOTES:
-- ----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lms7002_top is
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
end lms7002_top;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lms7002_top is
--declare signals,  components here

signal inst1_delayf_loadn     : std_logic;
signal inst1_delayf_move      : std_logic;
signal inst1_delayf_direction : std_logic;

signal tx_data_loadn          : std_logic;
signal tx_data_move           : std_logic;

signal rx_data_loadn          : std_logic;
signal rx_data_move           : std_logic;

signal inst0_loadn            : std_logic;
signal inst0_move             : std_logic;

signal smpl_cmp_done_sync     : std_logic;
signal smpl_cmp_error_sync    : std_logic;

  
begin

   sync_reg0 : entity work.sync_reg 
   port map(clk, reset_n, smpl_cmp_done, smpl_cmp_done_sync);
   
   sync_reg1 : entity work.sync_reg 
   port map(clk, reset_n, smpl_cmp_error, smpl_cmp_error_sync);
-- ----------------------------------------------------------------------------
-- Clocks
-- ----------------------------------------------------------------------------
inst0 :  entity work.lms7002_clk
   port map(
      -- Clock inputs and resets 
      mclk1          => MCLK1, 
      mclk1_reset_n  => reset_n, 
      mclk2          => MCLK2, 
      mclk2_reset_n  => reset_n, 
      -- Forwarded clock outputs
      fclk1          => FLCK1,
      fclk2          => FLCK2,
      -- Clock for internal logic
      mclk1_int      => tx_clk,
      mclk2_int      => rx_clk,
      -- Configuration
      sel            => '0',  -- 0 - fclk1 control, 1 - fclk2 control
      cflag          => open,
      direction      => '0', 
      loadn          => inst0_loadn,
      move           => inst0_move
   );
   
   inst0_loadn <= inst1_delayf_loadn    when delay_sel = "00" else '1';
   inst0_move  <= inst1_delayf_move     when delay_sel = "00" else '0';
   

   
-- ----------------------------------------------------------------------------
-- TX path (DIQ1)
-- ----------------------------------------------------------------------------   
   tx_path : entity work.lms7002_txiq
   generic map (
      iq_width => 12
   )
   port map(
      --
      clk            => tx_clk,
      reset_n        => reset_n,
      txiq           => DIQ1_D,
      txiqsel        => ENABLE_IQSEL1,
      --Delay control
      data_loadn     => tx_data_loadn,
      data_move      => tx_data_move,
      data_direction => '0',
      data_cflag     => open,
		-- from internal logic
      tx_diq1_h      => tx_diq1_h,
      tx_diq1_l      => tx_diq1_l
   );
   
-- ----------------------------------------------------------------------------
-- RX path (DIQ2)
-- ----------------------------------------------------------------------------   
   rx_path : entity work.lms7002_rxiq
   generic map(
      iq_width => 12
   )
   port map(
      --input ports 
      clk            => rx_clk,
      reset_n        => reset_n,
      rxiq           => DIQ2_D,
      rxiqsel        => ENABLE_IQSEL2,
      --Delay control
      data_loadn     => rx_data_loadn,
      data_move      => rx_data_move,
      data_direction => '0',
      data_cflag     => open,
      --output ports to internal logic
      rx_diq2_h      => rx_diq2_h,
      rx_diq2_l      => rx_diq2_l
   );
   
-- ----------------------------------------------------------------------------
-- Delay control module
-- ----------------------------------------------------------------------------
   inst1_delay_ctrl : entity work.delay_ctrl_top
   port map(
      clk               => clk, 
      reset_n           => reset_n,
      --
      delay_en          => delay_en   ,
      delay_sel         => delay_sel  ,
      delay_dir         => delay_dir  ,
      delay_mode        => delay_mode ,
      delay_done        => delay_done ,
      delay_error       => delay_error,
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       => smpl_cmp_en,   
      smpl_cmp_done     => smpl_cmp_done_sync, 
      smpl_cmp_error    => smpl_cmp_error_sync,
      smpl_cmp_cnt      => smpl_cmp_cnt, 
      
      delayf_loadn      => inst1_delayf_loadn,
      delayf_move       => inst1_delayf_move,
      delayf_direction  => inst1_delayf_direction
      
   );
   
   tx_data_loadn <= inst1_delayf_loadn    when delay_sel = "01" else '1';
   tx_data_move  <= inst1_delayf_move     when delay_sel = "01" else '0';
   
   rx_data_loadn <= inst1_delayf_loadn    when delay_sel = "11" else '1';
   rx_data_move  <= inst1_delayf_move     when delay_sel = "11" else '0';
   
   
  
end arch;   