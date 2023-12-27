# SocketChatRoom
Build your own real-time chat application using SocketChatApp. This GitHub repository provides a client-server architecture for seamless communication. Easy to use, easy to customize. ( c/c++)

# Prerecquest:
  > Knowledge of the c or c++ programming language.<br>
  > Your intrest in learning socket programming.
  
# Systrem Requirements:
  Linux or Unix (MacOs) based operating system.<br>
  (and on Windows system you might find difficult you have to import winsock2.h , ws2tcpip.h and windows.h blah blah blah...)<br>

# How to use:
Clone this repository<br>
Open terminal 

```terminal  
 cd socketChatRoom<br>
 gcc server.c -o server && ./server<br>
````

open Another terminal<br>
```terminal
 cd socketChatRoom<br>
 gcc client.c -o client && ./client<br>
```

# Header File  Required <br>
For  standerd input output( for printf()  scanf() perror() )<br>
```c
#include <stdio.h>
``` 
For exit function (exit(int))<br>
```c
#include <stdlib.h>
```

For socket programming ([socket()](https://pubs.opengroup.org/onlinepubs/009604499/functions/socket.html) , [bind()](https://pubs.opengroup.org/onlinepubs/009604499/functions/bind.html) , [listen()](https://pubs.opengroup.org/onlinepubs/009604499/functions/listen.html), [accept()](https://pubs.opengroup.org/onlinepubs/009604499/functions/accept.html) , [send()](https://pubs.opengroup.org/onlinepubs/009604499/functions/send.html) , [recv()](https://pubs.opengroup.org/onlinepubs/009604499/functions/recv.html) )<br>
```c
#include <sys/socket.h>
```
for close function (close(int))<br>
```c
#include <unistd.h>
```
for string length  we use strlen() function<br>
```c
#include <string.h>
```
for struct adder_in (sockaddr_in)<br>
```c
#include <netinet/in.h>
```
for inet_pton() function<br>
```c
#include <arpa/inet.h>
```
for boolean data type<br>
```c    
#include <stdbool.h>
```


# Working of SocketChatRoom!
 ### first we have to create a socket using socket() function<br>
```c
int server_socket = socket(Int domanin, int type, int protocol
```
int domain = AF_INET (IPv4 protocol)<br>
int type = SOCK_STREAM (TCP protocol)<br>
int protocol = 0 (for IPv4 protocol)<br>
If the protocol argument is non-zero, it shall specify a protocol that is supported by the address family. If the protocol argument is zero, the default protocol for this address family and type shall be used. The protocols supported by the system are implementation-defined.

on success, a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.<br>


### Then we have to bind the socket to a port using bind() function<br>
 ### but before that we have to create a struct sockaddr_in object and initialize it with the port and ip address<br>
```c++
struct sockaddr_in server_address;
address.sin_family = AF_INET;
address.sin_port = htons(6969);
char ip[]= "127.0.0.1" // localHost
inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
```
------------------------------------------------------------------------------------------------------------------------------
 >**warning**:  if you are using port between 0 and 1023 (system port) then this might cause faild in binding cause they are reserve for essential system services and application that are used across different operating systems. <br>
``` terminal
 netstat -a |grep LISTEN
```
or 
``` terminal
 lsof -i -p | grep LISTEN
``` 

by using this command in terminal you can see all the port that are in use.<br>
------------------------------------------------------------------------------------------------------------------------------

### Now we can bind the socket to the port using bind() function<br>
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
Upon successful completion, bind() shall return 0; otherwise, -1 shall be returned and errno set to indicate the error.<br>
#### And after succesfull binding then we have to listen for the incoming connection using listen() function<br>
```c
int listen_result= listen( server_socket, backlog);
```
The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.<br>
##### Upon successful completion, listen() shall return 0; otherwise, -1 shall be returned and errno set to indicate the error.<br>

### Now we have to accept the incoming connection using accept() function<br>
```c
int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
```
The accept() function shall extract the first connection on the queue of pending connections, create a new socket with the same socket type protocol and address family as the specified socket, and allocate a new file descriptor for that socket.<br>
##### Upon successful completion, accept() shall return the non-negative file descriptor of the accepted socket. Otherwise, -1 shall be returned and errno set to indicate the error.<br>

### Now we can send and receive data using send() and recv() function<br>
```c
int send_result = send(client_socket, server_message, sizeof(server_message), 0);
```
The send() function shall initiate transmission of a message from the specified socket to its peer. The send() function shall send a message only when the socket is connected (including when the peer of a connectionless socket has been set via connect()).<br>
##### Upon successful completion, send() shall return the number of bytes sent. Otherwise, -1 shall be returned and errno set to indicate the error.<br>

```c  
int recv_result = recv(client_socket, client_response, sizeof(client_response), 0);
```
The recv() function shall receive a message from a connection-mode or connectionless-mode socket. It is normally used with connected sockets because it does not permit the application to retrieve the source address of received data.<br>
##### Upon successful completion, recv() shall return the length of the message in bytes. If no messages are available to be received and the peer has performed an orderly shutdown, recv() shall return 0. Otherwise, -1 shall be returned and errno set to indicate the error.<br>



