#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char parent[1], child[1];

    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        read(p[0], child, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], "c", 1);
    }
    else
    {
        write(p[1], "p", 1);
        wait();
        read(p[0], parent, 1);
        fprintf(1, "%d: received pong\n", getpid());
    }

    exit();
}
