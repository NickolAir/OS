#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define HEAP_SIZE (1024 * 1024)

void *heap = NULL;

// Функция выделения памяти
void *my_malloc(size_t size) {
    // Проверяем, был ли уже создан регион памяти
    if (heap == NULL) {
        // Создаем анонимный регион памяти
        heap = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (heap == MAP_FAILED) {
            perror("mmap");
            return NULL;
        }
    }

    uintptr_t ptr = (uintptr_t)heap;
    while (ptr < (uintptr_t)heap + HEAP_SIZE) {
        size_t *block_size = (size_t *)ptr;
        // Проверяем, является ли текущий блок свободным и достаточно большим для запрошенного размера
        if (*block_size == 0 && (uintptr_t)heap + HEAP_SIZE - ptr >= size + sizeof(size_t)) {
            *block_size = size;
            return (void *)(ptr + sizeof(size_t));
        }
        ptr += *block_size + sizeof(size_t);
    }

    return NULL; // Если свободного блока достаточного размера не найдено
}

// Функция освобождения памяти
void my_free(void *ptr) {
    if (ptr == NULL) {
        return; // Ничего не делаем, если передан нулевой указатель
    }

    uintptr_t block_ptr = (uintptr_t)ptr - sizeof(size_t);
    size_t *block_size = (size_t *)block_ptr;
    *block_size = 0; // Помечаем блок памяти как свободный
}

int main() {
    printf("pid: %d\n", getpid());
    getchar();

    // Выделение массива целочисленных данных
    int *array = (int *)my_malloc(5 * sizeof(int));

    getchar();

    if (array != NULL) {
        printf("Allocated memory for array\n");

        // Заполнение массива
        for (int i = 0; i < 5; i++) {
            array[i] = i;
        }

        // Вывод содержимого массива
        printf("Array elements: ");
        for (int i = 0; i < 5; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        // Освобождение памяти
        my_free(array);
        printf("Freed memory\n");
    } else {
        printf("Failed to allocate memory\n");
    }
    return 0;
}