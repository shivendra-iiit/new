#include<iostream>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

using namespace std;

#define PORT 4000

struct msg{
  char text[100];
};

int sockfd,len;
struct sockaddr_in myaddr,remaddr;

void *function(void *arg)
{

    struct msg buff;
    bzero(&buff,sizeof(buff));
    int n= 0;
    cout<<"Enter message : ";
    while((buff.text[n++]=getchar()) != '\n');
    cout<<endl;

    sendto(sockfd,&buff,sizeof(buff),0,(struct sockaddr *)&remaddr,len);
}

int main()
{
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd == -1)
    {
      cout<<"socket creation failed..."<<endl;
      exit(0);
    }
    else
      cout<<"socket created successfully..."<<endl;

    bzero(&myaddr,sizeof(myaddr));

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT);

    if(bind(sockfd,(struct sockaddr*)&myaddr,sizeof(myaddr)) != 0 )
    {
      cout<<"Binding failed..."<<endl;
      exit(0);
    }
    else
      cout<<"Binding successfull..."<<endl;

    struct msg buff;
    pthread_t id[100];
    int i=0;
    while(1)
    {
      bzero(&buff,sizeof(buff));
      socklen_t len = sizeof(remaddr);
      i++;
      //recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
      if(recvfrom(sockfd,&buff,sizeof(buff),0,(struct sockaddr *)&remaddr,&len) >0)
      {
        cout<<"Client port number: "<<ntohs(remaddr.sin_port)<<endl;
        cout<<"Client address : "<<inet_ntoa(remaddr.sin_addr)<<endl;
        cout<<"Message from client : "<<buff.text<<endl;

        if(pthread_create(&id[i],NULL,function,&sockfd) != 0)
          cout<<"Thread creation unsuccessfull..."<<endl;
        else
          cout<<"Thread created successfully..."<<endl;
      }
      else
        cout<<"Something went wrong : "<<endl;

    }

    close(sockfd);

}
