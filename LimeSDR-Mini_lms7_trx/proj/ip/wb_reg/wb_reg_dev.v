`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////
//
// A simple register device with three registers:
// ----------------------------------------------
//
//////////////////////////////////////////////////////////////////////////////
module wb_reg_dev
   #(
      parameter CLK_MHZ = 25,
      parameter reg_08_int_val = 32'h1234abcd
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
      out_pins//------------------Output pins
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
//------------------------------------------------------------------------
//
// Registers
//
// reg_00 : read/write 32-bit register, general purpose
//
// reg_04 : read-only 32-bit register that contains the WISHBONE
// platform clock frequency (MHz)
//
// reg_08 : read-only register that contains a constant specified when
// instantiating this component in a platform
//
//------------------------------------------------------------------------
   reg [31:0] reg_00;//------------------------32-bits, RW, offset 0
   reg [31:0] reg_04;//------------------------32-bits, RW, offset 4
   //reg_08 constant ------------------------32-bits, RO, offset 8
   reg write_ack;//-----------------------------write-ack
//------------------------------------------------------------------------
//
// Wires
//
//------------------------------------------------------------------------
   wire reg_00_sel;//---------------------------reg_00 selected
   wire reg_04_sel;//---------------------------reg_04 selected
   wire reg_08_sel;//---------------------------reg_08 selected
   wire read_ack;//-----------------------------read-ack
   wire [31:0] read_data;//---------------------reg data mux (reads)
//
// assign register-select signals:
// since there are only two registers, use bit-2 of the
// address bus since addressing is word addressing for LatticeMico32
//
   assign reg_00_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[3:2] == 2'b00) ) ? 1'b1 : 1'b0;
   assign reg_04_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[3:2] == 2'b01) ) ? 1'b1 : 1'b0;
   assign reg_08_sel = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_adr[3:2] == 2'b10) ) ? 1'b1 : 1'b0;
//
// assign read ack: unregistered as data is presented
// immediately. Can make it registered to improve timing
//
   assign read_ack = ((wb_stb == 1'b1) && (wb_cyc == 1'b1)
   && (wb_we == 1'b0)) ? 1'b1:1'b0;
//
// assign asynchronous data-output mux
//
   assign read_data = (reg_00_sel == 1'b1)? reg_00 :
   (reg_04_sel == 1'b1)? reg_04 :
   (reg_08_sel == 1'b1)? reg_08_int_val :
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
// register_04 write process: supports byte-writes
//
   function integer i_clk_mhz;
      input integer clk_mhz;
      begin
         i_clk_mhz = clk_mhz;
      end
   endfunction // i_clk_mhz
   parameter CLK_MHZ_INT_VALUE = i_clk_mhz(CLK_MHZ);
   always @( posedge wb_clk or posedge wb_reset )
      if ( wb_reset ) begin
         reg_04 <= CLK_MHZ_INT_VALUE;
      end
      else begin
         if ( (reg_04_sel == 1'b1) && (wb_we == 1'b1) &&
            (write_ack == 1'b0) ) begin
            if( wb_sel[0] == 1'b1 ) begin
               reg_04[7:0] <= wb_master_data[7:0];
            end
            if( wb_sel[1] == 1'b1 ) begin
               reg_04[15:8] <= wb_master_data[15:8];
            end
            if( wb_sel[2] == 1'b1 ) begin
               reg_04[23:16] <= wb_master_data[23:16];
            end
            if( wb_sel[3] == 1'b1 ) begin
               reg_04[31:24] <= wb_master_data[31:24];
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
	
	endmodule