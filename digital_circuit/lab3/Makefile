VERILATOR = verilator
BUILD_TOOL = make
WAVE_READER = gtkwave
VSRC_DIR = $(abspath vsrc)
VSRC = $(wildcard $(VSRC_DIR)/*.v)
CSRC_DIR = $(abspath csrc)
CSRC = $(wildcard $(CSRC_DIR)/*.cpp)
NXDC_DIR = $(abspath nxdc)
NXDC = $(wildcard $(NXDC_DIR)/*.nxdc)
BUILD_DIR = $(abspath build)
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir
TOP_NAME = adder8
BIN = $(BUILD_DIR)/V$(TOP_NAME)
WAVE = $(BUILD_DIR)/wave.vcd
include ../../Makefile

default: sim

VERILATOR_CFLAGS = -DV_TOP_NAME="\"V$(TOP_NAME)\"" -Wall
VERILATOR_FLAGS = --cc --Mdir $(OBJ_DIR) --exe -o $(BIN) -Wall --top-module $(TOP_NAME) --trace $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))

gen: $(VSRC) $(CSRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $^ ; echo $$?

MAKE_FLAGS = -C $(OBJ_DIR) -f V$(TOP_NAME).mk

build: gen
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)

sim: build
	$(BIN)
	$(WAVE_READER) $(WAVE)
	$(call git_commit, "sim RTL in digital circuit") 

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk
INCFLAGS = $(addprefix -I, $(INC_PATH))
NV_VERILATOR_CFLAGS=$(INCFLAGS) -DNV_SIM
NV_VERILATOR_FLAGS=$(addprefix -CFLAGS , $(NV_VERILATOR_CFLAGS))
LDFLAGS = -lSDL2 -lSDL2_image
NV_VERILATOR_FLAGS_2=$(addprefix -LDFLAGS , $(LDFLAGS))

nvgen: $(VSRC) $(CSRC) $(NVBOARD_ARCHIVE) $(SRC_AUTO_BIND)
	$(VERILATOR) $(VERILATOR_FLAGS) $(NV_VERILATOR_FLAGS) $^ $(NV_VERILATOR_FLAGS_2)

nvbuild: nvgen
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)

nvsim: nvbuild
	$(BIN)
	$(call git_commit, "nvsim RTL in digital circuit") 

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : default gen buid sim nvgen nvbluid nvsim clean
