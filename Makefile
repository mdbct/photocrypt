CC     = g++
CFLAGS = -O -std=c++14 `pkg-config --cflags opencv openssl`
LFLAGS = -O -std=c++14 `pkg-config --libs opencv openssl`

all: steg unsteg

steg: steg.o encrypt.o bit.o
	$(CC) $(LFLAGS) $^ -o $@

unsteg: unsteg.o encrypt.o bit.o
	$(CC) $(LFLAGS) $^ -o $@

%.o: %.cc *.h
	$(CC) $(CFLAGS) $< -c

clean:
	rm steg unsteg *.o
