#include "toy_stdio.h"


int main(int argc, char *argv[]) {
	
	int integers_array[] = {10,11,12,13,14};
	char* strings_array[] = {"aaa", "bbb", "ccc", "ddd", "eee"};
	int c_array[] = {50, 51, 52, 53, 54};
	int array_size = 5;
	int chars_printed=0;

//****signed and unsigned***
	chars_printed += toy_printf("Hex unsigned: %x\n", -1);
	chars_printed += toy_printf("Octal unsigned: %o\n", -1);
	chars_printed += toy_printf("Unsigned value: %u\n", 15);
	chars_printed += toy_printf("Unsigned value: %u\n", -15);
	chars_printed += toy_printf("Unsigned value: %b\n", 15);
	chars_printed += toy_printf("Unsigned value: %b\n", -15);
	chars_printed += toy_printf("Unsigned value: %o\n", 15);
	chars_printed += toy_printf("Unsigned value: %o\n", -15);
	chars_printed += toy_printf("Unsigned value: %x\n", 15);
	chars_printed += toy_printf("Unsigned value: %x\n", -15);
	chars_printed += toy_printf("Unsigned value: %X\n", 15);
	chars_printed += toy_printf("Unsigned value: %X\n", -15);

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
	chars_printed += toy_printf("W: %07d\n", -15);

	chars_printed += toy_printf("W: %07u\n", -15);
	chars_printed += toy_printf("W: %07b\n", -15);
	chars_printed += toy_printf("W: %07o\n", -15);
	chars_printed += toy_printf("W: %07x\n", -15);
	chars_printed += toy_printf("W: %07X\n", -15);
	chars_printed += toy_printf("W: %07c\n", 50);
	chars_printed += toy_printf("W: %011d\n", 15);
	chars_printed += toy_printf("W: %011u\n", 15);
	chars_printed += toy_printf("W: %011b\n", 15);
	chars_printed += toy_printf("W: %011o\n", 15);
	chars_printed += toy_printf("W: %011x\n", 15);
	chars_printed += toy_printf("W: %011X\n", 15);

}