CC=c99
CFLAGS= -Wall -Wextra -pedantic -O0 -g -lm -Wno-unused-variable -Wno-unused-parameter
CLIBS= -lm
NASM=nasm
NASMFLAGS=-f elf64 -g -F DWARF

all: main ejemplos.o

main: main.c ejemplos.o checkpoint1.o checkpoint2.o checkpoints.h
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS) 

ejemplos.o: ejemplos.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS) 

checkpoint1.o: checkpoint1.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS) 

checkpoint2.o: checkpoint2.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS) 
	 

clean:
	rm -f *.o
	rm -f main
	rm -f salida.*

