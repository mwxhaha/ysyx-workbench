VERILATOR = verilator
BUILD_TOOL = make
WAVE_READER = gtkwave
ROOT_DIR = $(NPC_HOME)
VSRC_DIR = $(ROOT_DIR)/vsrc
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(ROOT_DIR)/csrc
CSRC = $(shell find $(CSRC_DIR) -name "main.cpp" -or -name "sim*.cpp" -or -name "$(TOP_NAME)_sim.cpp")
NXDC_DIR = $(ROOT_DIR)/nxdc
NXDC = $(wildcard $(NXDC_DIR)/*.nxdc)
BUILD_DIR = $(ROOT_DIR)/build
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir
PREFIX_NAME = Vtop
TOP_NAME = ifu
BIN = $(BUILD_DIR)/$(PREFIX_NAME)
MAKE_FILE = $(OBJ_DIR)/$(PREFIX_NAME).mk
WAVE = $(BUILD_DIR)/wave.vcd
NVBOARD = 0
.DEFAULT_GOAL = sim



SRC_AUTO_BIND = $(BUILD_DIR)/auto_bind.cpp
ifeq ($(NVBOARD),1)
CSRC += $(SRC_AUTO_BIND)
endif

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk



VERILATOR_CFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -g -DTOP_NAME=$(TOP_NAME)
VERILATOR_LDFLAGS = 
ifeq ($(NVBOARD),0)
VERILATOR_CFLAGS +=
VERILATOR_LDFLAGS += -fsanitize=address
else
VERILATOR_CFLAGS += -I$(NVBOARD_HOME)/include -DNV_SIM
VERILATOR_LDFLAGS += -lSDL2 -lSDL2_image
endif
VERILATOR_FLAGS = --cc --Mdir $(OBJ_DIR) -j 0 --exe -o $(BIN) -Wall -Wpedantic --top-module $(TOP_NAME) --prefix $(PREFIX_NAME) --trace $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))
VERILATOR_FLAGS_LIB = $(addprefix -LDFLAGS , $(VERILATOR_LDFLAGS))

ifeq ($(NVBOARD),0)
verilate: $(VSRC) $(CSRC)
else
verilate: $(VSRC) $(CSRC) $(NVBOARD_ARCHIVE)
endif
	@echo "-----verilate------"
	- $(VERILATOR) $(VERILATOR_FLAGS) $^ $(VERILATOR_FLAGS_LIB)
	@echo "-----verilate done------"



MAKE_FLAGS = -C $(OBJ_DIR) -f $(MAKE_FILE) -j $(shell nproc)

build: $(BIN)

$(BIN): verilate
	@echo "-----build------"
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)
	@echo "-----build done------"



include ../Makefile

sim: $(BIN)
	@echo "-----simulate------"
	$(BIN)
ifeq ($(NVBOARD),0)
	$(WAVE_READER) $(WAVE)
endif
	$(call git_commit, "sim RTL") 
	@echo "-----simulate done------"



all: build

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : verilate build sim all clean
