/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE revB.2 compliant I2C Master controller Top-level  ////
////                                                             ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/projects/i2c/    ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2001 Richard Herveille                        ////
////                    richard@asics.ws                         ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////

//  CVS Log
//
//  $Id: i2c_master_top.v,v 1.11 2005/02/27 09:26:24 rherveille Exp $
//
//  $Date: 2005/02/27 09:26:24 $
//  $Revision: 1.11 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: i2c_master_top.v,v $
//               Revision 1.11  2005/02/27 09:26:24  rherveille
//               Fixed register overwrite issue.
//               Removed full_case pragma, replaced it by a default statement.
//
//               Revision 1.10  2003/09/01 10:34:38  rherveille
//               Fix a blocking vs. non-blocking error in the wb_dat output mux.
//
//               Revision 1.9  2003/01/09 16:44:45  rherveille
//               Fixed a bug in the Command Register declaration.
//
//               Revision 1.8  2002/12/26 16:05:12  rherveille
//               Small code simplifications
//
//               Revision 1.7  2002/12/26 15:02:32  rherveille
//               Core is now a Multimaster I2C controller
//
//               Revision 1.6  2002/11/30 22:24:40  rherveille
//               Cleaned up code
//
//               Revision 1.5  2001/11/10 10:52:55  rherveille
//               Changed PRER reset value from 0x0000 to 0xffff, conform specs.
//

// MSB Required Define and Includes
`ifndef I2C_MASTER_CORE_V
`define I2C_MASTER_CORE_V
`include "system_conf.v"

// synopsys translate_off
`include "./timescale.v"
// synopsys translate_on

`include "./i2c_master_defines.v"

 module i2cm_opencores 
 		#(parameter SPEED = 400,
		  parameter SYSCLK = 50)
 		(SDA        ,       // Serial Data Line of the I2C bus
		 SCL        ,       // Serial Clock Line of the I2C bus
		 CLK_I      ,       // MP CLK_I 
		 RST_I      ,       // Reset, active low
		 INTR_N     ,       // Interupt Request, active low
		 // Wishbone interface
		 I2CM_ADR_I  ,
		 I2CM_DAT_I  ,
		 I2CM_WE_I   ,
		 I2CM_CYC_I  ,
		 I2CM_STB_I  ,
		 I2CM_SEL_I  ,
		 I2CM_CTI_I  ,
		 I2CM_BTE_I  ,
		 I2CM_LOCK_I ,
		 I2CM_DAT_O  ,
		 I2CM_ACK_O  ,
		 I2CM_ERR_O  ,
		 I2CM_RTY_O  
		 );      // data bus to/from attached device(NOTE: Data(7) is MSB                         

   inout       SDA;
   inout       SCL;
   input       CLK_I;
   input       RST_I;
   
   output      INTR_N;
   
   input [31:0]	I2CM_ADR_I;  
   input [31:0]	I2CM_DAT_I;
   input		I2CM_WE_I;   
   input		I2CM_CYC_I;   
   input		I2CM_STB_I;   
   input [3:0]	I2CM_SEL_I;   
   input [2:0]	I2CM_CTI_I;
   input [1:0]	I2CM_BTE_I;
   input		I2CM_LOCK_I;
   output [31:0] I2CM_DAT_O;   
   output		I2CM_ACK_O;   
   output		I2CM_ERR_O;   
   output		I2CM_RTY_O;


	// parameters
	parameter ARST_LVL = 1'b0; // asynchronous reset level
	parameter PRESCALE = (((SYSCLK*1000000)/(5*SPEED*1000))-1);

	//
	// inputs & outputs
	//

	// wishbone signals
	wire        wb_clk;     // master clock input
	assign wb_clk = CLK_I;
	wire        wb_rst;     // synchronous active high reset
	assign wb_rst = 1'b0;
	wire        arst;       // asynchronous reset
	assign arst =	~RST_I;
	wire  [2:0] wb_adr;     // lower address bits
	assign wb_adr = I2CM_ADR_I[4:2];
	wire  [7:0] wb_dat_i;     // databus input
	assign wb_dat_i = I2CM_DAT_I[7:0];
	reg [7:0] wb_dat_o;     // databus output
	assign I2CM_DAT_O[7:0] = wb_dat_o;
	wire        wb_we;      // write enable input
	assign wb_we = I2CM_WE_I;
	wire        wb_stb;     // stobe/core select signal
	assign wb_stb = I2CM_STB_I;
	wire        wb_cyc;     // valid bus cycle input
	assign wb_cyc = I2CM_CYC_I;
	reg       wb_ack;     // bus cycle acknowledge output
	assign I2CM_ACK_O = wb_ack;
	reg       wb_inta;    // interrupt request signal output
	assign INTR_N = wb_inta;
	assign I2CM_ERR_O = 1'b0;
	assign I2CM_RTY_O = 1'b0;

	// I2C signals
	// i2c clock line
	wire scl_i;       // SCL-line input
	wire scl_o;       // SCL-line output (always 1'b0)
	wire scl_oen;     // SCL-line output enable (active low)
	//assign SCL = (scl_oen==1'b0) ? scl_o : 1'bZ;
	//assign scl_i = SCL;
	BB scl_inst(.I(scl_o), .T(scl_oen), .O(scl_i), .B(SCL));

	// i2c data line
	wire sda_i;       // SDA-line input
	wire sda_o;       // SDA-line output (always 1'b0)
	wire sda_oen;     // SDA-line output enable (active low)
	//assign 	SDA = (sda_oen==1'b0) ? sda_o : 1'bZ;
	//assign sda_i = SDA;
	BB sda_inst(.I(sda_o), .T(sda_oen), .O(sda_i), .B(SDA));
	
	//
	// variable declarations
	//

	// registers
	reg  [15:0] prer; // clock prescale register
	reg  [ 7:0] ctr;  // control register
	reg  [ 7:0] txr;  // transmit register
	wire [ 7:0] rxr;  // receive register
	reg  [ 7:0] cr;   // command register
	wire [ 7:0] sr;   // status register

	// done signal: command completed, clear command register
	wire done;


	// status register signals
	wire irxack;
	reg  rxack;       // received aknowledge from slave
	reg  tip;         // transfer in progress
	reg  irq_flag;    // interrupt pending flag
	wire i2c_busy;    // bus busy (start signal detected)
	wire i2c_al;      // i2c bus arbitration lost
	reg  al;          // status register arbitration lost bit

	// core enable signal
	wire core_en;
	wire ien;

	//
	// module body
	//

	// generate internal reset
	wire rst = arst ^ ARST_LVL;

	// generate wishbone signals
	wire wb_wacc = wb_cyc & wb_stb & wb_we;

	// generate acknowledge output signal
	always @(posedge wb_clk)
	  wb_ack <= #1 wb_cyc & wb_stb & ~wb_ack; // because timing is always honored

	// assign DAT_O
	always @(posedge wb_clk)
	begin
	  case (wb_adr) // synopsis parallel_case
	    3'b000: wb_dat_o <= #1 prer[ 7:0];
	    3'b001: wb_dat_o <= #1 prer[15:8];
	    3'b010: wb_dat_o <= #1 ctr;
	    3'b011: wb_dat_o <= #1 rxr; // write is transmit register (txr)
	    3'b100: wb_dat_o <= #1 sr;  // write is command register (cr)
	    3'b101: wb_dat_o <= #1 txr;
	    3'b110: wb_dat_o <= #1 cr;
	    3'b111: wb_dat_o <= #1 0;   // reserved
	  endcase
	end

	// generate registers
	always @(posedge wb_clk or negedge rst)
	  if (!rst)
	    begin
	        prer <= #1 16'hffff;
//	        prer <= #1 16'PRESCALE;
	        ctr  <= #1  8'h0;
	        txr  <= #1  8'h0;
	    end
	  else if (wb_rst)
	    begin
	        prer <= #1 16'hffff;
//	        prer <= #1 16'dPRESCALE;
	        ctr  <= #1  8'h0;
	        txr  <= #1  8'h0;
	    end
	  else
	    if (wb_wacc)
	      case (wb_adr) // synopsis parallel_case
	         3'b000 : prer [ 7:0] <= #1 wb_dat_i;
	         3'b001 : prer [15:8] <= #1 wb_dat_i;
	         3'b010 : ctr         <= #1 wb_dat_i;
	         3'b011 : txr         <= #1 wb_dat_i;
	         default: ;
	      endcase

	// generate command register (special case)
	always @(posedge wb_clk or negedge rst)
	  if (~rst)
	    cr <= #1 8'h0;
	  else if (wb_rst)
	    cr <= #1 8'h0;
	  else if (wb_wacc)
	    begin
	        if (core_en & (wb_adr == 3'b100) )
	          cr <= #1 wb_dat_i;
	    end
	  else
	    begin
	        if (done | i2c_al)
	          cr[7:4] <= #1 4'h0;           // clear command bits when done
	                                        // or when aribitration lost
	        cr[2:1] <= #1 2'b0;             // reserved bits
	        cr[0]   <= #1 1'b0;             // clear IRQ_ACK bit
	    end


	// decode command register
	wire sta  = cr[7];
	wire sto  = cr[6];
	wire rd   = cr[5];
	wire wr   = cr[4];
	wire ack  = cr[3];
	wire iack = cr[0];

	// decode control register
	assign core_en = ctr[7];
	assign ien = ctr[6];

	// hookup byte controller block
	i2c_master_byte_ctrl byte_controller (
		.clk      ( wb_clk     ),
		.rst      ( wb_rst     ),
		.nReset   ( rst        ),
		.ena      ( core_en      ),
		.clk_cnt  ( prer         ),
		.start    ( sta          ),
		.stop     ( sto          ),
		.read     ( rd           ),
		.write    ( wr           ),
		.ack_in   ( ack          ),
		.din      ( txr          ),
		.cmd_ack  ( done         ),
		.ack_out  ( irxack       ),
		.dout     ( rxr          ),
		.i2c_busy ( i2c_busy     ),
		.i2c_al   ( i2c_al       ),
		.scl_i    ( scl_i    ),
		.scl_o    ( scl_o    ),
		.scl_oen  ( scl_oen ),
		.sda_i    ( sda_i    ),
		.sda_o    ( sda_o    ),
		.sda_oen  ( sda_oen )
	);

	// status register block + interrupt request signal
	always @(posedge wb_clk or negedge rst)
	  if (!rst)
	    begin
	        al       <= #1 1'b0;
	        rxack    <= #1 1'b0;
	        tip      <= #1 1'b0;
	        irq_flag <= #1 1'b0;
	    end
	  else if (wb_rst)
	    begin
	        al       <= #1 1'b0;
	        rxack    <= #1 1'b0;
	        tip      <= #1 1'b0;
	        irq_flag <= #1 1'b0;
	    end
	  else
	    begin
	        al       <= #1 i2c_al | (al & ~sta);
	        rxack    <= #1 irxack;
	        tip      <= #1 ((rd | wr) & core_en);
	        irq_flag <= #1 (done | i2c_al | irq_flag) & ~iack; // interrupt request flag is always generated
	    end

	// generate interrupt request signals
	always @(posedge wb_clk or negedge rst)
	  if (!rst)
	    wb_inta <= #1 1'b0;
	  else if (wb_rst)
	    wb_inta <= #1 1'b0;
	  else
	    wb_inta <= #1 irq_flag && ien; // interrupt signal is only generated when IEN (interrupt enable bit is set)

	// assign status register bits
	assign sr[7]   = rxack;
	assign sr[6]   = i2c_busy;
	assign sr[5]   = al;
	assign sr[4:2] = 3'h0; // reserved
	assign sr[1]   = tip;
	assign sr[0]   = irq_flag;

endmodule
`endif // I2C_MASTER_CORE_V