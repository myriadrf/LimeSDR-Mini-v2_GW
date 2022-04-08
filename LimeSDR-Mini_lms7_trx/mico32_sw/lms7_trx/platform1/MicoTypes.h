/****************************************************************************
**
**  Name: MicoTypes.h
**
**  Description:
**        Declares legacy return-codes, used in MicoInterrupts.c
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

#ifndef MICO_MICOTYPES_HEADER_FILE
#define MICO_MICOTYPES_HEADER_FILE


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Mico Status */
typedef enum ENUM_MICO_STATUS {
    MICO_STATUS_OK              = 0,
    MICO_STATUS_E_FAIL          = -1,
    MICO_STATUS_E_INVALID_PARAM = -2,
    MICO_STATUS_E_SERVICE_EXISTS = -3
}mico_status;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

