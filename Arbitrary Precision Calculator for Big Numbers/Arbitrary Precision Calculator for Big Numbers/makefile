
all: calc


calc: calc.o operations.o
	gcc -g -Wall -o calc calc.o operations.o


calc.o: calc.c bigIntStack.h bigint.h
	gcc -g -Wall -c -o calc.o calc.c 
 
operations.o: operations.s
	nasm -g -f elf64 -w+all -o operations.o operations.s

.PHONY: clean


clean: 
	rm -f *.o calc

