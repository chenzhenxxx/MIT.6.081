#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
int 
main(int argc,char*argv[])
{ char ping[10]="ping";;
  char pong[10]="pong";
 
//不需要两个pipe的原因是，管道之间没有同时读或者同时写。对父子进程的pfd,都关闭两次
  
  int p[2];
  pipe(p);
  if(fork()==0){
      char tmp[10];
      read(p[0],tmp,10);
      fprintf(1,"%d: received %s\n",getpid(),tmp);
      close(p[0]);
      write(p[1],pong,sizeof(pong));
      close(p[1]);

   
  }
  else{
       
       write(p[1],ping,sizeof(ping));
       close(p[1]);
       char t[10];
       read(p[0],t,10);
       close(p[0]);
       wait(0);
      fprintf(1,"%d: received %s\n",getpid(),t);

  }
  exit(0);
}
