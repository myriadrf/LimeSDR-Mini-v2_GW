#-- Lattice Semiconductor Corporation Ltd.
#-- Synplify OEM project file

#device options
set_option -technology ECP5U
set_option -part LFE5U_45F
set_option -package MG285C
set_option -speed_grade -8

#compilation/mapping options
set_option -symbolic_fsm_compiler true
set_option -resource_sharing true

#use verilog 2001 standard option
set_option -vlog_std v2001

#map options
set_option -frequency 200
set_option -maxfan 1000
set_option -auto_constrain_io 0
set_option -disable_io_insertion false
set_option -retiming false; set_option -pipe true
set_option -force_gsr false
set_option -compiler_compatible 0
set_option -dup 1

add_file -constraint {D:/darbas/lattice/orange_crab/lm32_tutor/timing_sdc.sdc}
set_option -default_enum_encoding default

#simulation options


#timing analysis options



#automatic place and route (vendor) options
set_option -write_apr_constraint 1

#synplifyPro options
set_option -fix_gated_and_generated_clocks 1
set_option -update_models_cp 0
set_option -resolve_multiple_driver 0


set_option -seqshift_no_replicate 0

#-- add_file options
set_option -hdl_define -set SBP_SYNTHESIS
set_option -include_path {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/soc}
set_option -include_path {D:/darbas/lattice/orange_crab/lm32_tutor}
add_file -verilog {C:/lscc/diamond/3.12/cae_library/synthesis/verilog/pmi_def.v}
add_file -verilog {C:/lscc/diamond/3.12/module/reveal/src/ertl/ertl.v}
add_file -verilog {C:/lscc/diamond/3.12/module/reveal/src/rvl_j2w_module/rvl_j2w_module.v}
add_file -verilog {C:/lscc/diamond/3.12/module/reveal/src/rvl_j2w_module/wb2sci.v}
add_file -verilog {C:/lscc/diamond/3.12/module/reveal/src/ertl/JTAG_SOFT.v}
add_file -verilog -vlog_std v2001 {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/reveal_workspace/tmpreveal/top_la0_trig_gen.v}
add_file -verilog -vlog_std v2001 {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/reveal_workspace/tmpreveal/top_la0_gen.v}
add_file -verilog -vlog_std v2001 {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/reveal_workspace/tmpreveal/top_rvl_top.v}
add_file -vhdl -lib "work" {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/reveal_workspace/tmpreveal/top_rvl_top.vhd}

#-- top module name
set_option -top_module top

#-- set result format/file last
project -result_file {D:/darbas/lattice/orange_crab/lm32_tutor/platform1/platform1_platform1.edi}

#-- error message log file
project -log_file {platform1_platform1.srf}

#-- set any command lines input by customer


#-- run Synplify with 'arrange HDL file'
project -run hdl_info_gen -fileorder
project -run -clean
