#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define STDIN 0
#define MAX_ARG_LEN 32

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage xargs cmd ...");
        exit();
    }

    char argBuffer[MAX_ARG_LEN];
    char *subCmdArg[MAXARG];

    for (int i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) <= MAX_ARG_LEN)
        {
            subCmdArg[i - 1] = (char *)malloc(strlen(argv[i]));
            strcpy(subCmdArg[i - 1], argv[i]);
        }
        else
        {
            fprintf(2, "arg string length exceed limit");
            exit();
        }
    }

    int argBufIndex = 0;
    int argIndex = argc - 1;
    while (read(STDIN, argBuffer + argBufIndex, 1) > 0)
    {
        if (argBuffer[argBufIndex] == '\n' || argBuffer[argBufIndex] == ' ')
        {
            if (argIndex >= MAXARG)
            {
                fprintf(2, "arg length exceed limit");
                exit();
            }
            subCmdArg[argIndex] = (char *)malloc(argBufIndex);
            // strcpy(subCmdArg[argIndex++], argBuffer);
            memmove(subCmdArg[argIndex++], argBuffer, argBufIndex);
            // printf("%s %d", subCmdArg[argIndex - 1], strlen(subCmdArg[argIndex - 1]));
            argBufIndex = -1;

            if (argBuffer[argBufIndex] == '\n')
            {
                if (argIndex < MAXARG)
                {
                    subCmdArg[argIndex] = 0;
                }

                if (fork() == 0)
                {
                    exec(subCmdArg[0], subCmdArg);
                    printf("exec error");
                }
                else
                {
                    // wait();
                    argIndex = 0;
                }
            }
        }

        if (++argBufIndex >= MAX_ARG_LEN)
        {
            fprintf(2, "arg string length exceed limit");
            exit();
        }
    }

    if (argIndex < MAXARG)
    {
        subCmdArg[argIndex] = 0;
    }

    if (fork() == 0)
    {
        exec(subCmdArg[0], subCmdArg);
        printf("exec error");
    }

    wait();
    printf("main thread");
    exit();
}