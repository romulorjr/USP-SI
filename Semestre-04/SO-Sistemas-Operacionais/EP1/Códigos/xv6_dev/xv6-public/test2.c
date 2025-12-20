#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(int argc, char* argv[]) {
  for (int i = 0; i < 10; i++) {
    open("whatever", O_RDWR);
  }
  
  int x1 = getsyscallcount(SYS_open);
  
  int rc = fork();
  
  for (int i = 0; i < 10; i++) {
    open("whatever", O_RDWR);
  }
  
  if (rc > 0) {
    wait();
    int x2 = getsyscallcount(SYS_open);
  
    printf(1, "Saida do Pai (PID: %d): %d %d\n", getpid(), x1, x2);
    printf(1, "Saida do Pai (PID: %d): 10 20 (esperado)\n", getpid());
  } else if (rc == 0) {
    int x2 = getsyscallcount(SYS_open);
  
    printf(1, "Saida do Filho (PID: %d): %d %d\n", getpid(), x1, x2);
    printf(1, "Saida do Filho (PID: %d): 10 10 (esperado)\n", getpid());
  }
  
  exit();
}
