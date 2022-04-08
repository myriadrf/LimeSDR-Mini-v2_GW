################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
$(ROOT)/platform1/DDInit.c \
$(ROOT)/platform1/DDStructs.c \
$(ROOT)/platform1/FIFO_Comp.c \
$(ROOT)/platform1/LCD.c \
$(ROOT)/platform1/LatticeMico32.c \
$(ROOT)/platform1/LatticeMico32Uart.c \
$(ROOT)/platform1/LookupServices.c \
$(ROOT)/platform1/MicoFileClose.c \
$(ROOT)/platform1/MicoFileDevices.c \
$(ROOT)/platform1/MicoFileIsAtty.c \
$(ROOT)/platform1/MicoFileOpen.c \
$(ROOT)/platform1/MicoFileRead.c \
$(ROOT)/platform1/MicoFileSeek.c \
$(ROOT)/platform1/MicoFileStat.c \
$(ROOT)/platform1/MicoFileWrite.c \
$(ROOT)/platform1/MicoGPIO.c \
$(ROOT)/platform1/MicoGPIOService.c \
$(ROOT)/platform1/MicoInterrupts.c \
$(ROOT)/platform1/MicoSPI.c \
$(ROOT)/platform1/MicoSPIFlash.c \
$(ROOT)/platform1/MicoSPIService.c \
$(ROOT)/platform1/MicoSbrk.c \
$(ROOT)/platform1/MicoStdStreams.c \
$(ROOT)/platform1/MicoUart.c \
$(ROOT)/platform1/MicoUartService.c \
$(ROOT)/platform1/MicoUtils.c \
$(ROOT)/platform1/OpenCoresI2CMaster.c \
$(ROOT)/platform1/OpenCoresI2CMasterService.c \
$(ROOT)/platform1/Reg_Comp.c \
$(ROOT)/platform1/printf_shrink.c 

DEPS += \
${addprefix ./platform1/, \
DDInit.d \
DDStructs.d \
FIFO_Comp.d \
LCD.d \
LatticeMico32.d \
LatticeMico32DbgModule.d \
LatticeMico32Uart.d \
LatticeMicoUtils.d \
LookupServices.d \
MicoExit.d \
MicoFileClose.d \
MicoFileDevices.d \
MicoFileIsAtty.d \
MicoFileOpen.d \
MicoFileRead.d \
MicoFileSeek.d \
MicoFileStat.d \
MicoFileWrite.d \
MicoGPIO.d \
MicoGPIOService.d \
MicoInterrupts.d \
MicoSPI.d \
MicoSPIFlash.d \
MicoSPIService.d \
MicoSbrk.d \
MicoSleepHelper.d \
MicoStdStreams.d \
MicoUart.d \
MicoUartService.d \
MicoUtils.d \
OpenCoresI2CMaster.d \
OpenCoresI2CMasterService.d \
Reg_Comp.d \
crt0ram.d \
printf_shrink.d \
}


ASM_SRCS += \
$(ROOT)/platform1/LatticeMico32DbgModule.S \
$(ROOT)/platform1/LatticeMicoUtils.S \
$(ROOT)/platform1/MicoExit.S \
$(ROOT)/platform1/MicoSleepHelper.S \
$(ROOT)/platform1/crt0ram.S