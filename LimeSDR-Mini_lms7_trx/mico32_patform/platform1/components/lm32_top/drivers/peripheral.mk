#-------------------------------------------------------
# Identify source-paths for this device's driver-sources
# that are built as part of the library-build (and not
# the application
#-------------------------------------------------------
LIBRARY_C_SRCS += MicoInterrupts.c	\
		MicoUtils.c		\
		LookupServices.c	\
		LatticeMico32.c		\
		LatticeMico32Uart.c	\
		MicoFileDevices.c	\
		MicoFileWrite.c		\
		MicoFileRead.c		\
		MicoFileSeek.c		\
		MicoFileClose.c		\
		MicoFileOpen.c		\
		MicoFileStat.c		\
		MicoSbrk.c		\
		MicoFileIsAtty.c	\
		printf_shrink.c


LIBRARY_ASM_SRCS +=MicoSleepHelper.S		\
			LatticeMicoUtils.S	\
			MicoExit.S		\
			LatticeMico32DbgModule.S

LIBRARY_CXX_SRCS +=



#-------------------------------------------------------
# Identify paths and resources for the top-level
# application (and not the library)
#-------------------------------------------------------
APP_ASM_SRCS+=crt0ram.S

