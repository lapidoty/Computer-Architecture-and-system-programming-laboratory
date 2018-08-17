#include "toy_stdio.h"


int main(int argc, char *argv[]) {
	
	int integers_array[] = {10,11,12,13,14};
	char* strings_array[] = {"aaa", "bbb", "ccc", "ddd", "eee"};
	int c_array[] = {50, 51, 52, 53, 54};
	int array_size = 5;
	int chars_printed=0;

//****signed and unsigned***
	
	chars_printed += toy_printf("Unsigned value: %u\n", 15);
	chars_printed += toy_printf("Unsigned value: %u\n", -15);
	chars_printed += toy_printf("Binary value: %b\n", 15);
	chars_printed += toy_printf("Binary value: %b\n", -15);
	chars_printed += toy_printf("Octal value: %o\n", 15);
	chars_printed += toy_printf("Octal unsigned: %o\n", -1);
	chars_printed += toy_printf("Hex unsigned: %x\n", -1);
	chars_printed += toy_printf("Hex value: %x\n", 15);
	chars_printed += toy_printf("Hex value: %x\n", -15);
	chars_printed += toy_printf("Hex value: %X\n", 15);
	chars_printed += toy_printf("Hex value: %X\n", -15);
	
	

//****Arrays***

	chars_printed += toy_printf("P1: %Ad\n", integers_array, array_size);

	chars_printed += toy_printf("P1: %Au\n", integers_array, array_size);
	chars_printed += toy_printf("P1: %Ab\n", integers_array, array_size);
	chars_printed += toy_printf("P1: %Ao\n", integers_array, array_size);
	chars_printed += toy_printf("P1: %Ax\n", integers_array, array_size);
	chars_printed += toy_printf("P1: %AX\n", integers_array, array_size);
	
	chars_printed += toy_printf("P2: %As\n", strings_array, array_size);
	
	chars_printed += toy_printf("P3: %Ac\n", c_array, array_size);

//****width string***

	chars_printed += toy_printf("%s\n", "str");
	
	chars_printed += toy_printf("R: %5s\n", "str");
	
	chars_printed += toy_printf("L: %-5s\n", "str");

//****width numeric***

	chars_printed += toy_printf("W: %012d\n", 15);

	//toy_printf("Printed %d chars\n", chars_printed); 

	
	

}