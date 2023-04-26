#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int global_var_init = 4;
int global_var;
const int global_const = 6;

void foo() {
    int local_var = 1;
    static int static_local = 2;
    const int const_local = 3;

    printf("local: %p\n", &local_var);
    printf("local static: %p\n", &static_local);
    printf("local const: %p\n", &const_local);
}

int *var_address() {
    int a = 123;
    printf("value a, address a: %d %p\n", a, &a);
    return &a;
}

void heap_allocate() {
    char *array = (char*) malloc(100 * sizeof(char));
    strcpy(array, "hello world");
    printf("local array (heap): %s\n", array);
    free(array);
    printf("local array (heap) with free memory: %s\n", array);
    array = (char*) malloc(100 * sizeof(char));
    strcpy(array, "new hello world");
    printf("new local array (heap): %s\n", array);
    array += 50;
    //free(array);
    printf("new local array (heap) with free memory: %s\n", array);
}

int main() {
    foo();
    printf("global inited: %p\n", &global_var_init);
    printf("global not inited: %p\n", &global_var);
    printf("global const: %p\n", &global_const);

    int *ptr = var_address();
    printf("get address local var: %p\n", ptr);

    //Creating my environment var
    setenv("MY_ENV_VAR", "MY ENVIRONMENT VAR", 0);
    char *value = getenv("MY_ENV_VAR");
    printf("My environment var: %s\n", value);
    setenv("MY_ENV_VAR", "NEW ENV VAR", 1);
    value = getenv("MY_ENV_VAR");
    printf("My environment var: %s\n", value);

    heap_allocate();

    printf("pid: %d\n", getpid());
    sleep(30);
    return 0;
}
