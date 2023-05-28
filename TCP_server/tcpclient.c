#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    // IP-адрес и порт сервера
    const char* ip = "127.0.0.1";
    const int port = 8080;

    // Создание TCP-сокета
    int sock = 0;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket creation error\n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported\n");
        return -1;
    }
    // установление соединения с сервером, который слушает определенный IP-адрес и порт
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Connection Failed\n");
        return -1;
    }

    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin); // считываем сообщение

    while (1) {
        send(sock, buffer, strlen(buffer), 0); // отправляем его
        printf("Message sent\n");

        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, sizeof(buffer) - 1); // принимаем мообщение в буфер
        printf("Received message: %s\n", buffer);
        sleep(2);
    }
    return 0;
}