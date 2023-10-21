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
ifeq ($(NVBOARD),0)
RECORD_WAVE = 1
FSANITIZE = 1
endif
ifeq ($(RECORD_WAVE),1)
DISPLAY_WAVE = 0
endif
DEBUG = 0
TRACE = 1
ifeq ($(TRACE),1)
ITRACE = 1
MTRACE = 0
EXPR_MATCH = 0
endif
WATCHPOINT = 1
DIFFTEST = 1
ifeq ($(DEBUG),0)
COPTIMIZE = -O3
VOPTIMIZE = -O3
endif

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
EXPR_MATCH = 0
endif
ifeq ($(DEBUG),1)
COPTIMIZE = -Og
VOPTIMIZE = -O0
endif



SRC_AUTO_BIND = $(BUILD_DIR)/auto_bind.cpp
ifeq ($(NVBOARD),1)
VERILATOR_SRC += $(SRC_AUTO_BIND)
endif



PREFIX_NAME = Vtop
BIN = $(BUILD_DIR)/$(PREFIX_NAME)
VERILATOR_CFLAGS = -std=c++20 -Wall -DTOP_NAME=$(TOP_NAME) -I$(INCLUDE_DIR) $(COPTIMIZE)
VERILATOR_LDFLAGS =  
VERILATOR_SRC = $(VSRC) $(CSRC)
VERILATOR_FLAGS = --cc --exe --Mdir $(OBJ_DIR) -y $(INCLUDE_DIR) --prefix $(PREFIX_NAME) --top-module $(TOP_NAME) -o $(BIN) -j $(VOPTIMIZE) -Wall -Wno-UNUSEDSIGNAL $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))
VERILATOR_FLAGS_LIB = $(addprefix -LDFLAGS , $(VERILATOR_LDFLAGS))
VERILATE = $(OBJ_DIR)/verilate.txt

ifeq ($(TOP_NAME),cpu)
VERILATOR_CFLAGS += -DSIM_ALL
VERILATOR_LDFLAGS += -lreadline
endif

ifeq ($(ISA),RV32I)
VERILATOR_CFLAGS += -DCONFIG_ISA=0
else ifeq ($(ISA),RV32E)
VERILATOR_CFLAGS += -DCONFIG_ISA=1
else ifeq ($(ISA),RV64I)
VERILATOR_CFLAGS += -DCONFIG_ISA=2
else
$(error "do not support ISA $(ISA)")
endif

ifeq ($(NVBOARD),1)
VERILATOR_CFLAGS += -I$(NVBOARD_HOME)/include -DNV_SIM
VERILATOR_LDFLAGS += -lSDL2 -lSDL2_image
VERILATOR_SRC += $(NVBOARD_ARCHIVE)
endif

ifeq ($(FSANITIZE),1)
VERILATOR_CFLAGS += -fsanitize=address
VERILATOR_LDFLAGS += -fsanitize=address
endif

ifeq ($(DEBUG),1)
VERILATOR_CFLAGS += -g
VERILATOR_FLAGS += --debug
endif

ifeq ($(RECORD_WAVE),1)
VERILATOR_CFLAGS += -DCONFIG_RECORD_WAVE
VERILATOR_FLAGS += --trace
endif

ifeq ($(ITRACE),1)
VERILATOR_CFLAGS += -I/usr/lib/llvm-14/include -DCONFIG_ITRACE
VERILATOR_LDFLAGS += -lLLVM-14
endif

ifeq ($(MTRACE),1)
VERILATOR_CFLAGS += -DCONFIG_MTRACE
endif

ifeq ($(EXPR_MATCH),1)
VERILATOR_CFLAGS += -DCONFIG_EXPR_MATCH
endif

ifeq ($(WATCHPOINT),1)
VERILATOR_CFLAGS += -DCONFIG_WATCHPOINT
endif

ifeq ($(DIFFTEST),1)
VERILATOR_CFLAGS += -DCONFIG_DIFFTEST
endif



VERILATOR_MAKE_FILE = $(OBJ_DIR)/$(PREFIX_NAME).mk
MAKE_FLAGS = -C $(OBJ_DIR) -f $(VERILATOR_MAKE_FILE) -j $(shell nproc)



NPC_FLAGS =
WAVE = $(BUILD_DIR)/wave.vcd
