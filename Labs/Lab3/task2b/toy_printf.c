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
enum printf_state state;

typedef struct state_args{
	char* fs;
	int* int_array;
	int array_size;
	int int_value;

	char** string_array;
	char *string_value;
	char char_value;
	int is_array;


	int char_cases[128];
	int digit_operation_cases[3];
	int operation;
} state_args;


struct state_result {
	int printed_chars;
	enum printf_state next_state;
};

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

void print_string(char* fs) {
	for (; *fs != '\0'; ++fs) {
		putchar(*fs);
		++chars_printed;
	}
}

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

}

struct state_result init_state_handler(va_list args, state_args* state_args){

	struct state_result to_return;
	char *fs = state_args->fs;
	switch (*fs) {

		case '\\':
		to_return.next_state = st_printf_meta_char;
		break;

		case '%':
		to_return.next_state = st_printf_percent;
		break;

		default:
		putchar(*fs);
		++chars_printed;
	}
	return to_return;

}

void signed_digit_printing(va_list args, state_args* state_args, int radix){

				// check for printing
	if(print_from_start==1){

		print_placeholders(numeric_value, radix, digit , 0 , 0 , 0);
	}

				//check for array
	if(state_args->operation==1)
	{

		state_args->int_array = va_arg(args, int*);
		state_args->array_size = va_arg(args, int);

		for (int index_array=0; index_array != state_args->array_size; ++index_array) {
			state_args->int_value = state_args->int_array[index_array];

			chars_printed += print_int(state_args->int_value, radix, digit);

			if(index_array!=state_args->array_size-1)
			{
				putchar(',');
				++chars_printed;
				putchar(' ');
				++chars_printed;
			}

		}

		state_args->operation=0;
		putchar('}');
		++chars_printed;
	}

	else
	{
					// check for width case
		if(wasPrinting==1)
		{
			state_args->int_value = numeric_value;
			wasPrinting=0;
		}

		else
		{
			state_args->int_value = va_arg(args, int);
		}

		chars_printed += print_int(state_args->int_value, radix, digit);
		
	}

				// check for printing
	if(print_from_start==0){
		print_placeholders(numeric_value, radix, digit , 0 , 0 , 0);
	}
				// setup vars
	setup_vars();
}

void unsigned_digit_printing(va_list args, state_args* state_args, int radix , const char * dig){

				// check for printing
	if(print_from_start==1){

		print_placeholders(numeric_value, radix, digit , 0 , 0 , 1);
	}

				//check for array
	if(state_args->operation==1)
	{

		state_args->int_array = va_arg(args, int*);
		state_args->array_size = va_arg(args, int);

		for (int index_array=0; index_array != state_args->array_size; ++index_array) {
			state_args->int_value = state_args->int_array[index_array];

			chars_printed += print_unsigned_int(state_args->int_value, radix, dig);

			if(index_array!=state_args->array_size-1)
			{
				putchar(',');
				++chars_printed;
				putchar(' ');
				++chars_printed;
			}

		}

		state_args->operation=0;
		putchar('}');
		++chars_printed;
	}

	else
	{
					// check for width case
		if(wasPrinting==1)
		{
			state_args->int_value = numeric_value;
			wasPrinting=0;
		}

		else
		{
			state_args->int_value = va_arg(args, int);
		}
		chars_printed += print_unsigned_int(state_args->int_value, radix, dig);
		
	}

				// check for printing
	if(print_from_start==0){
		print_placeholders(numeric_value, radix, digit , 0 , 0 , 1);
	}
				// setup vars
	setup_vars();
}

struct state_result d_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	signed_digit_printing(args,state_args,10);
	to_return.next_state = st_printf_init;
	return to_return;

}

struct state_result u_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	unsigned_digit_printing(args,state_args,10,digit);
	to_return.next_state = st_printf_init;
	return to_return;

}

struct state_result b_char_handler(va_list args, state_args* state_args){

	struct state_result to_return;

	unsigned_digit_printing(args,state_args,2,digit);
	to_return.next_state = st_printf_init;

	return to_return;

}

struct state_result o_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	unsigned_digit_printing(args,state_args,8,digit);
	to_return.next_state = st_printf_init;

	return to_return;

}

struct state_result x_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	unsigned_digit_printing(args,state_args,16,digit);
	to_return.next_state = st_printf_init;

	return to_return;

}

struct state_result X_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	unsigned_digit_printing(args,state_args,16,DIGIT);
	to_return.next_state = st_printf_init;
	return to_return;

}

struct state_result s_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

			// check for printing
	if(print_from_start==1)
	{
		print_placeholders(0, 0, digit , 0 , 1 , 0);
	}

				//check for array
	if(state_args->operation==1)
	{
		
		state_args->string_array = va_arg(args, char**);
		state_args->array_size = va_arg(args, int);
		int index_string=0;

		while(index_string!=state_args->array_size){
			putchar('"');
			++chars_printed;
			print_string(state_args->string_array[index_string]);
			putchar('"');
			++chars_printed;
			if(index_string!=state_args->array_size-1){
				putchar(',');
				++chars_printed;
				putchar(' ');
				++chars_printed;
				
			}
			++index_string;
		}


		to_return.next_state = st_printf_init;
		state_args->operation=0;
		putchar('}');
		++chars_printed;
	}

	else
	{

					// check for width case
		if(wasPrinting==1){
			state_args->string_value = s_value;
		}
		else
		{
			
			state_args->string_value = va_arg(args, char *);

		}
		

		print_string(state_args->string_value);

		to_return.next_state = st_printf_init;
		
		

	}

				// check for printing
	if(print_from_start==0){
		print_placeholders(0, 0, digit , 0 , 1 , 0);
	}

				// setup vars
	setup_vars();
	return to_return;

}

struct state_result c_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

				// check for printing
	if(print_from_start==1)
	{
		print_placeholders(numeric_value, 0, digit , 1 , 0 , 0);
	}

				//check for array
	if(state_args->operation==1)
	{
		int* char_array = va_arg(args, int*);
		state_args->array_size = va_arg(args, int);
		for (int index_array=0; index_array != state_args->array_size; ++index_array) {
			state_args->char_value = (char)char_array[index_array];
			putchar(state_args->char_value);
			++chars_printed;
			to_return.next_state = st_printf_init;
			if(index_array!=state_args->array_size-1){
				putchar(',');
				++chars_printed;
				putchar(' ');
				++chars_printed;
			}
		}
		state_args->operation=0;
		putchar('}');
		++chars_printed;
	}
	else
	{

					// check for width case
		if(wasPrinting==1){
			state_args->char_value = (char)numeric_value;
		}
		else
		{
			state_args->char_value = va_arg(args, int);
		}

		putchar(state_args->char_value);
		++chars_printed;
		to_return.next_state = st_printf_init;
	}

				// check for printing
	if(print_from_start==0)
	{
		print_placeholders(numeric_value, 0, digit , 1, 0 , 0);
	}

				// setup vars
	setup_vars();
	return to_return;

}

struct state_result number_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	char *fs = state_args->fs;
	case_number(fs);
	to_return.next_state = st_printf_percent;
	return to_return;

}

struct state_result zero_char_handler(va_list args, state_args* state_args){

	struct state_result to_return;
	char *fs = state_args->fs;

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

	to_return.next_state = st_printf_percent;
	return to_return;

}

struct state_result leftPadded_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	print_from_start=1;
	to_return.next_state = st_printf_percent;
	return to_return;

}

struct state_result A_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	putchar('{');
	++chars_printed;
	to_return.next_state = st_printf_percent;
	state_args->operation=1;
	return to_return;

}	

struct state_result meta_char_state_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	char *fs = state_args->fs;
	switch (*fs) {
		case '\\':
		putchar('\\');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case '\"':
		putchar('\"');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 't':
		putchar('\t');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 'T':
		chars_printed += toy_printf("<tab>\t");
		to_return.next_state = st_printf_init;
		break;

		case 'f':
		putchar('\f');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 'F':
		chars_printed += toy_printf("<formfeed>\f");
		to_return.next_state = st_printf_init;
		break;

		case 'n':
		putchar('\n');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 'N':
		chars_printed += toy_printf("<newline>\n");
		to_return.next_state = st_printf_init;
		break;

		case 'r':
		putchar('\r');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 'R':
		chars_printed += toy_printf("<return>\r");
		to_return.next_state = st_printf_init;
		break;

		case 'c':
		chars_printed += toy_printf("CASPL'2018");
		to_return.next_state = st_printf_init;
		break;

		case 'C':
		chars_printed += toy_printf("<caspl magic>");
		chars_printed += toy_printf("\\c");
		to_return.next_state = st_printf_init;
		break;

		default:
		toy_printf("Unknown meta-character: \\%c", *fs);
		exit(-1);
		
		
	}
	return to_return;
}

struct state_result percent_state_handler(va_list args, state_args* state_args){

	struct state_result to_return;
	char *fs = state_args->fs;

	switch (*fs) {
		case '%':
		putchar('%');
		++chars_printed;
		to_return.next_state = st_printf_init;
		break;

		case 'A':
		to_return = A_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		case 'd':
		to_return = d_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		
		case 'u':
		to_return = u_char_handler(args, state_args);
		state = to_return.next_state;
		break;

		case 'b':
		to_return = b_char_handler(args, state_args);
		state = to_return.next_state;
		break;

		case 'o':
		to_return = o_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		
		case 'x':
		to_return = x_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		
		case 'X':
		to_return = X_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		
		case 's':
		to_return = s_char_handler(args, state_args);
		state = to_return.next_state;
		break;
		
		case 'c':
		to_return = c_char_handler(args, state_args);
		state = to_return.next_state;
		break;

		case ('-'):
		to_return = leftPadded_char_handler(args, state_args);
		state = to_return.next_state;
		break;

	}

	if(*fs > 47 && *fs < 58){

		if((*fs)=='0'){
			to_return = zero_char_handler(args, state_args);
			state = to_return.next_state;
		}
		else{
			to_return = number_char_handler(args, state_args);
			state = to_return.next_state;
		}
		
	}
	return to_return;

}

void init_states_array(void* states_array[]){
	struct state_result (*init)(va_list, struct state_args*);
	init = &init_state_handler;
	states_array[0] = init;

	struct state_result (*meta)(va_list, struct state_args*);
	meta = &meta_char_state_handler;
	states_array[1] = meta;

	struct state_result (*precent)(va_list, struct state_args*);
	precent = &percent_state_handler;
	states_array[2] = precent;


}

int toy_printf(char *fs, ...) {

	va_start(args, fs);
	struct state_args state_args;
	struct state_result result;
	state = st_printf_init; 
	void* states_array[3];
	init_states_array(states_array);
	struct state_result (*p)(va_list, struct state_args*);

	for (; *fs != '\0'; ++fs) {
		state_args.fs = fs;
		p = states_array[state];
		result = (*p)(args,&state_args);
		state = result.next_state;

	}



	va_end(args);

	return chars_printed;
}





