#################################################################
#################################################################
#
#This file is included from top-level makefile.  Hence,
#the paths for perl scripts need to be adjusted for the
#debug configuration relative to the project directory.
#
#################################################################
#################################################################

#****************************************************************************
# This makefile builds drivers for non-rtos option and adds that as a 
# library-dependency for building the project.  It also provides rule
# for building the library.
#****************************************************************************
.PHONY: build_driverfiles
-include build_driverfiles


#-----------------------------------------------------------------
# Add the path to the makefile for building drivers archive;
# This will be invoked by the top-makefile.  the makefile in this
# directory must be named "makefile" (without the quotes).
#-----------------------------------------------------------------
DIST_LIB_MAKE += $(PLATFORM_DIRECTORY)/$(BUILD_CONFIGURATION)



#
#---------------------------------------------------------------------
#TODO!!: Add MSB platform file as a dependency and user-preferences...
#	perl $(PLATFORM_PERL_FILE_PATH)\build_ddinit.pl $(PLATFORM_LIBRARY_SETTINGS_FILE)
#	perl $(PLATFORM_PERL_FILE_PATH)\build_devicedrivers_makefile.pl $(PLATFORM_LIBRARY_SETTINGS_FILE)
#---------------------------------------------------------------------
#$(PROJECT_DIRECTORY)/$(PLATFORM_NAME)/$(BUILD_CONFIGURATION)/drivers.mk: $(PLATFORM_RULES_MAKEFILE)
#	perl $(PLATFORM_PERL_FILE_PATH)\build_ddinit.pl ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) $(PLATFORM_LIBRARY_CURR_CFG_DIR)
#	perl $(PLATFORM_PERL_FILE_PATH)\build_DeviceDrivers_makefile.pl ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) $(PLATFORM_LIBRARY_CURR_CFG_DIR) ../../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH) ../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH)
#	perl $(PLATFORM_PERL_FILE_PATH)\build_platform_makefile.pl $(PLATFORM_LIBRARY_SETTINGS_FILE)
#
#


#drivers.mk: $(PLATFORM_RULES_MAKEFILE) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) makefile
drivers.mk: $(PLATFORM_RULES_MAKEFILE) ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE)
	perl $(PLATFORM_PERL_FILE_PATH)\build_ddinit.pl ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) $(PLATFORM_LIBRARY_CURR_CFG_DIR)
	perl $(PLATFORM_PERL_FILE_PATH)\build_DeviceDrivers_makefile.pl ../$(PLATFORM_FILE_PATH)/$(PLATFORM_FILE) $(PLATFORM_LIBRARY_CURR_CFG_DIR) ../../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH) ../$(MDK_PERIPHERAL_DRIVERS_DIR_PATH)
	perl $(PLATFORM_PERL_FILE_PATH)\build_platform_makefile.pl $(PLATFORM_LIBRARY_SETTINGS_FILE)


#----------------------------------------------------------------------------------
# For building, include driver_incs.mk that provides include-paths to the top-level
# application should the application decide to use the drivers.
# For clean, there's nothing to do as this makefile depends on the processor-build
# services and that takes care of removing the directories it creates.
#----------------------------------------------------------------------------------
build_driverfiles:
ifneq "$(MAKECMDGOALS)" "clean"
-include drivers.mk
else
	rm -f drivers.mk
endif


