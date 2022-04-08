################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#************************************************************************
# Version=3.0
#************************************************************************
#************************************************************************
# PROJECT SETTINGS
#************************************************************************
PROJECT_NAME=lms7_trx
BUILD_CONFIGURATION=Debug
PROJECT_DIRECTORY=..

PLATFORM_DIRECTORY= $(PROJECT_DIRECTORY)/platform1
PLATFORM_NAME=platform1
PLATFORM_FILE=platform1.msb
PLATFORM_FILE_PATH=../../mico32_patform/platform1/soc
PROJECT_USER_PREF=$(PROJECT_DIRECTORY)/user.pref


PLATFORM_PERL_FILE_PATH = C:/lscc/diamond/3.12/micosystem/utilities/perlscript/build/
PATH:=$(MICO32_GTOOLS_BIN):$(PATH)
#************************************************************************
# Installation path for peripherals
#************************************************************************
MDK_PERIPHERAL_DRIVERS_DIR_PATH=$(PLATFORM_FILE_PATH)/../components
PERL_BUILD_SCRIPTS_PATH=../$(PLATFORM_FILE_PATH)/../components/lm32_top/gnu/
APP_BUILD_PERIPHERALS_PATH=../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH)



#************************************************************************
# Identify project-sources: MANAGED BY ECLIPSE!!
#************************************************************************
OUTPUT_DIR = $(BUILD_CONFIGURATION)
ROOT = ..
SUBDIRS := \
. \


-include $(SUBDIRS:%=%/subdir.mk)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif

CC = lm32-elf-gcc
SYSCFLAGS += -ffunction-sections -O0 -g2 -Wall -D_USE_LSCC_PRINTF_
LDFLAGS +=-Wl,--gc-sections
INCLUDE_PATH+=
PLATFORM_LIBRARY=./$(BUILD_CONFIGURATION)/lib$(PLATFORM_NAME).a
PROJECT_LIBRARY+=
PROJECT_LIBRARY+=$(PLATFORM_LIBRARY)
ARCHIVE_OBJS+=

#************************************************************************
# obtain root for compiler toolchain
#************************************************************************
CC_PATH = $(dir $(shell which $(CC)))

#************************************************************************
# Identify USE_STANDALONE_SMALL_PRINTF
#************************************************************************
USE_STANDALONE_SMALL_PRINTF = TRUE

#************************************************************************
# COMPONENTS/PLATFORM MAKEFILES
#************************************************************************
include $(PERL_BUILD_SCRIPTS_PATH)/build_processor.mk
include $(PERL_BUILD_SCRIPTS_PATH)/build_drivers.mk
include $(PERL_BUILD_SCRIPTS_PATH)/gnu_rules.mk

LD = lm32-elf-gcc
CFLAGS += -ffunction-sections -O0 -g2 -Wall -D_USE_LSCC_PRINTF_
VPATH += ./../$(PLATFORM_NAME)
C_SRCS+= $(APP_C_SRCS)
ASM_SRCS+= $(APP_ASM_SRCS)
CXX_SRCS += $(APP_CXX_SRCS)
ARCHIVE_OBJS+= $(APP_ARCHIVE_OBJS)
INCLUDE_PATH+=./../$(PLATFORM_NAME)

#************************************************************************
# Identify executable-name for this application
#************************************************************************
PROJECT_EXE=$(PROJECT_NAME).elf

#************************************************************************
# IDENTIFY C LIBRARY
#************************************************************************
# USE_SMALL_LIBC is populated by managed-build process
USE_SMALL_LIBC = FALSE
# special processing when using small-libc
ifeq "$(strip $(USE_SMALL_LIBC))" "TRUE"
C_LIB = -lsmallc
else
C_LIB = -lc
endif
#

#************************************************************************
# FUNCTIONS
#************************************************************************
define fn_remove_file
	rm -f $1
	@echo
endef

PLATFORM_LINKER_FILE = $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/$(BUILD_CONFIGURATION)/linker.ld

.PHONY: all $(DIST_LIB_MAKE) dummy

#************************************************************************
# Default rule: build everything
#************************************************************************
all: dummy $(PLATFORM_LINKER_FILE) $(DIST_LIB_MAKE) force_clean_archive_objs $(PROJECT_EXE)



#************************************************************************
# Rule to build dependent libraries
#************************************************************************
$(DIST_LIB_MAKE):
	@$(MAKE) --directory=$@

#************************************************************************
# Dummy rule to create output directory as well as
# run make on the "synchronous" makefiles
#************************************************************************
dummy:
	@mkdir -p $(OUTPUT_DIR)
	@echo $(INCLUDE_PATHS)

#************************************************************************
# Rule to build the project (application)
#************************************************************************
$(PROJECT_EXE) : $(ARCHIVE_OBJS) $(PROJECT_LIBRARY) 
	@echo building application...
	$(LD) $(CPU_CONFIG) $(PROJECT_LINKER_SCRIPT) -o$@ $(ARCHIVE_OBJS) $(PROJECT_LIBRARY) -lm $(C_LIB) -lgcc $(PROJECT_LIBRARY) -lnosys $(LDFLAGS)
	lm32-elf-size $(PROJECT_EXE)

#************************************************************************
# clean archive objects due to changes from Eclipse
#************************************************************************
force_clean_archive_objs: $(PROJECT_USER_PREF) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE)
	@echo detected change in user.pref or platform...
	@echo force-cleaning archive objects...
	$(foreach object,$(ARCHIVE_OBJS),$(call fn_remove_file,$(object)))
	touch force_clean_archive_objs
	@echo

#************************************************************************
# Rule to clean everything
#************************************************************************
clean:
	@echo cleaning project objects..
	$(foreach object,$(ARCHIVE_OBJS),$(call fn_remove_file,$(object)))
	rm -f -r $(OUTPUT_DIR)
	rm -f *.o
	rm -f *.elf

