#include <stdio.h>
#include <string.h>
#include "coder.h"
#include "command.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("not enough parametrs\n");
    }
    const char *command = argv[1];
    const char *in_file = argv[2];
    const char *out_file = argv[3];

    if (strcmp(command, "encode") == 0)
    {
        if (encode_file(in_file, out_file))
        {
            printf("program stops\n");
        }
    }
    else
    {
        if (strcmp(command, "decode") == 0)
        {
            if (decode_file(in_file, out_file))
            {
                printf("program stops\n");
            }
        }
        else
        {
            printf("wrong command\n");
        }
    }
    return 0;
}