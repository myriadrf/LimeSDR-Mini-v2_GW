#################################################################
#################################################################
#
#This file is included from top-level makefile.  Hence,
#the paths for perl scripts need to be adjusted for the
#debug configuration relative to the project directory.
#
#################################################################
#################################################################

#---------------------------------------------------------------
# this makefile generates cpu-specific compiler options as well
# as creates directory structure for Lattice platform library.
#---------------------------------------------------------------
.PHONY: build_library_structure
-include build_library_structure



#------------------------------------------------
# convert platform root directory to cygwin-type
#------------------------------------------------
#PLATFORM_LATTICE_PERIPHERALS_ROOT = $(addprefix /cygdrive/,$(subst /cygdrive/, ,$(subst :,/,$(subst \,/,$(MDK_PERIPHERAL_DRIVERS_DIR_PATH)))))
#platform peripherals-root relative to library directory
PLATFORM_LATTICE_PERIPHERALS_ROOT = ../../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH)


#PLATFORM_LIBRARY_PATH=$(addprefix /cygdrive/,$(subst /cygdrive/, ,$(subst :,/,$(subst \,/,$(PROJECT_DIRECTORY)))))/$(PLATFORM_NAME)
##Form library path relative to the project directory
PLATFORM_LIBRARY_PATH=$(PROJECT_DIRECTORY)/$(PLATFORM_NAME)


#Form path for MSB file relative to the library path
PLATFORM_LIBRARY_MSB_FILEPATH=./../$(PLATFORM_FILE_PATH)



PLATFORM_LIBRARY_CURR_CFG_DIR = $(PLATFORM_LIBRARY_PATH)/$(BUILD_CONFIGURATION)
PLATFORM_LIBRARY_SETTINGS_FILE = $(PLATFORM_LIBRARY_CURR_CFG_DIR)/settings.xml
PLATFORM_LIBRARY_DERIVED_SETTINGS_FILE = $(PLATFORM_LIBRARY_CURR_CFG_DIR)/inherited_settings.mk
PLATFORM_RULES_MAKEFILE = $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/$(BUILD_CONFIGURATION)/platform_rules.mk


PLATFORM_LINKER_SETTINGS_MAKEFILE = $(PLATFORM_LIBRARY_CURR_CFG_DIR)/linker_settings.mk
PLATFORM_DEFAULT_LINKER_FILE = $(PLATFORM_LIBRARY_CURR_CFG_DIR)/linker.ld


#---------------------------------------------------------------#
# FUNCTION: generate_derived_rules_makefile						#
#---------------------------------------------------------------#
# generates toplevel_rules.mk containing definitions			#
# from the project-makefile, for use by the library-make		#
#	@echo PLATFORM_LATTICE_PERIPHERALS_ROOT = $(PLATFORM_LATTICE_PERIPHERALS_ROOT)	>> $1
#	@echo PLATFORM_LIBRARY_PATH=$(PLATFORM_LIBRARY_PATH)							>> $1
#---------------------------------------------------------------#
define generate_derived_rules_makefile
	@echo CC = $(CC)    >  $1
	@echo CFLAGS+= $(SYSCFLAGS)    >>  $1
	@echo PLATFORM_LIBRARY_OUTPUT_DIR = ../../$(BUILD_CONFIGURATION)/$(BUILD_CONFIGURATION) >> $1
	@echo PLATFORM_LIBRARY_NAME=$(PLATFORM_NAME)    >> $1
	@echo PLATFORM_BUILD_CFG=$(BUILD_CONFIGURATION)     >> $1
endef




#---------------------------------------------------------------#
# FUNCTION: generate_library_settings							#
#---------------------------------------------------------------#
# generates settings to be used by perl-scripts when generating #
# other makefiles.												#
#---------------------------------------------------------------#
define generate_library_settings
	@echo \<\?xml version=\"1.0\" encoding=\"UTF-8\"\?\>    >  $1
	@echo \<PROJECT path=\"$(PROJECT_DIRECTORY)\"\> >> $1
		@echo \<SYSLIB\>    >> $1
			@echo \<MSB_FILE path=\"$(PLATFORM_LIBRARY_MSB_FILEPATH)\"  name=\"$(PLATFORM_FILE)\" \/\>  >> $1
			@echo \<LATTICE_MDK_PERIPHERALS rootdir=\"$(PLATFORM_LATTICE_PERIPHERALS_ROOT)\"\/\>    >> $1
			@echo \<BUILD_CFG name=\"$(BUILD_CONFIGURATION)\"\/\>   >> $1
			@echo \<USER_PREFS name=\"user.pref\" path=\"$(PROJECT_DIRECTORY)\"\/\>	    >> $1
			@echo \<BUILD\>	    >> $1
				@echo \<COMPILER flags=\"$(CFLAGS)\"\/\>    >> $1
			@echo \<\/BUILD\>   >> $1
		@echo \<\/SYSLIB\>  >> $1
	@echo \<\/PROJECT\> >> $1
endef





#-------------------------------------------------------------------------------------
#rule for generating cpu-specific compiler settings:
# 1. Create directory structure
# 2. Create platform-library settings for the current configuration
# 3. Generate platform-rules file for inclusion in the top-level project makefile
# 4. Generate drivers initialization/header/makefile
# 5. Generate library makefile
#  dependency
#
#-------------------------------------------------------------------------------------

#$(PLATFORM_RULES_MAKEFILE) : $(PROJECT_DIRECTORY)/$(BUILD_CONFIGURATION)/makefile  $(PROJECT_USER_PREF) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE)
$(PLATFORM_RULES_MAKEFILE) :  $(PROJECT_USER_PREF) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE)
	@mkdir -p -v $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)
	@mkdir -p -v $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/$(BUILD_CONFIGURATION)
	@rm -f $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/*.c
	@rm -f $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/*.h
	$(call generate_library_settings, $(PLATFORM_LIBRARY_SETTINGS_FILE))
	@perl $(PLATFORM_PERL_FILE_PATH)\build_platform_rules.pl $(PLATFORM_LIBRARY_SETTINGS_FILE)
	$(call generate_derived_rules_makefile, $(PLATFORM_LIBRARY_DERIVED_SETTINGS_FILE))



#----------------------------------------------------------------------------
# linker-settings file depends on user-preference file and the platform file
#
#----------------------------------------------------------------------------
$(PLATFORM_LINKER_SETTINGS_MAKEFILE)	:$(PLATFORM_RULES_MAKEFILE) $(PROJECT_USER_PREF) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE)
	perl $(PLATFORM_PERL_FILE_PATH)/lm32/\build_default_linker_script.pl $(PLATFORM_DEFAULT_LINKER_FILE) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) $(PROJECT_DIRECTORY)/user.pref
	perl $(PLATFORM_PERL_FILE_PATH)\build_linker_settings.pl $(PROJECT_DIRECTORY)/user.pref $(PLATFORM_LINKER_SETTINGS_MAKEFILE) $(PLATFORM_LIBRARY_CURR_CFG_DIR)/linker.ld
	perl $(PLATFORM_PERL_FILE_PATH)\build_stdstream_src.pl $(PROJECT_DIRECTORY)/user.pref $(PLATFORM_DIRECTORY)/MicoStdStreams.c



#------------------------------------------
#Phony target to intercept "clean" requests
#------------------------------------------
build_library_structure:
ifeq "$(MAKECMDGOALS)" "clean"
	@echo cleaning platform library
	@rm -r -f -v $(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/$(BUILD_CONFIGURATION)
else
-include $(PLATFORM_RULES_MAKEFILE)
-include $(PLATFORM_LINKER_SETTINGS_MAKEFILE)
endif

