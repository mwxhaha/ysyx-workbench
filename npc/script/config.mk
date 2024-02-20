VERILATOR = verilator
WAVE_READER = gtkwave
ROOT_DIR = $(NPC_HOME)
VSRC_DIR = $(ROOT_DIR)/vsrc
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(ROOT_DIR)/csrc
CSRC = $(shell find $(CSRC_DIR) -name "*.cpp")
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
STUDENT_ID = ysyx_23060075

TOP_NAME = cpu
ISA = RV32E
NVBOARD = 0
HAVE_CLK = 1
RECORD_WAVE = 1
ifeq ($(RECORD_WAVE),1)
DISPLAY_WAVE = 0
endif
ifeq ($(DEBUG),0)
COPTIMIZE = -O3
endif
LINKOPT = 1
DEBUG = 0
FSANITIZE = 1
TRACE = 1
ifeq ($(TRACE),1)
EXPR_MATCH = 0
ITRACE = 1
MTRACE = 1
FTRACE = 1
DTRACE = 1
endif
WATCHPOINT = 1
DIFFTEST = 1
MEM_RANDOM = 1
DEVICE = 1
ifeq ($(DEVICE),1)
HAS_SERIAL = 1
HAS_TIMER = 1
HAS_KEYBOARD = 1
HAS_VGA = 1
HAS_AUDIO = 1
endif
ifeq ($(HAS_VGA),1)
VGA_SHOW_SCREEN = 1
endif
VOPTIMIZE = -O3
CWARNING = -Wall
VWARNING = -Wall -Wno-fatal

ifeq ($(RECORD_WAVE),0)
DISPLAY_WAVE = 0
endif
ifeq ($(DEBUG),1)
COPTIMIZE = -Og
endif
ifeq ($(TRACE),0)
EXPR_MATCH = 0
ITRACE = 0
MTRACE = 0
FTRACE = 0
DTRACE = 0
endif
ifeq ($(DEVICE),0)
HAS_SERIAL = 0
HAS_TIMER = 0
HAS_KEYBOARD = 0
HAS_VGA = 0
HAS_AUDIO = 0
endif
ifeq ($(HAS_VGA),0)
VGA_SHOW_SCREEN = 0
endif
