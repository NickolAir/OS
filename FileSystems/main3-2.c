#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <pid>\n", argv[0]);
        //return 1;
    }

    pid_t pid = (pid_t)atoi(argv[1]);
    unsigned long long start_addr = atoi(argv[2]);
    unsigned long long end_addr = atoi(argv[3]);
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/pagemap", pid);

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening pagemap");
        return 1;
    }

    uint64_t entry;
    ssize_t read_bytes;

    for (unsigned long long i = start_addr; i < end_addr; i += 0x1000) {
        read_bytes = read(fd, &entry, sizeof(entry));
        printf("%016llx\n", (unsigned long long)entry);
    }

//    while ((read_bytes = read(fd, &entry, sizeof(entry))) == sizeof(entry)) {
//        printf("%016llx\n", (unsigned long long)entry);
//    }

    if (read_bytes < 0) {
        perror("Error reading pagemap");
    } else if (read_bytes != sizeof(entry)) {
        printf("Error reading pagemap: incomplete entry\n");
    }

    close(fd);
    return 0;
}