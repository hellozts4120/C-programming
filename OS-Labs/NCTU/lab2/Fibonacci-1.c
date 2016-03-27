#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 

int Fibonacci(int n) {
    if (n == 0 || n == 1) {
        return n;
    }
    else return (Fibonacci(n - 1) + Fibonacci(n - 2));
}

int main() {
    int n, fib0 = 0, fib1 = 1;
    pid_t pid;
    printf("Enter an interger that means the length of Fibonacci sequence:");
    scanf("%d",&n);
    if (n <= 0) {
        printf("You should enter a non-negative interger!\n");
    }
    else {
        pid = fork();
        if (pid == 0) {
            printf("Child starts generating the sequence...\n");
            for (int cur = 0; cur < n; cur++) {
                printf("%d ", Fibonacci(cur));
            }
            printf("\n");
            printf("Child ends...\n");
        }
        else if (pid > 0) {
            printf("Parent waiting....\n");
            wait(NULL);
            printf("Parent ends...\n");
        }
        else {
            fprintf(stderr, "Fork failed");
            return 1;
        }
    }
    return 0;
}