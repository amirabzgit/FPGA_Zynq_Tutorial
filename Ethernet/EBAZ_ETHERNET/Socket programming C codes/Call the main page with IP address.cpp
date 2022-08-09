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

int main()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char Data[MAXLINE];
    //char * Data;
    const char * GET_request = "GET /\r\n";

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
 serv_addr.sin_addr.s_addr= inet_addr("34.223.124.45");
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
    return 0;
}
