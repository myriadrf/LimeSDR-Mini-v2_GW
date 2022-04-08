/****************************************************************************
**
**  Name: MicoGPIO.h 
**
**  Description:
**       Declares GPIO register structure and
**       macros/functions for manipulating GPIO
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
**  3.1   Oct-23-2008  Updated macros to provide for
**                     reading of the interrupt-mask register
**                     and writing to the edge-capture register
**                     Updated comments in the GPIO register
**                     structure definition
**  3.0   Mar-25-2008  Added Header
**
**
**---------------------------------------------------------------------------
*****************************************************************************/

#ifndef MICO32_MICOGPIO_HEADER_FILE
#define MICO32_MICOGPIO_HEADER_FILE

#include "MicoTypes.h"
#include "DDStructs.h"

/****************************************************************************
 * Mico-GPIO driver does not provide any specific user-routine other than   *
 * linking it to GPIO services for lookup capability.                       *
 *--------------------------------------------------------------------------*
 * Mico GPIOs must be located in a non-cached region to use this driver     *
 ****************************************************************************/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /*
     *****************************************
     *****************************************
              GPIO REGISTER MAPPING
     *****************************************
     *****************************************
     */
    typedef struct st_MicoGPIO_t{
        volatile unsigned int data;         /* R/W: 
                                                  R for in-only GPIO,
                                                  W for out-only GPIO,
                                                  R/W for tristates  */
        volatile unsigned int tristate;     /* R/W: 
                                                  tristate enable reg for 
                                                  tristate GPIOs */
        volatile unsigned int irqMask;      /* R/W:
                                                  irq mask for interrupt-
                                                  enabled GPIOs */
        volatile unsigned int edgeCapture;  /* R/W:
                                                  applicable to GPIOs with
                                                  edge-capture ability */
    }MicoGPIO_t;



    /*
     ********************************************
     ********************************************
         MACROS FOR ACCESSING GPIO REGISTERS
     ********************************************
     NOTE: FOR THE MACROS, THE FOLLOWING RULES
           APPLY:
           X is pointer to a valid MicoGPIOCtx_t structure
           Y is unsigned int variable
     */


    /* reads data register */
    #define MICO_GPIO_READ_DATA(X,Y)    \
        (Y)=((volatile MicoGPIO_t *)((X)->base))->data


    /* writes data-register */
    #define MICO_GPIO_WRITE_DATA(X,Y)   \
        ((volatile MicoGPIO_t *)((X)->base))->data=(Y)


    /* reads tristate register */
    #define MICO_GPIO_READ_TRISTATE(X,Y)   \
        (Y) = ((volatile MicoGPIO_t *)((X)->base))->tristate


    /* writes tristate register */
    #define MICO_GPIO_WRITE_TRISTATE(X,Y)  \
        ((volatile MicoGPIO_t *)((X)->base))->tristate = (Y)


    /* reads irq-mask register */
    #define MICO_GPIO_READ_IRQ_MASK(X,Y)  \
        (Y) = ((volatile MicoGPIO_t *)((X)->base))->irqMask


    /* writes irq-mask register */
    #define MICO_GPIO_WRITE_IRQ_MASK(X,Y)  \
        ((volatile MicoGPIO_t *)((X)->base))->irqMask = (Y)


    /* reads edge-capture register */
    #define MICO_GPIO_READ_EDGE_CAPTURE(X,Y)  \
        (Y) = ((volatile MicoGPIO_t *)((X)->base))->edgeCapture


    /* writes edge-capture register */
    #define MICO_GPIO_WRITE_EDGE_CAPTURE(X,Y)  \
        ((volatile MicoGPIO_t *)((X)->base))->edgeCapture = (Y)


/******************************************************************************
 * functions                                                                  *
 ******************************************************************************/

/* initializes Mico32 GPIO peripheral */
void MicoGPIOInit( MicoGPIOCtx_t *ctx );


#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /*MICO32_MICOGPIO_HEADER_FILE */

