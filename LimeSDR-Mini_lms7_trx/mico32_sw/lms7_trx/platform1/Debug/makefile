.PHONY: clean
.PHONY: platform_directory.phny

LIBRARY_C_SRCS=
LIBRARY_CXX_SRCS=
LIBRARY_ASM_SRCS=
LIBRARY_INCLUDE_PATH=
INCLUDE_PATH=
#declare C sources needed for archive
C_SRCS += 		DDInit.c	\
		DDStructs.c	\
		MicoStdStreams.c	\




OUTPUT_DIR = $(PLATFORM_LIBRARY_OUTPUT_DIR)
INCLUDE_PATH +=.
#include other makefiles
include drivers.mk
include inherited_settings.mk
include platform_rules.mk



C_SRCS +=$(LIBRARY_C_SRCS)
CXX_SRCS +=$(LIBRARY_CXX_SRCS)
ASM_SRCS +=$(LIBRARY_ASM_SRCS)
INCLUDE_PATH +=$(LIBRARY_INCLUDE_PATH)
VPATH +=.. $(LIBRARY_VPATH) ./$(LIBRARY_VPATH)/..



#include building-rules makefile
include ../../../../mico32_patform/platform1/soc/../components/lm32_top/gnu/gnu_rules.mk

#Targets for this library..
all: platform_directory.phny $(OUTPUT_DIR)/lib$(PLATFORM_LIBRARY_NAME).a

clean:
	@echo cleaning...
	rm -f $(OUTPUT_DIR)/*.o
	rm -f $(OUTPUT_DIR)/lib$(PLATFORM_LIBRARY_NAME).a

platform_directory.phny:
	@mkdir -p $(OUTPUT_DIR)

$(OUTPUT_DIR)/lib$(PLATFORM_LIBRARY_NAME).a : $(ARCHIVE_OBJS)
	@echo creating $@...
	@$(AR) $(ARFLAGS) $@ $(ARCHIVE_OBJS)

DEPS += \
${addprefix $(OUTPUT_DIR)/, \
DDInit.d \
DDStructs.d \
FIFO_Comp.d \
LCD.d \
LatticeMico32.d \
LatticeMico32Uart.d \
LookupServices.d \
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
MicoUart.d \
MicoUartService.d \
MicoUtils.d \
OpenCoresI2CMaster.d \
OpenCoresI2CMasterService.d \
Reg_Comp.d \
printf_shrink.d \
}
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
