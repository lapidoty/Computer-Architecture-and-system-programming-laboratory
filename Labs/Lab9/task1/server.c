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

        default:
            return -1;

    }
        

    }


int main(int argc , char *argv[])
{
    int socket_desc , c , soc_cli , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    state = (struct client_state*)malloc(sizeof(struct client_state));
    //gethostname(s , 100);
     
    //Create socket
    //  AF_INET = IPv4 Internet protocols , 
    //  SOCK_STREAM = Provides sequenced, reliable, two-way, connection-
    //                  based byte streams.  An out-of-band data transmission
    //                   mechanism may be supported.

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
    //When a socket is created with socket(2), it exists in a name space
    //   (address family) but has no address assigned to it.  bind() assigns
    //   the address specified by addr to the socket referred to by the file
    //   descriptor sockfd.

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    //Listen
    //listen() marks the socket referred to by sockfd as a passive socket,
    //   that is, as a socket that will be used to accept incoming connection
    //   requests using accept(2).

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
                printf("\nListed files at %s\n", cwd);
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

