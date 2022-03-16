#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define SIZE 2000

int main(int argc , char *argv[]) {
	
    int pipe_1[2], pipe_2[2], readbytes;;
    char buffer[SIZE], buffer2[SIZE];
    int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[SIZE];

    pipe(pipe_1);
    pipe(pipe_2);

    pid_t pid = fork();

    if (pid == 0) {
        // HIJO 9999
        
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1) {
            printf("Could not create socket");
        }
        puts("Socket created");
        
        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 9999 );
        
        //Bind the socket to the address and port number specified
        if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
            //print the error message
            perror("bind failed. Error");
            return 1;
        }
        puts("bind done Child");
        
        //Listen
        listen(socket_desc , 3);
        
        //Accept and incoming connection
        puts("Waiting for incoming connections in Child...");
        c = sizeof(struct sockaddr_in);
        
        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");
        
        while(1) {
            memset ( client_message, 0, 2000 );
            //Receive a message from client
            if (recv(client_sock , client_message , 2000 , 0) > 0) {
                printf("hola antes");
                printf("received message: %s\n", client_message);
                close(pipe_1[0]);
                write( pipe_1[1], client_message, SIZE);
                break;
            }
            printf("hola1");
        }


    } else {
        
        // PADRE 8888

        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1) {
            printf("Could not create socket");
        }
        puts("Socket created");
        
        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 8888 );
        
        //Bind the socket to the address and port number specified
        if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
            //print the error message
            perror("bind failed. Error");
            return 1;
        }
        puts("bind done Father");
        
        //Listen
        listen(socket_desc , 3);
        
        //Accept and incoming connection
        puts("Waiting for incoming connections in Father...");
        c = sizeof(struct sockaddr_in);
        
        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        //espera el mensaje del hijo
        close(pipe_1[1]);

        while((readbytes=read(pipe_1[0], buffer, SIZE )) > 0){
            printf("Received string in pipe 1: %s", buffer);
            break;
        }

        while(1) {
            memset ( client_message, 0, SIZE );
            
            
            if (recv(client_sock , client_message , SIZE , 0) > 0) {
                printf("received message in father socket: %s\n", client_message);
                //Send the message back to client
                send(client_sock , client_message , SIZE, 0);

                int sock;
                struct sockaddr_in server;
                char message[SIZE] , server_reply[SIZE];
                
                //Create socket
                sock = socket(AF_INET , SOCK_STREAM , 0);
                if (sock == -1) {
                    printf("Could not create socket");
                }
                puts("Socket created");
                
                server.sin_addr.s_addr = inet_addr("127.0.0.1");
                server.sin_family = AF_INET;
                server.sin_port = htons(7777);

                //Connect to remote server
                if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
                    perror("connect failed. Error");
                    return 1;
                }
	
	            puts("Connected\n");

                if( send(sock , client_message , SIZE , 0) < 0) {
                    puts("Send failed");
                    return 1;
                } else {
                    puts("send ok");
                }

                
            }

            client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

            
        }
        
    }
    wait(NULL);
    printf("Parent Complete\n");
    return 0;
	
}