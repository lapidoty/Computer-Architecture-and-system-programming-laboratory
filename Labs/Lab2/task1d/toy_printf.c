/* caspl-lab-1.c
 * Limited versions of printf
 *
 * Programmer: Mayer Goldberg, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* the states in the printf state-machine */
enum printf_state {
	st_printf_init,
	st_printf_meta_char,
	st_printf_percent,
	st_printf_octal2,
	st_printf_octal3
};

#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')

int toy_printf(char *fs, ...);

const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";
int numeric_view=0;
int numeric_value=0;
int number_of_placeholders=0;
int print_spaces=0;
int print_from_start=0;
int print_zeros=0;
int print_width=0;
int wasPrinting=0;
int numeric_len =0;
int len = 0;
int first_time=1;
int is_not_numric_view = 0;
char c;
char* s_value;
va_list args;
int chars_printed = 0;

int print_int_helper(int n, int radix, const char *digit) {
	int result;

	if (n < radix) {
		putchar(digit[n]);
		return 1;
	}
	else {
		result = print_int_helper(n / radix, radix, digit);


		putchar(digit[n % radix]);
		return 1 + result;
	}
}

int print_int(int n, int radix, const char * digit) {
	if (radix < 2 || radix > 16) {
		toy_printf("Radix must be in [2..16]: Not %d\n", radix);
		exit(-1);
	}

	if (n > 0) {
		return print_int_helper(n, radix, digit);
	}
	if (n == 0) {
		putchar('0');
		return 1;
	}
	else {
		putchar('-');
		return 1 + print_int_helper(-n, radix, digit);
	}
}
int print_unsigned_int_helper(unsigned int n, int radix, const char *digit) {
	int result;

	if (n < radix) {
		putchar(digit[n]);
		return 1;
	}
	else {
		result = print_unsigned_int_helper(n / radix, radix, digit);


		putchar(digit[n % radix]);
		return 1 + result;
	}
}

int print_unsigned_int(unsigned int n, int radix, const char * digit) {
	if (radix < 2 || radix > 16) {
		toy_printf("Radix must be in [2..16]: Not %d\n", radix);
		exit(-1);
	}

	if (n > 0) {
		return print_unsigned_int_helper(n, radix, digit);
	}
	if (n == 0) {
		putchar('0');
		return 1;
	}
	else {
		putchar('-');
		return 1 + print_unsigned_int_helper(-n, radix, digit);
	}
}
int count_signed_int_helper(int n, int radix, const char *digit) {
	int result;

	if (n < radix) {
		return 1;
	}
	else {
		result = count_signed_int_helper(n / radix, radix, digit);


		return 1 + result;
	}
}

int count_signed_int(int n, int radix, const char * digit) {
	if (n > 0) {
		return count_signed_int_helper(n, radix, digit);
	}
	if (n == 0) {
		return 1;
	}
	else {
		return 1 + count_signed_int_helper(-n, radix, digit);
	}
}
int count_unsigned_int_helper(unsigned int n, int radix, const char *digit) {
	int result;

	if (n < radix) {
		return 1;
	}
	else {
		result = count_unsigned_int_helper(n / radix, radix, digit);


		return 1 + result;
	}
}

int count_unsigned_int(unsigned int n, int radix, const char * digit) {
	if (n > 0) {
		return count_unsigned_int_helper(n, radix, digit);
	}
	if (n == 0) {
		return 1;
	}
	else {
		return 1 + count_unsigned_int_helper(-n, radix, digit);
	}
}
/* SUPPORTED:
 *   \f, \t, \n, \r -- formfeed, tab, newline, return
 *   \F, \T, \N, \R -- extensions: visible versions of the above
 *   \c -- extension: CASPL'2018
 *   \C -- extension: visible version of the above
 *   %b, %d, %o, %x, %X -- 
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */

void print_placeholders(unsigned int n, int radix, const char *digit, int is_char, int is_string , int is_unsigned){
	
	if(print_width==1){
		if(is_string==1){
		number_of_placeholders = number_of_placeholders - len;
	}

	else{
		if(is_char==0){
			if(is_unsigned==1){
		numeric_len = count_unsigned_int(n,radix,digit);
	}

	else{
		
		numeric_len = count_signed_int(numeric_value,radix,digit);
		
	}

	}
	else{

		numeric_len = 1;
	}
		
	number_of_placeholders = number_of_placeholders - numeric_len;


}
		if(print_from_start==1){

			if((print_spaces==1) && (print_zeros==0)){
				for(int i=0; i<number_of_placeholders;++i){
					putchar(' ');
					++chars_printed;
				}
			}

			if((print_spaces==1) && (print_zeros==1)){
				if(numeric_view==1){
					if(numeric_value < 0){
						putchar('-');
						++chars_printed;

		if(is_unsigned==0){
			
			numeric_value=numeric_value*(-1);
		}
						

					}
				
				}
				
	if(number_of_placeholders<0){
		return;
	}
				for(int i=0; i<number_of_placeholders;++i){
					putchar('0');
					++chars_printed;
				}
				
			}
			print_from_start=0;
			print_spaces=0;

		}
		else{

			if((print_spaces==1) && (print_zeros==0)){
				for(int i=0; i<number_of_placeholders;++i){
					putchar(' ');
					++chars_printed;
				}
				putchar('#');
				++chars_printed;
				print_spaces=0;
			}
		}
		number_of_placeholders=0;
		
	}
}

int stringLen(char* s){
	int i;
	for(i = 0; s[i] != '\0'; ++i);
		return i;
}

void setup_vars(){
	numeric_view=0;
	numeric_value=0;
	number_of_placeholders=0;
	print_spaces=0;
	print_from_start=0;
	print_zeros=0;
	print_width=0;
	wasPrinting=0;
	numeric_len =0;
	len = 0;
	is_not_numric_view = 0;
	first_time=1;
}

void case_number(char* fs){
	print_width=1;
	print_spaces=1;
	c = *fs;
	int tmp = c - '0';
	number_of_placeholders = number_of_placeholders*10;
	number_of_placeholders = number_of_placeholders + tmp;
	if(first_time==1){
		if(numeric_view==0){
			s_value = va_arg(args, char *);
			wasPrinting=1;
			len = stringLen(s_value);
		}
		else
		{
			is_not_numric_view = 1;
		}
		first_time=0;
	}

}

int toy_printf(char *fs, ...) {
	int int_value = 0;
	char *string_value;
	char char_value;
	char octal_char;
	enum printf_state state;
	int array_size;
	int* int_array;
	char** string_array;
	va_start(args, fs);
	int is_array=0;
	
	
	state = st_printf_init; 

	for (; *fs != '\0'; ++fs) {
	//	printf("%s\n", fs);
		switch (state) {
			case st_printf_init:
			switch (*fs) {
				case '\\':
				state = st_printf_meta_char;
				break;

				case '%':
				state = st_printf_percent;
				break;

				default:
				putchar(*fs);
				++chars_printed;
			}
			break;

			case st_printf_meta_char:
			switch (*fs) {
				case '\\':
				putchar('\\');
				++chars_printed;
				state = st_printf_init;
				break;

				case '\"':
				putchar('\"');
				++chars_printed;
				state = st_printf_init;
				break;

				case 't':
				putchar('\t');
				++chars_printed;
				state = st_printf_init;
				break;

				case 'T':
				chars_printed += toy_printf("<tab>\t");
				state = st_printf_init;
				break;

				case 'f':
				putchar('\f');
				++chars_printed;
				state = st_printf_init;
				break;

				case 'F':
				chars_printed += toy_printf("<formfeed>\f");
				state = st_printf_init;
				break;

				case 'n':
				putchar('\n');
				++chars_printed;
				state = st_printf_init;
				break;

				case 'N':
				chars_printed += toy_printf("<newline>\n");
				state = st_printf_init;
				break;

				case 'r':
				putchar('\r');
				++chars_printed;
				state = st_printf_init;
				break;

				case 'R':
				chars_printed += toy_printf("<return>\r");
				state = st_printf_init;
				break;

				case 'c':
				chars_printed += toy_printf("CASPL'2018");
				state = st_printf_init;
				break;

				case 'C':
				chars_printed += toy_printf("<caspl magic>");
				chars_printed += toy_printf("\\c");
				state = st_printf_init;
				break;

				default:
				if (is_octal_char(*fs)) {
					octal_char = *fs - '0';
					state = st_printf_octal2;
				}
				else {
					toy_printf("Unknown meta-character: \\%c", *fs);
					exit(-1);
				}
			}
			break;

			case st_printf_octal2:
			if (is_octal_char(*fs)) {
				octal_char = (octal_char << 3) + (*fs - '0');
				state = st_printf_octal3;
				break;
			}
			else {
				toy_printf("Missing second octal char. Found: \\%c", *fs);
				exit(-1);
			}

			case st_printf_octal3:
			if (is_octal_char(*fs)) {
				octal_char = (octal_char << 3) + (*fs - '0');
				putchar(octal_char);
				++chars_printed;
				state = st_printf_init;
				break;
			}
			else {
				toy_printf("Missing third octal char. Found: \\%c", *fs);
				exit(-1);
			}

			case st_printf_percent:
			switch (*fs) {
//*******************start % case*********************
				case '%':
				putchar('%');
				++chars_printed;
				state = st_printf_init;
				break;
//*******************end % case*********************

//*******************start A case*********************
				case 'A':
				putchar('{');
				++chars_printed;
				is_array=1;
				break;
//*******************end A case*********************

//*******************start d case*********************
				case 'd':
				// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 10, digit , 0 , 0 , 0);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_int(int_value, 10, digit);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}

					chars_printed += print_int(int_value, 10, digit);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 10, digit , 0 , 0 , 0);
				}
				// setup vars
				setup_vars();
				break;
//*******************end d case*********************

//*******************start u case*********************				
				case 'u':
				// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 10, digit , 0 , 0 , 1);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_unsigned_int(int_value, 10, digit);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}


					chars_printed += print_unsigned_int(int_value, 10, digit);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 10, digit , 0 ,0 , 1);
				}
				// setup vars
				setup_vars();
				break;
//*******************end u case*********************

//*******************start b case*********************
				case 'b':
					// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 2, digit , 0 , 0 , 1);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_unsigned_int(int_value, 2, digit);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}
					chars_printed += print_unsigned_int(int_value, 2, digit);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 2, digit , 0 , 0 , 1);
				}
				// setup vars
				setup_vars();
				break;
//*******************end b case*********************

//*******************start o case*********************
				case 'o':
					// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 8, digit , 0 , 0 ,1);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_unsigned_int(int_value, 8, digit);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}

					chars_printed += print_unsigned_int(int_value, 8, digit);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 8, digit , 0 , 0 ,1);
				}
				// setup vars
				setup_vars();
				break;
//*******************end o case*********************

//*******************start x case*********************				
				case 'x':
					// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 16, digit , 0 , 0 ,1);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_unsigned_int(int_value, 16, digit);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}

					chars_printed += print_unsigned_int(int_value, 16, digit);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 16, digit , 0 , 0 ,1);
				}
				// setup vars
				setup_vars();
				break;
//*******************end x case*********************

//*******************start X case*********************				
				case 'X':
				// check for printing
				if(print_from_start==1){
					print_placeholders(numeric_value, 16, DIGIT , 0 , 0 ,1);
				}

				//check for array
				if(is_array==1)
				{
					int_array = va_arg(args, int*);
					array_size = va_arg(args, int);

					for (int index_array=0; index_array != array_size; ++index_array) {
						int_value = int_array[index_array];
						chars_printed += print_unsigned_int(int_value, 16, DIGIT);
						state = st_printf_init;

						if(index_array!=array_size-1)
						{
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}

					}

					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{
					// check for width case
					if(wasPrinting==1)
					{
						int_value = numeric_value;
						wasPrinting=0;
					}

					else
					{
						int_value = va_arg(args, int);
					}

					chars_printed += print_unsigned_int(int_value, 16, DIGIT);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(numeric_value, 16, DIGIT , 0 , 0 ,1);
				}
				// setup vars
				setup_vars();
				break;
//*******************end X case*********************

//*******************start s case*********************				
				case 's':
				// check for printing
				if(print_from_start==1)
				{
					print_placeholders(0, 0, digit , 0 , 1 , 0);
				}

				//check for array
				if(is_array==1)
				{
					
					string_array = va_arg(args, char**);
					array_size = va_arg(args, int);
					int index_string=0;

					while(index_string!=array_size){
						putchar('"');
						++chars_printed;
						toy_printf(string_array[index_string]);
						putchar('"');
						++chars_printed;
						if(index_string!=array_size-1){
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
							
						}
						++index_string;
					}


					state = st_printf_init;
					is_array=0;
					putchar('}');
					++chars_printed;
				}

				else
				{

					// check for width case
					if(wasPrinting==1){
						string_value = s_value;
					}
					else
					{
						string_value = va_arg(args, char *);
					}
					

					//chars_printed += 
					toy_printf(string_value);
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0){
					print_placeholders(0, 0, digit , 0 , 1 , 0);
				}

				// setup vars
				setup_vars();
				break;
//*******************end s case*********************

//*******************start c case*********************				
				case 'c':
				// check for printing
				if(print_from_start==1)
				{
					print_placeholders(numeric_value, 0, digit , 1 , 0 , 0);
				}

				//check for array
				if(is_array==1)
				{
					int* char_array = va_arg(args, int*);
					array_size = va_arg(args, int);
					for (int index_array=0; index_array != array_size; ++index_array) {
						char_value = (char)char_array[index_array];
						putchar(char_value);
						++chars_printed;
						state = st_printf_init;
						if(index_array!=array_size-1){
							putchar(',');
							++chars_printed;
							putchar(' ');
							++chars_printed;
						}
					}
					is_array=0;
					putchar('}');
					++chars_printed;
				}
				else
				{

					// check for width case
					if(wasPrinting==1){
						char_value = (char)numeric_value;
					}
					else
					{
						char_value = va_arg(args, int);
					}

					putchar(char_value);
					++chars_printed;
					state = st_printf_init;
				}

				// check for printing
				if(print_from_start==0)
				{
					print_placeholders(numeric_value, 0, digit , 1, 0 , 0);
				}

				// setup vars
				setup_vars();
				break;
//*******************end c case*********************	

//*******************start numbers cases*********************					
				case ('9'):
				case_number(fs);
				break;

				case ('8'):
				case_number(fs);
				break;

				case ('7'):
				case_number(fs);
				break;

				case ('6'):
				case_number(fs);
				break;

				case ('5'):
				case_number(fs);
				break;

				case ('4'):
				case_number(fs);
				break;

				case ('3'):
				case_number(fs);
				break;

				case ('2'):
				case_number(fs);
				break;

				case ('1'):
				case_number(fs);
				break;

				case ('0'):
				if(is_not_numric_view==1){
					print_width=1;
					c = *fs;
					number_of_placeholders = c - '0';
					print_spaces=1;
				}
				else{
					print_from_start=1;
					print_width=1;
					print_zeros=1;
					numeric_value = va_arg(args, int);
					wasPrinting=1;
					numeric_view=1;
				}
				break;
//*******************end numbers cases*********************	

				case ('-'):
				print_from_start=1;
				break;



				default:
				toy_printf("Unhandled format %%%c...\n", *fs);
				exit(-1);
			}
			break;

			default:
			toy_printf("toy_printf: Unknown state -- %d\n", (int)state);
			exit(-1);
		}
	}

	va_end(args);

	return chars_printed;
}




