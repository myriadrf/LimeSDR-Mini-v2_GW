-- ----------------------------------------------------------------------------
-- FILE:          delayf_ctrl.vhd
-- DESCRIPTION:   Control module for DELAYF primitive
-- DATE:          Marh 29, 2022
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
entity delayf_ctrl is
   port (
      clk         : in  std_logic;
      reset_n     : in  std_logic;
      en          : in  std_logic;  -- rising edge triggers dynamic phase shift
      busy        : out std_logic;  -- 1 - busy, 0 - not busy
      done        : out std_logic;
      ndelay      : in  std_logic_vector(7 downto 0);	-- phase value in steps (max=128, min=0 resets to default delay)
      updown      : in  std_logic;  -- 0- UP, 1- DOWN      
      --delayf ports
      delayf_loadn       : out std_logic;
      delayf_move        : out std_logic;
      delayf_direction   : out std_logic
   );
end delayf_ctrl;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of delayf_ctrl is
--declare signals,  components here
type state_type is (idle, load_default, trigger_move,  move, move_wait, check_move_cnt, move_done);
signal current_state, next_state : state_type;

signal en_reg           : std_logic_vector(1 downto 0);
signal ndelay_reg       : std_logic_vector(ndelay'length-1 downto 0);
signal move_cnt         : unsigned(ndelay'length-1 downto 0);
signal load_default_cnt : unsigned(3 downto 0);
signal move_delay_cnt   : unsigned(3 downto 0);
signal move_wait_cnt    : unsigned(3 downto 0);

  
begin

-- To capture rising edge of en
process(reset_n, clk)
   begin
      if reset_n='0' then
         en_reg <= (others=>'0');
      elsif rising_edge(clk) then
         en_reg <= en_reg(0) & en;
      end if;
end process;

-- Capture all required inputs only on idle state
process (all)
   begin 
      if reset_n = '0' then 
         ndelay_reg        <= (others=>'0');
         delayf_direction  <= '0';
      elsif rising_edge(clk) then 
         if current_state = idle then
            ndelay_reg        <= ndelay;
            delayf_direction  <= updown;
         else 
            ndelay_reg        <= ndelay_reg;
            delayf_direction  <= delayf_direction;
         end if;
      end if;
end process;


-- Count trigger_move cycles
process(clk, reset_n)
begin
   if reset_n = '0' then 
      move_cnt <= (others=>'0');
   elsif rising_edge(clk) then
      if current_state = trigger_move then 
         move_cnt <= move_cnt + 1;
      elsif current_state = idle then 
         move_cnt <= (others=>'0');
      else 
         move_cnt <= move_cnt;
      end if;
   end if;
end process;
    
  
-- ----------------------------------------------------------------------------
--state machine
-- ----------------------------------------------------------------------------
fsm_f : process(clk, reset_n)begin
   if(reset_n = '0')then
      current_state <= idle;
   elsif(clk'event and clk = '1')then 
      current_state <= next_state;
   end if;	
end process;


-- ----------------------------------------------------------------------------
-- State machine combo
-- ----------------------------------------------------------------------------
fsm : process(all) begin
   next_state <= current_state;
   case current_state is
   
      when idle => 
         if en_reg = "01" then  -- rising edge of en
            if unsigned(ndelay_reg) = 0 then 
               next_state <= load_default;
            else 
               next_state <= trigger_move;
            end if;
         else 
            next_state <= idle;
         end if;
         
      when load_default =>
         if unsigned(load_default_cnt) > 0 then
            next_state <= load_default;
         else 
            next_state<= move_done;
         end if;
         
      when trigger_move => 
         next_state <= move;
         
      when move => 
         if move_delay_cnt > 0 then
            next_state <= move;
         else 
            next_state <= check_move_cnt;
         end if;
         
      when check_move_cnt =>
         if move_cnt < unsigned(ndelay_reg) then 
            next_state <= move_wait;
         else 
            next_state <= move_done;
         end if;
      
      when move_wait => 
         if move_wait_cnt > 0 then 
            next_state <= move_wait;
         else 
            next_state<= trigger_move;
         end if;
         
      when move_done => 
         next_state <= idle;
         
      when others => 
      
   end case;
end process fsm;


-- ----------------------------------------------------------------------------
-- Output registers depending from fsm
-- ----------------------------------------------------------------------------
process(clk, reset_n)
begin
   if reset_n = '0' then 
      delayf_loadn      <= '1';
      delayf_move       <= '0';
      busy              <= '1';
      done              <= '0';
      load_default_cnt  <= (others=>'0');
      move_delay_cnt    <= (others=>'0');
      move_wait_cnt     <= (others=>'0');
   elsif rising_edge(clk) then
   
      if current_state = idle then 
         busy <= '0';
      else 
         busy <= '1';
      end if;
      
      if current_state = load_default then 
         delayf_loadn <= '0';
      else 
         delayf_loadn <= '1';
      end if;
      
      if current_state = load_default then 
         load_default_cnt <= load_default_cnt - 1; 
      else 
         load_default_cnt <= (others=>'1');
      end if;
      
      if current_state = move then 
         delayf_move <= '1';
      else 
         delayf_move <= '0';
      end if;
      
      if current_state = move then 
         move_delay_cnt <= move_delay_cnt - 1;
      else 
         move_delay_cnt <= (others=>'1');
      end if;
      
      if current_state = move_wait then 
         move_wait_cnt <= move_wait_cnt - 1;
      else 
         move_wait_cnt <= (others=>'1');
      end if;
      
      if current_state = move_done then  
         done <= '1';
      else 
         done <= '0';
      end if;
      
   end if;
end process;


end arch;   


