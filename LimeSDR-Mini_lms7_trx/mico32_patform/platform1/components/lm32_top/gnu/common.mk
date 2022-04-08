OUTPUT_DIR =./lm32
THAL_ROOT=/cygdrive/c/EEMBCTest
MICO_FRAMEWORK_DIR=/cygdrive/c/lm32/LatticeMico32/source/framework

#AUTOMOTIVE_DIR := /cygdrive/c/EEMBC/automotive
NETWORKING_DIR := /cygdrive/c/EEMBC/networking

include_dirs :=	-I$(THAL_ROOT)\th_lite			    	\
		        -I$(THAL_ROOT)\th_lite\mico32\al	\
		        -I$(THAL_ROOT)\th_lite\src           	\
			-I$(NETWORKING_DIR)
#                	-I$(AUTOMOTIVE_DIR)


VPATH = \


