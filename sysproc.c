#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

extern int lookForPid(int);

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_shutdown(void){
  outw(0xB004, 0x0 | 0x2000);
  return 0;
}

int sys_reboot(void){
  outb(0x64, 0xfe);
  return 0;
}

int sys_set_priority(void){
  int priority;
  if(argint(0, &priority) < 0){
    return -1;
  }
  myproc() -> priority = priority;
  return 0;
}

/*int sys_signal(signum, handler){
  int signum1 = signum;
  int function = handler;
  if(argint(0, &signum1) < 0){
    return -1;
  }
  if(argint(0, &function) < 0){
    return -1;
  }
  myproc() -> signals[signum1] = (sighandler_t)function;
}

int sys_getppid(void){
  //return myproc()->pid;
  //return proc->parent->pid;
  return myproc()->parent->pid;
}
*/

/*
//run the signal function
int sys_killsignal(pid, signum) {
 int pid1 = pid;
 int signum1 = signum;
 struct proc *p;
 if(argint(0, &pid1) < 0) return -1;
   if(argint(1, &signum1) < 0) return -1;
 if(signum1 > 4 || signum1 < 1) return -1;
 //Try to find the process with the matching pid.
 for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
        if(p->pid == pid1) break;

   //If the pid is not found finish
   if(p->pid != pid1) return -1;

   //Default option finish the process
   signum1 -=1;
   if((int)p->signals[signum1] == -1) kill(p->pid);

 //TODO Else execute the function
 myproc() -> signals[signum1] = (sighandler_t)function;
 //Move the stack to the next position
 p->tf->esp -= 4;
 //Point to the function
 p->tf->eip = (uint)p->signals[signum1];
 return 1;
}*/
//signals functions
int sys_killsignal(int pid, int signum) {
  struct proc *p;
  p = myproc();
  if(argint(0, &pid) < 0){
    return -1;
  }
  if(argint(1, &signum) < 0){
    return -1;
  }
  if(signum > 4 || signum < 1){
    return -1;
  }
  //Try to find the process with the matching pid.
  //for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    //if(p->pid == pid) break;
  //}

  int response = lookForPid(pid);
  if(response == -1){
    return -1;
  }

   //If the pid is not found finish
   if(p->pid != pid){
    return -1;
   }
   //Default option finish the process
   signum -=1;
   if((int)p->signals[signum] == -1){
    kill(p->pid);
   }
  //Else execute the function
  //Move the stack to the next position
  p->tf->esp -= 4;
  //Point to the function
  p->tf->eip = (uint)p->signals[signum];
  return 1;
}

int sys_signal(int signum,sighandler_t * handler){
  myproc()->signals[signum] = handler;
  return 1;
}

int sys_getppid(void){
  return myproc()->parent->pid;
}
