#include <stdio.h>
#include <stdlib.h> // malloc
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define NAME_LENGTH 50
#define LOOPS 1000    

//Usage: read_write_test filename

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
int main(int argc, char *argv[]) {
    char name[50];
    strcpy(name, argv[1]);
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
