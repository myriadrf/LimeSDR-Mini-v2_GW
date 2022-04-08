#include "DDStructs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void LatticeDDInit(void)
{

    /* initialize LM32 instance of lm32_top */
    LatticeMico32Init(&lm32_top_LM32);
    
    /* initialize SPIFlash_ instance of spi_flash */
    MicoSPIFlash_Initialize(&spi_flash_SPIFlash_);
    
    /* initialize LED instance of gpio */
    MicoGPIOInit(&gpio_LED);
    
    /* initialize GPIO instance of gpio */
    MicoGPIOInit(&gpio_GPIO);
    
    /* initialize GPO instance of Reg_Comp */
    init_reg_device(&Reg_Comp_GPO);
    
    /* initialize spi instance of spi */
    MicoSPIInit(&spi_spi);
    
    /* initialize FIFO instance of FIFO_Comp */
    init_fifo_device(&FIFO_Comp_FIFO);
    
    /* initialize uart instance of uart_core */
    MicoUartInit(&uart_core_uart);
    
    /* initialize lms_ctr_gpio instance of gpio */
    MicoGPIOInit(&gpio_lms_ctr_gpio);
    
    /* initialize SPIFlash_USRMCLKTS_ instance of gpio */
    MicoGPIOInit(&gpio_SPIFlash_USRMCLKTS_);
    
    /* initialize i2cm_oc_ instance of i2cm_opencores */
    OpenCoresI2CMasterInit(&i2cm_opencores_i2cm_oc_);
    
    /* initialize dac_spi_ instance of spi */
    MicoSPIInit(&spi_dac_spi_);
    
    /* invoke application's main routine*/
    main();
}



#ifdef __cplusplus
};
#endif /* __cplusplus */
