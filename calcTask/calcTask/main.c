#include <stdio.h>
#include <stdlib.h> // malloc
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define NAME_LENGTH 50

int main(int argc, char *argv[])
{
    FILE* in;
    while ( in == NULL ) {
        printf("%s %s : %s\n", "Cannot open input file", argv[1], strerror(errno));
        in = fopen(argv[1], "r");
    }

    char file[50];
    strcpy(file, argv[2]);
    strcat(file, ".temp");

    FILE* out = fopen(file, "w");
    if (out == NULL) {
        printf("Cannot create out file: %s", strerror(errno));
        return 0;
    }

    int val, val2, sum;
    while(!feof(in))    {
        if(fscanf(in,"%d\n",&val)) {
            if(fscanf(in,"%d\n",&val2)) {
                sum = val + val2;
                fprintf(out, "%d\n", sum);
            }
        }
    }
    fclose(in);
    fclose(out);
    int ret = rename(file, argv[2]);            // what if can`t?
    return 0;
}

