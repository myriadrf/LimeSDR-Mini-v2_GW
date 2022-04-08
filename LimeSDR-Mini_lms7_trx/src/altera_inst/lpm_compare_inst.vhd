-- ----------------------------------------------------------------------------	
-- FILE: 	lpm_cnt_inst.vhd
-- DESCRIPTION:	describe file
-- DATE:	Jan 27, 2017
-- AUTHOR(s):	Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity lpm_compare_inst is
   generic(
      lpm_pipeline         : NATURAL;-- length of pipeline
      lpm_representation   : STRING; -- valid vals are "SIGNED" or "UNSIGNED"
      lpm_type             : STRING; -- ignored
      lpm_width            : NATURAL
   );
   port (
      clock : IN STD_LOGIC;
      aclr  : IN STD_LOGIC := '0';
      clken : IN STD_LOGIC := '1';
      dataa : IN STD_LOGIC_VECTOR (lpm_width-1 DOWNTO 0);
      datab : IN STD_LOGIC_VECTOR (lpm_width-1 DOWNTO 0);
      alb   : OUT STD_LOGIC; -- dataa < datab
      aeb   : OUT STD_LOGIC; -- dataa = datab
      agb   : OUT STD_LOGIC; -- dataa > datab
      ageb  : OUT STD_LOGIC; -- dataa >=datab
      aneb  : OUT STD_LOGIC; -- dataa !=datab
      aleb  : OUT STD_LOGIC  -- dataa <=datab
   );
end lpm_compare_inst;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of lpm_compare_inst is

   type pipeline_type is array(lpm_pipeline downto 0) of std_logic_vector(5 downto 0);
   signal pipeline : pipeline_type;
   
   function bool_to_logic( Input : boolean) return std_logic is
   begin
      if Input then
         return '1';
      else
         return '0';
      end if;
   end bool_to_logic;

begin

   COMPARE_AND_PIPELINE : process(all)
   begin
   if aclr = '1' then
      pipeline <= (others => (others => '0'));
   else
      if lpm_representation = "SIGNED" then
         pipeline(0)(0) <= bool_to_logic(signed(dataa) <  signed(datab)); -- alb
         pipeline(0)(1) <= bool_to_logic(signed(dataa) =  signed(datab)); -- aeb
         pipeline(0)(2) <= bool_to_logic(signed(dataa) >  signed(datab)); -- agb
         pipeline(0)(3) <= bool_to_logic(signed(dataa) >= signed(datab)); -- ageb
         pipeline(0)(4) <= bool_to_logic(signed(dataa) <= signed(datab)); -- aleb
         pipeline(0)(5) <= bool_to_logic(signed(dataa) /= signed(datab)); -- aneb
      else
         pipeline(0)(0) <= bool_to_logic(unsigned(dataa) <  unsigned(datab)); -- alb
         pipeline(0)(1) <= bool_to_logic(unsigned(dataa) =  unsigned(datab)); -- aeb
         pipeline(0)(2) <= bool_to_logic(unsigned(dataa) >  unsigned(datab)); -- agb
         pipeline(0)(3) <= bool_to_logic(unsigned(dataa) >= unsigned(datab)); -- ageb
         pipeline(0)(4) <= bool_to_logic(unsigned(dataa) <= unsigned(datab)); -- aleb
         pipeline(0)(5) <= bool_to_logic(unsigned(dataa) /= unsigned(datab)); -- aneb
      end if;
   
      if(rising_edge(clock)) then
         if clken = '1' then
            if lpm_pipeline > 0 then
               for i in 1 to lpm_pipeline loop
                  pipeline(i) <= pipeline(i-1);
               end loop;
            end if;
         end if;
       end if;
       
   end if;
   end process;

   alb   <=pipeline(lpm_pipeline)(0) ;
   aeb   <=pipeline(lpm_pipeline)(1) ;
   agb   <=pipeline(lpm_pipeline)(2) ;
   ageb  <=pipeline(lpm_pipeline)(3) ;
   aleb  <=pipeline(lpm_pipeline)(4) ;
   aneb  <=pipeline(lpm_pipeline)(5) ;

  
end arch;   





