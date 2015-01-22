#	
#	Makefile, to build the test GUI program.
#
#	This GUI is written in gtkmm (GTK+ binding for C++).
#
################################################################



##################################### MACROS

TARGET := gui
SOURCES := main.cc Win.cc
OBJECTS := $(SOURCES:.cc=.o)

CC      := g++
CFLAGS  := `pkg-config --cflags gtkmm-2.4` -std=c++14 -Wall -O
LFLAGS  := `pkg-config --libs gtkmm-2.4`



##################################### RULES

## Build the target

$(TARGET): $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@


## Build the objects

$(OBJECTS): %.o: %.cc
	$(CC) $(CFLAGS) $< -c

main.o: Win.h
Win.o:	Win.h info.h


## Clean

clean:
	rm -rf $(TARGET) $(OBJECTS)
