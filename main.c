#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    char *path = argv[1];

    dir = opendir(path);
    if (!dir) {
        perror("diropen");
        exit(1);
    }

    while ( (entry = readdir(dir)) != NULL) {
        printf("inode: %d, name: %s, type: [%d], length: %d\n",
               entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
    }

    closedir(dir);

    return 0;
}