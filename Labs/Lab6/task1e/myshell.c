#include <linux/limits.h>
#include "LineParser.h"
#include "job_control.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define LINE_MAX 2048
#define COMMAND_FG 4
#define COMMAND_BG 5


struct job* first_in_list=NULL;
struct termios* first_termios=NULL;

void sig_handler(int signum)
{
    //printf("Received signal %d\n", signum);

}


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

int get_command_type(char *command) {
 	if (strcmp(command, "fg") == 0) {
        return COMMAND_FG;
    } else if (strcmp(command, "bg") == 0) {
        return COMMAND_BG;
    }
  return -1;
}

int execute_proccess(job* job , cmdLine *pCmdLine , int in_fd, int out_fd)
{
	// Change status
	job->status = RUNNING;

	// Handeling executing
	pid_t cpid;
	//pid_t w;
	//int wstatus;

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (cpid == 0) {
		//Set the signal handlers back to default.
		signal(SIGQUIT, SIG_DFL);
		signal(SIGCHLD , SIG_DFL);

		//Set the process group id
		job->pgid = getpid();
    	setpgid(0, job->pgid);


		if(pCmdLine->inputRedirect!=NULL){
			int fdIn = open(pCmdLine->inputRedirect, 0002|0200, 0644);
			if (fdIn < 0) { perror("open failed1"); return getpid();}
			if (dup2(fdIn, 0) < 0) { }
    		close(fdIn);
		}
		if(pCmdLine->outputRedirect!=NULL){
			int fdOut = open(pCmdLine->outputRedirect, 0002|0200|0100, 0644);
			if (fdOut < 0) { perror("open failed2"); }
			if (dup2(fdOut, 1) < 0) { }
    		close(fdOut);
		}

		// Choose the correct input and output streams
		if (in_fd != 0) {
            dup2(in_fd, 0);
            close(in_fd);
        }

        if (out_fd != 1) {
            dup2(out_fd, 1);
            close(out_fd);
        }
		execute(pCmdLine);
		
	} else {
		job->pgid = cpid;
    	setpgid(0, job->pgid);


	}

return cpid;
}



void execute_job(job* job , cmdLine *pCmdLine){
if(job!=NULL){

 int in_fd = 0, fd[2];
 int last_fd = -1;
 //int wstatus;
    for (struct cmdLine *proc = pCmdLine; proc != NULL; proc = proc->next) {

        if (proc->next != NULL) {
            pipe(fd);
            execute_proccess(job , proc, in_fd, fd[1]);
            close(fd[1]);
            in_fd = fd[0];
        } else {
             int out_fd = 1;
             last_fd = execute_proccess(job , proc, in_fd, out_fd);
        }
    }
    //Non blocking mode   
    if(last_fd!=-1 && pCmdLine->blocking==1)
    {

    	//while (waitpid(last_fd, &wstatus, WNOHANG )!=-1);
    	 run_job_in_foreground(&first_in_list , job , 0 , first_termios , getpid()); //Command is executed in the foreground
    	 
    	 
    }
    else if(pCmdLine->blocking==0 && pCmdLine->next!=NULL){
    	 run_job_in_foreground(&first_in_list , job , 0 , first_termios , getpid()); //Command is executed in the foreground
    }
    //Blocking mode
    else
    {
    	run_job_in_background(job, 0);
    	
    }
    	
}

else if(get_command_type(pCmdLine->arguments[0])==COMMAND_FG){
    run_job_in_foreground(&first_in_list , find_job_by_index(first_in_list,atoi(pCmdLine->arguments[1])) 
    , atoi(pCmdLine->arguments[1]) ,first_termios, getpid()); //fg command is used
}
else if(get_command_type(pCmdLine->arguments[0])==COMMAND_BG){
    run_job_in_background(find_job_by_index(first_in_list,atoi(pCmdLine->arguments[1])) , atoi(pCmdLine->arguments[1]));
}


}

int main(int argc, char *argv[]) {

	//Signals treatment 
	signal(SIGTTOU, sig_handler);
    //signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGQUIT, sig_handler);
	signal(SIGCHLD , sig_handler);

	//Set the process group of the shell to its process id (getpid).
	pid_t pid = getpid();
    setpgid(pid, pid);
	first_termios = malloc (sizeof(struct termios));


	//Main loop
	while (1) {

		printDir();

		char line[LINE_MAX];

		if (fgets(line, LINE_MAX, stdin) != NULL) {

		if(strcmp(line,"quit\n")==0){

			break;
		}

		// Implement jobs shell command
		if(strcmp(line,"jobs\n")==0){
			print_jobs(&first_in_list);
		}
		else{
			tcgetattr(0, first_termios); //Save default terminal attributes to restore them back when a process running in the foreground ends.
			if(strstr(line,"fg")!=NULL || strstr(line,"bg")!=NULL){
				execute_job(NULL, parseCmdLines(line));
			}
			else{
			job* curr_job = add_job(&first_in_list , line); // Adding job to the list
			tcgetattr(0, curr_job->tmodes); 
			execute_job(curr_job, parseCmdLines(line));
		}
		}
		
		}
		

		
		

	}
	
	return 0;
}

