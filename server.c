#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//Platform Used:
//Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
//Apple LLVM version 6.1.0 (clang-602.0.49) (based on LLVM 3.6.0svn)
//Target: x86_64-apple-darwin14.1.1
//Thread model: posix

//Run the server first before the client.
//argv[1] stores the port number
// Example: ./server 3008
int main(int argc, char *argv[]) {
    if(argc!=2){//check the argument size
        printf("1 command line argument is required.(port number) \n");
		exit(-1);
    }
	regex_t regex; // POSIX REGEX(pattern matching).
	regcomp(&regex, "^[0-9]{1,5}$", REG_EXTENDED | REG_NEWLINE); //Create a pattern for regex
	int IEntry = regexec(&regex, argv[1], 0, NULL, 0);	//Execute Regex

	if (IEntry) {	//if the not in the pattern
		printf("%s is an illegal entry \n", argv[1]);
	} else {
		short port_number = atoi(argv[1]);	//Convert argument to port number.
		int soket_result = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//Create a socket.
	
		if (soket_result < 0) {	//if socket returns negative one then there is an error.
			printf("Socket Result Error");
		}
        //sockaddr_in is a structure that save an address.
		struct sockaddr_in server_side;	//Store server address.
		struct sockaddr_in client_side;	//store client address.
		
		server_side.sin_port = htons(port_number);	
		server_side.sin_family = AF_INET;
		server_side.sin_addr.s_addr = htonl(INADDR_ANY);
	

		int bindRes = bind(soket_result, (struct sockaddr *) &server_side,
		sizeof(server_side));	
		if (bindRes < 0) {
			printf("Bind Result Error \n");
			exit(-1);
		}
		int listenRes = listen(soket_result, 15);// creates a socket queue. size of the queue is 15.
		if (listenRes < 0) {
			printf("Listen Result Error \n");
			exit(-1);
		}
		close(0);

		while (1) {//continue until the user terminates.
			unsigned int client_len = sizeof(client_side);
			int sock_client = accept(soket_result,
					(struct sockaddr *) &client_side, &client_len);
			if (sock_client < 0) {
				printf("Accept Result Error \n");
				exit(-1);
			}
			char buffer[512];
			int recieveRes;
			recieveRes = recv(sock_client, buffer, 512, 0);
			if (recieveRes < 0) {
				printf("Recieving Data Error \n");
				exit(-1);
			} else {
				printf("Received Message From: %s\n",
						inet_ntoa(client_side.sin_addr));
			    buffer[recieveRes]=0;

				printf("Received Message was: %s\n", buffer);
			}
            int sendRes= send(sock_client, buffer, recieveRes, 0);
            if(sendRes<0){
                printf("Sending Data Error");
				exit(-1);
            }
			close(sock_client);

		}
	}
}

