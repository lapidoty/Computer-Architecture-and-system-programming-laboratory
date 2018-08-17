
all: root


root: calc.o
	gcc -g -Wall -o root complex_root.o 

calc.o: complex_root.s
	nasm -g -f elf64 -w+all -o complex_root.o complex_root.s

.PHONY: clean


clean: 
	rm -f *.o root


