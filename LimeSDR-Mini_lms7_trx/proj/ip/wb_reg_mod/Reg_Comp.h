#ifndef _REG_COMP_HEADER_FILE_
#define _REG_COMP_HEADER_FILE_

#include "MicoTypes.h"
#include "DDStructs.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* device initialization function */
void init_reg_device( struct st_reg_device * ctx );
/* macro for reading/writing registers */
#define REG_DEV_REGISTER(BASE,OFFSET) \
 *((volatile unsigned int *)(BASE + OFFSET))
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif//_REG_COMP_HEADER_FILE_