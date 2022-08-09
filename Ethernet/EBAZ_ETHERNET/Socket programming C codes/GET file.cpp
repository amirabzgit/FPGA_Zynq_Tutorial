#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h>	//hostent
#include<arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>

#define PORT_NUMBER "80"
#define SERVER_ADDR "34.223.124.45"

#define MAXLINE 5096 /*max text line length*/
using namespace std;


int hostname_to_ip(char *  , char *);

int main()
{
	
	char hostname[50] ; 
	strcpy(hostname,"drivehq.com");
	char ip[100];
	char path[] = "features/dropbox.aspx";
	
	hostname_to_ip(hostname , ip);
	cout<< hostname << " resolved to " << ip <<endl;
	
	
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char Data[MAXLINE]; 
    char GET_request[] = "GET /";
    //strcpy(GET_request[],"GET /")
    strcat(GET_request,path);
    strcat(GET_request,"\r\n");
    // the final format of the request should be 
    // "GET /features/dropbox.aspx\r\n"

    portno = atoi(PORT_NUMBER);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
      cout<<"ERROR opening socket"<<endl;
      exit(0);
    }
        cout<<"successfully opened a socket"<<endl;

    server = gethostbyname(SERVER_ADDR);
    cout<<"Lets initialize the socket"<<endl;
// setting up the socket

 //Creation of the socket
 memset(&serv_addr, 0, sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;
 //serv_addr.sin_addr.s_addr= inet_addr("34.223.124.45");
 serv_addr.sin_addr.s_addr= inet_addr(ip);
 serv_addr.sin_port =  htons(atoi(PORT_NUMBER)); //convert to big-endian order

    cout<<"The socket is initialized, lets connect it."<<endl;

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      cout << "ERROR in connecting" << endl;
      exit(0);
    }
        cout << "Connected to the server, lets send the request" << endl;
        bzero(Data,MAXLINE);
        cout << "Connected to the server, lets send the request" << endl;
        if(send(sockfd,GET_request,strlen(GET_request),0)<0)
        {
        cout <<"can't send the request"<<endl;
        exit(0);
        }
       
        cout <<"The request is successfully sent"<<endl;
        bzero(Data,MAXLINE);
        recv(sockfd,Data,MAXLINE,0);
        cout << "the content of the reply is "<<endl;
        cout <<Data<<endl;
        cout<<"done the programming"<<endl;

close(sockfd);
return 1 ;
	
}
/*
	Get ip from domain name
 */

int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
		
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	
	return 1;
}
