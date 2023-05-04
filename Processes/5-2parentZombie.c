#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // one child process
        printf("Child pid: %d\nParent pid: %d\n\n", getpid(), getppid());

        pid_t new_child_pid = fork();
        if (new_child_pid == 0) {
            // another child process
            printf("New child pid: %d\nParent pid: %d\n", getpid(), getppid());
            sleep(5);
            printf("New child pid: %d\nParent pid: %d\n", getpid(), getppid());
            sleep(5);
        } else if (new_child_pid > 0) {
            _exit(0);
        } else {
            printf("fork failed: %s\n", strerror(errno));
        }
    } else if (pid > 0) {
        // parent process
        wait(NULL);
    } else {
        printf("fork failed: %s\n", strerror(errno));
    }
    return 0;
}