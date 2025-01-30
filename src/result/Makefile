CXX = gcc

CFLAGSDB = -std=c17 -O2 -g

RESULTDIR = ./
BUILD = test

all: compile run
compile: $(RESULTDIR)test.c
	$(CXX) $(CFLAGSDB) -o $(RESULTDIR)$(BUILD) $(RESULTDIR)test.c $(RESULTDIR)result.c

run: $(RESULTDIR)$(BUILD)
	$(RESULTDIR)$(BUILD)

clean: $(RESULTDIR)$(BUILD) 
	rm $(RESULTDIR)$(BUILD)
