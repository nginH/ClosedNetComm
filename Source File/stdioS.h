/* MIT License

Copyright (c) 2023 Harsh Anand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef stdioS_h
#define stdioS_h

/*=====================================================*/

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <sys/select.h>

//int port    = 6969;
int backlog = 10;

/*
 * Definitions related to Server and Client
 */

/*
 * Data types
 */
struct ClientDetails{
  int ClientSocketFileDiscription;
  int serverSocketFileDiscription;
  int whichClient[20];
  int NumberOfClient;
  char msgSend[1024];
  char MsgResv[2048];
  int tempfd;
  ClientDetails( int fd){
       this->ClientSocketFileDiscription=fd;
       this->tempfd=-1;
       this->NumberOfClient=0;
       memset(this->whichClient, -1, 20);
       memset(this->MsgResv, '\0', 1023);
       memset(this->msgSend, '\0', 1023);
  }
};
typedef struct ClientDetails *CliDetails;

/*
 * function Prototype
 */
void msgSend           (CliDetails);
void msgrecv           (CliDetails);
void savedmsg          (CliDetails);
void messageBroadcast  (CliDetails);
int *acceptFxn (int *, struct sockaddr*, int *);



int Server(char ip[16], int port){

     int serverSocketFileDiscription= socket(AF_INET, SOCK_STREAM, 0);
     if (serverSocketFileDiscription<0){
          perror("socket creation error\n");
          exit(1);
     }

     struct  sockaddr_in address;
     address.sin_family=AF_INET;
     address.sin_port= htons(port);

     inet_pton(AF_INET,ip, &address.sin_addr.s_addr);
     int BindResult= bind(serverSocketFileDiscription, (struct sockaddr*) &address,sizeof address);

     //  BindResult?exit(2):printf("binding sucessful\n");
     if (BindResult==0){
          printf("binding successful\n");
     }else{
          perror("biding unsuccessful\n");
          exit(2);
     }

     int listenRusult = listen(serverSocketFileDiscription, backlog);

     if (listenRusult==0){
          printf("listning successful\n");
     }else{
          perror("listening unsuccessful\n");
          exit(3);
     }

     CliDetails clientDetails = new ClientDetails(0);

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
               perror("error in accepting incoming connection\n");
               errno;
               break;
          }
     }

     /*
      * clean up
      */

     close(serverSocketFileDiscription);
     shutdown(serverSocketFileDiscription, SHUT_RDWR);
     close(clientDetails->ClientSocketFileDiscription);
     shutdown(clientDetails->ClientSocketFileDiscription, SHUT_RDWR);
     return 0;
}

/*
 * function to send message to all the connected client
 */

void msgSend(CliDetails client){
     char name[1024]="";
     char msg[1024];
     uint64_t namelen= strlen(name);

     while(true) {
          fgets(msg, 1024, stdin);
          int x=strcmp(msg, "exit\n");
          if (x==0) {
               exit(EXIT_SUCCESS);
          }
          if (strlen(msg)>1) {
               strcat(name, " ");
               strcat(name, msg );
               strcpy(client->msgSend, name);
               for (int i=0; i<client->NumberOfClient; ++i)
                    send(client->whichClient[i], name, strlen(name), 0);
          }
          for (int i=0;msg[i]; i++) {
               name[i+namelen]='\0';
          }
     }
}
/*
 * Function to recive the message from client//
 */


void msgrecv(CliDetails client){
     char revMsgBuffer[1024];
     memset(revMsgBuffer, '\0', 1024);

     fd_set readfd;
     int maxfd=-1;
     while (true) {
          FD_ZERO(&readfd);
          for (int i=0; i<client->NumberOfClient; ++i) {
                FD_SET(client->whichClient[i], &readfd);
               if (client->whichClient[i]>maxfd) {
                    maxfd=client->whichClient[i];
               }
          }
          int ready= select(maxfd+1, &readfd, nullptr, nullptr, nullptr);
          if (ready==-1) {
               perror("select error\n");
               exit(EXIT_FAILURE);
          }
          for (int i=0; i<client->NumberOfClient; ++i) {
               if (FD_ISSET(client->whichClient[i], &readfd)) {
                    char buffer[1024];
                    int64_t byteResv=read(client->whichClient[i], buffer, 1024);
                    if (byteResv<0) {
                         printf("1. shutting down....\n");
                         close(client->serverSocketFileDiscription);
                         shutdown(client->serverSocketFileDiscription, SHUT_RDWR);
                         for (int j=0; j<client->NumberOfClient; ++j) {
                              close(client->whichClient[j]);
                              shutdown(client->whichClient[j], SHUT_RDWR);
                         }
                         exit(EXIT_SUCCESS);
                    }else if (byteResv>1){
                         client->tempfd=client->whichClient[i];
                         printf("%s\n", buffer);
                         strcpy(client->MsgResv, buffer);
                         messageBroadcast(client);
                         memset(buffer, '\0', 1024);
                    }
               }
          }
     }
}

/*
 * function to broadcast all the message resv to Server to to all the client except the client who send
 */

void messageBroadcast(CliDetails client){
     for (int i=0; i<client->NumberOfClient; ++i) {
          if (client->whichClient[i]!=client->tempfd)
               send(client->whichClient[i], client->MsgResv, strlen(client->MsgResv), 0);
     }
     memset(client->MsgResv, '\0', 1024);
}

#endif /* stdioS_h */
