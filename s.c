#include <pthread.h>
#include "tcp.c"
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
int main(){
  int port = 8080;
  int lfd = tcp_listen(port);
  pthread_t tid;
  if(lfd == -1)
    printf("tcp_listen error\n");
  else
    printf("wait port : %d\n", port);
  while(1){
    int cfd = tcp_accept(lfd);
    if(cfd == -1){
      printf("tcp_accept error\n");
      break;
    }
    else
      printf("connect success !\n");
    pthread_create(&tid, NULL, thread_fun, &cfd);
  }
  close(lfd);
  return 0;
}
