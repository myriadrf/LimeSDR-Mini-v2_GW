#ifndef LATTICE_DDINIT_HEADER_FILE
#define LATTICE_DDINIT_HEADER_FILE
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
#include "LookupServices.h"
/* platform frequency in MHz */
#define MICO32_CPU_CLOCK_MHZ (77500000)

/*Device-driver structure for lm32_top*/
#define LatticeMico32Ctx_t_DEFINED (1)
typedef struct st_LatticeMico32Ctx_t {
    const char*   name;
} LatticeMico32Ctx_t;


/* lm32_top instance LM32*/
extern struct st_LatticeMico32Ctx_t lm32_top_LM32;

/* declare LM32 instance of lm32_top */
extern void LatticeMico32Init(struct st_LatticeMico32Ctx_t*);


/*Device-driver structure for spi_flash*/
#define MicoSPIFlashCtx_t_DEFINED (1)
typedef struct st_MicoSPIFlashCtx_t {
    const char*   name;
    unsigned int   memory_base;
    unsigned int   memory_size;
    unsigned int   memory_wbSize;
    unsigned int   control_base;
    unsigned int   control_port;
    unsigned int   control_wbSize;
    unsigned int   program_buf_en;
    unsigned int   read_buf_en;
    unsigned int   page_pgm_buff_ebr;
    unsigned int   page_pgm_buff_dist_ram;
    unsigned int   page_rd_buff_ebr;
    unsigned int   page_rd_buff_dist_ram;
    unsigned int   page_size;
    unsigned int   sector_size;
    DeviceReg_t   lookupReg;
    void *   prev;
    void *   next;
} MicoSPIFlashCtx_t;


/* spi_flash instance SPIFlash_*/
extern struct st_MicoSPIFlashCtx_t spi_flash_SPIFlash_;

/* declare SPIFlash_ instance of spi_flash */
extern void MicoSPIFlash_Initialize(struct st_MicoSPIFlashCtx_t*);


/*Device-driver structure for gpio*/
#define MicoGPIOCtx_t_DEFINED (1)
typedef struct st_MicoGPIOCtx_t {
    const char*   name;
    unsigned int   base;
    unsigned int   intrLevel;
    unsigned int   output_only;
    unsigned int   input_only;
    unsigned int   in_and_out;
    unsigned int   tristate;
    unsigned int   data_width;
    unsigned int   input_width;
    unsigned int   output_width;
    unsigned int   intr_enable;
    unsigned int   wb_data_size;
    DeviceReg_t   lookupReg;
    void *   prev;
    void *   next;
} MicoGPIOCtx_t;


/* gpio instance LED*/
extern struct st_MicoGPIOCtx_t gpio_LED;

/* declare LED instance of gpio */
extern void MicoGPIOInit(struct st_MicoGPIOCtx_t*);


/* gpio instance GPIO*/
extern struct st_MicoGPIOCtx_t gpio_GPIO;

/* declare GPIO instance of gpio */
extern void MicoGPIOInit(struct st_MicoGPIOCtx_t*);


/*Device-driver structure for Reg_Comp*/
#define reg_device_DEFINED (1)
typedef struct st_reg_device {
    unsigned int   reg_08_value;
    unsigned int   b_addr;
} reg_device;


/* Reg_Comp instance GPO*/
extern struct st_reg_device Reg_Comp_GPO;

/* declare GPO instance of Reg_Comp */
extern void init_reg_device(struct st_reg_device*);


/*Device-driver structure for spi*/
#define MicoSPICtx_t_DEFINED (1)
typedef struct st_MicoSPICtx_t {
    const char *   name;
    unsigned int   base;
    unsigned int   intrLevel;
    unsigned int   master;
    unsigned int   slaves;
    unsigned int   dataLength;
    unsigned int   shiftDir;
    DeviceReg_t   lookupReg;
    unsigned int   control;
    void *   onRx;
    void *   onTx;
    void *   prev;
    void *   next;
} MicoSPICtx_t;


/* spi instance spi*/
extern struct st_MicoSPICtx_t spi_spi;

/* declare spi instance of spi */
extern void MicoSPIInit(struct st_MicoSPICtx_t*);


/*Device-driver structure for FIFO_Comp*/
#define fifo_device_DEFINED (1)
typedef struct st_fifo_device {
    unsigned int   reg_16_value;
    unsigned int   b_addr;
    DeviceReg_t   lookupReg;
} fifo_device;


/* FIFO_Comp instance FIFO*/
extern struct st_fifo_device FIFO_Comp_FIFO;

/* declare FIFO instance of FIFO_Comp */
extern void init_fifo_device(struct st_fifo_device*);


/*Device-driver structure for uart_core*/
#define MicoUartCtx_t_DEFINED (1)
typedef struct st_MicoUartCtx_t {
    const char *   name;
    unsigned int   base;
    unsigned char   intrLevel;
    unsigned char   intrAvail;
    unsigned int   baudrate;
    unsigned int   databits;
    unsigned int   stopbits;
    unsigned char   rxBufferSize;
    unsigned char   txBufferSize;
    unsigned char   blockingTx;
    unsigned char   blockingRx;
    unsigned int   fifoenable;
    unsigned char   *rxBuffer;
    unsigned char   *txBuffer;
    DeviceReg_t   lookupReg;
    unsigned char   rxWriteLoc;
    unsigned char   rxReadLoc;
    unsigned char   txWriteLoc;
    unsigned char   txReadLoc;
    unsigned int   timeoutMicroSecs;
    volatile unsigned char   txDataBytes;
    volatile unsigned char   rxDataBytes;
    unsigned int   errors;
    unsigned char   ier;
    void *   prev;
    void *   next;
} MicoUartCtx_t;


/* uart_core instance uart*/
extern struct st_MicoUartCtx_t uart_core_uart;

/* declare uart instance of uart_core */
extern void MicoUartInit(struct st_MicoUartCtx_t*);


/* gpio instance lms_ctr_gpio*/
extern struct st_MicoGPIOCtx_t gpio_lms_ctr_gpio;

/* declare lms_ctr_gpio instance of gpio */
extern void MicoGPIOInit(struct st_MicoGPIOCtx_t*);


/* gpio instance SPIFlash_USRMCLKTS_*/
extern struct st_MicoGPIOCtx_t gpio_SPIFlash_USRMCLKTS_;

/* declare SPIFlash_USRMCLKTS_ instance of gpio */
extern void MicoGPIOInit(struct st_MicoGPIOCtx_t*);


/*Device-driver structure for i2cm_opencores*/
#define OpenCoresI2CMasterCtx_t_DEFINED (1)
typedef struct st_OpenCoresI2CMasterCtx_t {
    const char*   name;
    unsigned int   base;
    unsigned int   intrLevel;
    unsigned int   speed;
    DeviceReg_t   lookupReg;
    unsigned int   controlReg;
    void *   userCtx;
    void *   callback;
    void *   prev;
    void *   next;
} OpenCoresI2CMasterCtx_t;


/* i2cm_opencores instance i2cm_oc_*/
extern struct st_OpenCoresI2CMasterCtx_t i2cm_opencores_i2cm_oc_;

/* declare i2cm_oc_ instance of i2cm_opencores */
extern void OpenCoresI2CMasterInit(struct st_OpenCoresI2CMasterCtx_t*);


/* spi instance dac_spi_*/
extern struct st_MicoSPICtx_t spi_dac_spi_;

/* declare dac_spi_ instance of spi */
extern void MicoSPIInit(struct st_MicoSPICtx_t*);

extern int main();



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
