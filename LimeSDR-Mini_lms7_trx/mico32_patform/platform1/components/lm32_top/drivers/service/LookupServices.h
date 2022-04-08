/****************************************************************************
**
**  Name: LookupServices.h
**
**  Description:
**       Declares prototypes for lookup service functions
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

#ifndef MICO32_LOOKUP_SERVICES_HEADER_FILE
#define MICO32_LOOKUP_SERVICES_HEADER_FILE


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
 * Data Structures/types                                                      *
 ******************************************************************************/

    /* device-registration structure */
    typedef struct DeviceReg_st{
        const char *name;           /* name of the device                   */
        const char *deviceType;     /* device-type                          */
        void *priv;                 /* device-specific context information  */
        void *prev;                 /* USED BY LATTICEMIOC32 LOOKUP IMPL.   */
        void *next;                 /* USED BY LATTICEMICO32 LOOKUP IMPL.   */
    }DeviceReg_t;


    /* defines device-find context */
    typedef struct DevFind_st{
        const char   *name;    /* name of device-type being searched    */
        DeviceReg_t *head;     /* search start-point                    */
        DeviceReg_t *curr;     /* currently indexed device              */
    }DevFindCtx_t;



/******************************************************************************
 * functions                                                                  *
 ******************************************************************************/

    /* Function for registering a device for lookup-by-name.
     * Arguments:
     *   DeviceReg_t *pDevReg: Pointer to a valid allocation of 
     *      DeviceReg_t structure.  This must remain valid
     *      and should not be modified for the duration of the
     *      application.
     */
    unsigned int MicoRegisterDevice( DeviceReg_t *pDevReg );


    /*
     * Finds a device (that's registered with a registered-service)
     * Arguments:
     *      const char *Name: pointer to a character string
     *      representing device-name (case sensitive)
     * Returns:
     *  void *: pointer to the looked-up device's 
     *          instance-specific information.  Will be 0 if
     *          no device with matching name is found.
     */
    void *MicoGetDevice(const char *Name);


    /*
     * Finds the first device (that's registered) of the
     * specified type
     * Arguments:
     *   const char *deviceType : points to named device-type
     *          If this pointer is a null-pointer, the first
     *          context of the first device in the list of
     *          registered devices is returned, irrespective of
     *          the type under which the device is registered.
     *          If non-null pointer, it must point to a valid
     *          string (case-sensitive).
     *
     *   DevFind_st *FindCtx: pointer to a valid allocation of 
     *           DevFind_st that will be referenced by
     *           MicoGetFirstDev for future invocations to
     *           MicoGetNextDev
     * Returns:
     *
     *    void *: pointer to device context (is null if no matching
     *            device is found).
     */
     void *MicoGetFirstDev(const char *deviceType, DevFindCtx_t *FindCtx);


    /*
     * Finds the next registered device that matches the 
     * find-criteria provided in the prior MicoGetFirstDev
     * invocation.
     *
     * Arguments:
     *   DevFind_st *FindCtx: pointer to a valid allocation of 
     *           DevFind_st that was provided to MicoGetFirstDev
     *           invocation.  Caller must not modify the structure
     *           referenced by this pointer.
     *
     * Returns:
     *
     *    void *: pointer to device context (is null if no matching
     *            device is found).
     */
     void *MicoGetNextDev(DevFindCtx_t *FindCtx);


#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* MICO32_LOOKUP_SERVICES_HEADER_FILE */

