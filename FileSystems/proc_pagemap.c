#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <bits/stdint-uintn.h>

#define PAGE_SIZE 0x1000
#define MAX_BUF_SIZE 4096
#define SOFT_DIRTY_BIT 55
#define FILE_SHARED_BIT 61
#define PAGE_SWAP_BIT 62
#define PAGE_PRESENT_BIT 63
#define PFN_MASK 0x7fffffffffffff

char *string_cat(char *dest, char *source) {
    size_t dest_len = strlen(dest);
    size_t source_len = strlen(source);
    if (dest_len < dest_len + source_len) {
        dest = (char*) realloc(dest, (dest_len + source_len));
        if (dest == NULL) {
            printf("realloc failed\n");
        }
    }
    strcat(dest, source);
    return dest;
}

void print_file_content(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(-1);
    }

    printf("Enter number of pages: ");
    int pages;
    scanf("%d", &pages);

    for (int i = 0; i < pages; i++) {
        uint64_t data;
        uint64_t index = i * sizeof(data);
        if (pread(fd, &data, sizeof(data), index) != sizeof(data)) {
            perror("pread");
            break;
        }
        printf("pfn %-16lx soft-dirty %ld file-page or shared %ld "
               "swapped %ld present %ld\n",
               data & PFN_MASK,
               (data >> SOFT_DIRTY_BIT) & 1,
               (data >> FILE_SHARED_BIT) & 1,
               (data >> PAGE_SWAP_BIT) & 1,
               (data >> PAGE_PRESENT_BIT) & 1);
        usleep(10000);
    }
    close(fd);
}

int main() {
    int pid = 2938;
    char pid_str[20];
    sprintf(pid_str, "%d", pid);
    printf("pid = %s\n", pid_str);
    char *pagemap_str = (char*) malloc(6 * sizeof(char));
    strcpy(pagemap_str, "/proc/");

    pagemap_str = string_cat(pagemap_str, pid_str);
    pagemap_str = string_cat(pagemap_str, "/pagemap");

    print_file_content(pagemap_str);
    return 0;
}