VERILATOR = verilator
WAVE_READER = gtkwave
ROOT_DIR = $(NPC_HOME)
VSRC_DIR = $(ROOT_DIR)/vsrc
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(ROOT_DIR)/csrc
CSRC = $(shell find $(CSRC_DIR) -name "main.cpp" -or -name "sim_tool.cpp" -or -name "$(TOP_NAME)*.cpp" -or -name "$(TOP_NAME)*.c")
NXDC_DIR = $(ROOT_DIR)/nxdc
NXDC = $(shell find $(NXDC_DIR) -name "*.v")
INCLUDE_DIR = $(ROOT_DIR)/include
INCLUDE = $(shell find $(INCLUDE_DIR) -name "*.vh" -or -name "*.hpp" -or -name "*.h")
MAKE_FILE = $(ROOT_DIR)/Makefile
BUILD_DIR = $(ROOT_DIR)/build
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir
NVBOARD = 0
DISPLAY_WAVE = 0
.DEFAULT_GOAL = sim



SRC_AUTO_BIND = $(BUILD_DIR)/auto_bind.cpp
ifeq ($(NVBOARD),1)
CSRC += $(SRC_AUTO_BIND)
endif

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk



PREFIX_NAME = Vtop
TOP_NAME = cpu
BIN = $(BUILD_DIR)/$(PREFIX_NAME)
VERILATOR_CFLAGS = -std=c++20 -Wall -DTOP_NAME=$(TOP_NAME)
VERILATOR_LDFLAGS = 
ifeq ($(TOP_NAME),cpu)
VERILATOR_CFLAGS += -DSIM_ALL
endif
ifeq ($(NVBOARD),0)
VERILATOR_CFLAGS +=
VERILATOR_LDFLAGS += -fsanitize=address
VERILATOR_SRC = $(VSRC) $(CSRC)
else
VERILATOR_CFLAGS += -I$(NVBOARD_HOME)/include -DNV_SIM
VERILATOR_LDFLAGS += -lSDL2 -lSDL2_image
VERILATOR_SRC = $(VSRC) $(CSRC) $(NVBOARD_ARCHIVE)
endif
VERILATOR_FLAGS = --cc --exe --Mdir $(OBJ_DIR) -y $(INCLUDE_DIR) --prefix $(PREFIX_NAME) --top-module $(TOP_NAME) -o $(BIN) -j 0 -Wall -Wno-UNUSEDSIGNAL --trace $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))
VERILATOR_FLAGS_LIB = $(addprefix -LDFLAGS , $(VERILATOR_LDFLAGS))
VERILATE = $(OBJ_DIR)/verilate.txt

verilate: $(VERILATE)

$(VERILATE): $(VERILATOR_SRC) $(INCLUDE) $(MAKE_FILE)
	@echo "-----verilate------"
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILATOR_SRC) $(VERILATOR_FLAGS_LIB)
	touch $(VERILATE)
	@echo "-----verilate done------"



VERILATOR_MAKE_FILE = $(OBJ_DIR)/$(PREFIX_NAME).mk
MAKE_FLAGS = -C $(OBJ_DIR) -f $(VERILATOR_MAKE_FILE) -j $(shell nproc)

build: $(BIN)

$(BIN): $(VERILATE)
	@echo "-----build------"
	$(MAKE) $(MAKE_FLAGS) $(BIN)
	@echo "-----build done------"



include ../Makefile
NPC_FLAGS =
WAVE = $(BUILD_DIR)/wave.vcd

sim: $(BIN)
	@echo "-----simulate------"
	$(BIN) $(NPC_FLAGS)
ifeq ($(NVBOARD),0)
ifeq ($(DISPLAY_WAVE),1)
	$(WAVE_READER) $(WAVE)
endif
endif
	$(call git_commit, "sim RTL") 
	@echo "-----simulate done------"



all: build

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : verilate build sim all clean
