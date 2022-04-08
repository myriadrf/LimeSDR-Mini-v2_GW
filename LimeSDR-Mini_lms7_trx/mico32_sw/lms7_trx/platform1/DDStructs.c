#include "DDStructs.h"


/* lm32_top instance LM32*/
struct st_LatticeMico32Ctx_t lm32_top_LM32 = {
    "LM32"};


/* spi_flash instance SPIFlash_*/
struct st_MicoSPIFlashCtx_t spi_flash_SPIFlash_ = {
    "SPIFlash_",
    0x10000000,
    134217728,
    8,
    0x80000000,
    1,
    8,
    1,
    1,
    0,
    1,
    0,
    1,
    256,
    4096,
};


/* gpio instance LED*/
struct st_MicoGPIOCtx_t gpio_LED = {
    "LED",
    0x80000800,
    255,
    1,
    0,
    0,
    0,
    8,
    1,
    1,
    0,
    32,
};


/* gpio instance GPIO*/
struct st_MicoGPIOCtx_t gpio_GPIO = {
    "GPIO",
    0x80001000,
    255,
    0,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    32,
};


/* Reg_Comp instance GPO*/
struct st_reg_device Reg_Comp_GPO = {
    305441741,
    0x80001800};


/* spi instance spi*/
struct st_MicoSPICtx_t spi_spi = {
    "spi",
    0x80002000,
    1,
    1,
    2,
    32,
    0,
};


/* FIFO_Comp instance FIFO*/
struct st_fifo_device FIFO_Comp_FIFO = {
    305441741,
    0x80002800,
};


/* uart_core instance uart*/
  /* array declaration for rxBuffer */
   unsigned char _uart_core_uart_rxBuffer[4];
  /* array declaration for txBuffer */
   unsigned char _uart_core_uart_txBuffer[4];
struct st_MicoUartCtx_t uart_core_uart = {
    "uart",
    0x80003000,
    3,
    1,
    115200,
    8,
    1,
    4,
    4,
    1,
    1,
    0,
    _uart_core_uart_rxBuffer,
    _uart_core_uart_txBuffer,
};


/* gpio instance lms_ctr_gpio*/
struct st_MicoGPIOCtx_t gpio_lms_ctr_gpio = {
    "lms_ctr_gpio",
    0x80003800,
    255,
    1,
    0,
    0,
    0,
    4,
    1,
    1,
    0,
    32,
};


/* gpio instance SPIFlash_USRMCLKTS_*/
struct st_MicoGPIOCtx_t gpio_SPIFlash_USRMCLKTS_ = {
    "SPIFlash_USRMCLKTS_",
    0x80004000,
    255,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    0,
    32,
};


/* i2cm_opencores instance i2cm_oc_*/
struct st_OpenCoresI2CMasterCtx_t i2cm_opencores_i2cm_oc_ = {
    "i2cm_oc_",
    0x80004800,
    4,
    400,
};


/* spi instance dac_spi_*/
struct st_MicoSPICtx_t spi_dac_spi_ = {
    "dac_spi_",
    0x80005000,
    5,
    1,
    1,
    16,
    0,
};

