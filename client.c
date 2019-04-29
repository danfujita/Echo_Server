#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
//Apple LLVM version 6.1.0 (clang-602.0.49) (based on LLVM 3.6.0svn)
//Target: x86_64-apple-darwin14.1.1
//Thread model: posix

void send_message(int tcp_sock, char *message){
    int send_len = strlen(message);
    int sent_len = send(tcp_sock, message, send_len, 0);
    if (sent_len != send_len) {
        printf("Error: Send message \n");
        exit(-1);
    }
    return;
}

void receive_message(int tcp_sock) {
    char buffer[512];
    int rec_len = recv(tcp_sock, buffer, 512, 0);
    if (rec_len <= 0) {
        printf("Error: Receive message \n");
        exit(-1);
    }
    printf("Received: ");
    printf("%s", buffer);//Print received message
    printf("\n");
    return;
}

int get_tcp_sock(short port_num, char *ip_address) {
    int tcp_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_sock < 0) {
        printf("Error: Create socket  \n");
        exit(-1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_port = htons(port_num);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    int conn_err = connect(tcp_sock, (struct sockaddr *) &server_addr,
                           sizeof(server_addr));
    if (conn_err < 0) {
        printf("Error: Connect \n");
        exit(-1);
    }
    return tcp_sock;
}

//Run the server before the client.
//argv[1] contains the port number of the server
//argv[2] contains the ip address of the server (127.0.0.1 for localhost)
//argv[3] contains the message to send.
//Example: ./client 3008 "127.0.0.1" "hello"
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("3 command line arguments are required. Port number,ip address and message to send \n");
        exit(-1);
    }
    short port_num = (short) (atoi(argv[1]));
    char *ip_address = argv[2];
    char *message = argv[3];

    printf("Port number is %s \n", argv[1]);
    printf("IP address is %s \n", ip_address);
    printf("Sending a message: %s \n", message);

    int tcp_sock = get_tcp_sock(port_num, ip_address);
    send_message(tcp_sock, message);
    receive_message(tcp_sock);

    close(tcp_sock);
    exit(0);
}