#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Создание UDP сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // Настройка серверного адреса
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    // Привязка сокета к серверному адресу
    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }

    printf("UDP сервер запущен и ожидает сообщений...\n");

    while (1) {
        socklen_t clientLength = sizeof(clientAddress);

        // Получение сообщения от клиента
        ssize_t receivedBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                         (struct sockaddr *)&clientAddress, &clientLength);
        if (receivedBytes < 0) {
            perror("Ошибка при приеме сообщения");
            exit(1);
        }

        buffer[receivedBytes] = '\0';
        printf("Получено сообщение от клиента: %s\n", buffer);

        // Отправка ответного сообщения клиенту
        const char *response = "Привет, клиент!";
        ssize_t sentBytes = sendto(sockfd, response, strlen(response), 0,
                                   (struct sockaddr *)&clientAddress, clientLength);
        if (sentBytes < 0) {
            perror("Ошибка при отправке сообщения");
            exit(1);
        }

        printf("Отправлено ответное сообщение клиенту.\n");
    }
    // Закрытие сокета
    close(sockfd);
    return 0;
}