#include "job_control.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/**
* Receive a pointer to a job list and a new command to add to the job list and adds it to it.
* Create a new job list if none exists.
**/
job* add_job(job** job_list, char* cmd){
	job* job_to_add = initialize_job(cmd);
	
	if (*job_list == NULL){
		*job_list = job_to_add;
		job_to_add -> idx = 1;
	}	
	
	else{
		int counter = 2;
		job* list = *job_list;
		while (list -> next !=NULL){
			printf("adding %d\n", list->idx);
			list = list -> next;
			counter++;
		}
		job_to_add ->idx = counter;
		list -> next = job_to_add;
	}
	
	return job_to_add;
}


/**
* Receive a pointer to a job list and a pointer to a job and removes the job from the job list 
* freeing its memory.
**/
void remove_job(job** job_list, job* tmp){
	if (*job_list == NULL)
		return;
	job* tmp_list = *job_list;
	if (tmp_list == tmp){
		*job_list = tmp_list -> next;
		free_job(tmp);
		return;
	}
		
	while (tmp_list->next != tmp){
		tmp_list = tmp_list -> next;
	}
	tmp_list -> next = tmp -> next;
	free_job(tmp);
	
}

/**
* receives a status and prints the string it represents.
**/
char* status_to_str(int status)
{
  static char* strs[] = {"Done", "Suspended", "Running"};
  return strs[status + 1];
}


/**
*   Receive a job list, and print it in the following format:<code>[idx] \t status \t\t cmd</code>, where:
    cmd: the full command as typed by the user.
    status: Running, Suspended, Done (for jobs that have completed but are not yet removed from the list).
  
**/
void print_jobs(job** job_list){

	job* tmp = *job_list;
	update_job_list(job_list, FALSE);
	while (tmp != NULL){
		printf("[%d]\t %s \t\t %s", tmp->idx, status_to_str(tmp->status),tmp -> cmd); 
		
		if (tmp -> cmd[strlen(tmp -> cmd)-1]  != '\n')
			printf("\n");
		job* job_to_remove = tmp;
		tmp = tmp -> next;
		if (job_to_remove->status == DONE)
			remove_job(job_list, job_to_remove);
		
	}
 
}


/**
* Receive a pointer to a list of jobs, and delete all of its nodes and the memory allocated for each of them.
*/
void free_job_list(job** job_list){
	while(*job_list != NULL){
		job* tmp = *job_list;
		*job_list = (*job_list) -> next;
		free_job(tmp);
	}
	
}


/**
* receives a pointer to a job, and frees it along with all memory allocated for its fields.
**/
void free_job(job* job_to_remove){

        //free(job_to_remove->cmd);
        //free(job_to_remove->tmodes);
        //free(job_to_remove);
}



/**
* Receive a command (string) and return a job pointer. 
* The function needs to allocate all required memory for: job, cmd, tmodes
* to copy cmd, and to initialize the rest of the fields to NULL: next, pigd, status 
**/

job* initialize_job(char* cmd){

	job* newJob = (job*)malloc (sizeof(job));

	char* newCmd = malloc(strlen(cmd));
	strcpy(newCmd, cmd);
	newJob->cmd = newCmd;
	newJob->idx=0;
	newJob->pgid=0;
	newJob->status=0;
	struct termios* newTermios = malloc (sizeof(struct termios));
	newJob->tmodes = newTermios;
	newJob->next = NULL;

return newJob;

}


/**
* Receive a job list and and index and return a pointer to a job with the given index, according to the idx field.
* Print an error message if no job with such an index exists.
**/
job* find_job_by_index(job* job_list, int idx){
		job* list = job_list;
		while (list !=NULL){
			if(list->idx == idx){
				return list;
			}
			list = list -> next;
		}
		printf("No Index Found, return null\n");
		return NULL;
}


/**
* Receive a pointer to a job list, and a boolean to decide whether to remove done
* jobs from the job list or not. 
**/
void update_job_list(job **job_list, int remove_done_jobs){

		int w;
		int wstatus;
		job* tmp = *job_list;
		while (tmp !=NULL){
	if(tmp->status == RUNNING){
			w = waitpid(tmp->pgid, &wstatus, WNOHANG);
			if (w == -1) {
				perror("waitpid");
				//exit(EXIT_FAILURE);
			}
			else{
				tmp->status = DONE;
			if(remove_done_jobs==TRUE){
				printf("[%d]\t %s \t\t %s", tmp->idx, status_to_str(tmp->status),tmp -> cmd); 
		
			if (tmp -> cmd[strlen(tmp -> cmd)-1]  != '\n')
				printf("\n");
				job* job_to_remove = tmp;
				tmp = tmp -> next;
				if (job_to_remove->status == DONE)
					remove_job(job_list, job_to_remove);
			
		}
	}
}
			tmp = tmp -> next;
		}

	


}

/** 
* Put job j in the foreground.  If cont is nonzero, restore the saved terminal modes and send the process group a
* SIGCONT signal to wake it up before we block.  Run update_job_list to print DONE jobs.
**/

void run_job_in_foreground (job** job_list, job *j, int cont, struct termios* shell_tmodes, pid_t shell_pgid){
	int wstatus;
	//Check if the job is done
 	 if(waitpid(j->pgid, &wstatus, WNOHANG )==-1){
 	 	//update_job_list(job_list , TRUE);
 	 }
 	 else{
 	 	//If the job has not finished yet, then put it in the foreground
 	 	tcsetpgrp(0, j->pgid);
 	 	//If cont is 1 and the job's status was SUSPENDED, then set the attributes of the terminal to that of the job's
 	 	if(cont!=0 && j->status==SUSPENDED)
 	 		tcsetattr (j->pgid, TCSADRAIN, j->tmodes);

 	 	//Send SIGCONT signal to the process group of the job.
 	 	if (kill(-(j->pgid), SIGCONT) < 0) {
        	//printf("fg %d: job not found\n", j->pgid);
    	}

    	//Wait for the job to change status using waitpid
    	//Change the status of the job to SUSPENDED if the process group receives: a SIGTSTP (ctrl-z) - see WUNTRACED and WIFSTOPPED

    	waitpid(j->pgid, &wstatus, WUNTRACED);
    	if(WSTOPSIG(wstatus)){
    		j->status=SUSPENDED;
    	}
    	else if(WIFEXITED(wstatus) || WIFSIGNALED(wstatus)){
    		j->status=DONE;

    		if(cont!=0){
    		printf("[%d]\t %s \t\t %s", j->idx, status_to_str(j->status),j -> cmd);
    		remove_job(job_list, j);
    		}
    	}

    	//Put the shell back in the foreground.
    	signal(SIGTTOU, SIG_IGN);
    	tcsetpgrp(0, getpid());
    	signal(SIGTTOU, SIG_DFL);

    	//Save the terminal attributes in the job tmodes.
    	tcgetattr(0, j->tmodes);

		//Restore the shell’s terminal attributes using the shell tmodes
    	tcsetattr (0, TCSADRAIN, shell_tmodes);

    	//Check for status update of jobs that are running in the background
    	//update_job_list(job_list , TRUE);

 	 }
 	 



}

/** 
* Put a job in the background.  If the cont argument is nonzero, send
* the process group a SIGCONT signal to wake it up.  
**/

void run_job_in_background (job *j, int cont){	
	//sets its status to RUNNING
 	j->status = RUNNING;


 	//Send SIGCONT signal to the process group of the job
 	if(cont!=0){
 		if (kill(-(j->pgid), SIGCONT) < 0) {
 			//printf("fg %d: job not found\n", j->pgid);
 	}
   }
}
