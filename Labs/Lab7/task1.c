#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 15

char File_Name[100];
int Unit_Size;


void Set_File_Name(){
	char OPTION[MAX];
	int i;
	printf("Insert file name:\n");
	fgets(OPTION, 15, stdin);

  	/* remove newline, if present */
  	i = strlen(OPTION)-1;
  	if( OPTION[ i ] == '\n') 
      	OPTION[i] = '\0';

	memset(File_Name, '\0', sizeof(File_Name));
   	strcpy(File_Name, OPTION);

	}

void Set_Unit_Size(){
	char OPTION[MAX];
	int i;
	printf("Insert unit size:\n");
	fgets(OPTION, 15, stdin);

  	/* remove newline, if present */
  	i = strlen(OPTION)-1;
  	if( OPTION[ i ] == '\n') 
      	OPTION[i] = '\0';

      if(atoi(OPTION)==1 || atoi(OPTION)==2 || atoi(OPTION)==4)
      	Unit_Size=atoi(OPTION);
      else
      	printf("Error: Wrong size\n");

	}

void Quit(){

	exit(0);

	}



void File_Display(){
	
	FILE *ptr;
	int filedesc;
	if(File_Name==0)
		printf("Error: File Name is null\n");


    ptr = fopen(File_Name,"rb");

    if(ptr == NULL)
        printf("Error: Can not open file\n");

	printf("Please enter <location> <length> :\n");
	unsigned int location;
	int length;
	scanf("%x %d%*c", &location , &length);


	filedesc = fileno(ptr);
	if(lseek (filedesc, location, 1)==-1)
			printf("erro happend lseek\n");


	unsigned char buffer[length*Unit_Size];

	read(filedesc , buffer , length*Unit_Size);

	//printf("Hexadecimal Representation:\n");


	switch(Unit_Size){
		case 1:
		printf("Hexadecimal Representation:\n");
		for(int i = 0 ; i<=(length*Unit_Size)-1 ; i++)
   			printf("%02x ", buffer[i]);
   		printf("\nDecimal Representation:\n");
   		for(int i = 0 ; i<=(length*Unit_Size)-1 ; i++)
   			printf("%02u ", buffer[i]);
   		break;
   		
   		case 2:
   		printf("Hexadecimal Representation:\n");
   		for(int i = 0 ; i<=(length*Unit_Size)-2 ; i=i+2)
			printf ("%04x ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8);
		printf("\nDecimal Representation:\n");
		for(int i = 0 ; i<=(length*Unit_Size)-2 ; i=i+2)
			printf ("%04u ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8);
		break;

		case 4:
   		printf("Hexadecimal Representation:\n");
   		for(int i = 0 ; i<=(length*Unit_Size)-4 ; i=i+4)
			printf ("%08lx ", (unsigned long) buffer[i+2] | (unsigned long) buffer[i + 3] << 8| (unsigned long) buffer[i] << 16| (unsigned long) buffer[i + 1] << 32);
		printf("\nDecimal Representation:\n");
		for(int i = 0 ; i<=(length*Unit_Size)-4 ; i=i+4)
			printf ("%08lu ", (unsigned long) buffer[i] | (unsigned long) buffer[i + 1] << 8| (unsigned long) buffer[i + 2] << 16| (unsigned long) buffer[i + 3] << 32);
		break;
	}

/*/
	switch(Unit_Size){
		case 1:
		printf("Hexadecimal Representation:\n");
		for(int i = (length*Unit_Size)-1 ; i>=0 ; i--)
   			printf("%02x ", buffer[i]);
   		printf("Decimal Representation:\n");
   		for(int i = (length*Unit_Size)-1 ; i>=0 ; i--)
   			printf("%02u ", buffer[i]);
   		break;
   		
   		case 2:
   		printf("Hexadecimal Representation:\n");
   		for(int i = (length*Unit_Size)-2 ; i>=0 ; i=i-2)
			printf ("%04x ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8);
		printf("Decimal Representation:\n");
		for(int i = (length*Unit_Size)-2 ; i>=0 ; i=i-2)
			printf ("%04u ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8);
		break;

		case 4:
   		printf("Hexadecimal Representation:\n");
   		for(int i = (length*Unit_Size)-4 ; i>=0 ; i=i-4)
			printf ("%08lx ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8| (unsigned int) buffer[i + 2] << 16| (unsigned long) buffer[i + 3] << 32);
		printf("Decimal Representation:\n");
		for(int i = (length*Unit_Size)-4 ; i>=0 ; i=i-4)
			printf ("%08lu ", (unsigned int) buffer[i] | (unsigned int) buffer[i + 1] << 8| (unsigned int) buffer[i + 2] << 16| (unsigned long) buffer[i + 3] << 32);
		break;
	}
/*/

/*/
	for(int i = (length*Unit_Size)-2 ; i>=0 ; i=i-2){

   		 printf("%02x", buffer[i+1]);

   		 printf("%02x ", buffer[i]);
   		}
/*/
   		close(filedesc);
 
    	 return;
    	}

void File_Modify(){
	
	int filedesc;
	if(File_Name==0)
		printf("Error: File Name is null\n");


    filedesc = open(File_Name,0002);

	printf("Please enter <location> <val> :\n");
	unsigned int location;
	unsigned int val[100];
	scanf("%x %x%*c", &location , val);


	if(lseek (filedesc, location, 0)==-1)
			printf("erro happend lseek\n");

	write(filedesc,val,Unit_Size);

	close(filedesc);

    	 return;
    	}

void Copy_From_File(){
	
	FILE *ptr;
	int filedesc_dst;
	//Check if src_file or dst_file is null, and if it is print an error message and return.
	if(File_Name==0) 
		printf("Error: File Name is null\n");

	//Open dst_file for read & write (not write only). If this fails, print an error message and return.
    if((filedesc_dst = open(File_Name,0002))==-1)
    	printf("Error: Can not open file\n");

	printf("Please enter <src_file> <src_offset> <dst_offset> <length> :\n");
	char src_file[100];
	unsigned int src_offset;
	unsigned int dst_offset;
	unsigned int length;

	//Prompt the user for src_offset and dst_offset (in hexadecimal) and length (in decimal).
	scanf("%s %x %x %d%*c", src_file , &src_offset , &dst_offset , &length);
	//printf("%x\n", src_offset);
	int filedesc_src;
	//Open src_file for reading. If this fails, print an error message and return.
	ptr = fopen(src_file,"rb");

    if(ptr == NULL)
        printf("Error: Can not open file\n");
    filedesc_src = fileno(ptr);


	if(lseek (filedesc_src, src_offset, 0)==-1)
			printf("erro happend lseek\n");

	unsigned int read_buffer[length];

	//Read length bytes from src_file at offset src_offset and write them to dst_file at dst_offset
	read(filedesc_src , read_buffer , length);


	if(lseek (filedesc_dst, dst_offset, 0)==-1)
			printf("erro happend lseek\n");

	write(filedesc_dst,read_buffer,length);


	//Close the files.
	close(filedesc_dst);
	close(filedesc_src);
    	 return;
    	 
    	}






void init_functions_array_pointers(void* functions_array[]){
	void (*Set_File_Name_ptr)() = &Set_File_Name;
	functions_array[0] = Set_File_Name_ptr;

	void (*Set_Unit_Size_ptr)() = &Set_Unit_Size;
	functions_array[1] = Set_Unit_Size_ptr;

	void (*File_Display_ptr)() = &File_Display;
	functions_array[2] = File_Display_ptr;

	void (*File_Modify_ptr)() = &File_Modify;
	functions_array[3] = File_Modify_ptr;

	void (*Copy_From_File_ptr)() = &Copy_From_File;
	functions_array[4] = Copy_From_File_ptr;

	void (*Quit_ptr)() = &Quit;
	functions_array[5] = Quit_ptr;
	

	}

void functions_array_names(char* names[]){
	names[0] = "Set File Name";
	names[1] = "Set Unit Size";
	names[2] = "File Display";
	names[3] = "File Modify";
	names[4] = "Copy From File";
	names[5] = "Quit";
	names[6] = 0;

	}

int main()
{
	void* functions_pointers[MAX];
	init_functions_array_pointers(functions_pointers);

	char* functions_names[MAX];
	functions_array_names(functions_names);

    
while(1){
	
	char OPTION[MAX];
	int i;

	printf("Choose action:\n");
  for(int idx=0; functions_names[idx]!=0; idx++)
	{
   		printf("%d-%s\n", idx+1 ,functions_names[idx]);
	}

  	fgets(OPTION, 15, stdin);

  	/* remove newline, if present */
  	i = strlen(OPTION)-1;
  	if( OPTION[ i ] == '\n') 
      	OPTION[i] = '\0';
    void (*p)();
	p =(functions_pointers[atoi(OPTION)-1]);
	(*p)();

	printf("\n");
	}
 
    return 0;
}
