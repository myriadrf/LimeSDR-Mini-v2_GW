#Launch script using powershell
#C:\lscc\diamond\3.12\bin\nt64\fpgac.exe .\mico32_mem_init.tcl

eco_design open -ncd {C:/lattice/LimeSDR-Mini_GW/LimeSDR-Mini_lms7_trx/proj/impl1/lms7_trx_impl1.ncd} -prf {C:/lattice/LimeSDR-Mini_GW/LimeSDR-Mini_lms7_trx/proj/impl1/lms7_trx_impl1.lpf}
eco_config memebr -instance {inst0_nios_cpu/inst_cpu/lm32_inst/ebr/\genblk1.ram} -init_all no -mem {C:/lattice/LimeSDR-Mini_GW/LimeSDR-Mini_lms7_trx/mico32_sw/lms7_trx/lms7_trx.mem} -format hex -init_data static -module {pmi_ram_dpEhnonessen3213819232138192p13822039} -mode {RAM_DP} -depth {8192} -widtha {32} -widthb {32}
eco_design save -ncd {C:/lattice/LimeSDR-Mini_GW/LimeSDR-Mini_lms7_trx/proj/impl1/lms7_trx_impl1.ncd}
eco_design close
