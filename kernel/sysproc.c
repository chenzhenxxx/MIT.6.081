#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{  
  // lab pgtbl: your code here.
  struct proc *proc=myproc();
  uint64 va;
  argaddr(0,&va);
  int npage;
  argint(1,&npage);
  uint64 useaddr;
  uint64 abits=0;
  argaddr(2,&useaddr);
  if(npage>64)
  {
    printf("page overflow");
    return -1;
  }
  acquire(&tickslock);
  
  for (uint64 p=va;p<va+npage*PGSIZE;p+=PGSIZE)
  {
    pte_t *pte=walk(proc->pagetable,p,1);
    if(*pte&PTE_A)
    { 
      abits=abits|(1<<((p-va)/PGSIZE));
      *pte=(*pte)&(~(PTE_A));
    }
  }
  copyout(proc->pagetable,useaddr,(char *)&abits,sizeof(abits));
  release(&tickslock);

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
