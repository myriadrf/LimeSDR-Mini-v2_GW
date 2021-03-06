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
// File             : lm32_multiplier.v
// Title            : Pipelined multiplier.
// Dependencies     : lm32_include.v
// Version          : 6.1.17
//                  : Initial Release
// Version          : 7.0SP2, 3.0
//                  : No Change
// Version          : 3.1
//                  : No Change
// =============================================================================
                  
`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_multiplier (
    // ----- Inputs -----
    clk_i,
    rst_i,
    stall_x,
    stall_m,
    operand_0,
    operand_1,
    // ----- Ouputs -----
    result
    );

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                            // Clock 
input rst_i;                            // Reset
input stall_x;                          // Stall instruction in X stage
input stall_m;                          // Stall instruction in M stage
input [`LM32_WORD_RNG] operand_0;     	// Muliplicand
input [`LM32_WORD_RNG] operand_1;     	// Multiplier

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [`LM32_WORD_RNG] result;       	// Product of multiplication
reg    [`LM32_WORD_RNG] result;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

reg [`LM32_WORD_RNG] muliplicand; 
reg [`LM32_WORD_RNG] multiplier; 
reg [`LM32_WORD_RNG] product; 

/////////////////////////////////////////////////////
// Sequential logic
/////////////////////////////////////////////////////

always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        muliplicand <= #1 {`LM32_WORD_WIDTH{1'b0}};
        multiplier <= #1 {`LM32_WORD_WIDTH{1'b0}};
        product <= #1 {`LM32_WORD_WIDTH{1'b0}};
        result <= #1 {`LM32_WORD_WIDTH{1'b0}};
    end
    else
    begin
        if (stall_x == `FALSE)
        begin    
            muliplicand <= #1 operand_0;
            multiplier <= #1 operand_1;
        end
        if (stall_m == `FALSE)
            product <= #1 muliplicand * multiplier;
        result <= #1 product;
    end
end

endmodule
