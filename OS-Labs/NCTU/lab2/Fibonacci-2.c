# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# define MAX_SEQUENCE 10

typedef struct{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

char* shared_memory;

int main() {
    
    /*  to be continued...*/
    
    return 0;
}