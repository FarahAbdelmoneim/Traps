#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

static inline uint64 r_fp();

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
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

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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

uint64 sys_sigalarm(void){
  argint(0,&(myproc() -> al_interval));
  argaddr(1,&(myproc() -> handler));
  return 0;
}

uint64 sys_sigreturn(void){
  myproc()->alarm_state=0;
  myproc()->ticks_tracker=0;
  myproc()-> trapframe -> kernel_satp  =myproc() -> nkernel_satp;
  myproc()-> trapframe -> kernel_sp =myproc() -> nkernel_sp;
  myproc()-> trapframe -> kernel_trap=myproc() -> nkernel_trap;
  myproc()-> trapframe -> epc=myproc() -> nepc;
  myproc()-> trapframe -> kernel_hartid=myproc() -> nkernel_hartid;
  myproc()-> trapframe -> ra=myproc() -> nra;
  myproc()-> trapframe -> sp=myproc() -> nsp;
  myproc()-> trapframe -> gp=myproc() -> ngp;
  myproc()-> trapframe -> tp=myproc() -> ntp;
  myproc()-> trapframe -> t0=myproc() -> nt0;
  myproc()-> trapframe -> t1=myproc() -> nt1;
  myproc()-> trapframe -> t2=myproc() -> nt2;
  myproc()-> trapframe -> s0=myproc() -> ns0;
  myproc()-> trapframe -> s1=myproc() -> ns1;
  myproc()-> trapframe -> a0=myproc() -> na0;
  myproc()-> trapframe -> a1=myproc() -> na1;
  myproc()-> trapframe -> a2=myproc() -> na2;
  myproc()-> trapframe -> a3=myproc() -> na3;
  myproc()-> trapframe -> a4=myproc() -> na4;
  myproc()-> trapframe -> a5=myproc() -> na5;
  myproc()-> trapframe -> a6=myproc() -> na6;
  myproc()-> trapframe -> a7=myproc() -> na7;
  myproc()-> trapframe -> s2=myproc() -> ns2;
  myproc()-> trapframe -> s3=myproc() -> ns3;
  myproc()-> trapframe -> s4=myproc() -> ns4;
  myproc()-> trapframe -> s5=myproc() -> ns5;
  myproc()-> trapframe -> s6=myproc() -> ns6;
  myproc()-> trapframe -> s7=myproc() -> ns7;
  myproc()-> trapframe -> s8=myproc() -> ns8;
  myproc()-> trapframe -> s9=myproc() -> ns9;
  myproc()-> trapframe -> s10=myproc() -> ns10;
  myproc()-> trapframe -> s11=myproc() -> ns11;
  myproc()-> trapframe -> t3=myproc() -> nt3;
  myproc()-> trapframe -> t4=myproc() -> nt4;
  myproc()-> trapframe -> t5=myproc() -> nt5;
  myproc()-> trapframe -> t6=myproc() -> nt6;
  
  return 0;
}
