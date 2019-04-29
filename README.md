## Echo Server
Echo Server is an echo server written in C that works over TCP. The server receives a message from the client and echos back to the client.

### Building from source

You can also clone the repository and build using make:

    $ git clone https://github.com/danfujita/echo-server.git
    $ make

### Usage

To run the server:

    $ ./server [port number]
  
To run the client:

    $ ./client [port number of the server] [ip address of the server] [Message to send]

Example:

    $ ./server 3008
    $ ./client 3008 "127.0.0.1" "hello"
