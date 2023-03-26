#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void make_dir(char *path) {
    mkdir(path, ACCESSPERMS);
}

void print_dir(char *path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    if (!dir) {
        perror("diropen error");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void remove_dir(char *path) {
    remove(path);
}

int main(int argc, char *argv[]) {
    return 0;
}