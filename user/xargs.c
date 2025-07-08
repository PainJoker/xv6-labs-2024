#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


void
xargs(int argc, char *argv[])
{
    char buf[512];

    char *cmd_args[MAXARG];
    for(int i = 1; i < argc; ++i) {
        cmd_args[i - 1] = argv[i];
    }

    char c;
    int pos = 0;
    while(read(0, &c, 1) > 0) {
        if(c != '\n') {
            buf[pos++] = c;
            continue;
        }
        buf[pos] = 0;
        int pid = fork();
        if(pid == 0) {
            if(argc + 1 > MAXARG) {
                fprintf(2, "xargs: too many args\n");
                exit(1);
            }
            cmd_args[argc - 1] = buf;
            cmd_args[argc] = 0;
            exec(cmd_args[0], cmd_args);
            fprintf(2, "xargs: exec %s failed\n", argv[0]);
            exit(1);
        } else if(pid > 0) {
            wait(0);
            pos = 0;
        } else {
            fprintf(2, "fork failed\n");
            exit(1);
        }
    }
}

void
main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "xargs: args must be greater than 2\n");
        exit(1);
    }
    xargs(argc, argv);
}