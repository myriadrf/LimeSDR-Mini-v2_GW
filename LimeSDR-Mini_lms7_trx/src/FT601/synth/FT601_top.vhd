-- ----------------------------------------------------------------------------	
-- FILE: 	FT601_top.vhd
-- DESCRIPTION:	top module for FT601
-- DATE:	May 13, 2016
-- AUTHOR(s):	Lime Microsystems
-- REVISIONS:
-- ----------------------------------------------------------------------------	
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.FIFO_PACK.all;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
entity FT601_top is
	generic(
         FT_data_width        : integer := 32;
         FT_be_width          : integer := 4;
			EP02_rdusedw_width	: integer := 11; 
			EP02_rwidth				: integer := 8;
			EP82_wrusedw_width	: integer := 11;
			EP82_wwidth				: integer := 8;
			EP82_wsize  			: integer := 64;  --packet size in bytes, has to be multiple of 4 bytes
         EP03_rdusedw_width	: integer := 11;    
			EP03_rwidth				: integer := 32;
         EP83_wrusedw_width	: integer := 12;
			EP83_wwidth				: integer := 64;
			EP83_wsize  			: integer := 2048 --packet size in bytes, has to be multiple of 4 bytes	
	);
	port (
			--input ports 
			clk         	: in std_logic;			--FTDI CLK
			reset_n   		: in std_logic;
			--FTDI external ports
			FT_wr_n			: out std_logic;
			FT_rxf_n			: in std_logic;
			FT_data			: inout std_logic_vector(FT_data_width-1 downto 0);
			FT_be				: inout std_logic_vector(FT_be_width-1 downto 0);
			FT_txe_n			: in std_logic;
         FT_RESETn      : out std_logic;
		   --controll endpoint fifo PC->FPGA 
			EP02_rdclk		: in std_logic;
			EP02_rd			: in std_logic;
			EP02_rdata		: out std_logic_vector(EP02_rwidth-1 downto 0);
			EP02_rempty		: out std_logic;
			--controll endpoint fifo FPGA->PC
			EP82_wclk		: in std_logic;
			EP82_aclrn		: in std_logic;
			EP82_wr			: in std_logic;
			EP82_wdata		: in std_logic_vector(EP82_wwidth-1 downto 0);
			EP82_wfull		: out std_logic;
			--stream endpoint fifo PC->FPGA
         EP03_active    : out std_logic;
         EP03_aclrn     : in std_logic;
         EP03_rdclk		: in std_logic;
         EP03_rd			: in std_logic;
         EP03_rdata		: out std_logic_vector(EP03_rwidth-1 downto 0);
         EP03_rempty		: out std_logic;
         EP03_rusedw    : out std_logic_vector(EP03_rdusedw_width-1 downto 0);
			--stream endpoint fifo FPGA->PC
         EP83_active    : out std_logic;
			EP83_wclk		: in std_logic;
			EP83_aclrn		: in std_logic;
			EP83_wr			: in std_logic;
			EP83_wdata		: in std_logic_vector(EP83_wwidth-1 downto 0);
			EP83_wfull		: out std_logic;
			EP83_wrusedw	: out std_logic_vector(EP83_wrusedw_width-1 downto 0)
        
        );
end FT601_top;

-- ----------------------------------------------------------------------------
-- Architecture
-- ----------------------------------------------------------------------------
architecture arch of FT601_top is
--declare signals,  components here
signal reset 					: std_logic;
--EP02 fifo signals
signal EP82_aclr 				: std_logic;
signal EP02_empty			   : std_logic;
signal EP02_wrempty			: std_logic;
signal EP02_wr					: std_logic; 
signal EP02_wdata				: std_logic_vector(FT_data_width-1 downto 0);
--EP82 fifo signals
signal EP82_fifo_rdusedw	: std_logic_vector(FIFORD_SIZE(EP82_wwidth, FT_data_width, EP82_wrusedw_width)-1 downto 0);
signal EP82_fifo_q			: std_logic_vector(FT_data_width-1 downto 0);
signal EP82_fifo_rdreq		: std_logic;

--EP03 fifo signals
signal EP03_sclrn          : std_logic;signal EP03_sclr           : std_logic;
signal EP03_empty			   : std_logic;
signal EP03_wrempty			: std_logic; 
signal EP03_wr					: std_logic;
signal EP03_wdata				: std_logic_vector(FT_data_width-1 downto 0);
signal EP03_rdy            : std_logic;
signal EP03_wr_cnt         : unsigned(15 downto 0);
signal EP03_fifo_wusedw    : std_logic_vector(10 downto 0);
signal EP03_fifo_rusedw    : std_logic_vector(EP03_rdusedw_width-1 downto 0);

--EP83 fifo signals
signal EP83_aclr				: std_logic;
signal EP83_fifo_rdusedw	: std_logic_vector(FIFORD_SIZE(EP83_wwidth, FT_data_width, EP83_wrusedw_width)-1 downto 0);
signal EP83_fifo_q			: std_logic_vector(FT_data_width-1 downto 0);
signal EP83_fifo_rdreq		: std_logic;

--arbiter signals
signal arb_en 					: std_logic; 
signal arb_rd_wr				: std_logic;
signal arb_nth_ch				: std_logic_vector(3 downto 0);

--fsm signals
signal fsm_rdy					: std_logic;
signal fsm_rd_data_valid   : std_logic;
signal fsm_rd_data			: std_logic_vector(FT_data_width-1 downto 0);
signal fsm_wr_data_req     : std_logic;
signal fsm_wr_data  			: std_logic_vector(FT_data_width-1 downto 0);

attribute noprune : boolean;
attribute noprune of EP03_wr_cnt : signal is true;

    component fifodc_w32x1024_r128_outreg
        port (Data : in std_logic_vector(31 downto 0); 
        WrClock: in std_logic; RdClock: in std_logic; WrEn: in std_logic; 
        RdEn: in std_logic; Reset: in std_logic; RPReset: in std_logic; 
        Q : out std_logic_vector(127 downto 0); 
        WCNT : out std_logic_vector(10 downto 0); 
        RCNT : out std_logic_vector(8 downto 0); Empty: out std_logic; 
        Full: out std_logic
    );
    end component;
	 
	 component fifodc_w64x2024_r32
        port (Data : in std_logic_vector(63 downto 0); 
        WrClock: in std_logic; RdClock: in std_logic; WrEn: in std_logic; 
        RdEn: in std_logic; Reset: in std_logic; RPReset: in std_logic; 
        Q : out std_logic_vector(31 downto 0); 
        WCNT : out std_logic_vector(11 downto 0); 
        RCNT : out std_logic_vector(12 downto 0); Empty: out std_logic; 
        Full: out std_logic
    );
    end component;
	 
	 component fifodc_w32x1024_r128
    port (Data: in  std_logic_vector(31 downto 0); 
        WrClock: in  std_logic; RdClock: in  std_logic; 
        WrEn: in  std_logic; RdEn: in  std_logic; Reset: in  std_logic; 
        RPReset: in  std_logic; Q: out  std_logic_vector(127 downto 0); 
        WCNT: out  std_logic_vector(10 downto 0); 
        RCNT: out  std_logic_vector(8 downto 0); Empty: out  std_logic; 
        Full: out  std_logic);
	end component;
	 
attribute syn_noprune : boolean;
attribute syn_noprune of fifodc_w32x1024_r128_outreg : component is true;
attribute syn_noprune of fifodc_w64x2024_r32 : component is true;
--fifo for endpoints component
component fifo_inst is
  generic(dev_family	     : string  := "MAX 10";
          wrwidth         : integer := 64;
          wrusedw_witdth  : integer := 9; --12=2048 words 
          rdwidth         : integer := 32;
          rdusedw_width   : integer := 10;
          show_ahead      : string  := "OFF"
  );  

  port (

      reset_n       : in std_logic;
      wrclk         : in std_logic;
      wrreq         : in std_logic;
      data          : in std_logic_vector(wrwidth-1 downto 0);
      wrfull        : out std_logic;
		wrempty		  : out std_logic;
      wrusedw       : out std_logic_vector(wrusedw_witdth-1 downto 0);
      rdclk 	     : in std_logic;
      rdreq         : in std_logic;
      q             : out std_logic_vector(rdwidth-1 downto 0);
      rdempty       : out std_logic;
      rdusedw       : out std_logic_vector(rdusedw_width-1 downto 0)     
        
        );
end component;

--FT6001 arbiter component
component FT601_arb is
	generic(
         FT_data_width     : integer := 32;
			EP82_fifo_rwidth	: integer := 9;
			EP82_wsize       	: integer := 64;  --packet size in bytes, has to be multiple of 4 bytes
			EP83_fifo_rwidth	: integer := 11;
			EP83_wsize       	: integer := 2048 --packet size in bytes, has to be multiple of 4 bytes
	);
  port (
			--input ports 
			clk       			: in std_logic;
			reset_n   			: in std_logic;
			enable       		: in std_logic;
			--control EP PC->FPGA
			EP02_fifo_data		: out std_logic_vector(FT_data_width-1 downto 0);
			EP02_fifo_wr		: out std_logic;
			EP02_fifo_wrempty	: in std_logic;
			--control EP FPGA->PC
			EP82_fifo_data		: in std_logic_vector(FT_data_width-1 downto 0);
			EP82_fifo_rd 		: out std_logic;
			EP82_fifo_rdusedw	: in std_logic_vector(EP82_fifo_rwidth-1 downto 0);
			--stream EP PC->FPGA
			EP03_fifo_data		: out std_logic_vector(FT_data_width-1 downto 0);
			EP03_fifo_wr		: out std_logic;
			EP03_fifo_wrempty	: in std_logic;		
			--stream EP FPGA->PC
			EP83_fifo_data		: in std_logic_vector(FT_data_width-1 downto 0);
			EP83_fifo_rd 		: out std_logic;
			EP83_fifo_rdusedw	: in std_logic_vector(EP83_fifo_rwidth-1 downto 0);
			--fsm controll signals
			fsm_epgo				: out std_logic;
			fsm_rdwr				: out std_logic; -- 0- MASTER RD (PC->FPGA), 1-MASTER WR (FPGA->PC)
			fsm_ch				: out std_logic_vector(3 downto 0);
			fsm_rdy				: in std_logic;
         fsm_rddata_valid  : in std_logic;
         fsm_rddata        : in std_logic_vector(FT_data_width-1 downto 0);
         fsm_wrdata_req    : in std_logic;
         fsm_wrdata        : out std_logic_vector(FT_data_width-1 downto 0);
			
			ep_status			: in std_logic_vector(7 downto 0)
        
        );
end component;

--FT601 fsm component 
component FT601 is
    generic(
         FT_data_width     : integer := 32;
         FT_be_width       : integer := 4;
			EP82_wsize        : integer := 64;  	--packet size in bytes, has to be multiple of 4 bytes
			EP83_wsize        : integer := 2048 	--packet size in bytes, has to be multiple of 4 bytes
			);
  port (
			clk			   : in std_logic;
			reset_n		   : in std_logic;
			trnsf_en       : in std_logic;
			ready			   : out std_logic;
			rd_wr    	   : in std_logic;     		-- 0- rd, 1-wr
			ch_n     	   : in std_logic_vector(3 downto 0);
         RD_data_valid  : out std_logic;
			RD_data        : out  std_logic_vector(FT_data_width-1 downto 0);
         WR_data_req    : out std_logic;     
			WR_data        : in std_logic_vector(FT_data_width-1 downto 0); -- should be 2 cycle latency after WR_data_req   
			wr_n			   : out std_logic;
			rxf_n			   : in std_logic;
			data			   : inout std_logic_vector(FT_data_width-1 downto 0);
			be             : inout std_logic_vector(FT_be_width-1 downto 0);
			txe_n			   : in std_logic
        );
end component;
  
begin
	reset <= not reset_n;
   
   FT_RESETn <= reset_n;

   sync_reg0 : entity work.sync_reg 
   port map(clk, EP03_aclrn, '1', EP03_sclrn);
	
	-- FIFO resets
	EP82_aclr <= NOT EP82_aclrn;
	EP03_sclr <= NOT EP03_sclrn;
	EP83_aclr <= NOT EP83_aclrn;
   




-- ----------------------------------------------------------------------------
-- FTDI endpoint fifos
-- ----------------------------------------------------------------------------
-- controll PC->FPGA 	  
EP02_fifo : entity work.fifodc_w32x256_r32
    port map (
      Data(31 downto 0) => EP02_wdata, 
      WrClock           => clk, 
      RdClock           => EP02_rdclk, 
      WrEn              => EP02_wr, 
      RdEn              => EP02_rd, 
      Reset             => reset, 
      RPReset           => reset, 
      Q(31 downto 0)    => EP02_rdata, 
      WCNT              => open, 
      RCNT              => open, 
      Empty             => EP02_empty, 
      Full              => open
   );
	 
	 --TODO: Check this
	 EP02_wrempty <= EP02_empty;
	 EP02_rempty  <= EP02_empty;
		  
-- controll FPGA->PC	  
EP82_fifo : entity work.fifodc_w32x256_r32
    port map (
		Data(31 downto 0)=> EP82_wdata, 
		WrClock				=> EP82_wclk, 
		RdClock				=> clk, 
		WrEn					=> EP82_wr, 
		RdEn					=> EP82_fifo_rdreq, 
		Reset					=> EP82_aclr, 
		RPReset				=> EP82_aclr, 
		Q(31 downto 0)	=> EP82_fifo_q, 
		WCNT					=> open, 
		RCNT					=> EP82_fifo_rdusedw, 
		Empty					=> open, 
		Full					=> EP82_wfull
	 );
	 
--stream PC->FPGA 
EP03_fifo_status : entity work.busy_delay
   generic map (
      clock_period 	=> 10,  -- input clock period in ns
      delay_time 		=> 100 -- delay time in ms 
   )
   port map(
      --input ports 
      clk      => clk,
      reset_n  => reset_n,
      busy_in  => EP03_wr,
      busy_out => EP03_active
   );
   
EP03_fifo : fifodc_w32x1024_r128
    port map (
      Data(31 downto 0) => EP03_wdata, 
      WrClock           => clk, 
      RdClock           => EP03_rdclk, 
      WrEn              => EP03_wr, 
      RdEn              => EP03_rd, 
      Reset             => EP03_sclr, 
      RPReset           => EP03_sclr, 
      Q(127 downto 0)   => EP03_rdata, 
      WCNT              => EP03_fifo_wusedw, 
      RCNT              => EP03_fifo_rusedw, 
      Empty             => EP03_empty, 
      Full              => open
   );
   
   --TODO: Check this
   --EP03_wrempty <= '0' when unsigned(EP03_fifo_wusedw)> 0 else '1';
   EP03_wrempty <= EP03_empty;
   EP03_rusedw  <= EP03_fifo_rusedw;
   --EP03_rempty  <= '0' when unsigned(EP03_fifo_rusedw)> 0 else '1';
   EP03_rempty  <= EP03_empty;	
   
   proc_name : process(clk, reset_n)
   begin
      if reset_n = '0' then 
         EP03_wr_cnt <= (others=>'0');
      elsif (clk'event AND clk='1') then 
         if EP03_wr = '1' then 
            EP03_wr_cnt <= EP03_wr_cnt+1;
         else 
            EP03_wr_cnt <= (others=>'0');
         end if;
      end if;
   end process;
	
-- stream FPGA->PC
EP83_fifo_status : entity work.busy_delay
   generic map (
      clock_period 	=> 10,  -- input clock period in ns
      delay_time 		=> 100 -- delay time in ms 
   )
   port map(
      --input ports 
      clk      => clk,
      reset_n  => reset_n,
      busy_in  => EP83_fifo_rdreq,
      busy_out => EP83_active
   );		
      
EP83_fifo : fifodc_w64x2024_r32
    port map (
		Data(63 downto 0)=> EP83_wdata, 
		WrClock				=> EP83_wclk, 
		RdClock				=> clk, 
		WrEn					=> EP83_wr, 
		RdEn					=> EP83_fifo_rdreq, 
		Reset					=> EP83_aclr, 
		RPReset				=> EP83_aclr, 
		Q(31 downto 0)	=> EP83_fifo_q, 
		WCNT(11 downto 0)=> EP83_wrusedw, 
		RCNT(12 downto 0)=> EP83_fifo_rdusedw, 
		Empty					=> open, 
		Full					=> EP83_wfull
	 );

   process(clk, reset_n)
   begin
      if reset_n = '0' then 
         EP03_rdy <= '0';
      elsif (clk'event AND clk='1') then 
         if EP03_wrempty = '0' OR unsigned(EP03_fifo_wusedw) > 0 then 
            EP03_rdy <= '0';
         else 
            EP03_rdy <= '1';
         end if;
      end if;
   end process;
      
-- ----------------------------------------------------------------------------
-- FTDI arbiter
-- ----------------------------------------------------------------------------		
	ftdi_arbiter : FT601_arb
	generic map(
         FT_data_width     => FT_data_width,
			EP82_fifo_rwidth	=> FIFORD_SIZE(EP82_wwidth, FT_data_width, EP82_wrusedw_width),
			EP82_wsize       	=> EP82_wsize,
			EP83_fifo_rwidth	=> FIFORD_SIZE(EP83_wwidth, FT_data_width, EP83_wrusedw_width),
			EP83_wsize       	=> EP83_wsize
	)
  port map(
			clk       			=> clk, 
			reset_n   			=> reset_n,
			enable       		=> '1',
			EP02_fifo_data		=> EP02_wdata, 
			EP02_fifo_wr		=> EP02_wr, 
			EP02_fifo_wrempty	=> EP02_wrempty,
			EP82_fifo_data		=> EP82_fifo_q,
			EP82_fifo_rd 		=> EP82_fifo_rdreq,
			EP82_fifo_rdusedw	=> EP82_fifo_rdusedw,
			EP03_fifo_data		=> EP03_wdata,
			EP03_fifo_wr		=> EP03_wr,
			EP03_fifo_wrempty	=> EP03_rdy,
			EP83_fifo_data		=> EP83_fifo_q,
			EP83_fifo_rd 		=> EP83_fifo_rdreq,	
			EP83_fifo_rdusedw	=> EP83_fifo_rdusedw,

			fsm_epgo				=> arb_en, 
			fsm_rdwr				=> arb_rd_wr,
			fsm_ch				=> arb_nth_ch, 		 
			fsm_rdy				=> fsm_rdy, 
			fsm_rddata_valid	=> fsm_rd_data_valid,
			fsm_rddata			=> fsm_rd_data,
         fsm_wrdata_req    => fsm_wr_data_req,
			fsm_wrdata			=> fsm_wr_data,
			ep_status			=> FT_data(15 downto 8)       
			);
		  
-- ----------------------------------------------------------------------------
-- FTDI fsm 
-- ----------------------------------------------------------------------------		  
ft_fsm : FT601
generic map(
         FT_data_width  => FT_data_width,
         FT_be_width    => FT_be_width,  
			EP82_wsize	   => EP82_wsize,
			EP83_wsize	   => EP83_wsize 
			)
  port map (
			clk			   => clk,
			reset_n		   => reset_n,
			trnsf_en       => arb_en,
			ready			   => fsm_rdy,
			rd_wr    	   => arb_rd_wr,
			ch_n     	   => arb_nth_ch,
			RD_data_valid	=> fsm_rd_data_valid, 
			RD_data   	   => fsm_rd_data,
         WR_data_req    => fsm_wr_data_req,
			WR_data  	   => fsm_wr_data,  
			wr_n			   => FT_wr_n,
			rxf_n			   => FT_rxf_n,
			data			   => FT_data,
			be				   => FT_be,
			txe_n			   => FT_txe_n
			);
		  
  
end arch;





