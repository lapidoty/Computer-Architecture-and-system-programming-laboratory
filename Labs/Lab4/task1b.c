#include "lab4_util.h"


#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_SLEEK 8
#define SYS_GETDENTS 78
#define SYS_EXIT 60

#define STDOUT 1

extern int system_call();


int main (int argc , char* argv[])
{

char* fileName = argv[1];
char* ERROR = "ERROR";
int err = 0;
int file = system_call(SYS_OPEN , fileName, 0101 , 0777); 

if(file<0){
	system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
	system_call(SYS_EXIT,1);
	return 1;
}
err = system_call(SYS_SLEEK, file, 4117 , 0);  
if(err<0){
	system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
	system_call(SYS_EXIT,1);
	return 1;
}
int len=2;
char* str = argv[2];
if(sizeof(str)<5){
	len = len + (5-sizeof(str))+1;
}
err = system_call(SYS_WRITE, file, str, (sizeof(str)-len));  \

if(err<0){
	system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
	system_call(SYS_EXIT,1);
	return 1;
}
err = system_call(SYS_CLOSE,file);
if(err<0){
	system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
	system_call(SYS_EXIT,1);
	return 1;
}
  return 0;
}

