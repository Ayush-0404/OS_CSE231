#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int fibo(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibo(n - 1) + fibo(n - 2);
    }
}
void print_fibo(int n) {
    printf("Parent process printing Fibonacci Series: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", fibo(i));
    }
    printf("\n");
}

int fact(int n) {
    if (n == 0 ) {
        return 1;
    }
    else if(n==1){
        return 1;
    } else {
        return n * fact(n - 1);
    }
}


int main() {
    pid_t child_pid;

    // Create a child process
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(1);
    } else if (child_pid == 0) {
        // Child process

        // Child waits for the parent to complete
        wait(NULL);

        int facto = fact(4);
        printf("Child Process computing Factorial of 4: %d\n", facto);
    } else {
        // Parent process

        // Calculate and print the Fibonacci series
        print_fibo(16);

        printf("Child process will finish after parent.\n");
    }

    return 0;
}
