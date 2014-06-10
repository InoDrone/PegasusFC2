MCU		:= cortex-m4
CHIP	:= STM32F407VGT6

SYSCLK_FREQ         := 168000000
OSC_FREQ			:=   8000000

# Module to compile
MODULES += Sensors
MODULES += Telemetry

CFLAGS += -ffast-math

CPPSRC += $(HALDIR)/boards.cpp
CPPSRC += $(HALDIR)/pegasusv2b.cpp

SRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/*.c))
CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/uavlink_object/inc

CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/objects/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/uavlink_object/objects

include $(ROOT_DIR)/make/firmware.mk