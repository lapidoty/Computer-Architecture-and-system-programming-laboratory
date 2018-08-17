#include "toy_stdio.h"
void init_ops_array_digits(void* ops_array[]);

int main(int argc, char *argv[]) {
	
	int integers_array[] = {10,11,12,13,14};
	char* strings_array[] = {"aaa", "bbb", "ccc", "ddd", "eee"};
	int c_array[] = {50, 51, 52, 53, 54};
	int array_size = 5;
	int chars_printed=0;

//****signed and unsigned***
	
	chars_printed += toy_printf("Signed value: %d\n", 15);
	chars_printed += toy_printf("Unsigned value: %u\n", 15);
	chars_printed += toy_printf("Unsigned value: %u\n", -15);
	chars_printed += toy_printf("Binary value: %b\n", 15);
	chars_printed += toy_printf("Binary value: %b\n", -15);
	chars_printed += toy_printf("Octal value: %o\n", 15);
	chars_printed += toy_printf("Octal unsigned: %o\n", -1);
	chars_printed += toy_printf("Hex unsigned: %x\n", -1);
	chars_printed += toy_printf("Hex value: %x\n", 15);
	chars_printed += toy_printf("Hex unsigned: %x\n", -15);

	

//****Arrays***

	chars_printed += toy_printf("Signed array: %Ad\n", integers_array, array_size);
	chars_printed += toy_printf("Unsigned array: %Au\n", integers_array, array_size);
	chars_printed += toy_printf("Binary array: %Ab\n", integers_array, array_size);
	chars_printed += toy_printf("Octal array: %Ao\n", integers_array, array_size);
	chars_printed += toy_printf("Hex array: %Ax\n", integers_array, array_size);
	chars_printed += toy_printf("String array: %As\n", strings_array, array_size);
	chars_printed += toy_printf("Char array:: %Ac\n", c_array, array_size);


//****width numeric***

	chars_printed += toy_printf("W: %010d\n", -15);

	//****width string***

	chars_printed += toy_printf("%s\n", "str");
	chars_printed += toy_printf("%10s\n", "str");
	chars_printed += toy_printf("%-15s\n", "str");


	//toy_printf("Printed %d chars\n", chars_printed); 

	
	

}