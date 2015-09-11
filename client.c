#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
//Apple LLVM version 6.1.0 (clang-602.0.49) (based on LLVM 3.6.0svn)
//Target: x86_64-apple-darwin14.1.1
//Thread model: posix

//Run the server first before the client.
//argv[1] stores the port number
//argv[2] stores the ip address.(127.0.0.1 for localhost)
//argv[3] stores the actual message.
//Example: ./client 3008 "127.0.0.1" "hello"
int main(int argc, char *argv[]) {    
    if(argc!=4){//check the argument size
        printf("3 command line arguments are required. port number, IP address and an actual message to send \n");
		exit(-1);
    }
	regex_t regex; // POSIX REGEX(pattern matching) for port number
	regcomp(&regex, "^[0-9]{1,5}$", REG_EXTENDED | REG_NEWLINE); //Create a pattern for regex
	int IEntry = regexec(&regex, argv[1], 0, NULL, 0);	//Execute Regex

	regcomp(&regex, "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$",
			REG_EXTENDED | REG_NEWLINE); //Create a pattern for regex(IP address)
	int IEntry2 = regexec(&regex, argv[2], 0, NULL, 0);	//Execute Regex

	regcomp(&regex, "^[0-9a-zA-Z .-]+$",
			REG_EXTENDED | REG_NEWLINE); //Create a pattern for regex(Message)
	int IEntry3 = regexec(&regex, argv[3], 0, NULL, 0);	//Execute Regex for message
	
	if (IEntry ) {	//if the not in the pattern
		printf("%s is an illegal entry \n", argv[1]);
		exit(-1);
	}else if(IEntry2){
			printf("%s is an illegal entry \n", argv[2]);}
	 else if(IEntry3){
			printf("Illegal Entry.\n");
	} else {
		short port_number = (short)(atoi(argv[1])); 
		char *ip_address = argv[2];  
		char *sending_string = argv[3];

		printf("port number is %s \n", argv[1]); //print the port number.
		printf("IP is %s \n", ip_address); 
		printf("Sending a String:%s \n", sending_string); //print the string to send.
		int socketResult = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Create a socket.

		if (socketResult < 0) {	
			printf("Socket Result Error \n");
			exit(-1);
		}
		struct sockaddr_in server_side;	
		server_side.sin_port = htons(port_number);	

		server_side.sin_family = AF_INET;
		server_side.sin_addr.s_addr = inet_addr(ip_address);
		int connectRes = connect(socketResult, (struct sockaddr *) &server_side,
				sizeof(server_side));
		if (connectRes < 0) {
			printf("Connect Result Error \n");
			exit(-1);
		}

		int send_len = strlen(sending_string);	
		int sendLen = send(socketResult, sending_string, send_len, 0);	
		if (sendLen != send_len) {	
			printf("Send Result Error \n" );
			exit(-1);
		}

		char buffer[512];
		int recieved_size = recv(socketResult, buffer, 512, 0);	
		if (recieved_size <= 0) {
			printf("Receive error \n");
		    exit(-1);
		}
		printf("Received: ");
		printf("%s", buffer);//Print Received Message/
		printf("\n");

		close(socketResult);
		exit(0);
	}
}
