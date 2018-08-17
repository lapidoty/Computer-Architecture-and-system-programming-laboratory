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
#include "LineParser.h"
#include "common.h"

#define BUFF_MAX 2048
#define PORT_FTP        2018              /* FTP connection port */
#define localhost     "127.0.0.1"     /* localhost */

struct client_state * state;

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
    //This function converts the character string src into a network
    //   address structure in the af address family, then copies the network
    //  address structure to dst.  The af argument must be either AF_INET or
    //  AF_INET6.  dst is written in network byte order.
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
    		if(strstr(buffer,"nok")==NULL){
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