#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define STD_BUF 1024

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
        perror("diropen");
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

void create_slink(char *path) {
    if (symlink(path, "symlink") == -1) {
        perror("symlink");
        exit(-1);
    }
}

void print_symlink(char *path) {
    char buf[BUFSIZ];
    size_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len == -1) {
        perror("readlink");
        exit(-1);
    }
    printf("%s\n", buf);
}

void print_file_symlink(char *path) {
    print_file(path);
}

void remove_slink(char *path) {
    if (unlink(path) == -1) {
        perror("unlink");
        exit(-1);
    }
}

void create_hardlink(char *path) {
    if (link(path, "hardlink") == -1) {
        perror("link");
        exit(-1);
    }
}

void remove_hardlink(char *path) {
    if (unlink(path) == -1) {
        perror("unlink");
        exit(-1);
    }
}

void print_access() {

}

void change_access() {

}

int main(int argc, char *argv[]) {
    char *name = get_name(argv[0]);
    printf("%s\n", name);
    if (argc > 0) {
        if (strcmp(name, "make_dir") == 0) {
            make_dir(argv[1]);
        } else if (strcmp(name, "print_dir") == 0) {
            print_dir(argv[1]);
        } else if (strcmp(name, "remove_dir") == 0) {
            remove_dir(argv[1]);
        } else if (strcmp(name, "create_file") == 0) {
            create_file(argv[1]);
        } else if (strcmp(name, "print_file") == 0) {
            print_file(argv[1]);
        } else if (strcmp(name, "remove_file") == 0) {
            remove_file(argv[1]);
        } else if (strcmp(name, "create_slink") == 0) {
            create_slink(argv[1]);
        } else if (strcmp(name, "print_symlink") == 0) {
            create_slink(argv[1]);
        } else if (strcmp(name, "print_file_symlink") == 0) {
            print_file_symlink(argv[1]);
        } else if (strcmp(name, "remove_slink") == 0) {
            remove_slink(argv[1]);
        } else if (strcmp(name, "create_hardlink") == 0) {
            create_hardlink(argv[1]);
        } else if (strcmp(name, "remove_hardlink") == 0) {
            remove_hardlink(argv[1]);
        }
    }
    free(name);
    return 0;
}