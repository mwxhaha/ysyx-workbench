VERILATOR = verilator
WAVE_READER = gtkwave
MAKE = make

TOP_NAME = top
VSRCS = $(wildcard *.v)
CSRCS = $(wildcard *.cpp)
OBJ_DIR = obj_dir
WAVE_FILE = simx.vcd
NXDC_FILES = top.nxdc

CFLAGS = -DV_TOP_NAME="\"V$(TOP_NAME)\"" -Wall
VERILATOR_FLAGS = --cc --exe --trace -Wall --top-module $(TOP_NAME) $(addprefix -CFLAGS , $(CFLAGS))

gen: $(VSRCS) $(CSRCS)
	$(VERILATOR) $(VERILATOR_FLAGS) $^

MAKE_FLAGS = -C $(OBJ_DIR) -f V$(TOP_NAME).mk

build: gen
	$(MAKE) $(MAKE_FLAGS) V$(TOP_NAME)

sim: build
	$(OBJ_DIR)/V$(TOP_NAME)
	$(WAVE_READER) $(WAVE_FILE)

SRC_AUTO_BIND = auto_bind.cpp
CSRCS += $(SRC_AUTO_BIND)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS)
LDFLAGS = -lSDL2 -lSDL2_image

nvgen: $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	$(VERILATOR) $(VERILATOR_FLAGS) $^ $(addprefix -LDFLAGS , $(LDFLAGS))

nvbuild: nvgen
	$(MAKE) $(MAKE_FLAGS) V$(TOP_NAME)

nvsim: nvbuild
	$(OBJ_DIR)/V$(TOP_NAME)

clean: 
	rm -rf $(OBJ_DIR) $(WAVE_FILE) $(SRC_AUTO_BIND)

.PHONY : gen buid sim nvgen nvbluid nvsim clean
