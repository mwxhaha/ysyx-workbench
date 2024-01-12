VERILATOR = verilator
WAVE_READER = gtkwave
ROOT_DIR = $(NPC_HOME)
VSRC_DIR = $(ROOT_DIR)/vsrc
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(ROOT_DIR)/csrc
CSRC = $(shell find $(CSRC_DIR) -name "main.cpp" -or -name "$(TOP_NAME)*.cpp") $(shell find $(CSRC_DIR)/util -name "*.cpp" )
NXDC_DIR = $(ROOT_DIR)/nxdc
NXDC = $(shell find $(NXDC_DIR) -name "*.nxdc")
INCLUDE_DIR = $(ROOT_DIR)/include
INCLUDE = $(shell find $(INCLUDE_DIR) -name "*.vh" -or -name "*.hpp")
SCRIPT_DIR = $(ROOT_DIR)/script
SCRIPT = $(shell find $(SCRIPT_DIR) -name "*.mk")
MAKE_FILE = $(ROOT_DIR)/Makefile $(SCRIPT)
BUILD_DIR = $(ROOT_DIR)/build
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir

TOP_NAME = cpu
ISA = RV32E
NVBOARD = 0
HAVE_CLK = 1
RECORD_WAVE = 1
FSANITIZE = 1
ifeq ($(RECORD_WAVE),1)
DISPLAY_WAVE = 0
endif
DEBUG = 0
TRACE = 1
ifeq ($(TRACE),1)
ITRACE = 1
MTRACE = 1
FTRACE = 1
EXPR_MATCH = 0
endif
WATCHPOINT = 1
DIFFTEST = 1
ifeq ($(DEBUG),0)
COPTIMIZE = -O3
endif
VOPTIMIZE = -O3
CWARNING = -Wall
VWARNING = -Wall -Wno-UNUSEDSIGNAL

ifeq ($(NVBOARD),1)
RECORD_WAVE = 0
FSANITIZE = 0
endif
ifeq ($(RECORD_WAVE),0)
DISPLAY_WAVE = 0
endif
ifeq ($(TRACE),0)
ITRACE = 0
MTRACE = 0
FTRACE = 0
EXPR_MATCH = 0
endif
ifeq ($(DEBUG),1)
COPTIMIZE = -Og
endif
