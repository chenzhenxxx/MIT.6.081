#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
int p[50][2];
int Prime(int n){
    int count=0,first=0,num=0,tmp=0;
    pipe(p[n+1]);
    read(p[n][0],&first,sizeof(first));
    
    printf("prime %d\n",first);
    while(read(p[n][0],&num,sizeof(num)))
    {  count++;
      if(num%first!=0)
      write(p[n+1][1],&num,sizeof(int));
    }
      close(p[n+1][1]);
      
   
    if(count!=0)
    {
       if(fork()==0)
       {
        Prime(n+1);
        close(p[n-1][0]);
        exit(0);
       }
       else
       {
        wait(0);
        exit(0);
       }
    }



      
}
int main(){
  pipe(p[0]);
  for(int i=2;i<=35;i++)
  {
    write(p[0][1],&i,sizeof(int));
  }
    close(p[0][1]);
     if(fork()==0)
     {
       Prime(0);
       close(p[0][0]);
       exit(0);
     }
     else
     {
        wait(0);
        exit(0);
     }
}
