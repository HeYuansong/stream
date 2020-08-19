#include "tcp.c"

int main(int c,char**v){
  int sfd = tcp_connect(v[1],atoi(v[2]));
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
