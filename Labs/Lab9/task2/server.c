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
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <fcntl.h>
#include "LineParser.h"
#include "common.h"  
 
struct client_state * state;

void init_values(){


    state->conn_state = IDLE;
    state->server_addr = "nil";
    state->client_id = NULL;
    state->sock_fd = -1;
    }

int message_handler(char* message){

    switch(message[0]){

        case 'h':
            if(state->conn_state == IDLE){
                state->conn_state = CONNECTED; //Set conn_state to CONNECTED.
                state->client_id = "1";
                printf("Client %s connected\n", state->client_id);
                return 1;
            }
            else
                return -2;

        case 'b':
            if(state->conn_state == CONNECTED){
                printf("Client %s disconnected\n", state->client_id);
                return 2;
            }
            else
                return -2;

        case 'l':
            if(state->conn_state == CONNECTED){
                return 3;
            }
            else
                return -2;

        case 'g':
            if(state->conn_state == CONNECTED){
                return 4;
            }
            else
                return -2;

        case 'd':
                return 5;

        case 'n':
                return 6;

        default:
            return -1;

    }
        

    }


void sendfile_handler(char* filename , int socekt_fd_client , int size_to_send){
    int fd ;
    struct stat stat_buf;
    off_t offset = 0;
    int sent_bytes = 0;
    int peer_socket;
    int remain_data;
    peer_socket=socekt_fd_client;
     remain_data = size_to_send;
    /* Open the input file. */
    fd = open (filename, O_RDONLY);
    /* Stat the input file to obtain its size. */
    fstat (fd, &stat_buf);
    /* Blast the bytes from one file to the other. */
    while (((sent_bytes = sendfile(peer_socket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
        {
                remain_data -= sent_bytes;
        }
    /* Close up. */
    //close (fd);

    }

int main(int argc , char *argv[])
{
    int socket_desc , c , soc_cli , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    state = (struct client_state*)malloc(sizeof(struct client_state));
    //gethostname(s , 100);
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 2018 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    //Listen
    listen(socket_desc , 1);
     
    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
   soc_cli = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (soc_cli < 0)
    {
        perror("accept failed");
        return 1;
    }

    //Receive a message from client
    while( (read_size = recv(soc_cli , client_message , 2000 , 0)) > 0 )
    {
        int action = message_handler(client_message);
        char* s;
        char* file_name_string;
        int file_size_int;
        switch(action){

            case 1:
                if (send(soc_cli , "hello 1" , 10 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                    break;

            case 2:
                if (send(soc_cli , "bye" , 3 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                    close(soc_cli);
                    break;

            case -2:
                if (send(soc_cli , "nok state" , 9 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                    close(soc_cli);
                    break;

            case -1:
                if (send(soc_cli , "ERROR: Unknown message" , 50 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                    close(soc_cli);
                    break;

            case 3:
                s = list_dir();
                if (send(soc_cli , "ok" , 2 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                if (send(soc_cli , s , 2024 , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("Listed files at %s\n", cwd);
                break;

            case 4:
                // First, the server will calculate the size of <file_name> (use file_size in common.c)
                file_name_string = get_Second_Word(client_message);
                file_size_int = file_size(file_name_string);

                char s[3] = "ok ";
                char buffer_concat[8];
                sprintf(buffer_concat, "%d          ", file_size_int);
                strcat(s, buffer_concat);
                // Respond with "ok %d" where %d is the file size

                if (send(soc_cli , s , strlen(s) , 0) < 0)
                    {
                    perror("Send failed");
                    exit(errno);
                    }

                // The server will set the client state to DOWNLOADING
                    state->conn_state = DOWNLOADING ;

                // Send the file use sendfile
                    sendfile_handler(file_name_string , soc_cli , file_size_int);
                    char buffer_done[10];
                    bzero(buffer_done, 10);

                    break;

             case 5:
                            state->conn_state = CONNECTED ;
                            if (send(soc_cli , "ok" , 2 , 0) < 0)
                            {
                                perror("Send failed");
                                exit(errno);
                            }
                            printf("\nSent file '%s'\n", file_name_string);
                            break;

             case 6:           
                         if (send(soc_cli , "nok done" , 8 , 0) < 0)
                            {
                                perror("Send failed");
                                exit(errno);
                            }
                            break;
            }
         

    }

     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
    }

