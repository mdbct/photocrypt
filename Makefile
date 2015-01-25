###################################### 
##
## The project Makefile.
##
######################################

##### Macros

SOURCES   := Image.cc TextFile.cc util.cc
OBJECTS   := $(SOURCES:.cc=.o)
LIBRARIES := gtkmm-2.4 opencv openssl
CC        := g++
CFLAGS    := -O -std=c++14 `pkg-config --cflags $(LIBRARIES)`
LFLAGS    := -O -std=c++14 `pkg-config --libs $(LIBRARIES)`


##### Rules

all: steg unsteg

steg: $(OBJECTS) steg.o
	$(CC) $(LFLAGS) $^ -o $@

unsteg: $(OBJECTS) unsteg.o
	$(CC) $(LFLAGS) $^ -o $@

%.o: %.cc *.h
	$(CC) $(CFLAGS) $< -c

clean:
	rm steg unsteg *.o

help:
	@echo 	"The Makefile defines the following target:"
	@echo 	"   all    : Builds everything"
	@echo   "   steg   : Builds the steg program"
	@echo 	"   unsteg : Builds the unsteg program"
	@echo	"   clean  : Cleans the built files"
	@echo	"   help   : Displays this help text"
