#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 128

char *strrev(char *S) {
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
    return S;
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

void create_file(char *file_path, char *new_file_path, char *path, char *new_path, char *rev_file_name,
                 int name_len, struct dirent *entry) {
    strcpy(file_path, path);
    strcat(file_path, "/");
    strcat(file_path, entry->d_name);
    printf("%s\n", file_path);

    strcpy(new_file_path, new_path);
    strcat(new_file_path, "/");
    strcpy(rev_file_name, entry->d_name);
    strcat(new_file_path, strrev(rev_file_name));
    printf("%s\n", new_file_path);
}

long get_file_size(FILE *fin) {
    fseek(fin, 0, 2);
    return ftell(fin);
}

void copy(char *file_path, char *new_file_path) {
    FILE *fin, *fout;
    long file_size;
    fin = fopen(file_path, "r");
    fout = fopen(new_file_path, "w");
    if (fin == NULL || fout == NULL) {
        printf("Failed to open file\n");
        exit(-1);
    }
    file_size = get_file_size(fin);

    printf("%ld\n", file_size);

    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[]) {
    DIR *dir, *new_dir;
    struct dirent *entry;
    char *path = argv[1];
    int name_len;
    char *file_path = (char*) malloc(sizeof(char));
    char *new_file_path = (char*) malloc(sizeof(char));
    char *rev_file_name = (char*) malloc(sizeof(char));
    char *new_path = (char*) malloc(strlen(path) * sizeof(char));
    strcpy(new_path, path);

    dir = opendir(path);
    if (!dir) {
        perror("diropen");
        exit(1);
    }

    name_len = get_length(path);
    rev_dir(path, new_path, name_len);
    //mkdir(new_path, 0755);

    while ((entry = readdir(dir)) != NULL) {
        //regular files have type 8
        if (entry->d_type == 8) {
            name_len = strlen(entry->d_name);
            file_path = (char*) realloc(file_path, sizeof(char) * (strlen(path) + name_len + 1));
            new_file_path = (char*) realloc(new_file_path, sizeof(char) * (strlen(path) + name_len + 1));
            rev_file_name = (char*) realloc(rev_file_name, sizeof(char) * strlen(entry->d_name));

            create_file(file_path, new_file_path, path, new_path, rev_file_name,
                        name_len, entry);

            copy(file_path, new_file_path);

            printf("inode: %d, name: %s, type: [%d], length: %d\n",
                   entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        }
    }

    closedir(dir);

    free(rev_file_name);
    free(file_path);
    free(new_path);
    free(new_file_path);
    return 0;
}