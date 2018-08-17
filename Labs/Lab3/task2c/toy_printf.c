/* caspl-lab-1.c
 * Limited versions of printf
 *
 * Programmer: Mayer Goldberg, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
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
int number_of_placeholders=0;
int chars_printed = 0;
int numeric_len = 0;
va_list args;
enum printf_state state;

typedef struct state_args{
    char* fs;
	int* int_array;
	int array_size;
	unsigned int int_value;

	char** string_array;
	char *string_value;
	char char_value;
	int is_array;
	int radix;

	int leftPadded;
	int char_cases[128];
	int digit_operation_cases[3];
	int operation; //1-array , 2-numeric
	} state_args;

struct state_result {
  int printed_chars;
  enum printf_state next_state;
	};

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

int stringLen(char* s){
	int i;
	for(i = 0; s[i] != '\0'; ++i);
		return i;
	}

void setup_vars(){
	number_of_placeholders=0;
	numeric_len =0;
	}

void aggregate_number(char* fs){
	char c = *fs;
	int tmp = c - '0';
	number_of_placeholders = number_of_placeholders*10;
	number_of_placeholders = number_of_placeholders + tmp;

		}

void print_string(char* fs) {
	for (; *fs != '\0'; ++fs) {
	putchar(*fs);
				++chars_printed;
			}
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
				to_return.next_state = st_printf_init;
			}
			return to_return;

			}

struct state_result signed_array_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	int radix = state_args->radix;

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

	return to_return;

	}

struct state_result unsigned_array_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	int radix = state_args->radix;

					state_args->int_array = va_arg(args, int*);
					state_args->array_size = va_arg(args, int);

					for (int index_array=0; index_array != state_args->array_size; ++index_array) {
						state_args->int_value = state_args->int_array[index_array];

						chars_printed += print_unsigned_int(state_args->int_value, radix, digit);

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

	return to_return;

	}

struct state_result string_array_handler(va_list args, state_args* state_args){
	struct state_result to_return;

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

	return to_return;

	}

struct state_result char_array_handler(va_list args, state_args* state_args){
	struct state_result to_return;

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

	return to_return;

	}

void init_ops_array_digits(void* ops_array[]);

struct state_result numeric_print_handler(va_list args, state_args* state_args){
	struct state_result result;
	int radix = state_args->radix;

	va_list args2;
    va_copy(args2, args);
	int val =  va_arg(args2, int);

	numeric_len = count_signed_int(val,radix,digit);
	number_of_placeholders = number_of_placeholders - numeric_len;

	if(val < 0){
			putchar('-');
			++chars_printed;
			state_args->int_value = -1;
	}

	for(int i=0; i<number_of_placeholders;++i){
					putchar('0');
					++chars_printed;		
	}

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	state_args->operation=0;

	p = ops_array[state_args->operation+2];
	result = (*p)(args,state_args);
	state = result.next_state;	


	return result;

	}

struct state_result left_padded_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	state_args->string_value = va_arg(args, char *);

	number_of_placeholders = number_of_placeholders - stringLen(state_args->string_value );

	for(int i=0; i<number_of_placeholders;++i){
					putchar(' ');
					++chars_printed;
				}

		
		print_string(state_args->string_value);

	

	return to_return;

	}

struct state_result right_padded_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	state_args->string_value = va_arg(args, char *);

	number_of_placeholders = number_of_placeholders - stringLen(state_args->string_value );

	print_string(state_args->string_value);

	for(int i=0; i<number_of_placeholders;++i){
					putchar(' ');
					++chars_printed;
				}
				putchar('#');
				++chars_printed;

	return to_return;

	}

struct state_result signed_digit_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	int radix = state_args->radix;
	state_args->int_value *= va_arg(args, int);
	chars_printed += print_int(state_args->int_value, radix, digit);

	return to_return;
	}

struct state_result string_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	state_args->string_value = va_arg(args, char *);
	print_string(state_args->string_value);

	return to_return;
	}

struct state_result char_handler(va_list args, state_args* state_args){
	struct state_result to_return;

	state_args->char_value = va_arg(args, int);
	putchar(state_args->char_value);
	++chars_printed;

	return to_return;
	}

struct state_result unsigned_digit_handler(va_list args, state_args* state_args){

	struct state_result to_return;
	
	int radix = state_args->radix;
	state_args->int_value = va_arg(args, int);
	chars_printed += print_unsigned_int(state_args->int_value, radix, digit);

	return to_return;
				}

struct state_result d_char_handler(va_list args, state_args* state_args){
	struct state_result result;

	state_args->radix = 10;
	state_args->int_value = 1;
	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation+2];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

struct state_result u_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	
	state_args->radix = 10;

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

struct state_result b_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	
	state_args->radix = 2;

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

struct state_result o_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	
	state_args->radix = 8;

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

struct state_result x_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	
	state_args->radix = 16;

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

struct state_result X_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	
	state_args->radix = 16;

	void* ops_array[5];
	init_ops_array_digits(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;


	setup_vars();
	result.next_state = st_printf_init;
	return result;

	}

void init_ops_array_strings(void* ops_array[]);

struct state_result s_char_handler(va_list args, state_args* state_args){
		struct state_result result;
		
		int leftPadded = state_args->leftPadded;

		void* ops_array[4];
		init_ops_array_strings(ops_array);
		struct state_result (*p)(va_list, struct state_args*);

		p = ops_array[state_args->operation + leftPadded];
		result = (*p)(args,state_args);
		state = result.next_state;

		result.next_state = st_printf_init;
					
		setup_vars();
		return result;

		}

void init_ops_array_chars(void* ops_array[]);

struct state_result c_char_handler(va_list args, state_args* state_args){
	struct state_result result;
	

	void* ops_array[2];
	init_ops_array_chars(ops_array);
	struct state_result (*p)(va_list, struct state_args*);

	p = ops_array[state_args->operation];
	result = (*p)(args,state_args);
	state = result.next_state;
	
	result.next_state = st_printf_init;

	setup_vars();
	return result;

	}

struct state_result number_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	char *fs = state_args->fs;
	state_args->operation=2;
	aggregate_number(fs);
	to_return.next_state = st_printf_percent;
	return to_return;

	}

struct state_result leftPadded_char_handler(va_list args, state_args* state_args){
	struct state_result to_return;
	
	state_args->leftPadded = 1;
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
				case 'n':
				putchar('\n');
				++chars_printed;
				to_return.next_state = st_printf_init;
				break;
				//default:
				//	toy_printf("Unknown meta-character: \\%c", *fs);
					//exit(-1);
				
			
	}
	return to_return;
	}

void init_chars_array(void* chars_array[]);

struct state_result percent_state_handler(va_list args, state_args* state_args){
	
	struct state_result result;
	char *fs = state_args->fs;
	

	void* chars_array[128];
	init_chars_array(chars_array);
	struct state_result (*p)(va_list, struct state_args*);

	int idx = *fs;
	
	p = chars_array[idx];
	result = (*p)(args,state_args);
	state = result.next_state;


	
	return result;
	
	}
	
void init_ops_array_digits(void* ops_array[]){
	
	struct state_result (*unsign)(va_list, struct state_args*);
	unsign = &unsigned_digit_handler;
	ops_array[0] = unsign;

	struct state_result (*unsignA)(va_list, struct state_args*);
	unsignA = &unsigned_array_handler;
	ops_array[1] = unsignA;

	struct state_result (*sig)(va_list, struct state_args*);
	sig = &signed_digit_handler;
	ops_array[2] = sig;

	struct state_result (*signA)(va_list, struct state_args*);
	signA = &signed_array_handler;
	ops_array[3] = signA;

	struct state_result (*numeric)(va_list, struct state_args*);
	numeric = &numeric_print_handler;
	ops_array[4] = numeric;

	
	}

void init_ops_array_strings(void* ops_array[]){
	
	struct state_result (*ps)(va_list, struct state_args*);
	ps = &string_handler;
	ops_array[0] = ps;

	struct state_result (*sa)(va_list, struct state_args*);
	sa = &string_array_handler;
	ops_array[1] = sa;

	struct state_result (*left)(va_list, struct state_args*);
	left = &right_padded_handler;
	ops_array[2] = left;

	struct state_result (*right)(va_list, struct state_args*);
	right = &left_padded_handler;
	ops_array[3] = right;
	

	
	}	

void init_ops_array_chars(void* ops_array[]){
	
	struct state_result (*ps)(va_list, struct state_args*);
	ps = &char_handler;
	ops_array[0] = ps;

	struct state_result (*sa)(va_list, struct state_args*);
	sa = &char_array_handler;
	ops_array[1] = sa;

	}

void init_chars_array(void* chars_array[]){

	struct state_result (*init)(va_list, struct state_args*);
	init = &init_state_handler;
	for (int i = 0; i < 128; ++i)
	{
		chars_array[i]=init;
	}

	struct state_result (*A)(va_list, struct state_args*);
	A = &A_char_handler;
	chars_array['A'] = A;

	struct state_result (*d)(va_list, struct state_args*);
	d = &d_char_handler;
	chars_array['d'] = d;

	struct state_result (*u)(va_list, struct state_args*);
	u = &u_char_handler;
	chars_array['u'] = u;
	
	struct state_result (*b)(va_list, struct state_args*);
	b = &b_char_handler;
	chars_array['b'] = b;

	struct state_result (*o)(va_list, struct state_args*);
	o = &o_char_handler;
	chars_array['o'] = o;
	
	struct state_result (*x)(va_list, struct state_args*);
	x = &x_char_handler;
	chars_array['x'] = x;

	struct state_result (*X)(va_list, struct state_args*);
	X = &X_char_handler;
	chars_array['X'] = X;

	struct state_result (*s)(va_list, struct state_args*);
	s = &s_char_handler;
	chars_array['s'] = s;

	struct state_result (*c)(va_list, struct state_args*);
	c = &c_char_handler;
	chars_array['c'] = c;

	struct state_result (*lp)(va_list, struct state_args*);
	lp = &leftPadded_char_handler;
	chars_array['-'] = lp;

	struct state_result (*num)(va_list, struct state_args*);
	num = &number_char_handler;
	chars_array[48] = num;
	chars_array[49] = num;
	chars_array[50] = num;
	chars_array[51] = num;
	chars_array[52] = num;
	chars_array[53] = num;
	chars_array[54] = num;
	chars_array[55] = num;
	chars_array[56] = num;
	chars_array[57] = num;

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

	p =(states_array[state]);
	result = (*p)(args,&state_args);

	state = result.next_state;

	}

			
	
	va_end(args);

	return chars_printed;
	}








