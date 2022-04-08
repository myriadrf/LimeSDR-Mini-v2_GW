/****************************************************************************
**
**  Name: LatticeMico32DbgModule.S
**
**  Description:
**        Implements system-calls for the following operations
**          LatticeMico32DbgWrite:  write-char over LatticeMico32 jtag uart
**          LatticeMico32DbgRead: read-char over LatticeMico32 jtag uart
**          LatticeMico32DbgOpen: open file for LatticeMico32 jtag uart
**          LatticeMico32DbgClose: close file (LatticeMico32 jtag uart)
**
**        All of these calls do a branch to the LatticeMico32 debug
**        monitor and hence these functions require the processor's debug
**        Module enabled in MSB.
**        User does not have to bother calling these functions as
**        LatticeMico32 acts as a file-device and is available for
**        stdio operations including fopen and printfs (in addition to
**        fread, fgets, fscanf etc).
**
**  $Revision: $
**
** Disclaimer:
**
**   This source code is intended as a design reference which
**   illustrates how these types of functions can be implemented.  It
**   is the user's responsibility to verify their design for
**   consistency and functionality through the use of formal
**   verification methods.  Lattice Semiconductor provides no warranty
**   regarding the use or functionality of this code.
**
** --------------------------------------------------------------------
**
**                     Lattice Semiconductor Corporation
**                     5555 NE Moore Court
**                     Hillsboro, OR 97214
**                     U.S.A
**
**                     TEL: 1-800-Lattice (USA and Canada)
**                          (503)268-8001 (other locations)
**
**                     web:   http://www.latticesemi.com
**                     email: techsupport@latticesemi.com
**
** --------------------------------------------------------------------------
**
**  Change History (Latest changes on top)
**
**  Ver    Date        Description
** --------------------------------------------------------------------------
**
**  3.0   Mar-25-2008  Added Header
**
**---------------------------------------------------------------------------
*****************************************************************************/

/*
 * LatticeMico32 system calls.
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

#include <syscall.h>

/* 

        System call convention:
      - System call number in register r8 
      - Return value in r1 ( and r2 only if 64-bit value)
*/

        .extern errno
        .global LatticeMico32DbgWrite
        .type LatticeMico32DbgWrite, @function
LatticeMico32DbgWrite: 
        mvi     r8, SYS_write
        scall   
        mvhi    r4, hi(errno)
        ori     r4, r4, lo(errno)
        sw      (r4+0), r3
        ret
        .size     LatticeMico32DbgWrite, .-LatticeMico32DbgWrite
        .global LatticeMico32DbgRead
        .type LatticeMico32DbgRead, @function
LatticeMico32DbgRead:
        mvi     r8, SYS_read
        scall
        mvhi    r4, hi(errno)
        ori     r4, r4, lo(errno)
        sw      (r4+0), r3
        ret                
        .size     LatticeMico32DbgRead, .-LatticeMico32DbgRead
        .global LatticeMico32DbgOpen
        .type LatticeMico32DbgOpen, @function
LatticeMico32DbgOpen:
        mvi     r8, SYS_open
        scall
        mvhi    r4, hi(errno)
        ori     r4, r4, lo(errno)
        sw      (r4+0), r3
        ret
        .size     LatticeMico32DbgOpen, .-LatticeMico32DbgOpen
        .global LatticeMico32DbgClose
       .type LatticeMico32DbgClose, @function
LatticeMico32DbgClose:    
        mvi     r8, SYS_close
        scall
        mvhi    r4, hi(errno)
        ori     r4, r4, lo(errno)
        sw      (r4+0), r3
        ret                
        .size     LatticeMico32DbgClose, .-LatticeMico32DbgClose

