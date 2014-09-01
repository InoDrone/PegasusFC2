CC		= $(ARM_SDK_PREFIX)gcc
CXX 	= $(ARM_SDK_PREFIX)g++
AR		= $(ARM_SDK_PREFIX)ar
OBJCOPY	= $(ARM_SDK_PREFIX)objcopy
OBJDUMP = $(ARM_SDK_PREFIX)objdump
SIZE	= $(ARM_SDK_PREFIX)size
NM		= $(ARM_SDK_PREFIX)nm
STRIP	= $(ARM_SDK_PREFIX)strip

THUMB  	= -mthumb


TOPDIR 	= .
HALDIR = $(TOPDIR)
HALINCDIR = $(HALDIR)/inc
PEGASUSOS_CORE_INC = $(PEGASUSOS)/core/inc
PEGASUSOS_CORE = $(PEGASUSOS)/core

# FReeRTOS
FREERTOS_DIR = $(PEGASUSOS_CORE)/libraries/FreeRTOS/Source
SRC += $(sort $(wildcard $(FREERTOS_DIR)/*.c))
SRC += $(sort $(wildcard $(FREERTOS_DIR)/portable/MemMang/heap_2.c))
INCLUDEDIRS	+= $(FREERTOS_DIR)/include

ifeq ($(MCU),cortex-m4)
    include $(PEGASUSOS)/hal/stm32f4xx/library.mk
else
    $(error Unsupported MCU: $(MCU))
endif

# C Source code
SRC += $(sort $(wildcard $(PEGASUSOS_CORE)/*.c))
CPPSRC += $(sort $(wildcard $(PEGASUSOS_CORE)/*.cpp))
#SRC += $(PEGASUSOS_CORE)/syscalls.c

# C++ Source code
CPPSRC +=

# Modules
SRC += $(foreach mod, $(MODULES), $(sort $(wildcard $(MODULEDIR)/$(mod)/*.c)))
CPPSRC += $(foreach mod, $(MODULES), $(sort $(wildcard $(MODULEDIR)/$(mod)/*.cpp)))


INCLUDEDIRS += $(PEGASUSOS)
INCLUDEDIRS += $(PEGASUSOS_CORE_INC)
INCLUDEDIRS += $(HALINCDIR)

# Includes modules dir
INCLUDEDIRS += $(foreach mod, $(MODULES), $(MODULEDIR)/$(mod)/inc)

# Libs
EXTRA_LIBS += m 
#stdc++ supc++

ifeq ($(MCU),cortex-m4)
    LDFLAGS += $(addprefix -T,$(LINKER_SCRIPTS))
endif


#.PHONY: gccversion
#gccversion:
#	 @$(CC) --version
	
# HEX File
%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	
%.bin: %.o
	$(OBJCOPY) -O binary $< $@
	
%.lss: %.elf
	$(OBJDUMP) -h -S -C -r $< > $@

%.sym: %.elf
	$(NM) -n $< > $@
	
# Compile: create object files from C source files.
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
	$(CC) -c $(THUMB) $$(CFLAGS) $$(CONLYFLAGS) $$< -o $$@
endef

# Compile: create object files from C++ source files.
define COMPILE_CXX_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
	$(CXX) -c $(THUMB) $$(CFLAGS) $$(CPPFLAGS) $$(CXXFLAGS) $$(CXXONLYFLAGS) $$< -o $$@
endef

# Link: create ELF output file from object files.
#   $1 = elf file to produce
#   $2 = list of object files that make up the elf file
#   $3 = optional list of libraries to build and link
define LINK_TEMPLATE
.SECONDARY : $(1)
.PRECIOUS : $(2) $(3)
$(1):  $(2) $(3)
	$(CC) $(THUMB) $$(CFLAGS) $(2) $(3) --output $$@ $$(LDFLAGS)
endef

# Link: create ELF output file from object files.
#   $1 = elf file to produce
#   $2 = list of object files that make up the elf file
define LINK_CXX_TEMPLATE
.SECONDARY : $(1)
.PRECIOUS : $(2)
$(1):  $(2)
	$(CXX) $(THUMB) $$(CFLAGS) $(2) --output $$@ $$(LDFLAGS)
endef

CSTANDARD 	= -std=gnu99
CXXSTANDARD = -std=gnu++11

CFLAGS += $(ARCHFLAGS)
CFLAGS += $(CDEFS)
CFLAGS += -O0
CFLAGS += -g3
CFLAGS += -gdwarf-2
CFLAGS += -mapcs-frame
CFLAGS += -fomit-frame-pointer
CFLAGS += -fsingle-precision-constant
CFLAGS += -Wall -Wextra
CLFAGS += -Wfloat-equal -Wunsuffixed-float-constants -Wdouble-promotion
CFLAGS += -Wshadow
CFLAGS += -Werror
CFLAGS += $(patsubst %,-I%,$(INCLUDEDIRS)) -I.
CFLAGS += -Wa,-adhlns=$(addprefix $(OUTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

# DEBUG
CFLAGS += -DDEBUG

CONLYFLAGS += $(CSTANDARD)
CXXONLYFLAGS += $(CXXSTANDARD)

# Linker flags
LDFLAGS += -nostartfiles
LDFLAGS += -Wl,--warn-common,--fatal-warnings,--sort-common,--sort-section=alignment,--gc-sections
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(TARGET).map,--cref
#LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))
LDFLAGS += -lc -lgcc

# Debug
LDFLAGS += -Wl,-static

ALLSRC 		= $(SRC) $(CPPSRC)
ALLSRCBASE	= $(notdir $(basename $(ALLSRC)))

ALLOBJ 		= $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRCBASE)))

all: build

build: elf hex bin


.PHONY: elf hex bin check
elf: $(OUTDIR)/$(TARGET).elf
hex: $(OUTDIR)/$(TARGET).hex
bin: $(OUTDIR)/$(TARGET).bin

# Link: create ELF output file from object files.
$(eval $(call LINK_CXX_TEMPLATE, $(OUTDIR)/$(TARGET).elf, $(ALLOBJ), $(ALLLIB)))
$(foreach src, $(SRC), $(eval $(call COMPILE_C_TEMPLATE, $(src))))
$(foreach src, $(CPPSRC), $(eval $(call COMPILE_CXX_TEMPLATE, $(src))))


$(shell mkdir -p $(OUTDIR) 2>/dev/null)

.PHONY: all build