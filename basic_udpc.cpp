#include<iostream>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

#define PORT 4000

struct msg{
  char text[100];
};

int sockfd,len;
struct sockaddr_in myaddr,remaddr;

void function()
{
    struct msg buff;
    while(1)
    {
      bzero(&buff,sizeof(buff));
      socklen_t len = sizeof(remaddr);
      int n= 0;

      cout<<"Enter your message : ";
      while((buff.text[n++] = getchar()) !='\n');
      cout<<endl;

      sendto(sockfd,&buff,sizeof(buff),0,(struct sockaddr *)&remaddr,len);

      bzero(&buff,sizeof(buff));
      recvfrom(sockfd,&buff,sizeof(&buff),0,(struct sockaddr *)&remaddr,&len);
      cout<<"Message from server : "<<buff.text<<endl;

    }
}

int main()
{
  sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1)
  {
    cout<<"Socket creation failed..."<<endl;
    exit(0);
  }
  else
    cout<<"socket created successfully..."<<endl;

  bzero(&myaddr,sizeof(myaddr));

  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(1456);

  if(bind(sockfd,(struct sockaddr *)&myaddr,sizeof(myaddr)) != 0)
  {
    cout<<"Binding failed...."<<endl;
    exit(0);
  }
  else
    cout<<"Binding successfull....."<<endl;

  bzero(&remaddr,sizeof(remaddr));

  remaddr.sin_family = AF_INET;
  remaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  remaddr.sin_port = htons(PORT);

  function();

  close(sockfd);

  return 0;


}
