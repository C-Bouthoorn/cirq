#include <stdio.h>


int main(int argc, char *argv[]) {
    printf("Hello, world!\n");

    printf("Arguments:\n");
    for (int i=0; i < argc; i++) {
        printf("\t%i: %s\n", i, argv[i]);
    }
}
