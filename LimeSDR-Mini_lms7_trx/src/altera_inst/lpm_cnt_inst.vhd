-- ----------------------------------------------------------------------------	
-- FILE: 	lpm_cnt_inst.vhd
-- DESCRIPTION:	describe file
-- DATE:	Jan 27, 2017
-- AUTHOR(s):	Lime Microsystems
-- REVISIONS:
-- April 19, 2019 - changed counter instance to a pure vhdl one
-- ----------------------------------------------------------------------------	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

LIBRARY lpm;
USE lpm.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lpm_cnt_inst is
   generic(
      cnt_width   : integer := 64
   );
   port (

      clk      : in std_logic;
      reset_n  : in std_logic;
		cin		: in std_logic ;
		cnt_en	: in std_logic ;
		data		: in std_logic_vector (cnt_width-1 DOWNTO 0);
      sclr     : in std_logic;
		sload		: in std_logic ;
		cout		: out std_logic ;
		q		   : out std_logic_vector (cnt_width-1 DOWNTO 0)


        );
end lpm_cnt_inst;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lpm_cnt_inst is
--declare signals,  components here

signal aclr       : std_logic;
signal counter    : std_logic_vector(cnt_width-1 downto 0);  

begin

aclr <= NOT reset_n;

Counter_Operation : Process(all)
begin
   if aclr='1' then
      counter <= (others => '0');
   elsif(rising_edge(clk)) then
      if sclr='1' then
         counter <= (others => '0');
      elsif sload='1' then
         counter <= data;
      elsif cin='1' and cnt_en='1' then
         counter <= std_logic_vector(unsigned(counter) + 1);
      end if; 
   end if;
end process;

q    <= counter;
--cout <= (and counter); --VHDL2008 reduction operator

cout <= AND_REDUCE(counter); --non VHDL2008 reduction function



  
end arch;   





