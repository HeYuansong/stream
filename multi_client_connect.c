#include <pthread.h>
#include "tcp.c"
char * ip = "127.1";
int port = 8080;
void * multi_connect(void * args){
  int i = *((int*)args);
  int sfd = tcp_connect(ip,port);
  if(sfd == -1)
    printf("tcp connect error !\n");
  else
    printf("thread(%d) connect success !\n", i);
  char buf[4096];
  char user[4096];
  memset(buf,0,4096);
  memset(user,0,4096);
  sprintf(user,"ping");
  
  printf("thread(%d):send -> %s\n", i, user);
  write(sfd,user,4096);
  read(sfd,buf,4096);
  printf("thread(%d):recv <- %s\n", i,  buf);
  
  close(sfd);
}
int main(int c,char**v){
  pthread_t tid[50];
  for(int i=0;i<50;i++){
    pid_t pid = fork();
    if(pid){}
    else{
      multi_connect(&i);
      break;
    }
  }
    
    //pthread_create(&tid[i], NULL, multi_connect, &i);
  //for(int i=0;i<50;i++)
  //pthread_join(tid[i], NULL);
  return 0;
}
