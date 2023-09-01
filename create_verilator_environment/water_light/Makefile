VERILATOR = verilator
WAVE_READER = gtkwave
MAKE = make

VSRC_DIR = $(abspath vsrc)
VSRC = $(wildcard $(VSRC_DIR)/*.v)
CSRC_DIR = $(abspath csrc)
CSRC = $(wildcard $(CSRC_DIR)/*.cpp)
NXDC_DIR = $(abspath nxdc)
NXDC = $(wildcard $(NXDC_DIR)/*.nxdc)
BUILD_DIR = $(abspath build)
$(shell mkdir -p $(BUILD_DIR))
OBJ_DIR = $(BUILD_DIR)/obj_dir
TOP_NAME = light
BIN = $(BUILD_DIR)/V$(TOP_NAME)
WAVE = $(BUILD_DIR)/$(TOP_NAME).vcd

default: sim

CXXFLAGS = -DV_TOP_NAME="\"V$(TOP_NAME)\"" -Wall
VERILATOR_FLAGS = --cc --Mdir $(OBJ_DIR) --exe -o $(BIN) -Wall --top-module $(TOP_NAME) --trace $(addprefix -CFLAGS , $(CXXFLAGS))

gen: $(VSRC) $(CSRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $^

MAKE_FLAGS = -C $(OBJ_DIR) -f V$(TOP_NAME).mk

build: gen
	$(MAKE) $(MAKE_FLAGS) $(BIN)

sim: build
	$(BIN)
	$(WAVE_READER) $(WAVE)

SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS)
LDFLAGS = -lSDL2 -lSDL2_image
CSRC += $(SRC_AUTO_BIND)

nvgen: $(VSRC) $(CSRC) $(NVBOARD_ARCHIVE)
	$(VERILATOR) $(VERILATOR_FLAGS) $^ $(addprefix -LDFLAGS , $(LDFLAGS))

nvbuild: nvgen
	$(MAKE) $(MAKE_FLAGS) $(BIN)

nvsim: nvbuild
	$(BIN)

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : gen buid sim nvgen nvbluid nvsim clean

