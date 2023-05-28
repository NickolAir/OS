#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    // IP-адрес и порт для прослушивания
    const char* ip = "127.0.0.1";
    const int port = 8080;

    // Создание TCP-сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM используются для протоколов, которые гарантируют доставку и упорядочивание передаваемых данных
    if (server_socket < 0) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // преобразуем IP-адрес (ip) из текстового представления в двоичное представление и записываем в server_address.sin_addr
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "Error binding socket\n");
        close(server_socket);
        return 1;
    }

    // Начало прослушивания входящих соединений
    if (listen(server_socket, 5) < 0) { // этот сокет не принимает данные, он принимает соединения, 5 - число клиентов одновременно
        fprintf(stderr, "Error listening for connections\n");
        close(server_socket);
        return 1;
    }

    printf("Server listening on %s:%d\n", ip, port);

    while (1) {
        // Ожидание нового клиента
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        // функция, которая принимает входящее соединение на серверном сокете и создает новый сокет для общения с клиентом.
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) {
            fprintf(stderr, "Error accepting connection\n");
            continue;
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Создание нового процесса для клиента
        pid_t pid = fork();
        if (pid == 0) {
            // Дочерний процесс: чтение данных от клиента и отправка их обратно
            while (1) {
                char buffer[1024];
                ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

                printf("Data: %s\n", buffer);

                if (bytes_received <= 0) {
                    break;
                }
                send(client_socket, buffer, bytes_received, 0);
            }

            // Закрытие сокета клиента и завершение дочернего процесса
            close(client_socket);
            printf("Client disconnected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            return 0;
        } else if (pid < 0) {
            fprintf(stderr, "Error creating child process\n");
            close(client_socket);
        } else {
            // Родительский процесс: закрытие сокета клиента и продолжение прослушивания
            close(client_socket);
        }
    }
    // Закрытие серверного сокета
    close(server_socket);
    return 0;
}