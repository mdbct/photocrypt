# Variables
COMPILER 	= g++
CFLAGS 		= -std=c++14 `pkg-config --cflags --libs opencv`

# Make all
all: steg unsteg

# Make steg only
steg: steg.cc
	$(COMPILER) $(CFLAGS) $^ -o $@

# Make unsteg only
unsteg: unsteg.cc
	$(COMPILER) $(CFLAGS) $^ -o $@
