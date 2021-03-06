//   ==================================================================
//   >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
//   ------------------------------------------------------------------
//   Copyright (c) 2006-2011 by Lattice Semiconductor Corporation
//   ALL RIGHTS RESERVED 
//   ------------------------------------------------------------------
//
//   IMPORTANT: THIS FILE IS AUTO-GENERATED BY THE LATTICEMICO SYSTEM.
//
//   Permission:
//
//      Lattice Semiconductor grants permission to use this code
//      pursuant to the terms of the Lattice Semiconductor Corporation
//      Open Source License Agreement.  
//
//   Disclaimer:
//
//      Lattice Semiconductor provides no warranty regarding the use or
//      functionality of this code. It is the user's responsibility to
//      verify the user?s design for consistency and functionality through
//      the use of formal verification methods.
//
//   --------------------------------------------------------------------
//
//                  Lattice Semiconductor Corporation
//                  5555 NE Moore Court
//                  Hillsboro, OR 97214
//                  U.S.A
//
//                  TEL: 1-800-Lattice (USA and Canada)
//                         503-286-8001 (other locations)
//
//                  web: http://www.latticesemi.com/
//                  email: techsupport@latticesemi.com
//
//   --------------------------------------------------------------------
//                         FILE DETAILS
// Project          : LatticeMico32
// File             : lm_mc_arithmetic.v
// Title            : Multi-cycle arithmetic unit.
// Dependencies     : lm32_include.v
// Version          : 6.1.17
//                  : Initial Release
// Version          : 7.0SP2, 3.0
//                  : No Change
// Version          : 3.1
//                  : No Change
// =============================================================================

`include "lm32_include.v"
           
`define LM32_MC_STATE_RNG         2:0
`define LM32_MC_STATE_IDLE        3'b000
`define LM32_MC_STATE_MULTIPLY    3'b001
`define LM32_MC_STATE_MODULUS     3'b010   
`define LM32_MC_STATE_DIVIDE      3'b011 
`define LM32_MC_STATE_SHIFT_LEFT  3'b100
`define LM32_MC_STATE_SHIFT_RIGHT 3'b101

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_mc_arithmetic (
    // ----- Inputs -----
    clk_i,
    rst_i,
    stall_d,
    kill_x,
`ifdef CFG_MC_DIVIDE_ENABLED
    divide_d,
    modulus_d,
`endif
`ifdef CFG_MC_MULTIPLY_ENABLED
    multiply_d,
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
    shift_left_d,
    shift_right_d,
    sign_extend_d,
`endif
    operand_0_d,
    operand_1_d,
    // ----- Ouputs -----
    result_x,
`ifdef CFG_MC_DIVIDE_ENABLED
    divide_by_zero_x,
`endif
    stall_request_x
    );

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                    // Clock
input rst_i;                                    // Reset
input stall_d;                                  // Stall instruction in D stage
input kill_x;                                   // Kill instruction in X stage
`ifdef CFG_MC_DIVIDE_ENABLED
input divide_d;                                 // Perform divide
input modulus_d;                                // Perform modulus
`endif
`ifdef CFG_MC_MULTIPLY_ENABLED
input multiply_d;                               // Perform multiply
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
input shift_left_d;                             // Perform left shift
input shift_right_d;                            // Perform right shift
input sign_extend_d;                            // Whether to sign-extend (arithmetic) or zero-extend (logical)
`endif
input [`LM32_WORD_RNG] operand_0_d;
input [`LM32_WORD_RNG] operand_1_d;

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [`LM32_WORD_RNG] result_x;               // Result of operation
reg    [`LM32_WORD_RNG] result_x;
`ifdef CFG_MC_DIVIDE_ENABLED
output divide_by_zero_x;                        // A divide by zero was attempted
reg    divide_by_zero_x;
`endif
output stall_request_x;                         // Request to stall pipeline from X stage back
wire   stall_request_x;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

reg [`LM32_WORD_RNG] p;                         // Temporary registers
reg [`LM32_WORD_RNG] a;
reg [`LM32_WORD_RNG] b;
`ifdef CFG_MC_DIVIDE_ENABLED
wire [32:0] t;
`endif

reg [`LM32_MC_STATE_RNG] state;                 // Current state of FSM
reg [5:0] cycles;                               // Number of cycles remaining in the operation

`ifdef CFG_MC_BARREL_SHIFT_ENABLED
reg sign_extend_x;                              // Whether to sign extend of zero extend right shifts
wire fill_value;                                // Value to fill with for right barrel-shifts
`endif

/////////////////////////////////////////////////////
// Combinational logic
/////////////////////////////////////////////////////

// Stall pipeline while any operation is being performed
assign stall_request_x = state != `LM32_MC_STATE_IDLE;

`ifdef CFG_MC_DIVIDE_ENABLED
// Subtraction
assign t = {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]} - b;
`endif

`ifdef CFG_MC_BARREL_SHIFT_ENABLED
// Determine fill value for right shift - Sign bit for arithmetic shift, or zero for logical shift
assign fill_value = (sign_extend_x == `TRUE) & b[`LM32_WORD_WIDTH-1];
`endif

/////////////////////////////////////////////////////
// Sequential logic
/////////////////////////////////////////////////////

// Perform right shift
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        cycles <= #1 {6{1'b0}};
        p <= #1 {`LM32_WORD_WIDTH{1'b0}};
        a <= #1 {`LM32_WORD_WIDTH{1'b0}};
        b <= #1 {`LM32_WORD_WIDTH{1'b0}};
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
        sign_extend_x <= #1 1'b0;
`endif
`ifdef CFG_MC_DIVIDE_ENABLED
        divide_by_zero_x <= #1 `FALSE;
`endif
        result_x <= #1 {`LM32_WORD_WIDTH{1'b0}};
        state <= #1 `LM32_MC_STATE_IDLE;
    end
    else
    begin
`ifdef CFG_MC_DIVIDE_ENABLED
        divide_by_zero_x <= #1 `FALSE;
`endif
        case (state)
        `LM32_MC_STATE_IDLE:
        begin
            if (stall_d == `FALSE)                 
            begin          
                cycles <= #1 `LM32_WORD_WIDTH;
                p <= #1 32'b0;
                a <= #1 operand_0_d;
                b <= #1 operand_1_d;                    
`ifdef CFG_MC_DIVIDE_ENABLED
                if (divide_d == `TRUE)
                    state <= #1 `LM32_MC_STATE_DIVIDE;
                if (modulus_d == `TRUE)
                    state <= #1 `LM32_MC_STATE_MODULUS;
`endif                    
`ifdef CFG_MC_MULTIPLY_ENABLED
                if (multiply_d == `TRUE)
                    state <= #1 `LM32_MC_STATE_MULTIPLY;
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
                if (shift_left_d == `TRUE)
                begin
                    state <= #1 `LM32_MC_STATE_SHIFT_LEFT;
                    sign_extend_x <= #1 sign_extend_d;
                    cycles <= #1 operand_1_d[4:0];
                    a <= #1 operand_0_d;
                    b <= #1 operand_0_d;
                end
                if (shift_right_d == `TRUE)
                begin
                    state <= #1 `LM32_MC_STATE_SHIFT_RIGHT;
                    sign_extend_x <= #1 sign_extend_d;
                    cycles <= #1 operand_1_d[4:0];
                    a <= #1 operand_0_d;
                    b <= #1 operand_0_d;
                end
`endif
            end            
        end
`ifdef CFG_MC_DIVIDE_ENABLED
        `LM32_MC_STATE_DIVIDE:
        begin
            if (t[32] == 1'b0)
            begin
                p <= #1 t[31:0];
                a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b1};
            end
            else 
            begin
                p <= #1 {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]};
                a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            end
            result_x <= #1 a;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
            begin
                // Check for divide by zero
                divide_by_zero_x <= #1 b == {`LM32_WORD_WIDTH{1'b0}};
                state <= #1 `LM32_MC_STATE_IDLE;
            end
            cycles <= #1 cycles - 1'b1;
        end
        `LM32_MC_STATE_MODULUS:
        begin
            if (t[32] == 1'b0)
            begin
                p <= #1 t[31:0];
                a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b1};
            end
            else 
            begin
                p <= #1 {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]};
                a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            end
            result_x <= #1 p;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
            begin
                // Check for divide by zero
                divide_by_zero_x <= #1 b == {`LM32_WORD_WIDTH{1'b0}};
                state <= #1 `LM32_MC_STATE_IDLE;
            end
            cycles <= #1 cycles - 1'b1;
        end
`endif        
`ifdef CFG_MC_MULTIPLY_ENABLED
        `LM32_MC_STATE_MULTIPLY:
        begin
            if (b[0] == 1'b1)
                p <= #1 p + a;
            b <= #1 {1'b0, b[`LM32_WORD_WIDTH-1:1]};
            a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            result_x <= #1 p;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= #1 `LM32_MC_STATE_IDLE;
            cycles <= #1 cycles - 1'b1;
        end
`endif     
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
        `LM32_MC_STATE_SHIFT_LEFT:
        begin       
            a <= #1 {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            result_x <= #1 a;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= #1 `LM32_MC_STATE_IDLE;
            cycles <= #1 cycles - 1'b1;
        end
        `LM32_MC_STATE_SHIFT_RIGHT:
        begin       
            b <= #1 {fill_value, b[`LM32_WORD_WIDTH-1:1]};
            result_x <= #1 b;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= #1 `LM32_MC_STATE_IDLE;
            cycles <= #1 cycles - 1'b1;
        end
`endif   
        endcase
    end
end 

endmodule
