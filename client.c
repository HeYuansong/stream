#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>

int tcp_connect(char * ip, int port){
  struct hostent * he;
  struct sockaddr_in server;
  int sockfd;
  if((he = gethostbyname(ip)) == NULL)
    return -1;
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return -1;
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr = *((struct in_addr *)he->h_addr);
  if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    return -1;
  return sockfd;
}
int main(int c,char**v){
  int sfd = tcp_connect(v[1], atoi(v[2]));
  if(sfd == -1)
    printf("tcp connect error !\n");
  else
    printf("connect success !\n");
  char buf[4096];
  char user[4096];
  while(1){
    memset(buf,0,4096);
    memset(user,0,4096);
    scanf("%s",user);
    if(strcmp(user,"close") == 0)
      break;
    write(sfd,user,4096);
    read(sfd,buf,4096);
    printf("%s\n",buf);
  }
  close(sfd);
  return 0;
}
