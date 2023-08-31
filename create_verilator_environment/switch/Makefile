VERILATOR = verilator
WAVEREADER = gtkwave

CFLAGS = -Wall -Wpedantic
VFLAGS = --cc --exe --build -j 0 --trace

TOPFILE = top
SIMFILE = sim_main.cpp
OBJDIR = obj_dir
WAVEFILE = simx.vcd

sim: $(TOPFILE).v $(SIMFILE)
# sim:
	$(VERILATOR) $(VFLAGS) $(CFLAGS) $(TOPFILE).v $(SIMFILE)
	$(OBJDIR)/V$(TOPFILE)
	$(WAVEREADER) $(WAVEFILE)

.PHONY : sim
