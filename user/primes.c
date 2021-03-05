#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child(int *);

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        child(p);
    }
    else
    {
        fprintf(1, "prime 2\n");
        int prime = 2;
        for (int i = 3; i <= 35; i++)
        {
            if (i % prime != 0)
            {
                write(p[1], &i, 4);
            }
        }
        close(p[1]);
        wait();
    }

    exit();
}

void child(int *chan)
{
    close(chan[1]);

    int prime;
    int source = chan[0];
    read(source, &prime, 4);
    fprintf(1, "prime %d\n", prime);

    int num;
    int forked = 0;
    int p[2];
    while (read(source, &num, 4) > 0)
    {
        if (num % prime != 0)
        {
            if (forked == 0)
            {
                pipe(p);
                forked = 1;
                if (fork() == 0)
                {
                    child(p);
                }
                else
                {
                    write(p[1], &num, 4);
                }
            }
            else
            {
                write(p[1], &num, 4);
            }
        }
    }

    if (forked == 1)
    {
        close(p[1]);
        wait();
    }

    exit();
}