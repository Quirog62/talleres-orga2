CC=c99
CFLAGS= -Wall -Wextra -pedantic -O0 -g -lm -Wno-unused-variable -Wno-unused-parameter
NASM=nasm
NASMFLAGS=-f elf64 -g -F DWARF

all: main checkpoint2_asm.o checkpoint3_asm.o

main: main.c  checkpoint2_c.o  checkpoint2_asm.o  checkpoint3_c.o  checkpoint3_asm.o  extras_c.o  extras_asm.o
	$(CC) $(CFLAGS) $^ -o $@

checkpoint2_c.o: checkpoint2.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

checkpoint2_asm.o: checkpoint2.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

checkpoint3_c.o: checkpoint3.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

checkpoint3_asm.o: checkpoint3.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

extras_c.o: extras.c checkpoints.h
	$(CC) $(CFLAGS) -c $< -o $@

extras_asm.o: extras.asm checkpoints.h
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f main
	rm -f salida.caso.*

