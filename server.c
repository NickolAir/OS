#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>

#define CLIENTS_COUNT 5

int main() {
    int srv_sock;
    int clt_sock;
    struct sockaddr_un srv_sockaddr;
    struct sockaddr_un clt_sockaddr;
    int len = 0;
    int val = 0;
    int err;

    char *dsock_file = "./dsock_file";

    signal(SIGPIPE, SIG_IGN); // Ignore the SIGPIPE signal to prevent termination on broken pipe

    srv_sock = socket(AF_UNIX, SOCK_STREAM, 0); // Create a Unix domain socket
    if (srv_sock == -1) {
        printf("socket() failed %s\n", strerror(errno));
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un)); // Clear the server socket address structure
    srv_sockaddr.sun_family = AF_UNIX;
    strcpy(srv_sockaddr.sun_path, dsock_file); // Set the server socket file path

    unlink(dsock_file); // Remove any existing file with the same name
    err = bind(srv_sock, (struct sockaddr*) &srv_sockaddr, sizeof(srv_sockaddr)); // Bind the server socket to the address
    if (err == -1) {
        printf("bind() failed: %s\n", strerror(errno));
        close(srv_sock);
        exit(1);
    }

    err = listen(srv_sock, CLIENTS_COUNT); // Listen for client connections
    if (err == -1) {
        printf("listen() failed: %s\n", strerror(errno));
        close(srv_sock);
        exit(1);
    }

    while (1) {
        printf("Waiting connections in domain socket %s\n", dsock_file);

        memset(&clt_sockaddr, 0, sizeof(struct sockaddr_un)); // Clear the client socket address structure
        clt_sock = accept(srv_sock, (struct sockaddr*) &clt_sockaddr, &len); // Accept a client connection
        if (clt_sock == -1) {
            printf("Accept() failed %s\n", strerror(errno));
            close(srv_sock);
            exit(1);
        }

        len = sizeof(clt_sockaddr);
        err = getpeername(clt_sock, (struct sockaddr*) &clt_sockaddr, &len); // Get the peer (client) socket address
        if (err == -1) {
            printf("getpeername() failed: %s\n", strerror(errno));
            close(srv_sock);
            close(clt_sock);
            exit(1);
        }

        printf("Client socket filepath: %s\n", clt_sockaddr.sun_path);

        while (1) {
            err = write(clt_sock, &val, sizeof(val)); // Write data to the client socket
            if (err == -1) {
                printf("write() failed: %s\n", strerror(errno));
                break;
            }
            printf("%d\n", val);
            val++;
            sleep(1);
        }
        close(clt_sock); // Close the client socket
    }
    close(clt_sock); // Close the client socket (Note: unreachable code)
    close(srv_sock); // Close the server socket
    return 0;
}