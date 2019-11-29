/*
	Server Side
	Please pass port no as command line argument
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>
#include <arpa/inet.h>
int ans[50];
int n;
int cli_number=1;
int isPrime(int n)
{
    if (n <= 1)
        return 0;
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}
void primeGen()
{
  int cnt = 0;
  for(int i=2;i<100 && cnt<50 ;i++)
  {
    if(isPrime(i))
    {
      ans[cnt]=i;
      cnt++;
    }
  }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void Fun(int var,int clber)
{
    // int *new = (int *)var;
    int newsockfd = var;
    int num;
    for(int i=0;i<2;i++)
    {
      n = write(newsockfd,"Enter Number of elements : \n",strlen("Enter Number of elements : \n"));         //Ask for Number 1
      if (n < 0) error("ERROR writing to socket");
      read(newsockfd, &num, sizeof(int));
      printf("Client %d - Number of elements is : %d\n" , num,clber);
      write(newsockfd , &num , sizeof(num));
      write(newsockfd , ans , sizeof(ans));
    }
    close(newsockfd);
}
void *thread_handler(void *i)
{
      int sock = *(int*)i;
      cli_number++;
      Fun(sock,cli_number);
}
int main(int argc, char *argv[])
{
     primeGen();
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]); //port number

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
    while((newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen)))
    {
        puts("Connection accepted");

        char str[100];
        printf("\n REQUEST FROM %s PORT %d\n", inet_ntop(AF_INET,&cli_addr.
sin_addr,str,sizeof(str)),htons(cli_addr.sin_port));

        pthread_t ThID;
        int *new_sock = malloc(sizeof *new_sock);
        *new_sock = newsockfd;
        if(pthread_create(&ThID,NULL,thread_handler,(void*)new_sock) < 0)
        {
          perror("could not create thread");
          return 1;
        }
        puts("Handler assigned");
    }

    if (newsockfd<0)
    {
      perror("accept failed");
      return 1;
    }
     close(sockfd);
     close(newsockfd);
     return 0;
}
