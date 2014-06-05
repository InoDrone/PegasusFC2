MCU		:= cortex-m4
CHIP	:= STM32F407VGT6

SYSCLK_FREQ         := 168000000
OSC_FREQ			:=   8000000

# Module to compile
MODULES += Telemetry

CFLAGS += -ffast-math

CPPSRC += $(HALDIR)/pegasusv2b.cpp

include $(ROOT_DIR)/make/firmware.mk