VERILATOR = verilator
BUILD_TOOL = make
WAVE_READER = gtkwave
VSRC_DIR = $(abspath vsrc)
VSRC = $(shell find $(VSRC_DIR) -name "*.v")
CSRC_DIR = $(abspath csrc)
CSRC = $(shell find $(CSRC_DIR) -name "sim*.cpp" -or -name "$(TOP_NAME)_sim.cpp")
NXDC_DIR = $(abspath nxdc)
NXDC = $(wildcard $(NXDC_DIR)/*.nxdc)
BUILD_DIR = $(abspath build)
OBJ_DIR = $(BUILD_DIR)/obj_dir
PREFIX_NAME = Vtop
TOP_NAME = ps2_keyboard
BIN = $(BUILD_DIR)/$(PREFIX_NAME)
MAKE_FILE = $(OBJ_DIR)/$(PREFIX_NAME).mk
WAVE = $(BUILD_DIR)/wave.vcd
NVBOARD = 0
include ../../Makefile



default: sim



SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@



VERILATOR_CFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -g -I$(CSRC_DIR) -DTOP_NAME=$(TOP_NAME) 
VERILATOR_FLAGS = --cc --Mdir $(OBJ_DIR) --exe -o $(BIN) -Wall -Wpedantic --top-module $(TOP_NAME) --prefix $(PREFIX_NAME) --trace $(addprefix -CFLAGS , $(VERILATOR_CFLAGS))
NV_VERILATOR_CFLAGS=-I$(NVBOARD_HOME)/include -DNV_SIM
NV_VERILATOR_FLAGS=$(addprefix -CFLAGS , $(NV_VERILATOR_CFLAGS))
VERILATOR_LDFLAGS = -lSDL2 -lSDL2_image
NV_VERILATOR_FLAGS_2=$(addprefix -LDFLAGS , $(VERILATOR_LDFLAGS))

verilate: $(MAKE_FILE)
ifeq ($(NVBOARD),0)
$(MAKE_FILE): $(VSRC) $(CSRC)
else
$(MAKE_FILE): $(VSRC) $(CSRC) $(NVBOARD_HOME)/build/nvboard.a $(SRC_AUTO_BIND)
endif
	@echo "-----verilate------"
	mkdir -p $(BUILD_DIR)
ifeq ($(NVBOARD),0)
	$(VERILATOR) $(VERILATOR_FLAGS) $^ ; echo $$?
else
	$(VERILATOR) $(VERILATOR_FLAGS) $(NV_VERILATOR_FLAGS) $^ $(NV_VERILATOR_FLAGS_2) ; echo $$?
endif
	@echo "-----verilate done------"



MAKE_FLAGS = -C $(OBJ_DIR) -f $(MAKE_FILE)

build: $(BIN)
$(BIN): $(MAKE_FILE)
	@echo "-----build------"
	$(BUILD_TOOL) $(MAKE_FLAGS) $(BIN)
	@echo "-----build done------"



sim: $(BIN)
	@echo "-----simulate------"
	$(BIN)
ifeq ($(NVBOARD),0)
	$(WAVE_READER) $(WAVE)
endif
	$(call git_commit, "sim RTL in digital circuit") 
	@echo "-----simulate done------"



all: sim



clean: 
	rm -rf $(BUILD_DIR)



.PHONY : default verilate build sim all clean
