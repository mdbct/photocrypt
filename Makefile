###################################### 
##
## The project Makefile.
##
######################################


##### Macros

HDIR      := inc
CDIR      := src

SOURCES   := MatImage.cc TextFile.cc util.cc

LIBRARIES := gtkmm-2.4 opencv openssl
CC        := g++
CFLAGS    := -O -std=c++14 `pkg-config --cflags --libs $(LIBRARIES)` -I$(HDIR)

vpath %.h $(HDIR)
vpath %.cc $(CDIR)


##### Rules

all: steg unsteg gui

steg: $(SOURCES) steg.cc
	$(CC) $(CFLAGS) $^ -o $@

unsteg: $(SOURCES) unsteg.cc
	$(CC) $(CFLAGS) $^ -o $@

gui: $(SOURCES) main.cc Win.cc
	$(CC) $(CFLAGS) $^ -o $@

$(CDIR)/%.cc: $(HDIR)/*.h

clean:
	@if [ -f steg ]; then rm steg; fi
	@if [ -f unsteg ]; then rm unsteg; fi
	@if [ -f gui ]; then rm gui; fi

help:
	@echo 	"The Makefile defines the following target:"
	@echo 	"   all    : Builds everything"
	@echo   "   steg   : Builds the steg program"
	@echo 	"   unsteg : Builds the unsteg program"
	@echo   "   gui    : Builds the graphical program"
	@echo	"   clean  : Cleans the built files"
	@echo	"   help   : Displays this help text"

