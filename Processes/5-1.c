#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int global_var = 10;

int main(int argc, char *argv[]) {
    char local_var = 'A';
    printf("global variable: %d address %p\n", global_var, &global_var);
    printf("local variable: %c address %p\n", local_var, &local_var);
    pid_t pid;
    printf("pid: %d\n\n", getpid());

    pid = fork();
    if (pid == 0) {
        printf("I am child process\n");
        printf("My pid: %d\n", getpid());
        printf("Parent pid %d\n", getppid());
        printf("global variable: %d address %p\n", global_var, &global_var);
        printf("local variable: %c address %p\n", local_var, &local_var);

        global_var = 1000;
        local_var = 'B';
        printf("new value global variable from child process %d address %p\n", global_var, &global_var);
        printf("new value local variable from child process %c address %p\n\n", local_var, &local_var);

        exit(5);
    } else if (pid > 0) {
        sleep(5);
        printf("I am parent process\n");
        printf("My pid: %d\n", getpid());

        printf("global variable: %d address %p\n", global_var, &global_var);
        printf("local variable: %c address %p\n", local_var, &local_var);

        sleep(30);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("child process exit code %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("child process signal %d\n", WTERMSIG(status));
        }
    } else {
        printf("fork failed: %s\n", strerror(errno));
    }
}