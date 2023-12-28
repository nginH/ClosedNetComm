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

int port=6969;


void recvfxn(int *clientSocket);
void sendFxn(int *clientSocket);

int main(void ){

   int clientSocketFileDisctiption= socket(AF_INET, SOCK_STREAM, 0);
   if (clientSocketFileDisctiption<0){
      perror("socket creation faild\n");
      exit(1);
   }

   struct sockaddr_in address;
   address.sin_port= htons(port);
   address.sin_family= AF_INET;
   inet_pton(AF_INET, "172.16.56.159", &address.sin_addr.s_addr);

   int connectResult= connect(clientSocketFileDisctiption, (struct sockaddr*) &address, sizeof address);

   if (connectResult==0){
      printf("connection succesfull\n");
   }else{
      printf("%d\n",errno);
      perror("connection field Server not yet UP \n");
      exit(2);
   }

   std::thread sendthread(sendFxn , &clientSocketFileDisctiption);
   std::thread recvthread(recvfxn , &clientSocketFileDisctiption);

   sendthread.join();
   recvthread.join();

   close(clientSocketFileDisctiption);
   shutdown(clientSocketFileDisctiption, SHUT_RDWR);

}

void recvfxn(int *clientSocket){
   char reciveBUFFER[1024];
   int64_t recvByte=0;
   while (true) {
      recvByte= recv(*clientSocket, reciveBUFFER, 1023, 0);
      if     (recvByte<=0)  break;
      else if(recvByte==0) continue;
      else   std::cout<< reciveBUFFER << "\n";
      memset(reciveBUFFER, '\0', 1024);
   }
}

void sendFxn(int *clientSocket){
   char sendmsg[1024];
   char name[1024]="anoop ";
   uint64_t namelen= strlen(name);

   printf("Enter msg: ");
   while (true) {
      fgets(sendmsg, 1024, stdin);
      if (strcmp(sendmsg, "exit")==0) {
         break;
      }
      if (strlen(sendmsg)>1) {
         strcat(name, ": ");
         strcat(name,sendmsg );
         send(*clientSocket, name, strlen(name), 0);
      }
      for (int i=0; sendmsg[i]; ++i) {
         name[i+namelen]='\0';
      }
   }
}

