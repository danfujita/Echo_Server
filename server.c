#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//Platform Used:
//Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
//Apple LLVM version 6.1.0 (clang-602.0.49) (based on LLVM 3.6.0svn)
//Target: x86_64-apple-darwin14.1.1
//Thread model: posix

int get_tcp_sock(short port_num, struct sockaddr_in server_addr, struct sockaddr_in client_addr) {
    int tcp_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (tcp_sock < 0) { //
        printf("Error: Create socket ");
    }
    int bind_err = bind(tcp_sock, (struct sockaddr * ) & server_addr,
                        sizeof(server_addr));
    if (bind_err < 0) {
        printf("Error: Bind\n");
        exit(-1);
    }
    int lis_err = listen(tcp_sock, 15); // Creates a socket queue. Size of the queue is 15.
    if (lis_err < 0) {
        printf("Error: Listen\n");
        exit(-1);
    }
    close(0);
    return tcp_sock;
}

void receive_message(int tcp_sock, struct sockaddr_in server_addr, struct sockaddr_in client_addr) {
    unsigned int client_len = sizeof(client_addr);
    int sock_client = accept(tcp_sock,
                             (struct sockaddr * ) & client_addr, & client_len);
    if (sock_client < 0) {
        printf("Error: Accept\n");
        exit(-1);
    }
    char buffer[512];
    int rec_len = recv(sock_client, buffer, 512, 0);
    if (rec_len < 0) {
        printf("Error: Receive message \n");
        exit(-1);
    } else {
        printf("Received message from: %s\n",
               inet_ntoa(client_addr.sin_addr));
        buffer[rec_len] = 0;
        printf("Received message was: %s\n", buffer);
    }
    int sent_len = send(sock_client, buffer, rec_len, 0);
    if (sent_len < 0) {
        printf("Error: Send data");
        exit(-1);
    }
    close(sock_client);
}

//Run the server first before the client.
//argv[1] stores the port number
// Example: ./server 3008
int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Port number is required.() \n");
        exit(-1);
    }
    short port_num = atoi(argv[1]);

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_addr.sin_port = htons(port_num);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int tcp_sock = get_tcp_sock(port_num, server_addr, client_addr);

    while (1) {
        receive_message(tcp_sock, server_addr, client_addr);
    }
}