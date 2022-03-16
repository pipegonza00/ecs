#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc , char *argv[]) {
	int sockCLI;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server2 , client;
	char client_message[2000];
	
	//Create socket
	sockCLI = socket(AF_INET , SOCK_STREAM , 0);
	if (sockCLI == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(9999);

	//Connect to remote server
	if (connect(sockCLI , (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	puts("Socket created desc");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 7777 );
	
	//Bind the socket to the address and port number specified
	if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done desc");
	
	//Listen8888
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	puts("Connected\n");

    strcpy(message, "7777 127.0.0.1\n");

    if( send(sockCLI , message , strlen(message) , 0) < 0) {
		puts("Send failed");
		return 1;
	} else {
		puts("send ok");
    }

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
            printf("received message: %s\n", client_message);
            //Send the message back to client
            send(client_sock , client_message , strlen(client_message), 0);
			
    	}
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	}


	return 0;
}