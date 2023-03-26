#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

char *get_name(char *path) {
    int i = strlen(path);
    int count = 0;
    int index = 0;
    while (path[i] != '/'){
        count++;
        i--;
    }

    char *name = (char*) malloc(count * sizeof(char));
    for (int j = i + 1; j < i + count; ++j) {
        name[index] = path[j];
        index++;
    }
    return name;
}

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

void create_file(char *path){
    FILE *fd;
    fd = fopen(path, "w");
    fclose(fd);
}

void print_file(char *path) {
    FILE *fd;
    char *buf = (char *)malloc(BUFSIZ * sizeof(char));
    fd = fopen(path, "r");
    if (fd == NULL) {
        printf("Failed to open file\n");
        exit(-1);
    }

    while(fd != EOF) {
        fgets(buf, BUFSIZ, fd);
        puts(buf);
    }

    free(buf);
    fclose(fd);
}

void remove_file(char *path) {
    remove(path);
}

int main(int argc, char *argv[]) {
    char *name = get_name(argv[0]);
    free(name);
    return 0;
}