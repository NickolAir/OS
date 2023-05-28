#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Создание UDP сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    // Настройка серверного адреса
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    if (inet_aton(SERVER_IP, &serverAddress.sin_addr) == 0) {
        perror("Ошибка при настройке адреса сервера");
        exit(1);
    }

    printf("UDP клиент запущен.\n");

    while (1) {
        printf("Введите сообщение для отправки серверу (q - выход):\n");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Удаление символа новой строки из введенной строки
        size_t length = strlen(buffer);
        if (buffer[length - 1] == '\n')
            buffer[length - 1] = '\0';

        if (strcmp(buffer, "q") == 0)
            break;

        // Отправка сообщения серверу
        ssize_t sentBytes = sendto(sockfd, buffer, strlen(buffer), 0,
                                   (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (sentBytes < 0) {
            perror("Ошибка при отправке сообщения");
            exit(1);
        }

        // Ожидание ответного сообщения от сервера
        ssize_t receivedBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (receivedBytes < 0) {
            perror("Ошибка при приеме сообщения");
            exit(1);
        }

        buffer[receivedBytes] = '\0';
        printf("Получен ответ от сервера: %s\n", buffer);
    }

    // Закрытие сокета
    close(sockfd);

    return 0;
}
