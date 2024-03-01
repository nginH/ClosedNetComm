
//  iostreamS.h
/*-----------------------------NoTraceClassChat---------------------------*/


/* MIT License

 Copyright (c) 2024-2029 ----69k

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

/*
 * pragma
 */




#ifndef iostreamS_h
#define iostreamS_h


// instructing compiler for level 3 optimization
// for increasing the performance of the program
//#pragma GCC optimize("03")


#include <iostream>
#include <chrono>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#ifdef __MACH__
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#endif

#ifdef __unix__
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#define __DARWIN_FD_ZERO(p) FD_ZERO(p)
#define __DARWIN_FD_SET(fd, p) FD_SET(fd, p)
#define __DARWIN_FD_ISSET(fd, p) FD_ISSET(fd, p)
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define __DARWIN_FD_ZERO(p) FD_ZERO(p)
#define __DARWIN_FD_SET(fd, p) FD_SET(fd, p)
#define __DARWIN_FD_ISSET(fd, p) FD_ISSET(fd, p)
#endif

// this user difined input
//const char ip[]= "10.53.3.221";
//const uint32_t port = 6969;


const uint32_t backlog = 10;
const uint32_t SIZE = 4096;
const bool dbg = false;
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

/****--------- Container Construction----------*/
//Encapsulates information about a connected client within a server application.
struct ClientDetails {
       int32_t ClientSocketFileDiscription;
       int32_t serverSocketFileDiscription;
       int32_t whichClient[30];
       uint64_t NumberOfClient;
       bool finalExit;
       bool broadCastingPrevMsgToClient;
       bool ChatMsgStatus;
       char msgSend[4096];
       char MsgResv[4096];
       int32_t tempfd;
       explicit ClientDetails(int32_t fd) {
              this->ClientSocketFileDiscription = fd;
              this->serverSocketFileDiscription = -1;
              this->NumberOfClient = 0;
              this->tempfd = -1;
              this->finalExit = true;
              this->broadCastingPrevMsgToClient = false;
              this->ChatMsgStatus = false;
              memset(this->whichClient, -1, 30);
              memset(this->MsgResv, '\0', 1023);
              memset(this->msgSend, '\0', 1023);
       }
};

typedef struct ClientDetails *CliDetails;

/* structure for node*/

struct subNode {
       char *answer;
       subNode *down;
       explicit subNode(char ans[SIZE]) {
              this->answer = (char *) calloc(SIZE, sizeof(char));
              strcpy(this->answer, ans);
              this->down = nullptr;
              //      memset(this->answer, '\0', SIZE);
       }
};

struct node {
       int8_t setNo;
       node *next;
       subNode *down;
       explicit node(int32_t set) {
              this->setNo = set;
              this->next = nullptr;
              this->down = nullptr;
       }
} *head = nullptr, *tail = nullptr;

// for saving chat

struct Chat {
       char *msg;
       Chat *next;
       explicit Chat(char msg[SIZE]) {
              this->msg = (char *) calloc(SIZE, sizeof(char));
              strcpy(this->msg, msg);
              this->next = nullptr;
       }
} *chatHead = nullptr, *chatTail = nullptr;
/*__-_-_-_-_-*/

/*-----------------------function prototype Declaration-------------------------------*/
// chat save function

// for saving all kind of message recv from chat and client
void saveChat(char msg[SIZE]);
void printChat(CliDetails);
void option(CliDetails);
void msgSend(CliDetails);
void msgResv(CliDetails);
void messageBroadcast(CliDetails);
void case3_CreateSet(CliDetails);
void case2_ShowAnswer(CliDetails);
void case5_CreateSubset(CliDetails);
void creatSet(int32_t setNumber);
void printSubSet(int32_t setNumber);
bool printSuperSet(bool check, int32_t setNumber);
void createSubSet(int32_t setNumber, char data[SIZE]);
void MemoryManagement();
void ChatMemoryManagement();

void internetCheck(void) {
       int32_t pipe_arr[2];
       char buf[SIZE];
       pipe(pipe_arr);
       if (fork() == 0) {
              dup2(pipe_arr[1], STDOUT_FILENO);
              execl("/sbin/ping", "ping", "-c 1", "8.8.8.8", (char *) nullptr);
       }else{
              wait(NULL);
              read(pipe_arr[0], buf, SIZE);
              printf("%s\n", buf);
       }
}


//void __attrubute__((noreturn)) exit(int32_t status);

/*=====================-- Main fxn-- =====================*/
/* main diving code */
//const char ip[]= "10.53.3.221";
//const uint32_t port = 6969;

int32_t server(char *ip, int port){
       if (port <=1024) {
              perror("system reserve port you cant assign this may conflicetd with system\n");
              exit(1);
       }

       system("clear");

       //       std::thread internet(internetCheck);
       //       internet.join();
       int32_t serverSocketFileDiscription = socket(AF_INET, SOCK_STREAM, 0);
       if (serverSocketFileDiscription < 0) {
              perror("socket creation error\n");
              exit(1);
       }

       /// setting socket to nonblocking by default it's blocking
       //  fcntl(serverSocketFileDiscription,F_SETFL,O_NONBLOCK);

       struct sockaddr_in address {};
       address.sin_family = AF_INET;
       address.sin_port = htons(port);
       inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
       int32_t BindResult = bind(serverSocketFileDiscription, (struct sockaddr *) &address, sizeof address);

       ///  BindResult?exit(2):printf("binding successful\n");
       if (BindResult == 0) {
              printf("binding successful\n");
       } else {
              perror("biding unsuccessful\n");
              exit(2);
       }

       uint64_t listenRusult = listen(serverSocketFileDiscription, backlog);
       if (listenRusult == 0) {
              printf("listening successful\n");
       } else {
              perror("listening unsuccessful\n");
              exit(3);
       }

       ClientDetails * clientDetails = new ClientDetails(0);

       std::thread options(option, clientDetails);
       options.detach();

       while (true) {
              socklen_t addressLen = sizeof address;
              clientDetails->ClientSocketFileDiscription =
                  accept(serverSocketFileDiscription, (struct sockaddr *) &address, &addressLen);
              clientDetails->whichClient[clientDetails->NumberOfClient++] = clientDetails->ClientSocketFileDiscription;
              std::cout << GREEN << "New client joined the server\n"
                        << RESET;

              if (clientDetails->ClientSocketFileDiscription > 0) {
                     std::thread SendThread(msgSend, clientDetails);
                     std::thread resvThread(msgResv, clientDetails);

                     // for broadcasting previous message to newly connected client

                     SendThread.detach();
                     resvThread.detach();
                     clientDetails->broadCastingPrevMsgToClient = true;
                     printChat(clientDetails);
                     clientDetails->broadCastingPrevMsgToClient = false;

              } else {
                     perror("error in accepting incoming connection\n");
                     errno;
                     break;
              }
       }

       /* clean up */
       if (clientDetails->finalExit) {
              clientDetails->finalExit = false;
              close(serverSocketFileDiscription);
              shutdown(serverSocketFileDiscription, SHUT_RDWR);
              for (int32_t i = 0; i < clientDetails->NumberOfClient; ++i) {
                     close(clientDetails->whichClient[i]);
                     shutdown(clientDetails->whichClient[i], SHUT_RDWR);
              }
              MemoryManagement();
              ChatMemoryManagement();
              clientDetails->finalExit = false;
       }

       system("clear");
       //       system("exit");
       return 0;
}
/*=====++++++++++++++++++++++==========+++++=======+==+===++=+=+======+======+=+====+====*/

void msgSend(CliDetails client) {
       char msg[4096];
       while (client->ChatMsgStatus) {
              fgets(msg, 4096, stdin);
              if (strcmp(msg, "-1\n") == 0) {
                     system("clear");
                     return;
              }
              int32_t x = strcmp(msg, "exit\n");
              if (x == 0 and client->finalExit) {
                     client->finalExit = false;
                     printf("2. shutting down....\n");
                     close(client->serverSocketFileDiscription);
                     shutdown(client->serverSocketFileDiscription, SHUT_RDWR);
                     for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                            close(client->whichClient[i]);
                            shutdown(client->whichClient[i], SHUT_RDWR);
                     }
                     system("clear");
                     system("exit");
                     pid_t val = getpid();
                     std::cout << val << "\n";
                     std::exit(0);
              }
              //////------------------------------//////
              // else none of above then well send it to all the connected client;

              if (strlen(msg) > 1) {
                     char toSend[SIZE];
                     toSend[0] = '1';
                     toSend[1] = 'c';          // identifer of chat
                     for (int32_t len = 0; len < strlen(msg) && len < SIZE - 2; ++len) {
                            toSend[2 + len] = msg[len];          // concatinate the message to toSend buffer
                     }
                     strcpy(client->msgSend, toSend);          //
                     saveChat(toSend);
                     for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                            send(client->whichClient[i], toSend, strlen(toSend), 0);
                     }
                     memset(toSend, '\0', strlen(toSend));
              }
       }
}

// this fxn will manage the server and the reside message
// as well ad this will also look for the client connection state
// if client disconnect the serve then  i'll remove the client  FD
// then i'll remove the name from  the list of connected fd
// if not done it message broadcast fxn will assess the already
// close fd this will crash the program

void msgResv(CliDetails client) {
       char revMsgBuffer[4096];
       memset(revMsgBuffer, '\0', 4096);
       fd_set readfd;
       int32_t maxfd = -1;
       while (client->finalExit) {
              __DARWIN_FD_ZERO(&readfd);
              for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                     __DARWIN_FD_SET(client->whichClient[i], &readfd);
                     if (client->whichClient[i] > maxfd) {
                            maxfd = client->whichClient[i];
                     }
              }
              int32_t ready = select(maxfd + 1, &readfd, nullptr, nullptr, nullptr);
              if (ready == -1) {
                     perror("select error\n");
                     client->finalExit = false;
                     return;
              }

              for (int32_t i = 0; i < client->NumberOfClient and client->finalExit; ++i) {
                     if (__DARWIN_FD_ISSET(client->whichClient[i], &readfd)) {
                            char buffer[4096];
                            int64_t byteResv = read(client->whichClient[i], buffer, 4096);

                            if(dbg) printf("byteResv %lld | %s \n",byteResv, buffer);



                            if (byteResv <= 0) {
                                   std::cout << RED << "client " << client->whichClient[i] << " disconnected\n"
                                             << RESET;
                                   client->finalExit = false;
                                   /// shuting down the cline fd and read write
                                   close(client->whichClient[i]);
                                   shutdown(client->whichClient[i], SHUT_RDWR);
                                   /// marking it to -1 for not sending broadcast messaage to already disconnected client
                                   client->whichClient[i] = -1;
                                   continue;
                            } else if (byteResv > 1) {
                                   saveChat(buffer);
                                   client->tempfd = client->whichClient[i];
                                   strcpy(client->MsgResv, buffer);
                                   messageBroadcast(client);

                                   // for receiving  the chat message form connected client
                                   if (buffer[1] == 'c') {
                                          if (client->ChatMsgStatus) {
                                                 for (int32_t j = 2; buffer[j]; ++j) {
                                                        std::cout << buffer[j];
                                                 }
                                          }
                                          memset(buffer, '\0', 4096);
                                          continue;
                                   }

                                   /// for creating the set on server
                                   if (buffer[1] == 'a') {
                                          /// create set;
                                          std::cout << RED << "\nATTENTION! " << RESET << "some one Added Set : " << buffer[0] - 48 << "\n";
                                          /// as it recive char 1 so we have to convert it intiger one by subtracting the 48
                                          creatSet(buffer[0] - 48);
                                          memset(buffer, '\0', 4096);
                                          continue;
                                   }

                                   // if index encounter b then some  client have sended message to crate sub set;
                                   if (buffer[1] == 'b') {
                                          char arr[SIZE];
                                          for (int32_t j = 0; buffer[j]; ++j) {
                                                 arr[j] = buffer[j + 2];
                                          }
                                          std::cout << RED << "\nATTENTION! " << RESET << "New Answer added in set : " << buffer[0] - 48 << "\n";
                                          //                                          printf("attention !answer in subset %d\n", buffer[0] - 48);
                                          createSubSet(buffer[0] - 48, arr);
                                          memset(buffer, '\0', SIZE);
                                          continue;
                                   }

                                   memset(buffer, '\0', 4096);
                            }
                     }
              }
       }
}

/**
 * @brief Broadcasts a message to all connected clients except the sender.
 *
 * This function sends a received message (`client->MsgResv`) to all connected clients
 * except the sender identified by `client->tempfd`. It iterates through the list of
 * client socket file descriptors and sends the message using the `send` function.
 * After broadcasting, it clears the message buffer (`client->MsgResv`) to prepare for
 * the next message.
 *
 * @param client A pointer to the structure holding client details.
 */

void messageBroadcast(CliDetails client) {
       for (int32_t i = 0; i < client->NumberOfClient; ++i) {
              if (client->whichClient[i] != client->tempfd and client->whichClient[i] != -1)
                     send(client->whichClient[i], client->MsgResv, strlen(client->MsgResv), 0);
       }
       memset(client->MsgResv, '\0', 4096);
}

/***=============== save option ====================***/

void option(CliDetails client) {
       while (client->finalExit) {
              int32_t input;
              /* 1. CHAT
           * 2. Show set/Answer
           * 3. Create set
           * 4. Put answer in set
           * 5.EXIT
           */
              std::cout << "\n----------Select Option---------\n";
              std::cout << "1. CHAT\n";
              std::cout << "2. Show set/Answer\n";
              std::cout << "3. Create set\n";
              std::cout << "4. Put answer in set\n";
              std::cout << "5.EXIT \n";
              std::cin >> input;
              if (std::cin.fail()) {
                     std::cin.clear();
                     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                     system("clear");
                     std::cout << RED << "Invalid input. Please enter an integer: " << RESET;
                     continue;          // Repeat the loop for a new input attempt
              }
              switch (input) {
                     case 1:
                            client->ChatMsgStatus = true;
                            system("clear");
                            std::cout << BLUE << "entering into chat mode\n"
                                      << RESET;
                            printChat(client);
                            msgSend(client);
                            client->ChatMsgStatus = false;
                            system("clear");
                            break;
                     case 2:
                            client->ChatMsgStatus = false;
                            case2_ShowAnswer(client);
                            break;
                     case 3:
                            client->ChatMsgStatus = false;
                            case3_CreateSet(client);
                            break;
                     case 4:
                            client->ChatMsgStatus = false;
                            case5_CreateSubset(client);
                            break;
                     case 5:
                            client->finalExit = false;
                            system("clear");
//                            printf("shutting down server and cient \n");
                            std::cout<<RED<<"shutting down server and cient \n"<<RESET;
                            char exitNote[5];
                            memset(exitNote, '\0', sizeof exitNote);
                            exitNote[0] = '1';
                            exitNote[1] = 'e';
                            exitNote[2] = '\0';
                            for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                                   if (client->whichClient[i] != -1) {
                                          send(client->whichClient[i], exitNote, strlen(exitNote), 0);
                                   }
                            }
                            client->finalExit = false;

                            close(client->serverSocketFileDiscription);
                            shutdown(client->serverSocketFileDiscription, SHUT_RDWR);
                            for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                                   close(client->whichClient[i]);
                                   shutdown(client->whichClient[i], SHUT_RDWR);
                            }
                            //                            free(chatHead);   // there are  much more memory alloted from heap but not freeing
                            //                            free(chatTail);   // there are  much more memory alloted from heap but not freeing
                            //                            free(head);       // there are  much more memory alloted from heap but not freeing
                            //                            free(tail);       // there are  much more memory alloted from heap but not freeing
                            MemoryManagement();
                            ChatMemoryManagement();
                            exit(EXIT_SUCCESS);
                     default:
                            system("clear");
                            std::cout << RED << "INVALID INPUT\n"
                                      << RESET;
                            continue;
              }
       }
}
/*
 * memeory management after usning calloc and malloc fxn for array
 * function missing // coz it have lot of mess to do
 *
 */
/*** =============================================== ***/
// case 1 chat
void saveChat(char msg[SIZE]) {
       Chat *newChat = new Chat(msg);
       if (chatHead == nullptr) {
              chatHead = newChat;
              chatTail = newChat;
       } else {
              chatTail->next = newChat;
              chatTail = newChat;
       }
}
void printChat(CliDetails client) {
       Chat *temp = chatHead;

       while (temp) {
              /* this is for broadcasting previous message to newly connected client */
              if (client->broadCastingPrevMsgToClient) {
                     std::this_thread::sleep_for(std::chrono::milliseconds(250));
                     send(client->whichClient[client->NumberOfClient - 1], temp->msg, strlen(temp->msg), 0);
                     temp = temp->next;
                     continue;
              }
              /* printing chat if they start with 1c */
              if (temp->msg[1] == 'c') {
                     for (int32_t i = 2; temp->msg[i]; ++i) {
                            std::cout << temp->msg[i];
                     }
              }
              temp = temp->next;
       }

       if (client->broadCastingPrevMsgToClient){
              char msg[]={'1','Z','\0'};
              std::this_thread::sleep_for(std::chrono::seconds(1));
              send(client->whichClient[client->NumberOfClient-1], msg, strlen(msg),0);
       }
}

// case 2 show set and answer

void creatSet(int32_t setNumber) {
       node *temp = new node(setNumber);
       if (!head) {
              head = tail = temp;
       } else {
              tail->next = temp;
              tail = temp;
       }
}

void createSubSet(int32_t setNumber, char data[SIZE]) {
       node *horizontal = head;

       while (horizontal and (horizontal->setNo != setNumber)) {
              horizontal = horizontal->next;
       }          // horizontal triversal to that node
       auto *vertical = new subNode(data);
       if (horizontal->down == nullptr) {          // pushing element to first;
              horizontal->down = vertical;

       } else {
              vertical->down = horizontal->down;
              horizontal->down = vertical;
       }
}

// case 2
void printSubSet(int32_t setNumber) {
       node *temp = head;
       while (temp and temp->setNo != setNumber) {
              temp = temp->next;
       }
       system("clear");
       std::cout << "-------printing setNumber-------  " << setNumber << " :- \n";
       subNode *run = temp->down;
       while (run) {
              std::cout << run->answer;
              run = run->down;
       }
}

bool printSuperSet(bool check, int32_t setnumber) {
       node *temp = head;
       if (!check) {
              std::cout << "-----------printing set---------\n";
       }
       while (temp) {
              if (check) {
                     if (temp->setNo == setnumber) {
                            return true;
                     }
              } else {
                     std::cout << "Set Number: " <<(int)temp->setNo<< "\n";
              }
              temp = temp->next;
       }
       if (!check) {
              std::cout << "---------------------------------------------\n";
       }

       return false;
}
/* ----- ================================------*/

/// case 3

void case3_CreateSet(CliDetails client) {
       system("clear");
       printSuperSet(false, 0);
       int32_t setNumber;
       while (true) {
              std::cout << "enter set number: or -1 for exit: ";
              std::cin >> setNumber;
              if (std::cin.fail()) {
                     std::cin.clear();
                     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                     system("clear");
                     std::cout << RED << "Invalid input. Please enter an integer: \n"
                               << RESET;
                     return;
              }
              if (setNumber == -1) {
                     return;
              }
              if (setNumber > 9 or setNumber < 0) {
                     system("clear");
                     std::cout << RED << setNumber << " is not allowed to create maximum allowed set is between 1 to 9 \n"
                               << RESET;
                     continue;
              }
              if (printSuperSet(true, setNumber)) {
                     std::cout << RED << "Set Already there\n"
                               << RESET;
                     continue;
              } else {

                     creatSet(setNumber);
                     char arr[20];
                     memset(arr, '\0', sizeof arr);
                     arr[0] = (char) (48 + setNumber);
                     arr[1] = 'a';
                     arr[2] = '\0';
                     saveChat(arr);
                     for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                            send(client->whichClient[i], arr, strlen(arr), 0);
                     }
              }
       }
}

void case5_CreateSubset(CliDetails client) {
       system("clear");
       printSuperSet(false, 0);
       std::cout << "which set: or -1 for exit: ";
       int32_t setNumbet;
       std::cin >> setNumbet;
       if (std::cin.fail()) {
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cout << RED << "invalid input. plz enter a integer \n"
                        << RESET;
              return;
       }
       if (setNumbet == -1) {
              return;
       }
       if (printSuperSet(true, setNumbet)) {
              char Data[SIZE];
              char line[SIZE];
              uint64_t i = 0;
              std::cout << BLUE << "Copy Paste you Answer ' " << setNumbet << " ' -1 for exit \n"
                        << RESET;
              while (fgets(line, 4096, stdin)) {
                     if (strcmp(line, "-1\n") == 0) {
                            break;
                     } else {
                            strcpy(Data + i, line);
                            i += strlen(line);
                            if (i == SIZE) {
                                   std::cout << RED << "size limit reached\n"
                                             << RESET;
                                   break;
                            }
                     }
              }
              createSubSet(setNumbet, Data);
              char mesg[SIZE];
              mesg[0] = (char) (48 + setNumbet);
              mesg[1] = 'b';
              for (int32_t j = 0; Data[j] and j < SIZE - 2; ++j) {
                     mesg[j + 2] = Data[j];
              }
              saveChat(mesg);
              for (int32_t j = 0; j < client->NumberOfClient; ++j) {
                     if (client->whichClient[j] != -1) {
                            send(client->whichClient[j], mesg, strlen(mesg), 0);
                     }
              }
              memset(Data, '\0', SIZE);                  // o(n)
              memset(mesg, '\0', strlen(mesg));          // O(n)
       }
}

// show answer to user
void case2_ShowAnswer(CliDetails client) {
       system("clear");
       int32_t setNumber;
       printSuperSet(false, 0);          // call set print32_t function
       std::cout << "Enter set number: or -1 for back:  ";
       std::cin >> setNumber;
       if (std::cin.fail()) {
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cout << "Invalid input. Please enter an integer: ";
              return;
       }
       if (setNumber == -1) {
              return;
       }
       if (setNumber > 9 or 0 > setNumber) {
              system("clear");
              std::cout << RED << " WARNING set number only allowed between 0 to 9 \n"
                        << RESET;
              return;
       }

       if (!printSuperSet(true, setNumber)) {
              std::cout << RED << "Set not exist\n"
                        << RESET;
              std::cout << "do you want to create set? (y/n) : ";
              char ch;
              std::cin >> ch;
              if (std::cin.fail()) {
                     std::cin.clear();
                     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                     std::cout << RED << "Invalid input. Please enter an character: " << RESET;
                     return;
              }
              if (ch == 'y' or ch == 'Y') {
                     char toSend[20];
                     memset(toSend, '\0', sizeof toSend);
                     toSend[1] = 'a';
                     toSend[0] = (char) (48 + setNumber);
                     toSend[2] = '\0';
                     saveChat(toSend);
                     for (int32_t i = 0; i < client->NumberOfClient; ++i) {
                            if (client->whichClient[i] != -1) {
                                   send(client->whichClient[i], toSend, strlen(toSend), 0);
                            }
                     }
                     creatSet(setNumber);
              }
              if (ch == 'n' or ch == 'N')
                     return;
       } else {
              printSubSet(setNumber);
       }
}
/*
 * answe delete fxn
 */

void MemoryManagement() {
       while (head) {
              node *Setnext = head->next;
              subNode *currentSubnode = head->down;
              /* deleting subnode*/
              while (currentSubnode) {
                     subNode *SubNodedown = currentSubnode->down;
                     delete currentSubnode;
                     currentSubnode = SubNodedown;
              }
              /* deleting node*/
              delete head;
              head = Setnext;
       }
}
/*
 * chat delete
 *
 */

void ChatMemoryManagement() {
       while (chatHead) {
              Chat *ChatNext = chatHead->next;
              /* deleting chat node*/
              delete chatHead;
              chatHead = ChatNext;
       }
}

#endif /* iostreamS_h */

