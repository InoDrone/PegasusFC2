#
#
#

PEGASUSOS_HALLIB := $(dir $(lastword $(MAKEFILE_LIST)))

LINKER_SCRIPTS = $(PEGASUSOS_HALLIB)stm32f4xx.ld

# Compiler options
CDEFS += -DSTM32F40XX
CDEFS += -DSYSCLK_FREQ=$(SYSCLK_FREQ)
CDEFS += -DHSE_VALUE=$(OSC_FREQ)
CDEFS += -DUSE_STDPERIPH_DRIVER
CDEFS += -DARM_MATH_CM4 -D__FPU_PRESENT=1
ARCHFLAGS += -mcpu=cortex-m4 -march=armv7e-m -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Pegasus OS HAL source
SRC += $(sort $(wildcard $(PEGASUSOS_HALLIB)*.c))
CPPSRC += $(sort $(wildcard $(PEGASUSOS_HALLIB)*.cpp))
INCLUDEDIRS += $(PEGASUSOS_HALLIB)inc

# CMSIS
CMSIS_DIR := $(PEGASUSOS_HALLIB)libraries/CMSIS/Device/ST/STM32F4xx
SRC += $(sort $(wildcard $(CMSIS_DIR)/Source/*.c))
INCLUDEDIRS += $(CMSIS_DIR)/Include
INCLUDEDIRS += $(PEGASUSOS_HALLIB)libraries/CMSIS/Include

# ST Peripheral library
STMLIB = $(PEGASUSOS_HALLIB)libraries/STM32F4xx_StdPeriph_Driver
SRC += $(sort $(wildcard $(STMLIB)/src/*.c))
INCLUDEDIRS += $(STMLIB)/inc

# FreeRTOS
FREERTOS_DEVICEDIR := $(PEGASUSOS_HALLIB)libraries/FreeRTOS/Source
SRC += $(sort $(wildcard $(FREERTOS_DEVICEDIR)/portable/GCC/ARM_CM4F/*.c))
INCLUDEDIRS += $(FREERTOS_DEVICEDIR)/portable/GCC/ARM_CM4F