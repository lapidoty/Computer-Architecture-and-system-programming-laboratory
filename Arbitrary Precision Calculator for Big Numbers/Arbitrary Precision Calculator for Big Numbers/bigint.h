//this is the difference between the ASCII code for 0 and the integer 0
#define CHARZERO_ABOVEINTZERO 48


//This define defines a single byte unsigned integer
//if your system uses a different type for this kind of integer, substitute it here
typedef unsigned char digit;

//the type digit* represents an array of digit s


typedef struct bigint
{
	digit* number;///this is a pointer to an array of digit s
	 int length; // this is the length of the arrat referenced by number
	 int sign; // 1 is negative , 0 positive
    			
  
} bigint;

void newnum(bigint*);
bigint* newnumc(char* , int len);
bigint* alloc_bigint(int len);
void delnum(bigint*);
void print(bigint*);
int _compare();
void _sub();
void _add();
void _mult();
void _div();
