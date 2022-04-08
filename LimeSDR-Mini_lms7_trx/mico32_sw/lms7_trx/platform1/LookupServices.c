/****************************************************************************
**
**  Name: Lookupservices.c
**
**  Description:
**        Implements routines for component-instance lookup
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

#include "LookupServices.h"
#include "MicoTypes.h"
#include "MicoUtils.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /******************************************************************************
     * data memebers                                                              *
     ******************************************************************************/
    static DeviceReg_t *s_DeviceRegListHead = 0;


    /* find a device by name */
    static DeviceReg_t *MicoFindDeviceByName(const char *deviceName)
    {
        DeviceReg_t *pReg;
        pReg = s_DeviceRegListHead;

        if(pReg == (DeviceReg_t *)0){
            return((DeviceReg_t *)0);
        }else{
            /* see if the given service-name exists */
            do{
                if(MicoStrCompare(deviceName, pReg->name) == 0)
                    return(pReg);
                pReg = (DeviceReg_t *)pReg->next;
            }while(pReg != s_DeviceRegListHead);
        }

        /* none found */
        return((DeviceReg_t *)0);
    }


    /* initializes lookup service */
    static void InitMicoLookupService(void)
    {
        static int s_ServicesInitialized = 0;

        if(s_ServicesInitialized == 0){
            s_ServicesInitialized = 1;
            s_DeviceRegListHead = (DeviceReg_t *)0;
        }
    }


#ifdef __cplusplus
}
#endif /* __cplusplus */



/* used for registering a device */
unsigned int MicoRegisterDevice( DeviceReg_t *pDevReg )
{
    InitMicoLookupService();

    /* if the device-name is already registered, return a failure */
    if(MicoFindDeviceByName(pDevReg->name) != (DeviceReg_t *)0){
        return(1);
    }

    /* append this device to the tail */
    if(s_DeviceRegListHead == (DeviceReg_t *)0){
        s_DeviceRegListHead = pDevReg;
        s_DeviceRegListHead->next = pDevReg;
        s_DeviceRegListHead->prev = pDevReg;
        return(0);
    }

    pDevReg->prev = s_DeviceRegListHead->prev;
    ((DeviceReg_t*)pDevReg->prev)->next = pDevReg;
    s_DeviceRegListHead->prev = pDevReg;
    pDevReg->next = s_DeviceRegListHead;

    return(0);
}


/* finds a device that's registered, using device's registered name */
void *MicoGetDevice(const char *name)
{
    DeviceReg_t *regInfo;


    regInfo = MicoFindDeviceByName(name);

    if(regInfo != (DeviceReg_t *)0){
        return(regInfo->priv);
    }else{
        return((void *)0);
    }
}


/* finds first device of a given type */
void *MicoGetFirstDev(const char *deviceType, DevFindCtx_t *FindCtx)
{
    if(FindCtx == (DevFindCtx_t *)0)
        return((void *)0);


    /* save find-context information */
    FindCtx->name = deviceType;
    FindCtx->head = s_DeviceRegListHead;
    FindCtx->curr = s_DeviceRegListHead;


    /* if doing a non-named find, return the first element in the list */
    if(FindCtx->name == (char *)0)
        return((void *)((FindCtx->curr)->priv));


    /* if finding first device of a specific type, return the first that is found */
    do{
        if(MicoStrCompare(FindCtx->name, FindCtx->curr->deviceType) == 0)
            return((void *)(FindCtx->curr->priv));

        FindCtx->curr = (DeviceReg_t*)(FindCtx->curr->next);

    }while(FindCtx->curr != FindCtx->head);


    /* none found, so set the head to null */
    FindCtx->head = (DeviceReg_t *)0;
    FindCtx->curr = FindCtx->head;

    /* all done */
    return((void *)0);
}


/* finds next device of a given type */
void *MicoGetNextDev(DevFindCtx_t *FindCtx)
{
    if(FindCtx == (DevFindCtx_t *)0)
        return((void *)0);

    /* if head or curr is null, return */
    if( (FindCtx->curr == (void *)0) || (FindCtx->head == (void *)0) )
        return((void *)0);


    /* find another ... */
    FindCtx->curr = (DeviceReg_t*)(FindCtx->curr->next);
    if(FindCtx->curr == FindCtx->head)
        return((void *)0);


    /* if fetching all devices i.e. deviceType is not specified .. */
    if(FindCtx->name == (char *)0)
        return((void *)(FindCtx->curr->priv));


    /* find the next device that matches the device-type */
    do{
        if(MicoStrCompare(FindCtx->name, FindCtx->curr->deviceType) == 0)
            return((void *)(FindCtx->curr->priv));

        FindCtx->curr = (DeviceReg_t *)(FindCtx->curr->next);
    }while(FindCtx->curr != FindCtx->head);


    /* none found, so set the head to null */
    FindCtx->head = (DeviceReg_t *)0;
    FindCtx->curr = FindCtx->head;

    return((void *)0);
}

