-- ----------------------------------------------------------------------------
-- FILE:          delay_ctrl_fsm.vhd
-- DESCRIPTION:   FSM for delay control
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
entity delay_ctrl_fsm is
   port (
      clk               : in std_logic;
      reset_n           : in std_logic;
      
      delay_en          : in  std_logic;
      delay_sel         : in  std_logic_vector(1 downto 0); --0 FCLK1, 1 - TX_DIQ(not supported), 2 - FLCK2(not supported), 3 - RX_DIQ
      delay_dir         : in  std_logic;
      delay_mode        : in  std_logic;  -- 0 - manual, 1 - auto
      delay_done        : out std_logic;
      delay_error       : out std_logic;
      --signals from sample compare module (required for automatic phase searching)
      smpl_cmp_en       : out std_logic;
      smpl_cmp_done     : in  std_logic;
      smpl_cmp_error    : in  std_logic;
      smpl_cmp_cnt      : out std_logic_vector(15 downto 0);
      --
      delayf_ctrl_en    : out std_logic; -- rising edge triggers dynamic phase shift
      delayf_ctrl_busy  : in  std_logic;  -- 1 - busy, 0 - not busy
      delayf_ctrl_done  : in  std_logic;  -- 1 - busy, 0 - not busy      
      delayf_ctrl_ndelay: out std_logic_vector(7 downto 0);	-- phase value in steps (max=128, min=0 resets to default delay)
      delayf_ctrl_dir   : out std_logic  -- 1- Decrease, 0- Increase 
   );
end delay_ctrl_fsm;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of delay_ctrl_fsm is
--declare signals,  components here

--type state_type is (idle, reset_delay, move_delay, check_state, check_samples, min_found, max_found, move_to_middle, error, done);
--signal current_state, next_state : state_type;

signal delay_en_reg     : std_logic;
signal min_found_reg    : std_logic;
signal max_found_reg    : std_logic;
signal ndelay           : unsigned(delayf_ctrl_ndelay'length-1 downto 0);
signal move_cnt         : unsigned(delayf_ctrl_ndelay'length-1 downto 0);

signal min_delay        : unsigned(delayf_ctrl_ndelay'length-1 downto 0);
signal max_delay        : unsigned(delayf_ctrl_ndelay'length-1 downto 0);
signal delay_range      : unsigned(delayf_ctrl_ndelay'length-1 downto 0);
signal half_delay_range : unsigned(delayf_ctrl_ndelay'length-1 downto 0);
signal middle_delay     : unsigned(delayf_ctrl_ndelay'length-1 downto 0);

signal move_wait_cnt    : unsigned(3 downto 0);

signal smpl_cmp_done_reg :std_logic;

signal delayf_ctrl_en_reg : std_logic;
signal delayf_ctrl_done_reg : std_logic;

constant C_MAX_DELAY_STEPS : integer := 128;

--attribute syn_state_machine                  : boolean;
--attribute syn_state_machine of current_state : signal is true;
--attribute syn_encoding                    : string;
--attribute syn_encoding of current_state   : signal is "binary";
--attribute syn_encoding of next_state      : signal is "binary";

--attribute syn_enum_encoding : string;
--attribute syn_enum_encoding of state_type : type is "sequential"; 



--attribute enum_encoding: string;
--attribute enum_encoding of state_type: type is
--"0000 0001 0010 0011 0100 0101 0110 0111 1000 1001";
           




signal current_state, next_state, debug_curent_state : std_logic_vector(11 downto 0);

constant idle                 : std_logic_vector(11 downto 0) := 12x"800";  
constant reset_delay          : std_logic_vector(11 downto 0) := 12x"400";
constant move_delay           : std_logic_vector(11 downto 0) := 12x"200";
constant move_wait            : std_logic_vector(11 downto 0) := 12x"100";
constant check_state          : std_logic_vector(11 downto 0) := 12x"080";
constant check_smpl_cmp_done  : std_logic_vector(11 downto 0) := 12x"040";
constant check_samples        : std_logic_vector(11 downto 0) := 12x"020";
constant min_found            : std_logic_vector(11 downto 0) := 12x"010";
constant max_found            : std_logic_vector(11 downto 0) := 12x"008";
constant move_to_middle       : std_logic_vector(11 downto 0) := 12x"004";
constant error                : std_logic_vector(11 downto 0) := 12x"002";
constant done                 : std_logic_vector(11 downto 0) := 12x"001";

attribute syn_encoding                    : string;
attribute syn_encoding of current_state   : signal is "safe,onehot";
attribute syn_encoding of next_state      : signal is "safe,onehot";

--attribute keep: boolean;
--attribute keep of debug_curent_state      : signal is true;


begin

-- ----------------------------------------------------------------------------
-- Input registers
-- ----------------------------------------------------------------------------
process(clk, reset_n)
begin
   if reset_n = '0' then 
      delay_en_reg         <= '0';
      smpl_cmp_done_reg    <= '0';
      delayf_ctrl_done_reg <= '0';
   elsif rising_edge(clk) then 
      delay_en_reg      <= delay_en;
      smpl_cmp_done_reg <= smpl_cmp_done;
      delayf_ctrl_done_reg <= delayf_ctrl_done;
   end if;
end process;

-- ----------------------------------------------------------------------------
-- State machine
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
         if delay_en_reg = '0' and delay_en = '1' then
            if delay_mode = '1' then 
               next_state <= reset_delay;
            else 
               next_state <= idle;
            end if;
         else 
            next_state <= idle;
         end if;
         
      when reset_delay =>
         if delayf_ctrl_done_reg ='0' AND delayf_ctrl_done = '1' then
            if min_found_reg = '1' AND max_found_reg ='1' then 
               next_state <= move_to_middle;
            else   
               next_state <= check_smpl_cmp_done;
            end if;
         else
            next_state <= reset_delay;
         end if;
         
      when check_state =>
         if min_found_reg = '1' AND max_found_reg = '1' then 
            next_state <= reset_delay;
         else
            next_state <= check_smpl_cmp_done;
         end if;
         
      when check_smpl_cmp_done => 
         if smpl_cmp_done_reg = '1' OR smpl_cmp_done ='1' then
            next_state <= check_smpl_cmp_done;
         else 
            next_state <= check_samples;
         end if;
           
      when check_samples =>
         if smpl_cmp_done_reg = '0' AND smpl_cmp_done ='1' then
         
            if smpl_cmp_error = '0' AND min_found_reg = '0' AND max_found_reg = '0' then
               next_state <= min_found;
            elsif smpl_cmp_error = '0' AND min_found_reg = '1' AND max_found_reg = '1' then
               next_state <= done;
            elsif smpl_cmp_error = '1' AND min_found_reg = '1' AND max_found_reg = '0'then
               next_state <= max_found;
            elsif smpl_cmp_error = '1' AND min_found_reg = '1' AND max_found_reg = '1'then
               next_state <= error;
            else
               if unsigned(move_cnt) < C_MAX_DELAY_STEPS then
                  next_state <= move_delay;
               else
                  if min_found_reg = '1' then 
                     next_state <= max_found;
                  else 
                     next_state <= error;
                  end if;
               end if;
            end if;
            
         else 
            next_state <= check_samples;
         end if;
         
      when move_delay =>
         if delayf_ctrl_done_reg ='0' AND delayf_ctrl_done = '1' then 
            next_state <= move_wait;
         else 
            next_state <= move_delay;
         end if;
         
      when move_wait => 
         if unsigned(move_wait_cnt) > 0 then 
            next_state <= move_wait;
         else 
            next_state <= check_state;
         end if;
         
         
      when move_to_middle =>
         if delayf_ctrl_done_reg ='0' AND delayf_ctrl_done = '1' then 
            next_state <= check_smpl_cmp_done;
         else 
            next_state <= move_to_middle;
         end if;
         
      when min_found => 
         next_state <= check_state;
         
      when max_found => 
         next_state <= check_state;
         
      when done => 
         next_state <= idle;
      
      when error => 
         next_state <= idle;
           
      when others =>
         next_state <= idle;
      
   end case;
end process fsm;


delay_range <= max_delay - min_delay;
half_delay_range <= '0' & delay_range(delay_range'length-1 downto 1);
-- ----------------------------------------------------------------------------
-- FSM dependant registers
-- ----------------------------------------------------------------------------
process(clk, reset_n)
begin
   if reset_n = '0' then 
      ndelay               <= (others=>'0');
      delayf_ctrl_en_reg   <= '0';
      smpl_cmp_en          <= '0';
      min_found_reg        <= '0';
      max_found_reg        <= '0';
      move_cnt             <= (others=>'0');
      min_delay            <= (others=>'0');
      max_delay            <= (others=>'0');
      delay_done           <= '0';
      delay_error          <= '0';
      move_wait_cnt        <= (others=>'0');
   elsif rising_edge(clk) then 
      if current_state = reset_delay then 
         ndelay<= (others=>'0');
      elsif current_state = move_to_middle then 
         ndelay <= min_delay + half_delay_range;
      else 
         ndelay <= to_unsigned(1, ndelay'length);
      end if;
      
      if (current_state=move_delay OR current_state = reset_delay OR current_state = move_to_middle ) AND delayf_ctrl_en_reg ='0' then 
         delayf_ctrl_en_reg <= '1';
      elsif (delayf_ctrl_done_reg = '0' AND delayf_ctrl_done = '1') OR current_state = idle then 
         delayf_ctrl_en_reg <= '0';
      else 
         delayf_ctrl_en_reg <= delayf_ctrl_en_reg;
      end if;
      
      if current_state = check_samples then 
         smpl_cmp_en <= '1';
      else 
         smpl_cmp_en <= '0';
      end if;
      
      if current_state = min_found then 
         min_found_reg <= '1';
      elsif current_state = idle then
         min_found_reg <= '0';
      else   
         min_found_reg <= min_found_reg;
      end if;
      
      if current_state = max_found then 
         max_found_reg <= '1';
      elsif current_state = idle then
         max_found_reg <= '0';
      else   
         max_found_reg <= max_found_reg;
      end if;
      
      
      if current_state = idle then 
         move_cnt <= (others=>'0');
      elsif current_state = move_delay AND delayf_ctrl_en_reg = '0' then 
         move_cnt <= move_cnt + 1;
      else 
         move_cnt <= move_cnt;
      end if;
      
      if current_state = min_found then 
         min_delay <= move_cnt;
      elsif current_state = idle then 
         min_delay <= (others=>'0');
      else 
         min_delay <= min_delay;
      end if;
      
      if current_state = max_found then 
         max_delay <= move_cnt;
      elsif current_state = idle then 
         max_delay <= to_unsigned(C_MAX_DELAY_STEPS, max_delay'length);
      else 
         max_delay <= max_delay;
      end if;
      
      
      if current_state = done OR current_state = error then 
         delay_done <= '1';
      elsif delay_en_reg = '0' and delay_en = '1' then 
         delay_done <= '0';
      else 
         delay_done <= delay_done;
      end if;
      
      if current_state = done then 
         delay_error <= '0';
      elsif current_state = error then 
         delay_error <= '1';
      elsif delay_en_reg = '0' and delay_en = '1' then 
         delay_error <= '0';
      else 
         delay_error <= delay_error;
      end if;
      
      if current_state = move_wait then 
         move_wait_cnt <= move_wait_cnt - 1;
      else 
         move_wait_cnt <= (others=>'1');
      end if;
      
   end if;
end process;

-- ----------------------------------------------------------------------------
-- output ports
-- ----------------------------------------------------------------------------
delayf_ctrl_ndelay <= std_logic_vector(ndelay);

delayf_ctrl_en    <= delayf_ctrl_en_reg;
delayf_ctrl_dir   <= '0';


  
end arch;   