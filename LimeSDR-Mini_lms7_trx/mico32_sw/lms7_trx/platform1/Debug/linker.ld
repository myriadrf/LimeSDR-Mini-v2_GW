/*
 * Link script for LatticeMico32 programs.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



OUTPUT_FORMAT("elf32-lm32")
ENTRY(_start)
INPUT(crti.o crtbegin.o crtend.o crtn.o)
/*
 * This section defines memory attributes (name, origin, length) for the platform
 */
MEMORY
{
    ebr :  ORIGIN = 0x08000000,  LENGTH = 32768
    SPIFlash_S :  ORIGIN = 0x10000000,  LENGTH = 134217728
    SPIFlash_C :  ORIGIN = 0x80000000,  LENGTH = 2048
}



SECTIONS
{

  /* code */
  .boot : { *(.boot) } > ebr
  .text : 
  {
     . = ALIGN(4);
     _ftext = .;
     _ftext_rom = LOADADDR(.text);
     . = . + 4;
     KEEP (*(.vector))
     *(.text .stub .text.* .gnu.linkonce.t.*)
     *(.gnu.warning)
     KEEP (*(.init))
     KEEP (*(.fini))


     /* Exception handlers */
     *(.eh_frame_hdr)
     KEEP (*(.eh_frame))
     *(.gcc_except_table)


     /* Constructors and destructors */
     KEEP (*crtbegin*.o(.ctors))
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
     KEEP (*(SORT(.ctors.*)))
     KEEP (*(.ctors))
     KEEP (*crtbegin*.o(.dtors))
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
     KEEP (*(SORT(.dtors.*)))
     KEEP (*(.dtors))
     KEEP (*(.jcr))
    _etext = .;
  } > ebr =0 



  /* read-only data */
  .rodata : 
  { 
     . = ALIGN(4);
     _frodata = .;
     _frodata_rom = LOADADDR(.rodata);
     . = . + 4;
     *(.rodata .rodata.* .gnu.linkonce.r.*) 
     *(.rodata1)
     _erodata = .;
  } > ebr



  /* read/write data */
  .data : 
  {
     . = ALIGN(4);
     _fdata = .;
     _fdata_rom = LOADADDR(.data);
     . = . + 4;
     *(.data .data.* .gnu.linkonce.d.*)
     *(.data1)
     SORT(CONSTRUCTORS)
     _gp = ALIGN(16) + 0x7ff0;
     *(.sdata .sdata.* .gnu.linkonce.s.*)
     *(.gcc_except_table.*)
     _edata = .;
  } > ebr 



  /* bss */
  .bss : 
  { 
     . = ALIGN(4);
     _fbss = .;
     *(.dynsbss)
     *(.sbss .sbss.* .gnu.linkonce.sb.*)
     *(.scommon)
     *(.dynbss)
     *(.bss .bss.* .gnu.linkonce.b.*)
     *(COMMON)
     . = ALIGN(4);
     _ebss = .;
     _end = .;
     PROVIDE (end = .);
  } > ebr 



  /* first location in stack is highest address in ram */
  PROVIDE(_fstack = ORIGIN(ebr) + LENGTH(ebr) - 4);



  /* stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }



  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}

