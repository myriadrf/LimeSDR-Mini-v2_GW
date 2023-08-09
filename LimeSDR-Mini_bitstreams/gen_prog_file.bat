@echo off
title LimeSDR-MINI v2 bitstream generation

:: User editable Variables begin 
set device=LFE5U-45F
set bit_file_name=lms7_trx_impl1.bit
set golden_bit_file_name=lms7_trx_impl1_golden.bit
set alternate_bit_file_name=lms7_trx_impl1.bit
set mcs_output_file_name=lms7_trx_impl1_dualboot.mcs
set impl_dir=..\LimeSDR-Mini_lms7_trx\proj\impl1\
:: User editable Variables end 


:: Creating required variables for ddtcmd 
set ddtcmd_device="%device%"
set ddtcmd_if="%~dp0%bit_file_name%"
set ddtcmd_golden="%~dp0%golden_bit_file_name%"
set ddtcmd_altfile="%~dp0%alternate_bit_file_name%"
set ddtcmd_of="%~dp0%mcs_output_file_name%"

set generated_bit_file=%~dp0%impl_dir%%bit_file_name%

:: Copy required files
echo [36mCopying .bit file from project directory[0m
xcopy %generated_bit_file% %~dp0

:: Launch Lattice ddtcmd tool to create memory configuration programming file
echo [36mLaunching Lattice Diamond Deployment Tool 3.12, make sure that it is added to Windows path[0m
ddtcmd -oft -advanced -dev %ddtcmd_device% -if %ddtcmd_if% -format int -flashsize 128 -golden %ddtcmd_golden% -goldenadd 0x00140000 -multi 1 -altfile %ddtcmd_altfile% -address 0x00280000 -next prim -of %ddtcmd_of%

pause