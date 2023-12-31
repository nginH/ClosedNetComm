
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


#ifndef stdioC_h
#define stdioC_h

/*=====================================================*/
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <arpa/inet.h>
#include <cstring>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"





struct details {
  int clientSocketfileDiscription;
  char name[2048];
  int namelen;
  details() {
       this->clientSocketfileDiscription = -1;
       this->namelen = 0;
       memset(name, '\0', 2048);
  }
};
typedef struct details *ClientDetails;

void recvfxn(ClientDetails);
void sendFxn(ClientDetails);


int client(char ip[16], int port) {
     ClientDetails client = new details();

     client->clientSocketfileDiscription = socket(AF_INET, SOCK_STREAM, 0);
     if (client->clientSocketfileDiscription < 0) {
          perror("socket creation faild\n");
          exit(1);
     }

     struct sockaddr_in address;
     address.sin_port = htons(port);
     address.sin_family = AF_INET;
     inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

     int connectResult = connect(client->clientSocketfileDiscription, (struct sockaddr *) &address, sizeof address);
     if (connectResult == 0) {
          printf("connection successful\n");
     } else {
          printf("%d\n", errno);
          perror("connection field Server not yet UP \n");
          exit(2);
     }

     printf("enter your name:  ");
     fgets(client->name, 127, stdin);
     client->namelen = strlen(client->name);
     client->name[client->namelen-1]='\0';
     client->name[client->namelen+1]='\0';
//     char firstJoinMsg[]=" join the chat";
//     strcat(client->name, firstJoinMsg);
     std::thread sendthread(sendFxn, client);
     std::thread recvthread(recvfxn, client);

     sendthread.join();
     recvthread.join();

     close(client->clientSocketfileDiscription);
     shutdown(client->clientSocketfileDiscription, SHUT_RDWR);

}

void recvfxn(ClientDetails client) {
     char reciveBUFFER[8192];
     int64_t recvByte = 0;
     while (true) {
          recvByte = recv(client->clientSocketfileDiscription, reciveBUFFER, 8191, 0);
          if (recvByte <= 0) break;
          else if (recvByte == 0) continue;
          else std::cout << reciveBUFFER << "\n";
          memset(reciveBUFFER, '\0', 8192);
     }
}

void sendFxn(ClientDetails client) {
     char sendmsg[8192];
     printf("Enter msg: ");
     while (true) {
          fgets(sendmsg, 8192, stdin);
          if (strcmp(sendmsg, "exit\n") == 0) {
               close(client->clientSocketfileDiscription);
               shutdown(client->clientSocketfileDiscription, SHUT_RDWR);
               break;
          }
          strcat(client->name, ": ");
          strcat(client->name, sendmsg);
          if (strlen(sendmsg) > 1) {
               send(client->clientSocketfileDiscription, client->name, strlen(client->name), 0);
          }
          for (int i = 0; sendmsg[i]; ++i) {
               client->name[i + client->namelen] = '\0';
          }
     }
}





#endif /* stdioC_h */
