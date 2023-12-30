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


//=====================SERVER SIDE=========================//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <poll.h>
#include <sys/select.h>

//int ClientServerFileDiscription= accept(serverSocketFileDiscription,(struct sockaddr*)&address, &addressLen);\


int port    = 6969;
int backlog = 10;

struct Datails {
  int serverSocketFileDiscription;
  int clientFileDiscription;
  int NumberOfClient;
  int backlog;
  int port;
  int addressFamily;
  char name[1024];
  char msgRecv[1024];
  char msgSend[1024];
  Datails(){
       this->serverSocketFileDiscription=-1;
       this->clientFileDiscription=-1;
       this->NumberOfClient=0;
       this->backlog=10;
       this->port=6969;
       this->addressFamily=AF_INET;
       memset(this->name, '\0', 1024);
       memset(this->msgRecv, '\0', 1024);
       memset(this->msgSend, '\0', 1024);
  }
};


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


void msgSend           (CliDetails);
void msgrecv           (CliDetails);
void savedmsg          (CliDetails);
void messageBroadcast  (CliDetails);
int *acceptFxn (int *, struct sockaddr*, int *);


int main(void){

     int serverSocketFileDiscription= socket(AF_INET, SOCK_STREAM, 0);
     if (serverSocketFileDiscription<0){
          perror("socket creation error\n");
          exit(1);
     }

     struct  sockaddr_in address;
     address.sin_family=AF_INET;
     address.sin_port= htons(port);
     inet_pton(AF_INET,"172.16.56.159",&address.sin_addr.s_addr);
     int BindResult= bind(serverSocketFileDiscription, (struct sockaddr*) &address,sizeof address);

     //  BindResult?exit(2):printf("binding sucessful\n");
     if (BindResult==0){
          printf("binding sucessful\n");
     }else{
          perror("biding unsucessful\n");
          exit(2);
     }

     int listenRusult = listen(serverSocketFileDiscription, backlog);

     if (listenRusult==0){
          printf("listning sucessful\n");
     }else{
          perror("listning unsucessful\n");
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
               perror("error in acceptng incoming conection\n");
               errno;
               break;
          }
     }

     /* clean up */

     close(serverSocketFileDiscription);
     shutdown(serverSocketFileDiscription, SHUT_RDWR);
     close(clientDetails->ClientSocketFileDiscription);
     shutdown(clientDetails->ClientSocketFileDiscription, SHUT_RDWR);
     return 0;
}

void msgSend(CliDetails client){
     char name[1024]="";
     char msg[1024];
     uint64_t namelen= strlen(name);

     while(true) {
          fgets(msg, 1024, stdin);
          int x=strcmp(msg, "exit\n");
          if (x==0) {
               printf("shutting down....\n");
               close(client->serverSocketFileDiscription);
               shutdown(client->serverSocketFileDiscription, SHUT_RDWR);
               for (int i=0; i<client->NumberOfClient; ++i) {
                    close(client->whichClient[i]);
                    shutdown(client->whichClient[i], SHUT_RDWR);
               }
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
     return;
}



void msgrecv(CliDetails client){
     char revMsgBuffer[1024];
     memset(revMsgBuffer, '\0', 1024);
//     int64_t readbyte=0;
     struct pollfd fds[20];
     fds[0].fd= client->serverSocketFileDiscription;
     fds[0].events=POLLIN;
     while (true){
          for (int i=0; i<client->NumberOfClient; ++i) {
               if (client->whichClient[i]!=-1) {
                    fds[i+1].fd=client->whichClient[i];
                    fds[i+1].events=POLLIN;
               }
          }
          int x=   poll(fds, client->NumberOfClient, -1);
//          printf("<  %d > ", x);
          for (int i=0; i<client->NumberOfClient; ++i) {
               if (fds[i].revents&POLLIN) {
                    char buffer[1024];
                    int64_t byteread= recv(fds[i].fd, buffer, 1024, 0);
                    if (byteread<0) {
                         exit(5);
                    }else if (byteread==1) continue;
                    else{
                         std::cout<<"msg: "<<buffer<<"\n";
                         client->tempfd=fds[i].fd;
                         strcpy(client->MsgResv, buffer);
                         messageBroadcast(client);
                         memset(buffer, '\0', 1024);
                    }
               }
          }



     }



//     while (true) {
//               readbyte=recv(client->whichClient[0], revMsgBuffer, 1023, 0);
//          int pollResult= poll(client->whichClient, x, 500);
////          printf("msg resv : client fd: %d\n", client->ClientSocketFileDiscription);
//          if (readbyte<=0) {
//               break;
//          }else if (readbyte==1){
//               continue;
//          }else{
//               strcpy(client->MsgResv, revMsgBuffer);
//               messageBroadcast(client);
//               printf("%s\n", revMsgBuffer);
//          }
//     }
//     return;
}

void messageBroadcast(CliDetails client){
     for (int i=0; i<client->NumberOfClient; ++i) {
          if (client->whichClient[i]!=client->tempfd)
               send(client->whichClient[i], client->MsgResv, strlen(client->MsgResv), 0);
     }
     memset(client->MsgResv, '\0', 1024);

     return;
}

//int ClientServerFileDiscription= accept(serverSocketFileDiscription,(struct sockaddr*)&address, &addressLen);
int * acceptFxn( int * serverFileDiscription , struct sockaddr_in address){
     int clientServerFileDiscription;
     socklen_t addressLen= sizeof(address);
     while (true) {
          clientServerFileDiscription=accept(*serverFileDiscription, (struct sockaddr *)&address, &addressLen);

          std::this_thread::sleep_for(std::chrono::milliseconds(500));
     }

}
