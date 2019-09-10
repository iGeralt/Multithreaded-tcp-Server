/******************************************************************************
Server-client using tcp .
Server returns N Armstrong number.

******************************************************************************/


#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define MAX 80
#define PORT 50020
#define SA struct sockaddr

int sockfd;
struct sockaddr_in cli;

void sig_handler(int signo)
{
  if (signo == SIGINT)
  {
		cout<<"\nServer is shutting down...\n";
		close(sockfd);
		exit(0);
	}
}

struct msg {
	int num;
	int arr[100];
};
void *func(void *arg)
{
  int sock = *((int *)arg);
  struct msg buffer;

  for (;;) {
    bzero(buffer.arr,sizeof(buffer.arr));
    int origNum=152, num, rem, sum = 0,n;
    read(sock, &buffer, sizeof(buffer));
    char *str;
	printf("\nREQUEST FROM %s PORT %d\n",inet_ntoa(cli.sin_addr),htons(cli.sin_port));
    printf("from client:\t%d\n",buffer.num );
    n = buffer.num;
    n--;
    int i = 0;
    buffer.arr[i]=1;
    while(n != 0)
    {
      origNum++;
      num = origNum;
      int cp = num;
      sum = 0;
      int count = 0;
      while(cp > 0)
      {
        cp /= 10;
        count++;
      }
      while(num != 0)
      {
          rem = num % 10;
          sum += pow(rem,count);
          num /= 10;
      }

      if(sum == origNum)
      {
        i++;
        buffer.arr[i]=origNum;
        n--;
      }

    }
    write(sock,&buffer,sizeof(buffer));
  }
}

int main()
{
  int confd,len;
  struct sockaddr_in servaddr;
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd == -1)
  {
    cout<<"Socket creation failed ... \n";
    exit(0);
  }
  else
    cout<<"Socket successfully created..\n";
  bzero(&servaddr,sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if((bind(sockfd,(SA*)&servaddr,sizeof(servaddr))) != 0)
  {
    cout<<"Socket bind failed...\n";
		exit(0);
  }
  else
    cout<<"Socket successfully binded...\n";
  if((listen(sockfd,5)) != 0)
  {
    cout<<"Listen failed...\n";
    exit(0);
  }
  else
    cout<<"Server listening...\n";
  pthread_t id[50];
  int i = 0;
  for(;;)
  {
    i++;
    if (signal(SIGINT, sig_handler) == SIG_ERR)
      printf("\ncan't catch SIGINT\n");
    len = sizeof(cli);
    confd = accept(sockfd,(SA*)&cli,(socklen_t*)&len);
    if(confd < 0)
      cout<<"Server accept failed....\n";
    else
      cout<<"Server accept the client....\n";
    cout<<"Connection accepted!!!"<<endl;
    char *str;
	printf("\nREQUEST FROM %s PORT %d\n",inet_ntoa(cli.sin_addr),htons(cli.sin_port));

    if(pthread_create(&id[i],NULL,func,&confd) != 0)
      cout<<"Failed to create thread..."<<endl;
    else
      cout<<"Thread created successfully...\n";
  }

}
