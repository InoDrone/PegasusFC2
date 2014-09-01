MCU		:= cortex-m4
CHIP	:= STM32F407VGT6

SYSCLK_FREQ         := 168000000
OSC_FREQ			:=   8000000

#BANK 10
FS_BANK_BASE		:= 0x080C0000
#BANK SIZE 128 * 2 * 1024 -> 256 Kbytes
FS_BANK_SIZE		:= 0x00040000
#SECTOR SIZE	128 Kbytes
FS_SECTOR_SIZE		:= 0x00020000

# Module to compile
#MODULES += Sensors
MODULES += Attitude
MODULES += Altitude
MODULES += Stablization
MODULES += CmdControl
MODULES += Telemetry

CFLAGS += -ffast-math

CPPSRC += $(HALDIR)/boards.cpp
CPPSRC += $(HALDIR)/pegasusv2.cpp

SRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/*.c))
CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/uavlink_object/inc

CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/uavlink_object/objects/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/uavlink_object/objects

CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/Math/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/Math/inc

CPPSRC += $(sort $(wildcard $(LIBRARYDIR)/AHRS/*.cpp))
INCLUDEDIRS += $(LIBRARYDIR)/AHRS/inc

include $(ROOT_DIR)/make/firmware.mk