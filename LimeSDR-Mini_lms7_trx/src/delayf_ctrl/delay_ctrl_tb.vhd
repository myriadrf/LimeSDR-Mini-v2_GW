-- ----------------------------------------------------------------------------
-- FILE:          delay_ctrl_tb.vhd
-- DESCRIPTION:   
-- DATE:          Feb 13, 2014
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
entity delayf_ctrl_tb is
end delayf_ctrl_tb;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------

architecture tb_behave of delayf_ctrl_tb is
   constant clk0_period    : time := 10 ns;
   constant clk1_period    : time := 10 ns; 
   --signals
   signal clk0,clk1        : std_logic;
   signal reset_n          : std_logic; 
   
   signal dut0_en          : std_logic;
   signal dut0_phase       : std_logic_vector(7 downto 0);
   signal dut0_sel         : std_logic_vector(2 downto 0);
   signal dut0_updown      : std_logic;
   
   signal dut0_smpl_cmp_en    : std_logic;
   signal dut0_smpl_cmp_done  : std_logic;
   signal dut0_smpl_cmp_error : std_logic;
   
begin 
  
      clock0: process is
   begin
      clk0 <= '0'; wait for clk0_period/2;
      clk0 <= '1'; wait for clk0_period/2;
   end process clock0;

      clock: process is
   begin
      clk1 <= '0'; wait for clk1_period/2;
      clk1 <= '1'; wait for clk1_period/2;
   end process clock;
   
      res: process is
   begin
      reset_n <= '0'; wait for 20 ns;
      reset_n <= '1'; wait;
   end process res;
   
   process is
   begin
      dut0_en <= '0'; wait until reset_n = '1' and rising_edge(clk0);
      dut0_en <= '1'; wait;
   end process;
   
   
   -- only min found
   --process is 
   --begin  
   --   dut0_smpl_cmp_done      <= '0'; 
   --   dut0_smpl_cmp_error     <= '0';
   --   wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
   --   wait until rising_edge(clk0);
   --   dut0_smpl_cmp_done      <= '1'; 
   --   dut0_smpl_cmp_error     <= '0';
   --   wait until rising_edge(clk0);
   --   wait until rising_edge(clk0);
   --end process;
   
   
   -- ERROR
   --process is 
   --begin  
   --   dut0_smpl_cmp_done      <= '0'; 
   --   dut0_smpl_cmp_error     <= '0';
   --   wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
   --   wait until rising_edge(clk0);
   --   dut0_smpl_cmp_done      <= '1'; 
   --   dut0_smpl_cmp_error     <= '1';
   --   wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
   --   wait until rising_edge(clk0);
   --   dut0_smpl_cmp_done      <= '0'; 
   --   dut0_smpl_cmp_error     <= '0';
   --end process;
   
   process is 
   begin  
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      --ERROR
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '1';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      --ERROR
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '1';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      -- OK min
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '0';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      -- OK
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '0';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      -- ERRO MAX
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '1';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';
      -- OK midle
      wait until rising_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '1'; 
      dut0_smpl_cmp_error     <= '0';
      wait until falling_edge(dut0_smpl_cmp_en) and reset_n = '1';
      wait until rising_edge(clk0);
      dut0_smpl_cmp_done      <= '0'; 
      dut0_smpl_cmp_error     <= '0';

   end process;
   
   
  
   dut0_phase     <= x"00";
   dut0_sel       <= "000";
   dut0_updown    <= '1';
   
   
   --phsft_dut0 : entity work.delayf_ctrl
   --port map(
   --   clk               => clk0,
   --   reset_n           => reset_n,
   --   busy              => open,
   --   en                => dut0_en,
   --   phase             => dut0_phase,
   --   updown            => dut0_updown,      
   --   --delayf ports
   --   delayf_loadn      => open,
   --   delayf_move       => open,
   --   delayf_direction  => open
   --);
   
   dut0 : entity work.delay_ctrl_top
   port map (

      clk               => clk0,
      reset_n           => reset_n,
      --
      delay_en          => dut0_en,
      delay_sel         => "00",
      delay_dir         => '0',
      delay_mode        => '1',
      delay_done        => open,
      delay_error       => open,
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       => dut0_smpl_cmp_en,
      smpl_cmp_done     => dut0_smpl_cmp_done,
      smpl_cmp_error    => dut0_smpl_cmp_error,
      smpl_cmp_cnt      => open,
      
      delayf_loadn      => open,
      delayf_move       => open,
      delayf_direction  => open
      
   );
   

end tb_behave;

