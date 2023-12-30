/*
MIT License

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
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

     // ========================//

void msgSend( int *clientSocket){
     char msg[1024];
     printf("enter msg: ");
     while(true) {
               // std::cin >> msg;
          fgets(msg, 1024, stdin);
          send(*clientSocket, msg, strlen(msg), 0);
          memset(msg, '\0', 1024);
     }
}

     //=============================//

void msgrecv(int *clientSocket){
     char revMsgBuffer[1024];
     memset(revMsgBuffer, '\0', 1024);
     int readbyte=0;
     while (true) {
           readbyte=read(*clientSocket, revMsgBuffer, 1023);

          printf("recv msg: %s", revMsgBuffer);
     }
}

     // SERVER SIDE
int port= 6969;
int main(void){
     int serverSocketFileDiscription= socket(AF_INET, SOCK_STREAM, 0);
     if (serverSocketFileDiscription<0){
          perror("socket creation error\n");
          exit(1);
     }

     struct  sockaddr_in address;
     address.sin_family=AF_INET;
     address.sin_port= htons(port);
     inet_pton(AF_INET,"127.0.0.1",&address.sin_addr.s_addr);
     int BindResult= bind(serverSocketFileDiscription, (struct sockaddr*) &address,sizeof address);

     if (BindResult==0){
          printf("binding sucessful\n");
     }else{
          perror("biding unsucessful\n");
          exit(2);
     }

     int listenRusult = listen(serverSocketFileDiscription, 5);

     if (listenRusult==0){
          printf("listning sucessful\n");
     }else{
          perror("listning unsucessful\n");
          exit(3);
     }

     socklen_t addressLen= sizeof address;
     int ClientServerFileDiscription= accept(serverSocketFileDiscription,(struct sockaddr*)&address, &addressLen);\

     if (ClientServerFileDiscription>0){
          printf("ready for accepting incoming connection\n");
     }else{
          perror("faild in  accepting incoming connection\n");
          exit(4);
     }

     char buffer[1024];
     memset(buffer, '\0', 1024);
     read(ClientServerFileDiscription,buffer, 1023); // -1 for null char

     std::thread  SendThread(msgSend , &ClientServerFileDiscription);
     std::thread  resvThread(msgrecv , &ClientServerFileDiscription);

     SendThread.join();
     resvThread.join();



     close(serverSocketFileDiscription);
     close(ClientServerFileDiscription);
     shutdown(serverSocketFileDiscription, SHUT_RDWR);
     shutdown(ClientServerFileDiscription, SHUT_RDWR);

}

  
