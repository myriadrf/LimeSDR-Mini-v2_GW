/****************************************************************************
**
**  Name:  MicoInterrupts.c
**
**  Description:
**     Implements functions for enabling/disabling/registering
**     LatticeMico32 processor interupts.
**
**     Provides default framework handler for handling LatticeMico32
**     processor interrupts and calls appropriate registered callback
**     functions
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
** --------------------------------------------------------------------
**
** REVISION HISTORY
**
** --------------------------------------------------------------------
**   DATE        VERSION          DESCRIPTION
**
** Mar-25-2008     3.0       Sync'd version to 3.0 for MicoSystem release 7.1
** 
** Nov 27, 07      1.1       Modified original implementation
**                           so that im is re-read after servicing
**                           an interrupt, inside the while(1)
**                           rather than being read just once outside
**                           of the servicing loop.  This allows ISR
**                           routines to disable interrupts locally.
**
*****************************************************************************/
#include "MicoInterrupts.h"

/******************************************************************************
 * Local data structures                                                      *
 ******************************************************************************/

    /* isr entry struct */
    typedef struct st_ISREntry{
        ISRCallback Callback;
        void *Context;
    }ISREntry_t;

#ifndef __MICO_NO_INTERRUPTS__
    /* ISREntry table */
    static ISREntry_t ISREntryTable[MAX_MICO32_ISR_LEVEL+1];
#endif


/******************************************************************************
 * Interrupt Handler (invoked by low-level routine)                           *
 * Services interrupts based on priority                                      *
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

volatile static unsigned int s_uiInterruptContext = 0;

void MicoISRHandler(void){
    /*
     * If an interrupt-handler exists for the relevant interrupt (as detected
     * from ip and im cpu registers), then invoke the handler else disable the
     * interrupt in the im.
     */
    unsigned int ip, im, Mask, IntLevel;

    s_uiInterruptContext = 1;

    do {
        /* read ip and im and calculate effective ip */
        asm volatile ("rcsr %0,im":"=r"(im));
        asm volatile ("rcsr %0,ip":"=r"(ip));

        ip &= im;
        Mask = 0x1;
        IntLevel = 0;

        if( ip!=0 ){

            do{
                if(Mask & ip){
#ifndef __MICO_NO_INTERRUPTS__
                    if(ISREntryTable[IntLevel].Callback != 0){
                        (ISREntryTable[IntLevel].Callback)(IntLevel, ISREntryTable[IntLevel].Context);
                        asm volatile ("wcsr ip, %0"::"r"(Mask));
                        break;
                    }
                    else
#endif
                    {
                        asm volatile ("rcsr %0,im":"=r"(im));
                        im &= ~Mask;
                        asm volatile ("wcsr im, %0"::"r"(im));
                        asm volatile ("wcsr ip, %0"::"r"(Mask));
                        break;
                    }
                }
                Mask <<= 0x1;
                ++IntLevel;
            }while(1);

        }else{
            break;
        }

    }while(1);

    s_uiInterruptContext = 0;

    /* all done */
    return;
}

#ifdef __cplusplus
};
#endif /* __cplusplus */

/******************************************************************************
 *                                                                            *
 * Initializes interrupt-framework                                            *
 *                                                                            *
 ******************************************************************************/
#ifndef __MICO_NO_INTERRUPTS__
void
MicoISRInitialize(void){
    static int sui_Initialized = 0;
    unsigned int i, ie;

    if(sui_Initialized == 0) {
        // indicate we're done initializing
        sui_Initialized = 1;

        /* initialize ISR table */
        for(i = 0; i < (MAX_MICO32_ISR_LEVEL+1); i++){
            ISREntryTable[i].Callback = 0;
            ISREntryTable[i].Context  = 0;
        }

        /* register general interrupt-handler */
        /* _regintr(MicoISRHandler); */

        /* enable interrupts */
        asm volatile ("rcsr %0,ie":"=r"(ie));
        ie |= 0x1;
        asm volatile ("wcsr ie, %0"::"r"(ie));
    }
    return;
}


/******************************************************************************
 * Registers ISR callback-routine                                             *
 * Only a single source (of 32) can be registered for.                        *
 * Max IntNum is determined by MAX_ISR_LEVEL and its value goes from 0->n-1   *
 ******************************************************************************/
mico_status
MicoRegisterISR(unsigned int IntNum, void *Context, ISRCallback Callback){
    unsigned int Mask;
    unsigned int ie, im;

    /* if interrupt-service has't been initialized, initialize it */
    MicoISRInitialize();

    if(IntNum > MAX_MICO32_ISR_LEVEL)
        return(MICO_STATUS_E_INVALID_PARAM);

    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = 0x1 << IntNum;

    /* disable peripheral interrupts */
    asm volatile ("rcsr %0,ie":"=r"(ie));
    ie &= (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(ie));

    ISREntryTable[IntNum].Callback = Callback;
    ISREntryTable[IntNum].Context = Context;

    /* mask/unmask bit in the im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im = (Callback == 0)?(im & ~Mask) : (im | Mask);
    asm volatile ("wcsr im, %0"::"r"(im));

    /* enable interrupts - if not in interrupt-context */
    if(s_uiInterruptContext == 0){
        ie |= 0x1;
        asm volatile ("wcsr ie, %0"::"r"(ie));
    }

    /* all done */
    return(MICO_STATUS_OK);
}



/******************************************************************************
 * Disables a specific interrupt                                              *
 *----------------------------------------------------------------------------*
 * Inputs:                                                                    *
 *     unsigned int IntNum: Interrupt-level                                   *
 * Outputs:                                                                   *
 * Return values:                                                             *
 *            MICO_STATUS_E_INVALID_PARAM                                     *
 *            MICO_STATUS_OK                                                  *
 ******************************************************************************/
mico_status
MicoDisableInterrupt(unsigned int IntNum){
    unsigned int ie, im;
    unsigned int Mask;

    if(IntNum > MAX_MICO32_ISR_LEVEL)
        return(MICO_STATUS_E_INVALID_PARAM);

    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = ~(0x1 << IntNum);

    /* disable peripheral interrupts in case they were enabled */
    asm volatile ("rcsr %0,ie":"=r"(ie));
    ie &= (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(ie));

    /* disable mask-bit in im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im &= Mask;
    asm volatile ("wcsr im, %0"::"r"(im));

    /* enable ie - if not in interrupt-context */
    if(s_uiInterruptContext == 0){
        ie |= 0x1;
        asm volatile ("wcsr ie, %0"::"r"(ie));
    }

    /* all done */
    return(MICO_STATUS_OK);
}


/******************************************************************************
 * Enables a specific interrupt                                               *
 * ---------------------------------------------------------------------------*
 * Inputs:                                                                    *
 *     unsigned int IntNum: Interrupt-level                                   *
 * Outputs:                                                                   *
 * Return values:                                                             *
 *            MICO_STATUS_E_INVALID_PARAM                                     *
 *            MICO_STATUS_OK                                                  *
 ******************************************************************************/
mico_status
MicoEnableInterrupt(unsigned int IntNum){
    unsigned int ie, im;
    unsigned int Mask;

    /* if interrupt-service has't been initialized, initialize it */
    MicoISRInitialize();

    if(IntNum > MAX_MICO32_ISR_LEVEL)
        return(MICO_STATUS_E_INVALID_PARAM);


    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = (0x1 << IntNum);

    /* disable peripheral interrupts in-case they were enabled*/
    asm volatile ("rcsr %0,ie":"=r"(ie));
    ie &= (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(ie));

    /* enable mask-bit in im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im |= Mask;
    asm volatile ("wcsr im, %0"::"r"(im));

    /* enable ie - if not in interrupt context */
    if(s_uiInterruptContext == 0){
        ie |= 0x1;
        asm volatile ("wcsr ie, %0"::"r"(ie));
    }

    /* all done */
    return(MICO_STATUS_OK);
}


/******************************************************************************
 * Enables interrupts                                                         *
 ******************************************************************************/
void MicoEnableInterrupts(unsigned int mask)
{
    unsigned int ie;

    /* if interrupt-service has't been initialized, initialize it */
    MicoISRInitialize();

    /*
     * Disable peripheral interrupts in case
     * interrupts were enabled.
     */
    /* disable peripheral interrupts */
    asm volatile ("rcsr %0,ie":"=r"(ie));
    ie &= (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(ie));


    /* set im */
    asm volatile ("wcsr im, %0"::"r"(mask));

    /* re-enable peripheral interrupts - if not in interrupt-context */
    if(s_uiInterruptContext == 0){
        ie |= 0x1;
        asm volatile ("wcsr ie, %0"::"r"(ie));
    }

}


/******************************************************************************
 * Disables interrupts                                                        *
 ******************************************************************************/
unsigned int MicoDisableInterrupts(void)
{
    unsigned int ie, im;

    /* disable peripheral interrupts */
    asm volatile ("rcsr %0,ie":"=r"(ie));
    ie &= (~0x1);
    asm volatile ("wcsr ie, %0"::"r"(ie));

    /* read the interrupt-mask */
    asm volatile ("rcsr %0, im":"=r"(im));

    /* set the interrupt mask to zero */
    asm volatile ("wcsr im, %0"::"r"(0x0));

    /* 
     * Don't bother enabling the ie as one of the
     * enable-interrupt instructions will take care
     * of it
     */

    /* return the interrupt-mask */
    return(im);
}

#endif

