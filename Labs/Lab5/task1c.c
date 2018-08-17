#include <linux/limits.h>
#include "LineParser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define LINE_MAX 2048

void printDir()
{
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	printf("%s$ ", cwd);
}

void execute(cmdLine *pCmdLine){
	execvp(pCmdLine->arguments[0], pCmdLine->arguments);
		perror("execve");   /* execve() returns only on error */
		exit(EXIT_FAILURE);
}

void process(cmdLine *pCmdLine)
{

	if(pCmdLine->blocking == 1){
	pid_t cpid, w;
	int wstatus;

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (cpid == 0) {
		if(pCmdLine->inputRedirect!=NULL){
			int fdIn = open(pCmdLine->inputRedirect, 0002|0200, 0644);
			if (fdIn < 0) { perror("open failed1"); return;}
			if (dup2(fdIn, 0) < 0) { }
    		close(fdIn);
		}
		if(pCmdLine->outputRedirect!=NULL){
			int fdOut = open(pCmdLine->outputRedirect, 0002|0200|0100, 0644);
			if (fdOut < 0) { perror("open failed2"); }
			if (dup2(fdOut, 1) < 0) { }
    		close(fdOut);
		}

		execute(pCmdLine);
		
	} else {
		w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
		if (w == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}
	else{
	execute(pCmdLine);
	}


}



int main(int argc, char *argv[]) {


	while (1) {

		printDir();

		char line[LINE_MAX];

		if (fgets(line, LINE_MAX, stdin) != NULL) {



		if(strcmp(line,"quit\n")==0){
			break;
		}
		else{
			process(parseCmdLines(line));
		}
		}
		

		
		

	}
	return 0;
}

