# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
#include <sys/wait.h>
# include <unistd.h>
# define MAX_SEQUENCE 10

typedef struct{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main() {
    int n;
    long fib0 = 0, fib1 = 1;
    int segment_id;
    pid_t pid;
    
    printf("Enter an interger that means the length of Fibonacci sequence:");
    scanf("%d",&n);
    if (n <= 0) {
        printf("You should enter a non-negative interger!\n");
        return 1;
    }
    else if (n > MAX_SEQUENCE) {
        printf("Your input is larger than the maximum sequence!");
        return 2;
    }
    
    size_t segment_size = sizeof(shared_data);
    segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        perror("shmget failed");
        return 3;
    }
    
    shared_data *shared_memory = (shared_data *)(shmat(segment_id, NULL, 0));
    
    if ((void*)shared_memory == (void*)-1) {
        perror("shmat failed");
        shmctl(segment_id, IPC_RMID, NULL); 
        return 4;
    }
    
    shared_memory->sequence_size = n;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    else if (pid == 0) {
        printf("Child starts generating the sequence...\n");
        shared_memory->fib_sequence[0] = fib0;
        shared_memory->fib_sequence[1] = fib1;
        for (int i = 2; i < shared_memory->sequence_size; i++) {
            long temp = fib0 + fib1;
            shared_memory->fib_sequence[i] = temp;
            fib0 = fib1;
            fib1 = temp;
        }
        printf("Child ends generating...\n");
    }
    else {
        printf("Parent waiting....\n");
        wait(NULL);
        printf("Parent ends waiting, now printing...\n");
        for(int i = 0; i < shared_memory->sequence_size; i++) {
            printf("%ld ", shared_memory->fib_sequence[i]);
        }
        printf("\n");
        printf("Printing end...");
    }
    
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "Unable to detach the shared_memory...\n");
    }
    shmctl(segment_id, IPC_RMID, NULL); 
    
    return 0;
}