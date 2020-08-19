#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/in.h>
#include<netdb.h>

int c_close(int fd){
  return close(fd);
}
ssize_t c_write(char * buf , int fd) {
  return write(fd,buf,4096);
}

char * c_read(int fd) {
  static char data[4096];
  memset(data,0,4096);
  read(fd,data,4096);
  return data;
}

int tcp_listen(int port){
  struct sockaddr_in serv_addr;
  int lfd = socket(AF_INET,SOCK_STREAM,0);
  if(lfd == -1)
    return -1;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  int status = bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
  if(status == -1)
    return -1;
  listen(lfd, 128);
  return lfd;
}
int isSocketClosed(int clientSocket){ 
 char buff[32]; 
 int recvBytes = recv(clientSocket, buff, sizeof(buff), MSG_PEEK); 
 int sockErr = errno;
 if( recvBytes > 0)
   return 0; 
 if( (recvBytes == -1) && (sockErr == EWOULDBLOCK) )
   return 0;
 return 1;
}
int tcp_accept(int lfd){
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  char client_ip[128];
  int cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_len);
  return cfd;
}
int tcp_connect(char * ip, int port){
  struct hostent * he;
  struct sockaddr_in server;
  int sockfd;
  if((he = gethostbyname(ip)) == NULL)
    return -1;
  if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return -1;
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr = *((struct in_addr *)he->h_addr);
  if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    return -1;
  return sockfd;
}
