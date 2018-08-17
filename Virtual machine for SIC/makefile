
all: sic


sic: sic.o
	gcc -g -Wall -o sic sic.o 

sic.o: sic.s
	nasm -g -f elf64 -w+all -o sic.o sic.s

.PHONY: clean


clean: 
	rm -f *.o sic


