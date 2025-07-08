#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    const uchar packet = 'A';
    uchar buf[1];

    int to_child[2];
    int to_parent[2];
    pipe(to_child);
    pipe(to_parent);

    int pid = fork();
    if(pid > 0) {
        close(to_child[0]);
        close(to_parent[1]);

        write(to_child[1], &packet, 1);
        read(to_parent[0], buf, 1);
        if(buf[0] == packet) {
            printf("%d: received pong\n", getpid());
        }

        close(to_child[1]);
        close(to_parent[0]);
        exit(0);
    } else if(pid == 0) {
        close(to_child[1]);
        close(to_parent[0]);

        read(to_child[0], buf, 1);
        if(buf[0] == packet) {
            printf("%d: received ping\n", getpid());
        }
        write(to_parent[1], buf, 1);

        close(to_child[0]);
        close(to_parent[1]);
        exit(0);
    } else {
        printf("fork error\n");
        exit(1);
    }
}