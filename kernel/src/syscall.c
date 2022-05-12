#include <common.h>
#include <syscall_defs.h>
#include <thread.h>

int sys_kputc(char ch) {
  putch(ch);
  return 0;
}
int sys_getpid();
int sys_sleep(int second);
int64_t sys_uptime();
int sys_fork();
int sys_wait();
int sys_exit(int status);
int sys_kill(int pid);
void *sys_mmap(void *addr, int length, int prot, int flags);