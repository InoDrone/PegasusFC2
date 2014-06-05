
export ROOT_DIR    	:= $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
export BUILD_DIR 	:= $(ROOT_DIR)/build
export PEGASUSOS	:= $(ROOT_DIR)/pegasusos
export MODULEDIR 	:= $(ROOT_DIR)/modules
export LIBRARYDIR	:= $(ROOT_DIR)/libraries

export ARM_SDK_PREFIX ?= arm-none-eabi-

#################################
#	Make Target
#################################
.PHONY: all
all: pegasusfc_v2


.PHONY: clean
clean:
	rm -fr $(BUILD_DIR)/*

$(BUILD_DIR):
	mkdir -p $@
	
################################
#	BOARDS TARGETS
################################
.PHONY: pegasusfc_v2
pegasusfc_v2:
	cd $(ROOT_DIR)/targets/pegasusv2b && \
	make -r --no-print-directory \
	BOARD_NAME=pegasusv2b \
	TOPDIR=$(ROOT_DIR)/targets/pegasusv2b \
	OUTDIR=$(BUILD_DIR) \
	TARGET=pegasusv2b
	$$*

