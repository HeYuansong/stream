#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>

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
  printf("port %d for connect...\n",port);
  return lfd;
}
int tcp_accept(int lfd){
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  char client_ip[128];
  int cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_len);
  printf("client:%s\t%d\n",
	 inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
	 ntohs(client_addr.sin_port));
  return cfd;
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
void * thread_fun(void * args){
  int cfd = *((int*)args);
  char buf[4096];
  char serv[4096];
  while(1){
    memset(buf, 0, 4096);
    memset(buf, 0, 4096);
    if(isSocketClosed(cfd)){
      close(cfd);
      break;
    }
    int nbyte = read(cfd, buf, 4096);
    printf("recvd %s \n", buf);
    if(strcmp(buf,"hello") == 0)
      sprintf(serv,"world");
    else if(strcmp(buf,"ping") == 0)
      sprintf(serv,"pong");
    else
      sprintf(serv,"nothing");
    write(cfd, serv, 4096);
  }
}
int main(int argc, char*argv[]){
  int lfd = tcp_listen(8080);
  pthread_t tid;
  if(lfd==-1)
    printf("listen err\n");
  while(1){
    int cfd = tcp_accept(lfd);
    if(cfd==-1)
      printf("accept err\n");
    pthread_create(&tid, NULL, thread_fun, &cfd);
  }
  return 0;
} 
