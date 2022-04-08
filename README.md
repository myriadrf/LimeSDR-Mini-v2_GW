# LimeSDR-Mini_v2_GW

LimeSDR-Mini v2 (featuring Lattice FPGA) gateware project

## Getting started

**Lattice Diamond project location:**<br>
./LimeSDR-Mini_lms7_trx/proj<br>

**SRAM programming:**<br>
- Open programmer setup file: *./LimeSDR-Mini_bitstreams/impl1_sram.xcf*<br>
- Select .bit file : *./LimeSDR-Mini_bitstreams/lms7_trx_impl1.bit*<br>
- Programm

**FLASH programming:**<br>

First prgram bitstream file to enable background programming:
- Open programmer setup file: *./LimeSDR-Mini_bitstreams/impl1_sram.xcf*;<br>
- Select .bit file : *./LimeSDR-Mini_bitstreams/lms7_trx_preflash_gw.bit*;<br>
- Programm.

Program Flash:
- Open programmer setup file: *./LimeSDR-Mini_bitstreams/impl1_flash_dualboot.xcf*;<br>
- Select .mcs file : *./LimeSDR-Mini_bitstreams/lms7_trx_impl1_dualboot.mcs*;<br>
- Programm.

**Updating FLASH programming file (.mcs) with new bitstream:**<br>

Use saved Deployment Tool setup file *./LimeSDR-Mini_bitstreams/LimeSDR_Mini_DualBoot_Flash_serial_deployment.ddt*<br>
Setup uses two bitstream files:
- Primary - *./LimeSDR-Mini_bitstreams/lms7_trx_impl1.bit*
- Golden  - *./LimeSDR-Mini_bitstreams/lms7_trx_impl1_golden.bit*

**Mico32 platform update:**<br>
In LMS:
- Open platform from ./LimeSDR-Mini_lms7_trx/mico32_patform/platform1/soc/platform1.msb
- Make changes and save 
- Run DRC check *Platform Tools -> Run DRC*
- Regenerate platform files *Platform Tools -> Run Generator*

In Lattice Diamond:
- Manually edit platform1_top.v file if new components are added

**Mico32 SW update:**<br>
In LMS:
- Import project into your workspace from: *./LimeSDR-Mini_lms7_trx/mico32_sw*
- Right click on *Project -> Clean Project*
- Right click on *Project -> Build project*
- Select *Tools -> Software Deploiment...*
- Create new setup by selecting *"Mico32 On Chip Memory deployment"*
- Deploy memory file into: *LimeSDR-Mini_lms7_trx/mico32_sw/lms7_trx/lms7_trx.mem*

In Lattice Diamond:
- Open *Tools -> ECO Editor*
- Select *"Memory Initialization"* tab
- Add generated .mem file to *inst0_nios_cpu/inst_cpu/lm32_inst/ebr/\genblk1.ram*
- Update and Save
- Rerun only *"Bitstream File"* step 
- Reprogram FPGA
