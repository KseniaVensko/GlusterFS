#include <stdio.h>
#include <stdlib.h> // malloc
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define NAME_LENGTH 50
#define LOOPS 1000
// #define timing(a) start=clock(); a; diff = clock() - start; msec = diff * 1000 / CLOCKS_PER_SEC; printf("msecs: %d\n",msec);

void writetest(char* name, char* buffer) {
    for (int i=0; i < LOOPS; ++i) {
        FILE* out = fopen(name, "ab+");
        if (out == NULL) {
            return;
        }
        fwrite(buffer,sizeof(char), sizeof(buffer), out);
        fclose(out);
    }
}

void readtest(char* name, char* buffer) {
    for (int i=0; i < LOOPS; ++i) {
        FILE* in = fopen(name, "rb");
        if (in == NULL) {
            return;
        }
        fread(buffer,sizeof(char), sizeof(buffer), in);
        fclose(in);
    }
}

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

    // Read/Write tests
    char name[50];
    strcpy(name, argv[2]);
    strcat(name, ".rwtest");
    char* buffer = (char*) malloc(sizeof(char) * 1000);;
    for(int i=0; i<1000; ++i)
        buffer[i]=(char)i;

    clock_t start = clock(), diff;
    writetest(name,buffer);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken for write %d seconds %d milliseconds\n", msec/1000, msec%1000);

    start = clock();
    readtest(name,buffer);
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken for read %d seconds %d milliseconds\n", msec/1000, msec%1000);

    free(buffer);
    return 0;
}

