// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 5001
int main()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char helloD[] = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
		== 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for a client to connect ....\n");
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("A connection request is received and accepted...\n");
	
	
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, "welcom Zynq", strlen("welcom Zynq"), 0);
	printf("welcom Zynq is sent back\n");
	//send(new_socket, "hello", strlen("hello"), 0);
	//printf("Hello message sent\n");
	int CNTR = 0 ;
/*	
	while(1){
	valread = read(new_socket, buffer, 1024);
	if (buffer)
	{
		printf("%s\n", buffer);
		bzero(buffer,1024);
		CNTR++;	
	}
	if ((CNTR == 4) || (CNTR == 15))
	{
		printf("Now sending %s\n", helloD);
		send(new_socket, helloD, strlen(helloD), 0);	
		helloD[0]++;
		CNTR++;
	}
	
	//printf("CNTR = %d, received message is: %s\n",CNTR,  buffer);
	//CNTR++;
	}
	*/
	
// closing the connected socket
	close(new_socket);
// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

