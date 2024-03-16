include $(NPC_HOME)/script/arg.mk

.DEFAULT_GOAL = all

$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk

verilate: $(VERILATE)
$(VERILATE): $(VERILATOR_SRC) $(INCLUDE) $(MAKE_FILE)
	@echo "-----verilate-----"
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILATOR_SRC) $(VERILATOR_FLAGS_LIB)
	touch $(VERILATE)
	@echo "-----verilate done-----"

all: build
build: $(BIN)
$(BIN): $(VERILATE)
	@echo "-----build-----"
	$(MAKE) $(MAKE_FLAGS) $(BIN)
	@echo "-----build done-----"

include ../Makefile

run: sim
sim: $(BIN)
	@echo "-----simulate-----"
	$(NPC_EXEC)
ifeq ($(DISPLAY_WAVE),1)
	$(WAVE_READER) $(WAVE)
endif
	$(call git_commit, "sim RTL") 
	@echo "-----simulate done-----"

sta: build
	@echo "-----sta-----"
	$(MAKE) $(STA_FLAG) sta
	@echo "-----sta done-----"

gdb: build
	@echo "do not support make gdb"
clean: 
	rm -rf $(BUILD_DIR)
.PHONY : verilate all build run sim sta gdb clean
