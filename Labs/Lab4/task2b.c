#include "lab4_util.h"


#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_LSEEK 8
#define SYS_EXIT 60
#define SYS_MMAP 9
#define SYS_GETDENTS 78

#define O_RDONLY 0000
#define O_WRONLY 0001 
#define O_RDRW 0002
#define O_APPEND 0008
#define O_TRUNC 0200
#define O_CREAT 0100

#define S_IRWXU 0700 
#define S_IRUSR 0400 
#define S_IWUSR 0200 
#define S_IXUSR 0100 

struct linux_dirent {
	long           d_ino;
	long          d_off;
	unsigned short d_reclen;
	char           d_name[];
};

#define BUF_SIZE 1024

#define STDOUT 1

extern int system_call();

char nameToSearch[1024];
int nameSearch=0;
char * 
strcpy(char *s1, const char *s2) { 
	char *s = s1; 
	while ((*s++ = *s2++) != 0) ;
	return (s1);
}
int strLen(const char *_array)
{
	int str_len=0;
	while(*(_array++))  
		str_len++;

	return str_len;
}
char *
strcat(char *s1, const char *s2)
{
	strcpy(&s1[strLen(s1)], s2);
	return s1;
}


int strcmp (char *s1, char *s2)
{
	if (!s1 && !s2) return 0;
	if (s1 && !s2) return 1;
	if (!s1 && s2) return -1;

	while (*s1 && *s2 && *s1 == *s2) s1++, s2++;

	if (!*s1 && !*s2) return 0;
	else if (*s1 > *s2) return 1;
	else return -1;
}


void manager(char dirname[]){

	int nread,fd;
	char buf[BUF_SIZE];
	struct linux_dirent *d;
	int bpos;
	char d_type;
	char* ERROR = "Error";
	int err=0;
	fd=system_call(SYS_OPEN, dirname, O_RDONLY);
	if(fd<0){
		err = system_call(SYS_WRITE, STDOUT ,"Error",strLen(ERROR) );
		if(err<0){
			system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
			system_call(SYS_EXIT,1);
			return;
		}
	}
	for ( ; ; ) {

		nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);

		if (nread == 0)
			break;

		for (bpos = 0; bpos < nread;) {

			d = (struct linux_dirent *) (buf + bpos);
			d_type = *(buf + bpos + d->d_reclen - 1);

			int dirname_len = strLen(dirname);
			char subdir[1024];
			strcpy(subdir, dirname);
			strcat(subdir + dirname_len, "/");
			strcat(subdir + dirname_len + 1, d->d_name);

			if(d->d_name[0]!='.' && d->d_name[1]!='.'){
				if(nameSearch==0 || (strcmp(nameToSearch , d->d_name)==0)){
					err = system_call(SYS_WRITE, STDOUT ,subdir,strLen(subdir) );
					if(err<0){
						system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
						system_call(SYS_EXIT,1);
						return;
					}
					err = system_call(SYS_WRITE, STDOUT , "\n",1 );
					if(err<0){
						system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
						system_call(SYS_EXIT,1);
						return;
					}
				}
			}
			if(d->d_ino != 0 && d_type == 4 && d->d_name[0]!='.')
			{
				manager(subdir);	
			}

			bpos += d->d_reclen;
		}

	}

	err = system_call(SYS_CLOSE , fd);
	if(err<0){
		system_call(SYS_WRITE, STDOUT, ERROR, sizeof(ERROR));
		system_call(SYS_EXIT,1);
		return;
	}
}



int main (int argc , char* argv[])
{
	if(argc>1){
		if(strcmp(argv[1],"-n")==0){
			nameSearch=1;
			strcpy(nameToSearch,argv[2]);
		}
	}

	manager(".");
	return 0;
}

