###################################### 
##
## The project Makefile.
##
######################################

HDIR      := inc
ODIR      := obj
CDIR      := src
DESTDIR   := 

SOURCES   := MatImage.cc TextFile.cc util.cc Error.cc
OBJECTS   := $(SOURCES:%.cc=$(ODIR)/%.o)

LIBRARIES := gtkmm-2.4 opencv openssl
CC        := g++
CFLAGS    := -O `pkg-config --cflags $(LIBRARIES)` -I $(HDIR) -std=c++11
LFLAGS    := -O `pkg-config --libs $(LIBRARIES)` -std=c++11

vpath %.h $(HDIR)
vpath %.o $(ODIR)
vpath %.cc $(CDIR)


###################################################### Rules

all: cli gui

cli: steg unsteg

gui: photocrypt

photocrypt: $(OBJECTS) $(ODIR)/main.o $(ODIR)/Win.o
	@echo ":: Making $@"
	@$(CC) $^ -o $@ $(LFLAGS)

steg: $(OBJECTS) $(ODIR)/steg.o
	@echo ":: Making $@"
	@$(CC) $^ -o $@ $(LFLAGS)

unsteg: $(OBJECTS) $(ODIR)/unsteg.o
	@echo ":: Making $@"
	@$(CC) $^ -o $@ $(LFLAGS)

$(ODIR)/%.o: %.cc $(HDIR)/*.h
	@echo ":: Compiling $<"
	@mkdir -p $(ODIR)
	@$(CC) $(CFLAGS) $< -c -o $@

clean: clean-exe clean-obj clean-doc

clean-exe:
	@echo ":: Cleaning executables"
	@rm -f photocrypt steg unsteg

clean-obj:
	@echo ":: Cleaning objects"
	@rm -rf $(ODIR)

install: uninstall all
	@echo ":: Installing"
	@mkdir -p "$(DESTDIR)/opt/photocrypt"
	@mkdir -p "$(DESTDIR)/usr/bin"
	@cp photocrypt steg unsteg README.md icon.png $(DESTDIR)/opt/photocrypt/
	@ln -s /opt/photocrypt/photocrypt $(DESTDIR)/usr/bin/photocrypt
	@ln -s /opt/photocrypt/steg       $(DESTDIR)/usr/bin/steg
	@ln -s /opt/photocrypt/unsteg     $(DESTDIR)/usr/bin/unsteg

uninstall:
	@echo ":: Uninstalling"
	@rm -rf $(DESTDIR)/opt/photocrypt
	@rm -f $(DESTDIR)/usr/bin/photocrypt
	@rm -f $(DESTDIR)/usr/bin/steg
	@rm -f $(DESTDIR)/usr/bin/unsteg

doc: $(HDIR)/* $(CDIR)/* README.md Doxyfile
	@doxygen 1>/dev/null 2>&1
	@echo ":: Documentation generated."

clean-doc:
	@echo ":: Cleaning docs"
	@rm -rf doc

help:
	@echo "The Makefile defines the following target:"
	@echo "   make            : Builds the GUI program"
	@echo "   make cli        : Builds the CLI programs (steg & unsteg)"
	@echo "   make gui        : Builds the GUI program (photocrypt)"
	@echo "   make clean      : Cleans the built files"
	@echo "   make install    : Installs the program into the system"
	@echo "   make uninstall  : Uninstalls the program from the system"
	@echo "   make doc        : Generates documentation in html"
	@echo "   make help       : Displays this help text"

