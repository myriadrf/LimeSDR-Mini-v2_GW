# LimeSDR Mini v2 Gateware

LimeSDR-Mini v2 (featuring Lattice FPGA) gateware project

## General

**Required tools and versions:**
- Lattice Diamond 3.12 + SP1 
- LatticeMico System for Diamond 3.12

**Lattice Diamond project location:**<br>
./LimeSDR-Mini_lms7_trx/proj<br>

## Compiling and bitstream generation

There is no need to recompile Mico32 if changes are being done only in FPGA logic side. 
If there is a need to modify Mico32 platform or software follow steps below, otherwise skip to **Compiling FPGA gateware** part.


### Compiling Mico32 softcore CPU

**Import Mico32 project (has to be done once after cloning repo):**<br>
- Open LatticeMico System for Diamond (LMS)
- Import project *File->Import...->Existing Project into Workspace* from: *./LimeSDR-Mini_lms7_trx/mico32_sw*

**Regenerate Mico32 platform:**<br> 
- Select platform editing ***LMS1.0 D3.11*** perspective 
- Open platform from *./LimeSDR-Mini_lms7_trx/mico32_patform/platform1/soc/platform1.msb*
- Run DRC check *Platform Tools -> Run DRC*
- Regenerate platform files *Platform Tools -> Run Generator*
- Manually edit *platform1_top.v* file if new components are added or other changes are made

**Compile Mico32 SW:**<br>
- Select software editing ***C/C++*** perspective
- Right click on *Project -> Clean Project*
- Right click on *Project -> Build project*
- Select *Tools -> Software Deployment...*
- Create new setup by selecting *"Mico32 On Chip Memory deployment"*
- Deploy memory file into: *./LimeSDR-Mini_lms7_trx/mico32_sw/lms7_trx/lms7_trx.mem*

### Compiling FPGA gateware
- Open Lattice Diamond software and open *File->Open->Project...* project file from *./LimeSDR-Mini_lms7_trx/proj/lms7_trx.ldf*
- Recompile project right click on *Bitstram file->Rerun All* in *Process* tab
- Open *Tools -> ECO Editor* and Select *"Memory Initialization"* tab
- Add *LimeSDR-Mini_lms7_trx/mico32_sw/lms7_trx/lms7_trx.mem* file to *inst0_cpu/inst_cpu/lm32_inst/ebr/\genblk1.ram*
- Update and Save
- Rerun only *"Bitstream File"* step 
- Reprogram FPGA

## Board programming

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

