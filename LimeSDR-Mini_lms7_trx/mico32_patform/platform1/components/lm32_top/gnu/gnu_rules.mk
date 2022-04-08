#********************************************
#Identify Mico-32 compiler toolchain binaries
#********************************************
#CC=lm32-elf-gcc
LD=lm32-elf-ld
AS=lm32-elf-as
AR=lm32-elf-ar


#********************************************
# Add to CPP flags
#********************************************
CPPFLAGS += -D__lm32__



#*********************************************
# Add to C-flags
#*********************************************
CFLAGS += 


#*********************************************
# Add to CPP Flags
#*********************************************
CPPFLAGS+=


#*********************************************
# Add to linker Flags
#*********************************************
LDFLAGS +=


#*******************************************************************************
# Enhance dependencies to depend on all the makefiles as well as source-files
# as well as object-files
#*******************************************************************************
OBJS=$(sort $(C_SRCS:.c=.o)			\
			$(patsubst %.cxx, %.o, $(patsubst %.cc, %.o, $(patsubst %.cpp, %.o, $(patsubst %.C, %.o,$(CXX_SRCS)))))	\
			$(patsubst %.S, %.o, $(patsubst %.s, %.o, $(ASM_SRCS))))


ARCHIVE_OBJS=$(addprefix $(OUTPUT_DIR)/, $(OBJS))



CPPFLAGS += $(foreach inc_path, $(INCLUDE_PATH), -I$(inc_path))



###############################################################################
# BUILD-RULES 
# TODO: ADD CPP RULES AND OTHER RULES...
###############################################################################
# Enhance dependency to include makefiles as well as source-file
$(OUTPUT_DIR)/%.o : %.c
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	echo -n $(dir $@) > $(@:%.o=%.d) && \
	$(CC) $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) -MM -MG -P -w $< >> $(@:%.o=%.d)

# Enhance dependency to include makefiles as well as source-files
%.o: %.c
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $(OUTPUT_DIR)/$@

$(OUTPUT_DIR)/%.o : %.cpp
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	echo -n $(dir $@) > $(@:%.o=%.d) && \
	$(CC) $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) -MM -MG -P -w $< >> $(@:%.o=%.d)

%.o: %.cpp
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $(OUTPUT_DIR)/$@


$(OUTPUT_DIR)/%.o : %.cc
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	echo -n $(dir $@) > $(@:%.o=%.d) && \
	$(CC) $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) -MM -MG -P -w $< >> $(@:%.o=%.d)

%.o: %.cc
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $(OUTPUT_DIR)/$@


$(OUTPUT_DIR)/%.o : %.cxx
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	echo -n $(dir $@) > $(@:%.o=%.d) && \
	$(CC) $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) -MM -MG -P -w $< >> $(@:%.o=%.d)

%.o: %.cxx
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $(OUTPUT_DIR)/$@


$(OUTPUT_DIR)/%.o : %.C
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $@
	echo -n $(dir $@) > $(@:%.o=%.d) && \
	$(CC) $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) -MM -MG -P -w $< >> $(@:%.o=%.d)

%.o: %.C
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $< -o $(OUTPUT_DIR)/$@

# Enhance dependency to include makefiles as well as source-files
$(OUTPUT_DIR)/%.o: %.S
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(ASMFLAGS) $< -o $@


# Enhance dependency to include makefiles as well as source-files
%.o: %.S
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(ASMFLAGS) $< -o $(OUTPUT_DIR)/$@


$(OUTPUT_DIR)/%.o: %.s
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(ASMFLAGS) $< -o $@


# Enhance dependency to include makefiles as well as source-files
%.o: %.s
	$(CC) -c $(CPU_CONFIG) $(CFLAGS) $(CPPFLAGS) $(ASMFLAGS) $< -o $(OUTPUT_DIR)/$@

# Enhance dependency to include other stuff
$(OUTPUT_DIR)/%.elf : $(PROJECT_LIBRARY) $(ARCHIVE_OBJS)
	$(CC) $(CPU_CONFIG) $(LDFLAGS) $(PROJECT_LINKER_SCRIPT) -o $@ $(ARCHIVE_OBJS) $(PROJECT_LIBRARY)

