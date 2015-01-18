COMPILER = clang++
CFLAGS = -std=c++14 `pkg-config --cflags --libs opencv`

all: steg unsteg

steg: steg.cc
	$(COMPILER) $(CFLAGS) $^ -o $@

unsteg: unsteg.cc
	$(COMPILER) $(CFLAGS) $^ -o $@
