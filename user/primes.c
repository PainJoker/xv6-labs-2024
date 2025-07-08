#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes_sieve(int read_fd)
{
    int prime;
    if(!read(read_fd, &prime, sizeof(prime))) {
        close(read_fd);
        return;
    }
    printf("prime %d\n", prime);

    int fd[2];
    pipe(fd);
    int pid = fork();

    int num;
    if(pid > 0) {
        close(fd[0]);
        while(read(read_fd, &num, sizeof(num))) {
            if(num % prime != 0) {
                write(fd[1], &num, sizeof(num));
            }
        }
        close(read_fd);
        close(fd[1]);
        wait(0);
    } else if(pid == 0) {
        close(read_fd);
        close(fd[1]);
        primes_sieve(fd[0]);
    } else {
        fprintf(2, "primes sieve fork failed\n");
        exit(1);
    }

    return;
}

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);

    int pid = fork();
    if(pid > 0) {
        close(fd[0]);
        for(int i = 2; i <= 280; ++i) {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);
        wait(0);
    } else if(pid == 0) {
        close(fd[1]);
        primes_sieve(fd[0]);
    } else {
        fprintf(2, "main fork failed\n");
        exit(1);
    }
    exit(0);
}