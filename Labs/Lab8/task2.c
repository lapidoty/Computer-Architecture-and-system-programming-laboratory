#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define MAX 15

char File_Name[100];
int Unit_Size;
int verbos_main = 0;
int fd = -1;
void *map_start; /* will point to the start of the memory mapped file */
struct stat fd_stat; /* this is needed to  the size of the file */
Elf64_Ehdr *header; /* this will point to the header structure */
int num_of_section_headers;


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
	munmap(map_start, fd_stat.st_size);
   	close(fd);
   	fd=-1;
   	exit(0);
   	return;
	

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

void Toggle_Debug_Mode(){
	verbos_main = 1;
	}

char * section_type_name(int type){
	switch (type) {
            case SHT_NULL:                          
                return "NULL";
            case SHT_PROGBITS:                     
                return "PROGBITS";
            case SHT_SYMTAB:                       
                return "SYMTAB";
            case SHT_STRTAB:                        
                return "STRTAB";
            case SHT_RELA:                        
                return "RELA";
            case SHT_HASH:                        
                return "HASH";
            case SHT_DYNAMIC:                        
                return "DYNAMIC";
            case SHT_NOTE:                        
                return "NOTE";
            case SHT_NOBITS:                        
                return "NOBITS";
            case SHT_REL:                        
                return "REL";
            case SHT_SHLIB:                        
                return "SHLIB";
            case SHT_DYNSYM:                        
                return "DYNSYM";
            case SHT_LOOS:                        
                return "LOOS";
            case SHT_HIOS:                        
                return "HIOS";
            case SHT_LOPROC:                        
                return "LOPROC";
            case SHT_HIPROC:                        
                return "HIPROC";
            default:
                return "0000000";
        }
	}

void Print_Section_Names(){

	printf("\nThere are %d section headers, starting at offset %#lx:\n",
	    header->e_shnum, header->e_shoff);

	printf("\nSection Headers:\n");

	printf("[Nr] Name                     Addr               Off             Size        \t         Type\n");

	Elf64_Shdr * S_header;
	S_header = (Elf64_Shdr *) (map_start + header->e_shoff);
	char *section_header_string_table = (char*)(map_start + S_header[header->e_shstrndx].sh_offset);


    for(int i = 0; i < header->e_shnum; i++)
        printf("[%d]\t %-20s %016lx\t %08lx\t %016lx\t %s\n", i, section_header_string_table + S_header[i].sh_name, S_header[i].sh_addr,
               S_header[i].sh_offset, S_header[i].sh_size, section_type_name(S_header[i].sh_type));

	}

void Print_Symbols(){

	int idx_SHT_SYMTAB=-1;
	int idx_SHT_DYNSYM=-1;
	Elf64_Shdr * S_header;
	S_header = (Elf64_Shdr *) (map_start + header->e_shoff);
	char *section_header_string_table_S = (char*)(map_start + S_header[header->e_shstrndx].sh_offset);

	for(int i = 0; i < header->e_shnum; i++)
	{
		switch(S_header[i].sh_type){
			case SHT_SYMTAB:
				idx_SHT_SYMTAB=i;
			break;

			case SHT_DYNSYM :
				idx_SHT_DYNSYM=i;
			break;

			default:
			break;
		}	
	}

	printf("\n%s\t %s\t\t %s\t %s\t %s\n", "[index]", "value", "section_index", "section_name", "symbol_name");

	Elf64_Sym * s_sym = (Elf64_Sym *) (map_start + S_header[idx_SHT_SYMTAB].sh_offset);   
    int size = S_header[idx_SHT_SYMTAB].sh_size / sizeof(Elf64_Sym);    // number of symbols 
    char *section_header_string_table = (char *) (S_header[S_header[idx_SHT_SYMTAB].sh_link].sh_offset + map_start);


    for (int i = 0; i < size; i++) {
    	if(s_sym[i].st_shndx!=65521)
        printf("%d:\t %016lx\t %d\t %s\t\t %s\n", i, s_sym[i].st_value, s_sym[i].st_shndx,
                section_header_string_table_S + S_header[s_sym[i].st_shndx].sh_name,
                 section_header_string_table + s_sym[i].st_name);
    	else
    		printf("%d:\t %016lx\t %s\t %s\t\t %s\n", i, s_sym[i].st_value, "ABS",
                "", section_header_string_table + s_sym[i].st_name);
    }


	if(idx_SHT_DYNSYM!=-1){
    	Elf64_Sym * s_sym_D = (Elf64_Sym *) (map_start + S_header[idx_SHT_DYNSYM].sh_offset);   
    	int size_D = S_header[idx_SHT_DYNSYM].sh_size / sizeof(Elf64_Sym);     
    	char *section_header_string_table_D = (char *) (S_header[S_header[idx_SHT_DYNSYM].sh_link].sh_offset + map_start);


    	for (int i = 0; i < size_D; i++){
    		if(s_sym_D[i].st_shndx!=65521)
        	printf("%d:\t %016lx\t %d\t %s\t\t %s\n", i, s_sym_D[i].st_value, s_sym_D[i].st_shndx,
                	section_header_string_table_S + S_header[s_sym_D[i].st_shndx].sh_name,
                	section_header_string_table_D + s_sym_D[i].st_name);
    		else
    			printf("%d:\t %016lx\t %s\t %s\t\t %s\n", i, s_sym_D[i].st_value, "ABS",
                	"", section_header_string_table_D + s_sym_D[i].st_name);
    	}
	}

	}

void Examine_ELF_File(){
	Set_File_Name(); // Queries the user for an ELF file name to be used and examined henceforth.
	
	if(fd!=-1) // Close any currently open file.
		close(fd);

	if( (fd = open(File_Name, O_RDWR)) < 0 ) { // Open the file for reading.
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

   if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }

   header = (Elf64_Ehdr *) map_start;

   // Check that the number is consistent with an ELF file, and refuse to continue if it is not.
   if(header->e_ident[0] != 0x7f || header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F'){
   		printf("Not an ELF");
   		munmap(map_start, fd_stat.st_size);
   		close(fd);
   		fd=-1;
   		return;
   }

   printf("\nELF Header:\n  Magic:  ");
  	for (size_t i = 0; i < EI_NIDENT; ++i)
   		printf (" %02hhx", header->e_ident[i]);

 	printf ("\n  Class:                             %s\n",
	  header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32"
	  : header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "Error");

 	printf ("  Data:                              %s\n",
	  header->e_ident[EI_DATA] == ELFDATA2LSB
	  ? "2's complement, little endian"
	  : header->e_ident[EI_DATA] == ELFDATA2MSB
	  ? "2's complement, big endian" : "Error");

 	printf ("  Entry point address:               %#lx\n",
	  header->e_entry);

 	printf ("  Start of section headers:          %lu %s\n",
	  header->e_shoff , "(bytes into file)");

 	printf ("  Size of section header:            %d %s\n",
	  header->e_shentsize , "(bytes)");

 	printf ("  Number of section headers:         %d\n",
	  header->e_shnum);

 	printf ("  Start of program headers:          %lu %s\n",
	  header->e_phoff , "(bytes into file)");

 	printf ("  Size of program header:            %d %s\n",
	  header->e_phentsize , "(bytes)");

 	printf ("  Number of program headers:         %d\n",
	  header->e_phnum);

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

void MAIN_init_functions_array_pointers(void* functions_array[]){
	void (*Toggle_Debug_Mode_ptr)() = &Toggle_Debug_Mode;
	functions_array[0] = Toggle_Debug_Mode_ptr;

	void (*Examine_ELF_File_ptr)() = &Examine_ELF_File;
	functions_array[1] = Examine_ELF_File_ptr;

	void (*Print_Section_Names_ptr)() = &Print_Section_Names;
	functions_array[2] = Print_Section_Names_ptr;

	void (*Print_Symbols_ptr)() = &Print_Symbols;
	functions_array[3] = Print_Symbols_ptr;

	void (*Quit_ptr)() = &Quit;
	functions_array[4] = Quit_ptr;
	

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

void MAIN_functions_array_names(char* names[]){
	names[0] = "Toggle Debug Mode";
	names[1] = "Examine ELF File";
	names[2] = "Print Section Names";
	names[3] = "Print Symbols";
	names[4] = "Quit";
	names[5] = 0;

	}

int main(){

	void* MAIN_functions_pointers[MAX];
	MAIN_init_functions_array_pointers(MAIN_functions_pointers);

	char* MAIN_functions_names[MAX];
	MAIN_functions_array_names(MAIN_functions_names);

    
	while(1){
	
	char OPTION[MAX];
	int i;

	printf("Choose action:\n");
  
		switch(verbos_main){
			case 1:
			//TODO

			default:
			for(int idx=0; MAIN_functions_names[idx]!=0; idx++)
			{
				printf("%d-%s\n", idx+1 ,MAIN_functions_names[idx]);
			}
			break;
		
   		
	}

  	fgets(OPTION, 15, stdin);

  	/* remove newline, if present */
  	i = strlen(OPTION)-1;
  	if( OPTION[ i ] == '\n') 
      	OPTION[i] = '\0';
    void (*p)();
    p =(MAIN_functions_pointers[atoi(OPTION)-1]);
	(*p)();

	printf("\n");
	}
 
    return 0;
	}
