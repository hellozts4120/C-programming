#include<stdio.h>
#include<stdlib.h>
#define PAGE_SIZE 4096

int main(int argc, char **argv) {
    unsigned long long int address = atoll(argv[1]);
    unsigned long long int number = address / PAGE_SIZE;
    unsigned long long int offset = address % PAGE_SIZE;
    printf("The address %llu contains: \npage number = %llu\noffset = %llu\n", address, number, offset);
    return 0;
}