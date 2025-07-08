#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if((fstat(fd, &st)) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DEVICE:
        case T_FILE:
            char *q;
            for(q=path+strlen(path); q >= path && *q != '/'; q--)
                ;
            q++;
            if(strcmp(q, target) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0';
                find(buf, target);
            }
            break;
        default:
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(2, "find: wrong num of args.\n");
        exit(1);
    }
    char *path = argv[1];
    char *target = argv[2];
    find(path, target);
    exit(0);
}