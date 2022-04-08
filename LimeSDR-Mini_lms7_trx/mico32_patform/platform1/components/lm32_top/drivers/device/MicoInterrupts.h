/****************************************************************************
**
**  Name: MicoInterrupts.h
**
**  Description:
**        declares functions for manipulating LatticeMico32 processor
**        interrupts (such as registering interrupt handler,
**        enabling/disabling interrupts)
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

#ifndef MICO32_MICOINTERRUPTS_HEADER_FILE
#define MICO32_MICOINTERRUPTS_HEADER_FILE

#include "MicoTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/******************************************************************************
 * Manifest constants                                                         *
 ******************************************************************************
 * If not using higher-levels, reduces the data-allocation (8 bytes/level)    *
 ******************************************************************************/
#ifndef MAX_MICO32_ISR_LEVEL
#define MAX_MICO32_ISR_LEVEL (31)                 /* max. isr level 0 thru 31 */
#endif


/******************************************************************************
 * Data Structures                                                            *
 ******************************************************************************/
/* isr-callback typedef */
typedef void(*ISRCallback)(unsigned int, void *);


/******************************************************************************
 * functions                                                                  *
 *----------------------------------------------------------------------------*
 * Interrupt-levels go from 0 through 31; level 0 corresponds to bit-0 of the *
 * ip register and is the highest software-implemented priority.              *
 ******************************************************************************/
/* initializes interrupt fw-component */
void MicoISRInitialize(void);


/*
 * Registers and de-registers interrupt-handler routine.
 * To register, pass a valid function pointer to the Callback parameter.
 * To deregister, pass 0 as the callback parameter.
 * 
 * Arguments:
 *  unsigned int IntLevel: interrupt line number that your component is
 *      connected to (0 to 31).
 *  void *Context: pointer provided by user that will be passed to the
 *      interrupt-handler callback.
 *  ISRCallback Callback:   User-provided interrupt-handler routine.
 *
 * Return values:
 *      MICO_STATUS_OK if successful.
 */ 
mico_status MicoRegisterISR(unsigned int IntLevel, void *Context, ISRCallback Callback);



/*
 * Disables a specific interrupt
 * 
 * Arguments:
 *  unsigned int Intlevel: interrupt 0 through 31 that needs to
 *      be disabled.
 *
 * Return values:
 *      MICO_STATUS_OK if successful.
 */
mico_status MicoDisableInterrupt(unsigned int IntLevel);


/*
 * Enables a specific interrupt
 * 
 * Arguments:
 *  unsigned int Intlevel: interrupt 0 through 31 that needs to
 *      be enabled.
 *
 * Return values:
 *      MICO_STATUS_OK if successful.
 */ 
mico_status MicoEnableInterrupt(unsigned int IntLevel);


/*
 * Disables all external component interrupts
 * 
 * Arguments:
 *      None.
 *
 * Return values:
 *      unsigned int: 32-bit value that must be
 *          provided when re-enabling all interrupts
 *          for restoration to state that existed prior
 *          to calling this function.
 */
/* Disables all interrupts, returns mask */
unsigned int MicoDisableInterrupts(void);



/*
 * Enables selected interrupts from 0 through 31
 * as indicated by the unsigned int intrMask where
 * bit-0 corresponds to interrupt line 0 and 
 * bit-31 corresponds to interrupt line 31.
 * 
 * Arguments:
 *      None.
 *
 * Return values:
 *      unsigned int: 32-bit value that must be
 *          provided when re-enabling all interrupts
 *          for restoration to state that existed prior
 *          to calling this function.
 */
void MicoEnableInterrupts(unsigned int intrMask);


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif

