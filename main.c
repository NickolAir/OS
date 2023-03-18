#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

char strrev(char *S) {
    int i,j,l;
    char t;
    l = strlen(S);
    i = 0;
    j = l - 1;
    while (i < j){
        t = S[i];
        S[i] = S[j];
        S[j] = t;
        i++;
        j--;
    }
    return *S;
}

int get_length(char *path) {
    int count = 0;
    int i = strlen(path) - 1;
    while(path[i] != '/') {
        count++;
        i--;
    }
    return count;
}

void rev_dir(char *path, char *new_path, int len) {
    int i = strlen(path) - 1;
    int count = i - len + 1;
    while(path[i] != '/') {
        new_path[count] = path[i];
        count++;
        i--;
    }
}

int main(int argc, char *argv[]) {
    DIR *dir, *new_dir;
    struct dirent *entry;
    char *path = argv[1];
    int name_len;
    char *rev_name = (char*) malloc(sizeof(char));
    char *new_path = (char*) malloc(strlen(path) * sizeof(char));
    strcpy(new_path, path);

    dir = opendir(path);
    if (!dir) {
        perror("diropen");
        exit(1);
    }

    name_len = get_length(path);
    rev_dir(path, new_path, name_len);
    printf("%s\n", new_path);

    rev_name = (char*) realloc(rev_name, sizeof(char) * name_len);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == 8) {
            printf("inode: %d, name: %s, type: [%d], length: %d\n",
                   entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        }
    }

    closedir(dir);
    return 0;
}