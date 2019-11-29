/*
  Name: Rajendra Prajapat
	Client Side

	gcc client.c -o cli
  ./cli Server_IP port
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<ctype.h>
#include <pthread.h>
int ans[50],n;
char buffer[1024];

//error handling
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//fuction for communication
void Fun(int sockfd)
{
  for(int i=0;i<2;i++)
  {
    int num ;
    bzero(buffer,256);
    n = read(sockfd,buffer,255);				//Read Server String
    if (n < 0)
         error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d" , &num);						//Enter number of elements
    write(sockfd, &num, sizeof(int));     			//Send No to Server
    bzero(ans,sizeof(ans));
    n=read(sockfd , &num , sizeof(num));				//Read number of elements from Server
    if (n < 0)
         error("ERROR reading from socket");
    n=read(sockfd , ans , sizeof(ans));				//Read Answer from Server
    if (n < 0)
         error("ERROR reading from socket");
    printf("Server: ");
    for(int i=0;i<num;i++)
    {
      printf(" %d ",ans[i]);
    }
    printf("\n");
  }
  close(sockfd);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; //to store IP address
    if (argc < 3)
    {
       fprintf(stderr,"to run use : %s server_IP port \n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]); //port number
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket

    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]); //server IP

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); //coping server address to serv-addr

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    Fun(sockfd);
    return 0;
}
