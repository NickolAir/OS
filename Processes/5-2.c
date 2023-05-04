#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child pid: %d\nParent pid: %d\n", getpid(), getppid());
        exit(5);
    } else if (pid > 0) {
        sleep(30);
    } else {
        printf("fork failed: %s\n", strerror(errno));
    }
    return 0;
}