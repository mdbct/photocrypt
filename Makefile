###################################### 
##
## The project Makefile.
##
######################################

##### Macros

HDIR      := inc
ODIR      := obj
CDIR      := src

SOURCES   := MatImage.cc TextFile.cc util.cc Error.cc
OBJECTS   := $(SOURCES:%.cc=$(ODIR)/%.o)

LIBRARIES := gtkmm-2.4 opencv openssl
CC        := g++
CFLAGS    := -O `pkg-config --cflags $(LIBRARIES)` -I $(HDIR)
LFLAGS    := -O `pkg-config --libs $(LIBRARIES)`

vpath %.h $(HDIR)
vpath %.o $(ODIR)
vpath %.cc $(CDIR)


##### Rules

photocrypt: $(OBJECTS) $(ODIR)/main.o $(ODIR)/Win.o
	$(CC) $^ -o $@ $(LFLAGS)

cli: steg unsteg

all: photocrypt cli

steg: $(OBJECTS) $(ODIR)/steg.o
	$(CC) $^ -o $@ $(LFLAGS)

unsteg: $(OBJECTS) $(ODIR)/unsteg.o
	$(CC) $^ -o $@ $(LFLAGS)

$(ODIR)/%.o: %.cc $(HDIR)/*.h
	$(CC) $(CFLAGS) $< -c -o $@

install: all uninstall
	cp photocrypt steg unsteg README.md /opt/photocrypt/
	ln -s /opt/photocrypt/photocrypt /usr/local/bin/photocrypt
	ln -s /opt/photocrypt/steg       /usr/local/bin/steg
	ln -s /opt/photocrypt/unsteg     /usr/local/bin/unsteg

uninstall:
	rm -rf /opt/photocrypt
	rm -f /usr/local/bin/photocrypt
	rm -f /usr/local/bin/steg
	rm -f /usr/local/bin/unsteg

clean: clean-exe clean-obj

clean-exe:
	rm -f photocrypt steg unsteg

clean-obj:
	rm -f $(ODIR)/*.o

help:
	@echo "The Makefile defines the following target:"
	@echo "   make            : Builds the GUI program"
	@echo "   make cli        : Builds the CLI programs (steg & unsteg)"
	@echo "   make steg       : Builds the CLI steg program"
	@echo "   make unsteg     : Builds the CLI unsteg program"
	@echo "   make install    : Installs the program into the system"
	@echo "   make uninstall  : Uninstalls the program from the system"
	@echo "   make all        : Builds everything"
	@echo "   make clean      : Cleans the built files"
	@echo "   make help       : Displays this help text"

