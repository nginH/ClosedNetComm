Contents
========
* [Prerequisites](#Prerequisites)
* [System Requirement](#System-Requirements)
* [How to use](#How-to-use)
* [Header File Required](#Header-File-Required)
* [Coding Section](#coding-section)
    * [Server Side](#server)
        * [Socket](#socket)
        * [Bind and address](#bind)
        * [Listen](#listen)
        * [Accept](#accept)
        * [Send and recv](#recv)
    * [Client Side](#client)
        * [Socket](#C_socket)
        * [Connect](#C_connect)
        * [send and recv](#C_recv)
* [Multithreading](#milti)

# SocketChatRoom
Build your own real-time chat application using SocketChatApp. This GitHub repository provides a client-server architecture for seamless communication. Easy to use, easy to customize. ( c/c++)

#  <div id="Prerequisites">  Prerequisites: </div>
> Knowledge of the c or c++ programming language.<br>
> Your intrest in learning socket programming.

# <div id="System-Requirements">System Requirements:</div>
Linux or Unix (MacOs) based operating system.<br>
(and on Windows system you might find difficult you have to import winsock2.h , ws2tcpip.h and windows.h blah blah blah...)<br>

# <div id ="How-to-use">How to use:</div>
Clone this repository<br>
Open terminal

```terminal  
 cd socketChatRoom
 g++ server.cpp -o server  -std=c++17 -pthread && clear && ./server
````

open Another terminal<br>
```terminal
 cd socketChatRoom
 g++ client.cpp -o client -std=c++17 -pthread && clear && ./client
```
 >> here's breakdown of above terminal command : <br>
> > g++ :- telling terminal to compile for c++ file  the <client.cpp> this the directory of the cpp  file < -o client > and output it in a client file and  < -std=c++17 > w're stating compilar to use c++17 (you can use 11 -upper) and -pthread to include this while compailing cause this file is multithreaded  and  clear the output of terminal and  execute ./client and similarly for server.cpp
# <div id="Header-File-Required">Header File  Required </div><br>
For  standerd input output( for printf()  scanf() perror() )<br>
```c
#include <stdio.h>
#include <iostream>
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
for string length  we use strlen() strcmp()  strcat() function<br>
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
for multithreading
```c
#include <thread>
```

# <div id="coding-section"> Let's come to coding section, baby! </div><br>

<img src="https://github.com/harshuuanand/SocketChatRoom/assets/80211435/4437973d-c5b2-45dc-8ed1-ebb0916243a4" />


## <div id="server"> SERVER SIDE </div><br>
#### <div id="socket">first we have to create a socket using socket() function</div><br>
```c
int server_socket = socket(Int domanin, int type, int protocol
```
If the protocol argument is non-zero, it shall specify a protocol that is supported by the address family. If the protocol argument is zero, the default protocol for this address family and type shall be used. The protocols supported by the system are implementation-defined.

on success, a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.<br>


### <div id="bind"> Then we have to bind the socket to a port using bind() function<br></div>
### but before that we have to create a struct sockaddr_in object and initialize it with the port and ip address<br>
```c++
struct sockaddr_in server_address;
address.sin_family = AF_INET;
address.sin_port = htons(6969);
char ip[]= "127.0.0.1" ; // localHost
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

#### Now we can bind the socket to the port using bind() function<br>
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
Upon successful completion, bind() shall return 0; otherwise, -1 shall be returned and errno set to indicate the error.<br>
#### <div id ="listen">And after successful binding then we have to listen for the incoming connection using listen() function</div><br>
```c
int listen_result= listen( server_socket, backlog);
```
The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.<br>
##### Upon successful completion, listen() shall return 0; otherwise, -1 shall be returned and errno set to indicate the error.<br>

### <div id="accept"> Now we have to accept the incoming connection using accept() function<br></accept>
```c
int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
```
The accept() function shall extract the first connection on the queue of pending connections, create a new socket with the same socket type protocol and address family as the specified socket, and allocate a new file descriptor for that socket.<br>
##### Upon successful completion, accept() shall return the non-negative file descriptor of the accepted socket. Otherwise, -1 shall be returned and errno set to indicate the error.<br>

### <div id="recv">Now we can send and receive data using send() and recv() function<br></recv>
```c
int send_result = send(client_socket, server_message, sizeof(server_message), 0);
```
The send() function shall initiate transmission of a message from the specified socket to its peer. The send() function shall send a message only when the socket is connected (including when the peer of a connectionless socket has been set via connect()).<br>
##### Upon successful completion, send() shall return the number of bytes sent. Otherwise, -1 shall be returned and errno set to indicate the error.<br>

```c  
int recv_result = recv(client_socket, client_response, sizeof(client_response), 0);
```
The recv() function shall receive a message from a connection-mode or connectionless-mode socket. It is normally used with connected sockets because it does not permit the application to retrieve the source address of received data.<br>
<h5> Upon successful completion, recv() shall return the length of the message in bytes. If no messages are available to be received and the peer has performed an orderly shutdown, recv() shall return 0. Otherwise, -1 shall be returned and errno set to indicate the error.<br></h5>
--------------------------------------------------------------------------------------------------------------------------


## <div id="client">CLIENT SIDE<br></div>
#### <div id="C_socket"> like same as server side we have to create a socket using socket() function<br></div>
### first we have to create a socket using socket() function<br>
```c
 //int client_socket = socket(domanin,type, protocol)
   int client_socket = socket(AF_INET, SOCK_STREAM, 0);
```

###  <div id="C_connect">Then we have to connect to the server using connect() function<br></connect>
```c
int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
```
Upon successful completion, connect() shall return 0; otherwise, -1 shall be returned and errno set to indicate the error.<br>

### <div id="C_send")>Now we can send and receive data using send() and recv() function<br>
```c
int send_result = send(client_socket, client_message, sizeof(client_message), 0);
```
The send() function shall initiate transmission of a message from the specified socket to its peer. The send() function shall send a message only when the socket is connected (including when the peer of a connectionless socket has been set via connect()).<br>
##### Upon successful completion, send() shall return the number of bytes sent. Otherwise, -1 shall be returned and errno set to indicate the error.<br>
<div id="C_recv"> </div>
```c 
 int recv_result = recv(client_socket, server_response, sizeof(server_response), 0);
```
The recv() function shall receive a message from a connection-mode or connectionless-mode socket. It is normally used with connected sockets because it does not permit the application to retrieve the source address of received data.<br>
##### Upon successful completion, recv() shall return the length of the message in bytes. If no messages are available to be received and the peer has performed an orderly shutdown, recv() shall return 0. Otherwise, -1 shall be returned and errno set to indicate the error.<br>

and then we have to close the socket using close() function<br>
```c
close(client_socket);
```
#### And this will in the while loop for continues  transmission of message

-----
# <div id="milti">
> **caution** : This program run total of 3 threads and I'm using FileDescription in two separate thread and since i'm not manipulating content of FileDescription so there will no race or  <mark> DEADLOCK</mark> condition arriver;
> <br> But if you want you can use MUTEX for full proof deadlock
<br>
<div id="need"> <h3> Why needed?</h3><br>
 let assume your program is single threaded like

```c
int main(void){
   int x;
   std::cin>>x;
   int y=5;
   std::cout<<y
}
 ```
assume main fxn is executing std::cin and you want to print y but before that you have to wait for user input to input the value of x then only further execution win happen;<br>
now come to main topic of socket programming<br>
If your are sending some string and client send you data then it will missed by your program and you will get some garbage value in your output<br>
so to avoid this we have to use multithreading for seamlessly chat between the user <br>
```c
// std::thread  ThreadName(function , perimeter, perimeter....);
   std::thread  recvThread(msgrecv  , &ClientServerFileDiscription);
   resThread.join(); // 
```
here msgrecv is a function that will receive the message from the client and store it in the buffer and ClientServerFileDiscription is a file discription of client socket we are sending as a perimeter<br>
we must join the thread otherwise it will terminate the program as main function completed its process<br>

</div>



