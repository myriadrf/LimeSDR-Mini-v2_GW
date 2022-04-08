#ifndef _FIFO_COMP_HEADER_FILE_
#define _FIFO_COMP_HEADER_FILE_

#include "DDStructs.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* device initialization function */
void init_fifo_device( struct st_fifo_device * ctx );

/* macro for reading/writing registers */
#define FIFO_DEV_REGISTER(BASE,OFFSET) \
 *((volatile unsigned int *)(BASE + OFFSET))
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif//_FIFO_COMP_HEADER_FILE_