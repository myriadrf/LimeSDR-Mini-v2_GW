-- VHDL module instantiation generated by SCUBA Diamond (64-bit) 3.12.1.454
-- Module  Version: 5.8
-- Wed Feb 23 21:58:48 2022

-- parameterized module component declaration
component fifodc_w128x256_r128_outreg
    port (Data: in  std_logic_vector(127 downto 0); 
        WrClock: in  std_logic; RdClock: in  std_logic; 
        WrEn: in  std_logic; RdEn: in  std_logic; Reset: in  std_logic; 
        RPReset: in  std_logic; Q: out  std_logic_vector(127 downto 0); 
        WCNT: out  std_logic_vector(8 downto 0); 
        RCNT: out  std_logic_vector(8 downto 0); Empty: out  std_logic; 
        Full: out  std_logic);
end component;

-- parameterized module component instance
__ : fifodc_w128x256_r128_outreg
    port map (Data(127 downto 0)=>__, WrClock=>__, RdClock=>__, WrEn=>__, 
        RdEn=>__, Reset=>__, RPReset=>__, Q(127 downto 0)=>__, WCNT(8 downto 0)=>__, 
        RCNT(8 downto 0)=>__, Empty=>__, Full=>__);
