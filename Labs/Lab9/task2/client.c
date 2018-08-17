#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <sys/types.h>
#include <time.h> 
#include <ctype.h>
#include "LineParser.h"
#include "common.h"

#define BUFF_MAX 2048
#define PORT_FTP        2018              /* FTP connection port */
#define localhost     "127.0.0.1"     /* localhost */

struct client_state * state;


char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

void init_values(){


	state->conn_state = IDLE;
	state->server_addr = "nil";
	state->client_id = NULL;
	state->sock_fd = -1;
	}

void printLine(){

	printf("server:%s> ", state->server_addr);

	}

void debug_Function(char* message){

	if(debug_Flag==1)
		fprintf(stderr, "%s|Log: %s ", state->server_addr , message);

	}

void exec_connect(char* add){

    struct sockaddr_in dest;
    char buffer[BUFF_MAX];

    // Open socket for streaming
    if ( (state->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Socket");
        exit(errno);
    }

    // Initialize server address/port struct
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT_FTP);
    if(inet_pton(AF_INET, add , &dest.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        exit(errno);
    } 

    // Connect to server
    if ( connect(state->sock_fd, (struct sockaddr*)&dest, sizeof(dest)) != 0 ) //Set sock_fd to the connection's socket.
    {
        perror("Connect ");
        exit(errno);
    }
    // Send the string "hello"
    if( send(state->sock_fd , "hello" , 5 , 0) < 0)
        {
            perror("Send failed");
            exit(errno);
        }
    state->conn_state = CONNECTING; // The client will then set its conn_state to CONNECTING

    bzero(buffer, BUFF_MAX);
    recv(state->sock_fd, buffer, sizeof(buffer), 0); //*** blocking funcion***

    debug_Function(buffer);

    if(strstr(buffer,"hello")!=NULL){
    	state->server_addr = add;
		char* word;
		word = strtok(buffer, " ");
		word = strtok(NULL, " ");
		char* newCmd = malloc(sizeof(word));
		strcpy(newCmd, word);
		state->client_id = newCmd; //Set client_id to <client_id>.
		state->conn_state = CONNECTED; //Set conn_state to CONNECTED.

    }else{
    	perror("The server did not respond as: hello <client_id> ");
    	close(state->sock_fd);	
    }


    
	}



int exec(cmdLine *pCmdLine){

	char* cmd= pCmdLine->arguments[0];
	//char** args= pCmdLine->arguments;
	//int args_len= pCmdLine->argCount;

//*************************** connect *************************
	if(strstr(cmd,"conn")!=NULL){
		if(state->conn_state == IDLE){ //connect may only run in IDLE state
			exec_connect(pCmdLine->arguments[1]);
		}
		else{
			fprintf(stderr, "Server Error: %s", "Try to connect when not in IDLE state"); 
			init_values();
			return -2;
		}
	}
	
//*************************** disconnect *************************
	if(strstr(cmd,"bye")!=NULL){
		if(state->conn_state == CONNECTED){ //bye may only run in CONNECTED state

			close(state->sock_fd);

			init_values();
		}
		else{
			fprintf(stderr, "Server Error: %s", "Try to disconnect when not in CONNECTED state"); 
			init_values();
			return -2;
		}
	}

//*************************** ls *************************
	if(strstr(cmd,"ls")!=NULL){
		if(state->conn_state == CONNECTED){ //bye may only run in CONNECTED state

			if( send(state->sock_fd , "ls" , 2 , 0) < 0)
        	{

            perror("Send failed");
            exit(errno);

        	}

        	char buffer[BUFF_MAX];
        	bzero(buffer, BUFF_MAX);
    		recv(state->sock_fd, buffer, 2, 0); //*** blocking funcion***
    		if(strstr(buffer,"no")==NULL){
				if(strstr(buffer,"ok")!=NULL){
    				recv(state->sock_fd, buffer, sizeof(buffer) , 0);
    				printf("%s\n", buffer);
				}
			}

		}

		else{

			fprintf(stderr, "Server Error: %s", "Try to disconnect when not in CONNECTED state"); 
			init_values();
			return -2;

		}
	}


//*************************** get *************************
	if(strstr(cmd,"get")!=NULL){
		if(state->conn_state == CONNECTED){ //bye may only run in CONNECTED state

			// Get will cause the client to send the message "get %s" to the server, where %s is a file name
			char * s = (char*) malloc(strlen(pCmdLine->arguments[1])+4);
			strcpy(s , "get ");
			strcat(s , pCmdLine->arguments[1]);

			if( send(state->sock_fd , s , strlen(pCmdLine->arguments[1])+4 , 0) < 0)
        	{

            perror("Send failed");
            exit(errno);

        	}

        	char buffer[1024];
        	bzero(buffer, 1024);
    		recv(state->sock_fd, buffer, 10, 0); // Blocking funcion
    		// Reply with the message "ok <file_size>". 
    		if(strstr(buffer,"no")==NULL){
				if(strstr(buffer,"ok")!=NULL){
					char* file_size_string;
					int file_size_int;
					file_size_string = get_Second_Word(buffer);
					file_size_int = atoi(file_size_string);
					
					// Create <file_name>.tmp
					char * s = (char*) malloc(strlen(pCmdLine->arguments[1])+4);
					strcpy(s , pCmdLine->arguments[1]);
					strcat(s , ".tmp");

					// Set its state to DOWNLOADING
					state->conn_state = DOWNLOADING;

					// The client will fopen

   					// Loop that keeps recving BUFF_MAX chunks for the file and writing
					//bzero(buffer, 1024);
   					FILE *received_file;
   					ssize_t len;
   					int remain_data = 0;
        			received_file = fopen(s, "w");
        			if (received_file == NULL)
        				{
                			fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                			exit(EXIT_FAILURE);
        				}

        			remain_data = file_size_int;

        			//int first = 0;
        			//char *tmp_buffer;

        			while ((remain_data > 0) && ((len = recv(state->sock_fd, buffer, 1024, 0)) > 0)  )
        			{
        				/*/
        				if(first==0){
        					tmp_buffer = trimwhitespace(buffer);
        					first = 1;
        					fwrite(tmp_buffer, sizeof(char), strlen(tmp_buffer), received_file);
        				}
        				else
        				{
        					/*/
        					fwrite(buffer, sizeof(char), len, received_file);
        				//}
                			remain_data -= len;
                			//fprintf(stdout, "Receive %ld bytes and we hope :- %d bytes\n", len, remain_data);
        			}
        			//fclose(received_file);
    				
    				// Once <file_size> bytes are recved, the client will send "done"
    				if( send(state->sock_fd , "done" , 4 , 0) < 0)
        				{

            			perror("Send failed");
            			exit(errno);

        				}
        			// The client should wait at most 10 seconds for the "ok" message

        			
   					struct timeval tv;
					tv.tv_sec = 10;        // 10 Secs Timeout
					tv.tv_usec = 0;        // Not init'ing this can cause strange errors
					setsockopt(state->sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
					

					// wait for the server to reply with "ok"
					char buffer[BUFF_MAX];
        			bzero(buffer, BUFF_MAX);
    				recv(state->sock_fd, buffer, 2, 0); // Blocking funcion
    				// Reply with the message "ok <file_size>". 
    				if(strstr(buffer,"no")==NULL){
						if(strstr(buffer,"ok")!=NULL){
							// Client will move (i.e. rename) <file_name>.tmp to <file_name> and set its state to CONNECTED
								state->conn_state = CONNECTED;
								int ret;
   								ret = rename(s, "working!"); // Change to pCmdLine->arguments[1]
	
   								if(ret == 0) {
      								//printf("File renamed successfully\n");
   								}
   								else {
     								printf("Error: unable to rename the file\n");
   								}
						}
					}
					else
					{

					/*If the client receives a "nok …" message or no message at all after 10 seconds,
					 the client will delete <file_name>.tmp and print the error message "Error while downloading file %s" where %s is <file_name>.*/
					printf("Error while downloading file %s" , pCmdLine->arguments[1]);
					remove(s);

					}


				}
			}

		}

		else{

			fprintf(stderr, "Server Error: %s", "Try to disconnect when not in CONNECTED state"); 
			init_values();
			return -2;

		}
	}

		
	
	return 0;
	}



int main(int argc, char *argv[]) {
	state = (struct client_state*)malloc(sizeof(struct client_state));
	init_values();

	//Main loop
		while (1) {

			printLine(); //Display a prompt - "server:%s>" where %s is the address of your server (see below) or the string "nil" if not connected to any server

			char line[BUFF_MAX];

			if (fgets(line, BUFF_MAX, stdin) != NULL) {

			if(strcmp(line,"quit\n")==0){

				break;
			}

			exec(parseCmdLines(line));

			}

		}
	
	return 0;

	}