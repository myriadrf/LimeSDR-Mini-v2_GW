/****************************************************************************
**
**  Name: MicoSbrk.c
**
**  Description:
**        Implements _sbrk that is called by malloc (Newlib C library)
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

#include <sys/errno.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void* _sbrk(ptrdiff_t incr)
{
   extern char   end; /* Set by linker.  */
   static char * heap_end; 
   char *        prev_heap_end; 

   if (heap_end == 0)
     heap_end = & end; 

   prev_heap_end = heap_end; 
   heap_end += incr; 

   return ((void *) prev_heap_end);
}

#ifdef __cplusplus
}
#endif

