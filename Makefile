CXX = g++

CFLAGSDB = -O2 -g
LDFLAGS = -lglfw -lvulkan -lpthread -lX11 -lXi -lGLESv2

SRCDIR = ./src/

BUILDDIR = ./build/
BUILD = cpong

all: compile run
compile: $(SRCDIR)main.cpp
	$(CXX) $(CFLAGSDB) -o $(BUILDDIR)$(BUILD) $(SRCDIR)main.cpp $(LDFLAGS)

run: $(BUILDDIR)$(BUILD)
	$(BUILDDIR)$(BUILD)

clean: $(BUILDDIR)$(BUILD) 
	rm $(BUILDDIR)$(BUILD)

test:
	make -C $(SRCDIR)result/
