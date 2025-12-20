#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(int argc, char* argv[]) {
  int x1 = getsyscallcount(SYS_open);
  open("whatever", O_RDWR);
  int x2 = getsyscallcount(SYS_open);
  
  for (int i = 0; i < 99; i++) {
    open("whatever", O_RDWR);
  }
  
  int x3 = getsyscallcount(SYS_open);
  
  printf(1, "%d %d %d\n", x1, x2, x3);
  printf(1, "0 1 100 (esperado)\n");
  
  exit();
}
