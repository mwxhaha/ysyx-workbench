include script/config.mk
.DEFAULT_GOAL = all



$(SRC_AUTO_BIND): $(NXDC)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@



include $(NVBOARD_HOME)/scripts/nvboard.mk

verilate: $(VERILATE)

$(VERILATE): $(VERILATOR_SRC) $(INCLUDE) $(MAKE_FILE)
	@echo "-----verilate------"
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILATOR_SRC) $(VERILATOR_FLAGS_LIB)
	touch $(VERILATE)
	@echo "-----verilate done------"



build: $(BIN)

$(BIN): $(VERILATE)
	@echo "-----build------"
	$(MAKE) $(MAKE_FLAGS) $(BIN)
	@echo "-----build done------"



include ../Makefile

sim: $(BIN)
	@echo "-----simulate------"
	$(BIN) $(NPC_FLAGS) $(IMG)
ifeq ($(DISPLAY_WAVE),1)
	$(WAVE_READER) $(WAVE)
endif
	$(call git_commit, "sim RTL") 
	@echo "-----simulate done------"



all: build

clean: 
	rm -rf $(BUILD_DIR)

.PHONY : verilate build sim all clean
