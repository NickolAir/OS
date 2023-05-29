#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#define SIZE 4096

void mem_allocate_stack() {
    char array[SIZE];
    sleep(1);
    printf("Memory allocated!\n");
    mem_allocate_stack();
}

void mem_allocate_heap() {
    int *array[30];
    for (int i = 0; i < 30; ++i) {
        array[i] = (int*) malloc(SIZE * sizeof(int));
        sleep(1);
    }
    for (int i = 0; i < 30; ++i) {
        free(array[i]);
    }
}

void sigsegv_handler() {
    printf("SIGSEGV caught\n");
    exit(1);
}

void pid_print() {
    pid_t pid = getpid();
    printf("pid: %d\n", pid);
    sleep(20);
}

int main(int argc, char *argv[]) {
    pid_print();

    void *region = NULL;
    size_t region_size;

    while(1) {
        printf("Mapping anonymous memory region\n");
        region_size = 10 * sysconf(_SC_PAGESIZE);
        region = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        printf("Mapped anonymous memory region at: %p\n", region);

        sleep(7);

        signal(SIGSEGV, sigsegv_handler);

        printf("Changing region protection\n");

        mprotect(region, region_size, PROT_NONE);

        sleep(7);

        printf("Attempting to write to the region\n");
        *((char *)region) = 1;

        printf("Unmapping pages 4-6\n");
        void *unmap_start = (void *)((size_t)region + 4 * sysconf(_SC_PAGESIZE));
        munmap(unmap_start, 2 * sysconf(_SC_PAGESIZE));

        printf("Press ENTER to exit\n");
        getchar();
    }
}