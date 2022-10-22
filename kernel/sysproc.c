#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
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
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

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
uint64
sys_trace(void)
{
  int mask;
  argint(0, &mask); //从内核中获取参数int参数使用这个函数，把获得的数值放到mask里面

  myproc()->mask = mask; //把这个掩码进行设置，这个数值就是proc里面的mask，设置进去

  return 0;
}
uint64
sys_sysinfo(void)
{
  struct sysinfo s;
  proc_collect(&s.nproc);  //获取非UNUSED进程数
  kcollect(&s.freemem);    //获取剩余内存字节数
  uint64 addr;
  argaddr(0, &addr); //获取虚拟地址
  if (copyout(myproc()->pagetable, addr, (char *)&s, sizeof(s)) < 0)
  {
    return -1;
  }
  return 0;
}
