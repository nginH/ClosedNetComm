<center>

# NoTraceClassChat

</center>

----------------

⚠️ **Caution** ⚠️
  =============================


     * yet not fully tested </h3>
     * if you find any bug or error please report it to me </h3>
     * if you have any suggestion please let me know </h3>

 <div style="color:#D2042D;"><h3>Use at your own risk this is just for <s>learning</s> cheating purpose <br>
 The main idea behind this project is to cheat in exam by sending the question to your friend and get the answer from him/her and send it to your friends in exam hall**😈
</div>
 <div style="color:#EDBB89;">
 <h3>As in the exam hall you don't have access to internet (but you have LAN cable plugged in to your computer so this program exploit this loop hole to send and receive message from your friend you just have to import this header file and use call a single function with parameter ip address and port number and you are good to go )
</div>

## The explanation below may not correspond to the cpp file in this repository, but the idea behind this program is the same. This explanation is from a previous version of the program and has not been updated. * <div style="color:#76D7C4  ;">coz i'm lazy af 😴</div>


Contents
========
* [Prerequisites](#Prerequisites)
* [System Requirement](#System-Requirements)
* [How to use](#How-to-use)
* [Header File Required](#Header-File-Required)
* [Coding Section](#coding-section)
    * [Server Side](#server)
        * [structure](#struct)
        * [address](#address)
        * [Socket](#socket)
        * [Bind](#bind)
        * [Listen](#listen)
        * [Accept](#accept)
        * [Send ](#send)
        * [resv](#resv)
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


# <div id="coding-section"> Let's come to coding section, baby! </div><br>

<img src="https://github.com/harshuuanand/SocketChatRoom/assets/80211435/4437973d-c5b2-45dc-8ed1-ebb0916243a4" />
# <div id="Header-File-Required">Header File  Required <br>

```c++
#include <iostream>       // for input and output related functionality 
#include <sys/socket.h>   // for socket related functionality (like socket()
#include <netinet/in.h>   // for address related functionality ( like struct sockadder_in)
#include <unistd.h>
#include <arpa/inet.h>   // for converting address to computer understandable functionality (inet_pton)
#include <cstring>       // for string related functionality like strlen() strcat() strcmp() ...)
#include <thread>       // for multithreading ( std::threads)
#include <chrono>      // for time related functionality 
#include <sys/select.h> // for checking activity of clientsocketFileDiscription (select() __DARWIN_FD_SET().. )  

```
</div>

-----

<div id="#struct">
### In this section structure is created that are using throughout the program to get detail of program<br>
   
```c++
struct ClientDetails{
     int ClientSocketFileDiscription;
     int serverSocketFileDiscription;
     int whichClient[20];
     int NumberOfClient;
     char msgSend[1024];
     char MsgResv[2048];
     int tempfd;
     ClientDetails( int fd){                  // constructor for defined struct
          this->ClientSocketFileDiscription=fd;
          this->tempfd=-1;                    // tempfd to -1
          this->NumberOfClient=0;
          memset(this->whichClient, -1, 20);  //initialising this whickClient buffer to null character
          memset(this->MsgResv, '\0', 1023);  // initialising this MesResv buffer to null character
          memset(this->msgSend, '\0', 1023);  // initialising this msgSend buffer to null character
     }
};
```
</div>

> here we declare the prototype of function that are using in the program<br>


```c++
void msgSend           (CliDetails);
void msgrecv           (CliDetails);
void savedmsg          (CliDetails);
void messageBroadcast  (CliDetails);
int *acceptFxn (int *, struct sockaddr*, int *);
```

### Now come to main() fxn;

<div id="socket">
first we have to create a socket using [socket()](https://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html) function<br>

```c 
int serverSocketFileDiscription= socket(AF_INET, SOCK_STREAM, 0);
// we have to cross check the socket is created or not
if (serverSocketFileDiscription<0){
          perror("socket creation error\n");
          exit(1);
}
```
</div>
<div id="address">
This section after creating socket we have to assign address to the socket using bind()  function<br>
By using inet_pton i'm assigning the address and also checking wether address assigned or not. <br>
Ff assigned(if valid ip) then we proceed further otherwise we exit the program and close the serverSocketFileDiscription <br>

```c
struct  sockaddr_in address;
     address.sin_family=AF_INET;
     address.sin_port= htons(port);
     int addressAssignmentResult= inet_pton(AF_INET,"127.0.0.1",&address.sin_addr.s_addr);
     if (addressAssignmentResult<0) {
          perror("invlid result\n");
          close(serverSocketFileDiscription);
          shutdown(serverSocketFileDiscription, SHUT_RDWR);
          exit(EXIT_FAILURE);
     }
```
>**warning**:  if you are using port between 0 and 1023 (system port) then this might cause faild in binding cause they are reserve for essential system services and application that are used across different operating systems. <br>
> so use port between 1024 and 65535 (user port) <br>
</div>

<div id="bind"> Now we have to bind the socket to the port using [bind()](#https://pubs.opengroup.org/onlinepubs/009695399/functions/bind.html) function . <br> on seccessful binding we have to print the message otherwise we have to close the serverSocketFileDiscription and exit the program<br>

```C++
     int BindResult= bind(serverSocketFileDiscription, (struct sockaddr*) &address,sizeof address);
     if (BindResult==0){
          printf("binding sucessful\n");
     }else{
          close(serverSocketFileDiscription);
          shutdown(serverSocketFileDiscription, SHUT_RDWR);
          perror("biding unsucessful\n");
          exit(2);
     }
```
</div>

Now we have to listen for the incoming connection using [listen()](#https://pubs.opengroup.org/onlinepubs/009695399/functions/listen.html) function<br>
```C++
 int listenRusult = listen(serverSocketFileDiscription, backlog);

     if (listenRusult==0){
          printf("listning sucessful\n");
     }else{
          close(serverSocketFileDiscription);
          shutdown(serverSocketFileDiscription, SHUT_RDWR);
          perror("listning unsucessful\n");
          exit(3);
     }
   ```
Creating new variable for everything like  serverSocketFileDescription, number of client , resved msg , send message , array of client file description<br>
```C++
  CliDetails clientDetails = new ClientDetails(0);
```
<div id="accept">

now in while loop we have to accept the incoming connection using [accept()](#https://pubs.opengroup.org/onlinepubs/009695399/functions/accept.html) function<br>
```C++
 while (true) {
          socklen_t addressLen= sizeof address;
          clientDetails->ClientSocketFileDiscription= accept(serverSocketFileDiscription,(struct sockaddr*)&address, &addressLen);
          clientDetails->whichClient[clientDetails->NumberOfClient++]=clientDetails->ClientSocketFileDiscription;

          printf("New client joined the server\n");

          if (clientDetails->ClientSocketFileDiscription>0) {
               std::thread  SendThread(msgSend , clientDetails); // creating new thread for msg send 
               std::thread  resvThread(msgrecv , clientDetails); // creating new thread for msg recv
               SendThread.detach(); // detaching the sendThread from main thread  
               resvThread.detach();     //detaching ther resvThread from main thread
          }else{
               perror("error in acceptng incoming conection\n");
               break;
          }
     }
   ```
</div>

And for every new connection we have to create a new thread for sending and receiving message and detach it from main thread for not further interfacing with main threads<br>
```C++
 while (true) {
          socklen_t addressLen= sizeof address;
          clientDetails->ClientSocketFileDiscription= accept(serverSocketFileDiscription,(struct sockaddr*)&address, &addressLen);
          clientDetails->whichClient[clientDetails->NumberOfClient++]=clientDetails->ClientSocketFileDiscription;

          printf("New client joined the server\n");

          if (clientDetails->ClientSocketFileDiscription>0) {
               std::thread  SendThread(msgSend , clientDetails);
               std::thread  resvThread(msgrecv , clientDetails);
               SendThread.detach();
               resvThread.detach();
          }else{
               perror("error in acceptng incoming conection\n");
               break;
          }
     }
   ```

Now ending main thread we shutdown all the file description  <br>
SHUT_RDWR mean shutdown read and write ;
```C++
 close(serverSocketFileDiscription);
     shutdown(serverSocketFileDiscription, SHUT_RDWR);
     close(clientDetails->ClientSocketFileDiscription);
     shutdown(clientDetails->ClientSocketFileDiscription, SHUT_RDWR);
```
<div id="send">
std::thread  SendThread(msgSend , clientDetails);<br>
this will create new thread separate from main thread  this call msgSend with argument clientDetails <br>

```C++
void msgSend(CliDetails client){
     char name[1024]=""; // creating the name variable 
     char msg[1024];     // creatimng the msg varible through which we send message to all the client
     uint64_t namelen= strlen(name); // if we assign the name the namelen will store the the lenght of name
     while(true) {  //  for continusely send message we server want to send after each itration Fgets () will will wait for user input 
          fgets(msg, 1024, stdin);  // continusely taking input of string from terminal
          int x=strcmp(msg, "exit\n");  // comparing the input message with exit for exiting from programm
          if (x==0) {  // checking the value of strcmp()
               printf("shutting down....\n"); 
               close(client->serverSocketFileDiscription);// closing the server file discription
               shutdown(client->serverSocketFileDiscription, SHUT_RDWR); // shutting down read write for server Socket File Discription
               for (int i=0; i<client->NumberOfClient; ++i) { // itrate over the client file discription;
                    close(client->whichClient[i]); // closing the client socket file discriptiom
                    shutdown(client->whichClient[i], SHUT_RDWR); // shutting downn the read and write for client File discription
               }
               exit(EXIT_SUCCESS); // exiting the process 
          }
          if (strlen(msg)>1) { // if strlen is greater then 1 ( 1 for if enter itself 1 char ) we can use fflush(stdin);
               strcat(name, " ");   // copying the " " to name 
               strcat(name, msg ); // copying the mag to name
               strcpy(client->msgSend, name); // copying name to client->msgSend
               for (int i=0; i<client->NumberOfClient; ++i)  // brodcasting to all the  connected client
                    send(client->whichClient[i], name, strlen(name), 0);
          }
          for (int i=0;msg[i]; i++) { // setting the msg to null character except the name 
               name[i+namelen]='\0';
          }
     }
     return;
}

```

</div>

<div id="resv">



now message receive function<br>


```C++

void msgrecv(CliDetails client){
     char revMsgBuffer[1024]; // message buffer for receiving message from client
     memset(revMsgBuffer, '\0', 1024); // initialising the buffer to null character

     fd_set readfd; // creating the fd_set for checking the activity of client socket file discription
     int maxfd=-1; // creating the maxfd for storing the maximum file discription
     while (true) { // continusely checking the activity of client socket file discription
          __DARWIN_FD_ZERO(&readfd); // setting the readfd to zero
          for (int i=0; i<client->NumberOfClient; ++i) { // itrate over the client file discription
               __DARWIN_FD_SET(client->whichClient[i], &readfd); ,, // setting the client file discription to readfd
               if (client->whichClient[i]>maxfd) { // checking the maximum file discription
                    maxfd=client->whichClient[i]; // setting the maximum file discription
               }
          }
          int ready= select(maxfd+1, &readfd, NULL, NULL, NULL); // checking the activity of client socket file discription
          if (ready==-1) { // if ready is -1 then there is some error in select()
               perror("select error\n"); // printing the error
               exit(EXIT_FAILURE); // exiting the process
          }
          for (int i=0; i<client->NumberOfClient; ++i) {// itrate over the client file discription
               if (__DARWIN_FD_ISSET(client->whichClient[i], &readfd)) {// checking the activity of client socket file discription
                    char buffer[1024];// creating the buffer for storing the message
                    int64_t byteResv=read(client->whichClient[i], buffer, 1024);// reading the message from client socket file discription
                    if (byteResv<0) {// if byteResv is less then 0 then there is some error in reading
                         printf("shutting down....\n");
                         close(client->serverSocketFileDiscription); // closing the server socket file discription
                         shutdown(client->serverSocketFileDiscription, SHUT_RDWR);// shutting down read write for server Socket File Discription
                         for (int i=0; i<client->NumberOfClient; ++i) { // itrate over the client file discription;
                              close(client->whichClient[i]);// closing the client socket file discriptiom
                              shutdown(client->whichClient[i], SHUT_RDWR);// shutting downn the read and write for client File discription
                         }
                         exit(EXIT_SUCCESS);// exiting the process
                    }else if (byteResv>1){// if byteResv is greater then 1 then there is some message from client
                         client->tempfd=client->whichClient[i]; // setting the client->tempfd to client->whichClient[i]
                         printf("resv: %s\n",buffer);// 
                         messageBroadcast(client); // calling the messageBroadcast function
                         memset(buffer, '\0', 1024);// initialising the buffer to null character
                    }
               }
          }
     }
}
```
</div>

now message broadcast function<br>
```C++
void messageBroadcast(CliDetails client){
     for (int i=0; i<client->NumberOfClient; ++i) { // itrate over the client file discription
          if (client->whichClient[i]!=client->tempfd) // checking the client->whichClient[i] is not equal to client->tempfd (coz clientfd send the msg and we dont want to send the same msg to clientfd)
               send(client->whichClient[i], client->MsgResv, strlen(client->MsgResv), 0); // sending the message to all the client except the clientfd
     }
     memset(client->MsgResv, '\0', 8192);// initialising the client->MsgResv to null character

     return;
}
```

```
------------------------------------------------------------------------------------------------------------------------------
>**warning**:  if you are using port between 0 and 1023 (system port) then this might cause faild in binding cause they are reserve for essential system services and application that are used across different operating systems. <br>
> so use port between 1024 and 65535 (user port) <br>
``` terminal
 netstat -a |grep LISTEN
```
or
``` terminal
 lsof -i -p | grep LISTEN
``` 


### By using this command in terminal you can see all the port that are in use.<br>

-------------------

--------------------------

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

### <div id="C_send">Now we can send and receive data using send() and recv() function<br>
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



