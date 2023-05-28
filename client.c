#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>

int main() {
    int clt_sock;
    struct sockaddr_un srv_sockaddr;
    struct sockaddr_un clt_sockaddr;
    int len = 0;
    int val = 0;
    int err;

    char *dsock_file = "./dsock_file";
    char *dsock_file_clt = "./dsock_file-clt";

    signal(SIGPIPE, SIG_IGN); // Ignore the SIGPIPE signal to prevent termination on broken pipe

    clt_sock = socket(AF_UNIX, SOCK_STREAM, 0); // Create a Unix domain socket for the client
    if (clt_sock == -1) {
        printf("socket() failed %s\n", strerror(errno));
        exit(1);
    }

    memset(&clt_sockaddr, 0, sizeof(struct sockaddr_un)); // Clear the client socket address structure
    clt_sockaddr.sun_family = AF_UNIX;
    strcpy(clt_sockaddr.sun_path, dsock_file_clt); // Set the client socket file path

    unlink(dsock_file_clt); // Remove any existing file with the same name
    err = bind(clt_sock, (struct sockaddr*) &clt_sockaddr, sizeof(clt_sockaddr)); // Bind the client socket to the address
    if (err == -1) {
        printf("bind() failed: %s\n", strerror(errno));
        close(clt_sock);
        exit(1);
    }

    memset(&srv_sockaddr, 0, sizeof(struct sockaddr_un)); // Clear the server socket address structure
    srv_sockaddr.sun_family = AF_UNIX;
    strcpy(srv_sockaddr.sun_path, dsock_file); // Set the server socket file path

    err = connect(clt_sock, (struct sockaddr*) &srv_sockaddr, sizeof(srv_sockaddr)); // Connect to the server socket
    if (err == -1) {
        printf("connect() failed: %s\n", strerror(errno));
        close(clt_sock);
        exit(1);
    }

    while (1) {
        err = read(clt_sock, &val, sizeof(val)); // Read data from the server socket
        if (err == -1) {
            printf("read() failed %s\n", strerror(errno));
            sleep(1);
            continue;
        }
        printf("%d\n", val);
        fflush(stdout); // Flush the output buffer to display the data immediately
        sleep(1); // Wait for 1 second
    }
    close(clt_sock); // Close the client socket
    return 0;
}