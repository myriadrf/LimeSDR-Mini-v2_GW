# Get script path
set script_path [ file dirname [ file normalize [ info script ] ] ]
puts $script_path

# User editable variables
set device                    LFE5U-45F
set bit_file_name             lms7_trx_impl1.bit
set golden_bit_file_name      lms7_trx_impl1_golden.bit
set alternate_bit_file_name   lms7_trx_impl1.bit
set mcs_output_file_name      lms7_trx_impl1_dualboot.mcs
set impl_dir                  ../LimeSDR-Mini_lms7_trx/proj/impl1

puts "Copying .bit file from project directory"
file copy -force [file join $script_path $impl_dir $bit_file_name] [file join $script_path $bit_file_name]

#Creating required variables for ddtcmd 
set ddtcmd_device    $device
set ddtcmd_if        [file join $script_path $bit_file_name]
set ddtcmd_golden    [file join $script_path $golden_bit_file_name]
set ddtcmd_altfile   [file join $script_path $alternate_bit_file_name]
set ddtcmd_of        [file join $script_path $mcs_output_file_name]


# Execute Lattice Diamond Deployment Tool
puts "Launching Lattice Diamond Deployment Tool 3.12, make sure that it is added to system path"
exec ddtcmd -oft -advanced -dev $ddtcmd_device -if $ddtcmd_if -format int -flashsize 128 -golden $ddtcmd_golden -goldenadd 0x00140000 -multi 1 -altfile $ddtcmd_altfile -address 0x00280000 -next prim -of $ddtcmd_of