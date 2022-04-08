`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////
//
// A simple register device with three registers:
// ----------------------------------------------
//
//////////////////////////////////////////////////////////////////////////////
module wb_fifo_dev
   #(
      parameter reg_16_int_val = 32'h1234abcd
   )
   (
//---------------------------------------------------------------------
//
// WISHBONE clock/reset signals
//
//---------------------------------------------------------------------
      wb_reset,//-----------------WISHBONE reset
      wb_clk,//-------------------WISHBONE clock
//---------------------------------------------------------------------
//
// WISHBONE interface signals below.
// - This component does not support burst transfers.
//
//---------------------------------------------------------------------
      wb_adr,//-------------------Address from master
      wb_master_data,//-----------Data from master
      wb_cyc,//-------------------WISHBONE cycle-valid qualifier
      wb_stb,//-------------------WISHBONE transfer qualifier
      wb_sel,//-------------------Data byte-lane selection
      wb_we,//--------------------Write-enable
      wb_slave_data,//------------Data from slave
      wb_ack,//-------------------Data-valid qualifier from slave
      wb_err,//-------------------Error qualifier from slave (never asserted)
      wb_rty,//-------------------Retry qualifier from slave (never asserted)
//---------------------------------------------------------------------
//
// Interrupt line (active-high) that will be connected to the
// processor. Not used but for demonstrating custom component
// connectivity.
//
//
//---------------------------------------------------------------------
      wb_intr,//------------------Interrupt request from slave
//---------------------------------------------------------------------
//
// External pins exposed by this component
//
//---------------------------------------------------------------------
      out_pins, //------------------Output pins
      // External FIFO (outputs)
      of_d,
		of_wr,
		of_wrfull,
		// External FIFO (inputs)
		if_d,
		if_rd,
		if_rdempty,
		
		fifo_rst
   );
   input wb_reset;
   input wb_clk;
   input [31:0] wb_adr;
   input [31:0] wb_master_data;
   input wb_cyc;
   input wb_stb;
   input [3:0] wb_sel;
   input wb_we;
   output [31:0] wb_slave_data;
   output wb_ack;
   output wb_err;
   output wb_rty;
   output wb_intr;
   output [7:0] out_pins;
   output [31:0] of_d;
	output of_wr;
	input of_wrfull;
	input [31:0] if_d;
	output if_rd;
	input if_rdempty;
	output fifo_rst;
   
//------------------------------------------------------------------------
//
// Registers
//
// reg_00 : read/write 32-bit register, Data for writing to output FIFO
//
// reg_04 : read-only 32-bit register that contains data red from the input FIFO
//
// reg_08 : read-only register that contains a FIFO status: 
// reg_08[0]= if_rdempty;
// reg_08[1]= of_wrfull; 
//
// reg_12 : FIFO control register-select
// reg_12[0]= reset (Active-high)
//
// reg_16 : read-only register that contains a constant specified when
// instantiating this component in a platform
//
//------------------------------------------------------------------------
   reg [31:0] reg_00;//------------------------32-bits, RW, offset  0
   reg [31:0] reg_04;//------------------------32-bits, RO, offset  4
   reg [31:0] reg_08;//------------------------32-bits, RO, offset  8
   reg [31:0] reg_12;//------------------------32-bits, RW, offset 12
   //reg_16 constant ------------------------32-bits, RO, offset 16
   reg write_ack;//-----------------------------write-ack
   reg read_ack;//-----------------------------read-ack
   reg fiford_reg;
   reg [31:0] status_reg;
//------------------------------------------------------------------------
//
// Wires
//
//------------------------------------------------------------------------
   wire reg_00_sel;//---------------------------reg_00 selected
   wire reg_04_sel;//---------------------------reg_04 selected
   wire reg_08_sel;//---------------------------reg_08 selected
   wire reg_12_sel;//---------------------------reg_12 selected
   wire reg_16_sel;//---------------------------reg_16 selected
   wire [31:0] read_data;//---------------------reg data mux (reads)
   wire fiford; 
//
// assign register-select signals:
// since there are only four registers, use bit-3 of the
// address bus since addressing is word addressing for LatticeMico32
//
   assign reg_00_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[4:2] == 2'b000) ) ? 1'b1 : 1'b0;
   assign reg_04_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[4:2] == 2'b001) ) ? 1'b1 : 1'b0;
   assign reg_08_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[4:2] == 2'b010) ) ? 1'b1 : 1'b0;
   assign reg_12_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[4:2] == 2'b011) ) ? 1'b1 : 1'b0;
   assign reg_16_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[4:2] == 2'b100) ) ? 1'b1 : 1'b0;
   
//
// assign read ack: unregistered as data is presented
// immediately. Can make it registered to improve timing
//
//   assign read_ack = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
//   && (wb_we == 1'b0)) ? 1'b1:1'b0;

// assign read ack: registered, to capture read data from FIFO, 
// data is presented in next clock cycle
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         read_ack <= 0;
      end
      else begin
         if( (wb_stb == 1'b1) && (wb_cyc == 1'b1) && (wb_we == 1'b0) ) begin
            read_ack <= 1'b1;
         end
         else begin
            read_ack <= 1'b0;
         end
      end

//
// assign asynchronous data-output mux
//
   assign read_data =   (reg_00_sel == 1'b1)? reg_00 :
                        (reg_04_sel == 1'b1)? if_d :
                        (reg_08_sel == 1'b1)? status_reg :
                        (reg_12_sel == 1'b1)? reg_12 :
                        (reg_16_sel == 1'b1)? reg_16_int_val :
                        32'hdeadbeef;
//
// assign write-ack: registered
//
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         write_ack <= 0;
      end
      else begin
         if( (wb_stb == 1'b1) && (wb_cyc == 1'b1) &&
            (wb_we == 1'b1) && (write_ack == 1'b0) ) begin
            write_ack <= 1'b1;
         end
         else begin
            write_ack <= 1'b0;
         end
      end
 
//
// FIFO status reg_08
//
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         status_reg <= 0;
      end
      else begin
         status_reg[0] <= if_rdempty;
         status_reg[1] <= of_wrfull;
         status_reg[31:2] <= 0;
      end 
 
//
// Input FIFO read formation
//
   assign fiford = ((reg_04_sel== 1'b1) && (if_rdempty==1'b0)) ? 1'b1 : 1'b0;
   
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         fiford_reg <= 0;
      end
      else begin
         fiford_reg <= fiford;
      end
      
//
// register_00 write process: supports byte-writes
//
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         reg_00 <= 32'b0;
      end
      else begin
         if ( (reg_00_sel == 1'b1) && (wb_we == 1'b1) &&
            (write_ack == 1'b0) ) begin
            if( wb_sel[0] == 1'b1 ) begin
               reg_00[7:0] <= wb_master_data[7:0];
            end
            if( wb_sel[1] == 1'b1 ) begin
               reg_00[15:8] <= wb_master_data[15:8];
            end
            if( wb_sel[2] == 1'b1 ) begin
               reg_00[23:16] <= wb_master_data[23:16];
            end
            if( wb_sel[3] == 1'b1 ) begin
               reg_00[31:24] <= wb_master_data[31:24];
            end
         end
      end
      
//
// register_12 write process: supports byte-writes
//
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         reg_12 <= 0;
      end
      else begin
         if ( (reg_12_sel == 1'b1) && (wb_we == 1'b1) &&
            (write_ack == 1'b0) ) begin
            if( wb_sel[0] == 1'b1 ) begin
               reg_12[7:0] <= wb_master_data[7:0];
            end
            if( wb_sel[1] == 1'b1 ) begin
               reg_12[15:8] <= wb_master_data[15:8];
            end
            if( wb_sel[2] == 1'b1 ) begin
               reg_12[23:16] <= wb_master_data[23:16];
            end
            if( wb_sel[3] == 1'b1 ) begin
               reg_12[31:24] <= wb_master_data[31:24];
            end
         end
      end
      
      

//------------------------------------------------------------------------
//
// MODULE OUTPUTS
//
//------------------------------------------------------------------------
   // assign component ack
   assign wb_ack = read_ack | write_ack;
   // assign component data
   assign wb_slave_data = read_data;
   // unused rty/err
   assign wb_rty = 1'b0;
   assign wb_err = 1'b0;
   // unused interrupt (active-high)
   assign wb_intr = 1'b0;
   assign out_pins = reg_00[7:0];
   
   //Output FIFO
   assign of_d = wb_master_data;
   assign of_wr = reg_00_sel & wb_we & ~write_ack & ~of_wrfull;
   
   //Input FIFO
   assign if_rd = ((fiford_reg == 1'b0) && (fiford == 1'b1)) ? 1'b1 : 1'b0;
   
   //FIFO reset
   assign fifo_rst = reg_12[0];
   
   
   
	
	endmodule