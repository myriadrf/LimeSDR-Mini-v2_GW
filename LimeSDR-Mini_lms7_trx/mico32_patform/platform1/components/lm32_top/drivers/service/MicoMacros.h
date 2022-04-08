/****************************************************************************
**
**  Name: MicoMacros.h
**
**  Description:
**        defines macros for converting milliseconds and microsecond
**        values into CPU ticks based on the processor frequency.
**        The processor frequency is retrieved from DDStructs.h
**        auto-generated file that is dependent on the platform that
**        defines CPU frequency
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

#ifndef MICO32_MICOMACROS_HEADER_FILE
#define MICO32_MICOMACROS_HEADER_FILE

/* 
 * There is a dependency on system-header file for
 * fetching information on the CPU clock as it is
 * needed for the macros in here.
 */
#include "DDStructs.h"

/*
 ****************************************************
 * This file defines macros used in the framework
 * as well as the device-driver files
 ****************************************************
 */

/*
 * MACROS FOR TIME-CONVERSION
 */
#define MILLISECONDS_TO_TICKS(X_MS)                     \
    (X_MS*(MICO32_CPU_CLOCK_MHZ/1000))


#define MICROSECONDS_TO_TICKS(X_MICROSECS)              \
    (MILLISECONDS_TO_TICKS(X_MICROSECS)/1000)


#endif /*MICO32_MICOMACROS_HEADER_FILE */

