#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
/*    int i = 0;
    while (argv[++i] != NULL) {
        printf("%s ", argv[i]);
    }*/
    pid_t pid = getpid();
    printf("pid: %d\n", pid);

    sleep(1);
    int i = 0;
    while (argv[++i] != NULL) {
        printf("%s ", argv[i]);
    }
    fflush(NULL);
    execl("/home/nikolay/CLionProjects/project4/cmake-build-debug/project4"," ", "Hello", "world!", NULL);
}