VERILATOR = verilator
BUILD_TOOL = make
WAVE_READER = gtkwave
VSRC_DIR = $(abspath vsrc)
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(abspath csrc)
CSRC = $(shell find $(CSRC_DIR) -name "sim*.cpp" -or -name "$(TOP_NAME)*.cpp")
NXDC_DIR = $(abspath nxdc)
NXDC = $(wildcard $(NXDC_DIR)/*.nxdc)
BUILD_DIR = $(abspath build)
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir
TOP_NAME = fsm_bin
BIN = $(BUILD_DIR)/Vtop
WAVE = $(BUILD_DIR)/wave.vcd
include ../../Makefile

default: sim

VERILATOR_CFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -g -I$(CSRC_DIR) -DTOP_NAME=$(TOP_NAME) 
VERILATOR_FLAGS = --cc --Mdir $(OBJ_DIR) --exe -o $(BIN) -Wall -Wpedantic --top-module $(TOP_NAME) --prefix Vtop --trace $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))

gen: $(VSRC) $(CSRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $^ ; echo $$?

MAKE_FLAGS = -C $(OBJ_DIR) -f Vtop.mk

build: gen
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)

sim: build
	$(BIN)
	$(WAVE_READER) $(WAVE)
	$(call git_commit, "sim RTL in digital circuit") 

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

NV_VERILATOR_CFLAGS=-I$(NVBOARD_HOME)/include -DNV_SIM
NV_VERILATOR_FLAGS=$(addprefix -CFLAGS , $(NV_VERILATOR_CFLAGS))
VERILATOR_LDFLAGS = -lSDL2 -lSDL2_image
NV_VERILATOR_FLAGS_2=$(addprefix -LDFLAGS , $(VERILATOR_LDFLAGS))

nvgen: $(VSRC) $(CSRC) $(NVBOARD_HOME)/build/nvboard.a $(SRC_AUTO_BIND)
	$(VERILATOR) $(VERILATOR_FLAGS) $(NV_VERILATOR_FLAGS) $^ $(NV_VERILATOR_FLAGS_2) ; echo $$?

nvbuild: nvgen
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)

nvsim: nvbuild
	$(BIN)
	$(call git_commit, "nvsim RTL in digital circuit") 

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : default gen build sim nvgen nvbuild nvsim clean
