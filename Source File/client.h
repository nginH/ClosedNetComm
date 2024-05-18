
     // client.h

#ifndef client_h
#define client_h
/**********************************************************************
 * @project : CloseNetComm -- client header

       "┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n"\
       "│  ██████╗██╗      ██████╗ ███████╗███████╗██████╗ ███╗   ██╗███████╗████████╗ ██████╗ ██████╗ ███╗   ███╗███╗   ███╗ │\n"\
       "│ ██╔════╝██║     ██╔═══██╗██╔════╝██╔════╝██╔══██╗████╗  ██║██╔════╝╚══██╔══╝██╔════╝██╔═══██╗████╗ ████║████╗ ████║ │\n"\
       "│ ██║     ██║     ██║   ██║███████╗█████╗  ██║  ██║██╔██╗ ██║█████╗     ██║   ██║     ██║   ██║██╔████╔██║██╔████╔██║ │\n"\
       "│ ██║     ██║     ██║   ██║╚════██║██╔══╝  ██║  ██║██║╚██╗██║██╔══╝     ██║   ██║     ██║   ██║██║╚██╔╝██║██║╚██╔╝██║ │\n"\
       "│ ╚██████╗███████╗╚██████╔╝███████║███████╗██████╔╝██║ ╚████║███████╗   ██║   ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║ ╚═╝ ██║ │\n"\
       "│  ╚═════╝╚══════╝ ╚═════╝ ╚══════╝╚══════╝╚═════╝ ╚═╝  ╚═══╝╚══════╝   ╚═╝    ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚═╝ │\n"\
       "└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n"



     * Permission is hereby granted, free of charge, to any person obtaining a copy
     * of this software and associated documentation files (the "Software"), to deal
     * in the Software without restriction, including without limitation the rights
     * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
     * copies of the Software, and to permit persons to whom the Software is
     * furnished to do so, subject to the following conditions:
     *
     * The above copyright notice and this permission notice shall be included in all
     * copies or substantial portions of the Software.
     *
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
     * SOFTWARE.
     *
     *
     **********************************************************************/


#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#ifdef __MACH__

#include <arpa/inet.h>
#include <netinet/in.h>
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

#define auth_createSet "__creteSet__@123"
#define auth_createSetComplete "__createSetComplete__@123"
#define auth_putAnswer "__putAnswer__@123"
#define auth_putAnswerComplete "__putAnswerComplete__@123"


#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define IPurple "\033[0;95m"
// const int port = 6969;
     const int SIZE = 8192;
// const char *ip= "127.0.0.1";

// storing the chat
     struct Chat {
           char *msg;
           Chat *next;
           explicit Chat(char msg[SIZE]) {
                 this->msg = (char *)calloc(SIZE, sizeof(char));
                 strcpy(this->msg, msg);
                 this->next = nullptr;
           }
     } *chatHead = nullptr, *chatTail = nullptr;

// storing the set and answer
     struct subNode {
           char *answer;
           subNode *down;
           explicit subNode(char ans[SIZE]) {
                 this->answer = (char *)calloc(SIZE, sizeof(char));
                 strcpy(this->answer, ans);
                 this->down = nullptr;
           }
     };

// storing the set
     struct node {
           int setNo;
           node *next;
           subNode *down;
           explicit node(int set) {
                 this->setNo = set;
                 this->next = nullptr;
                 this->down = nullptr;
           }
     } *head = nullptr, *tail = nullptr;

     struct details {
           int clientSocketfileDiscription;
           bool ChatMsgStatus;
           char Msg[8192];
           char name[20];

           bool createSetCase3to2;
           bool authDone;
           std::vector<std::string> userConncted;
           size_t namelen;
           details() {
                 this->clientSocketfileDiscription = -1;
                 this->namelen = 0;
                 this->authDone = false;
                 this->ChatMsgStatus = false;
                 this->createSetCase3to2 = false;
                 memset(Msg, '\0', 8192);
                 memset(name, '\0', 20);
           }
     };

     typedef struct details *ClientDetails;

     void recvfxn(ClientDetails);
     void sendFxn(ClientDetails);
     void saveChat(char msg[SIZE]);
     void printChat();
// network related function
     void option(ClientDetails);
     void msgSend(ClientDetails);
     void case3_CreateSet(ClientDetails);
     void case2_ShowAnswer(ClientDetails);
     void case5_CreateSubset(ClientDetails);
// saved related function
     void creatSet(int setNumber);
     void printSubSet(int setNumber);
     bool printSuperSet(bool check, int setNumber);
     void createSubSet(int setNumber, char data[SIZE]);
     void MemoryManagement(void );
     void ChatMemoryManagement(void );
     void displayClear(void );
     void __attribute__((destructor)) MemoryManagement(void );
     void __attribute__((destructor)) ChatMemoryManagement(void);
     void __attribute__ ((destructor)) displayClear(){

           std::this_thread::sleep_for(std::chrono::seconds(3));
           system("clear");
           std::cout<<"Hello world\n";
     }
// driver code

     int32_t client(char *ip, int port) {
           system("clear");
           details *client = new details();
           client->clientSocketfileDiscription = socket(AF_INET, SOCK_STREAM, 0);
           if (client->clientSocketfileDiscription < 0) {
                 perror("socket creation faild\n");
                 exit(1);
           }

           struct sockaddr_in address;
           address.sin_port = htons(port);
           address.sin_family = AF_INET;
           inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

           int connectResult = connect(client->clientSocketfileDiscription,
                                       (struct sockaddr *)&address, sizeof address);
           if (connectResult == 0) {
                 printf("connection successful\n");
           } else {
                 printf("%d\n", errno);
                 perror("connection field Server not yet UP \n");
                 exit(2);
           }
           std::cout << GREEN << "GREETINGS!\n";


           std::thread sendthread(&sendFxn, client);
           std::thread recvthread(&recvfxn, client);


           sendthread.join();
           recvthread.join();
           close(client->clientSocketfileDiscription);
           shutdown(client->clientSocketfileDiscription, SHUT_RDWR);
           return 0;
     }

     void recvfxn(ClientDetails client) {
           char reciveBUFFER[8192];
           int64_t recvByte;

           while (true) {
                 recvByte = recv(client->clientSocketfileDiscription, reciveBUFFER, 1023, 0);
                 if (recvByte < 0) {
                       std::cout << "some error occurred\n";
                       return;
                 }
                 else if (recvByte == 0)
                       continue;
                 else {
                       saveChat(reciveBUFFER);
                       if (reciveBUFFER[0] == '1' and reciveBUFFER[1] == 'c') {
                             if (client->ChatMsgStatus) {
                                   for (int j = 2; reciveBUFFER[j]; ++j) {
                                         std::cout << reciveBUFFER[j];
                                   }
                             }
                             memset(reciveBUFFER, '\0', 8192);
                             continue;
                       }

                       if (reciveBUFFER[0]=='1' and reciveBUFFER[1]=='X'){
                             if(client->authDone) std::cout<<YELLOW<<"new join : "<<RESET<<GREEN<<reciveBUFFER+2<<RESET;
                             client->userConncted.push_back(reciveBUFFER+2);
                       }
                       if (reciveBUFFER[0]=='1' and reciveBUFFER[1]=='Y'){
                             if(client->authDone) std::cout<<RED<<"exit the chat: "<<GREEN<<reciveBUFFER+2<<RESET;
                             client->userConncted.erase(std::remove(client->userConncted.begin(),client->userConncted.end(),reciveBUFFER+2),client->userConncted.end());
                       }





                       if (reciveBUFFER[1] == 'Z') {
                             system("clear");
                             std::cout << IPurple << "Message copying completed\n" << RESET;
                             client->authDone=true;

                             /* after copying message from server to client
                              * create thread for option and detach it from main thread
                              */
                             std::thread optionThread(&option, client);
                             optionThread.detach();

                       }
                       /*
                        * \new set added to set (magic keyword 'a')
                        */
                       if (reciveBUFFER[1] == 'a') {
                             // create set;
                             std::cout << RED << "ATTENTION! " << RESET
                                       << "some one Added Set : " << reciveBUFFER[0] - 48 << "\n";
                             creatSet(reciveBUFFER[0] - 48);
                             memset(reciveBUFFER, '\0', strlen(reciveBUFFER));
                             continue;
                       }
                       /*
                        * new anser added to set recineBUFFER[0]-48
                        */
                       if (reciveBUFFER[1] == 'b') {
                             char arr[SIZE];
                             std::cout << RED << "ATTENTION! " << RESET
                                       << "New Answer added in set : " << reciveBUFFER[0] - 48
                                       << "\n";
                             for (int j = 0; reciveBUFFER[j]; ++j) {
                                   arr[j] = reciveBUFFER[j + 2];
                             }
                             createSubSet(reciveBUFFER[0] - 48, arr);
                             memset(reciveBUFFER, '\0', SIZE);
                             continue;
                       }
                       // responce to server exiting
                       if (reciveBUFFER[1] == 'e') {
                             std::cout << RED << " SERVER REQUESTED TO SHUT DOWN THIS PROGRAM\n"
                                       << RESET;
                             close(client->clientSocketfileDiscription);
                             shutdown(client->clientSocketfileDiscription, SHUT_RDWR);
                             exit(EXIT_SUCCESS);
                       }
                       memset(reciveBUFFER, '\0', 8192);
                 }
           }
     }

     void sendFxn(ClientDetails client) {
           char sendmsg[8192];
           while (client->ChatMsgStatus) {
                 memset(sendmsg, '\0', 8192);
                 fgets(sendmsg, 8192, stdin);
                 if (strcmp(sendmsg, "-1\n") == 0) {
                       return;
                 }
                 saveChat(sendmsg);     // storing msg for future load
                 client->Msg[0] = '1'; //
                 client->Msg[1] = 'c'; // magic number for chat
                 strcpy(client->Msg+2, client->name);

                 strcpy(client->Msg + (strlen(client->name)+2), sendmsg);
/*
 * msg wil send after recv
 * client->Msg= 1c+name+" : "+sendmsg
 */
                 if (strlen(sendmsg) > 1) {
                       if (send(client->clientSocketfileDiscription, client->Msg,
                                strlen(client->Msg), 0) == -1) {
                             std::cout << RED << "Server is not responding\n" << RESET;
                             exit(EXIT_FAILURE);
                       }
                 }
                 memset(client->Msg, '\0', 8192);
                 /*
                  * buffer freeing
                  */
           }
     }

     void option(ClientDetails client) {
           if(client->userConncted.size()>0)std::cout<<"list of connected user: \n";
           for (auto name:client->userConncted) {
                 std::this_thread::sleep_for(std::chrono::milliseconds(100));
                 std::cout<<name;
           }
           char name[20];
           name[0]='1';
           name[1]='X';
           bool ck= false;
           while (true){
                 std::cout << "Enter your name:  ";
                 fgets(name+2,18, stdin);
                 if(strcmp(name+2,"\n")==0) {
                       std::cout << RED << "Msg can't be empty\n" << RESET;
                       continue;
                 }
                 for(auto prevName:client->userConncted){
                       if(strcmp(prevName.c_str(),name+2)==0){
                             std::cout<<RED<<"Msg already exist\n"<<RESET;
                             ck=true;
                             break;
                       }
                 }
                 if (ck){
                       ck=false;
                       continue;
                 }else{
                       break;
                 }

           }
           client->authDone= true;
           strcpy(client->name,name+2);
           // removing new line from name
           int i;
           for (i = 0; client->name[i]; ++i) {
                 if (client->name[i] == '\n') {
                       client->name[i] = '\0';
                       break;
                 }
           }
           client->name[i++] = ':';
           client->name[i++] = ' ';
           client->name[i++] = '\0';

           std::cout<<GREEN<<"Welcome "<<name+2<<RESET;
           //telling server that @client joined the server
           send(client->clientSocketfileDiscription, name, strlen(name), 0);

           while (true) {
                 int input;
                 std::cout << "----------Select Option---------\n";
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
                       std::cout << RED << "Invalid input. Please enter an integer: \n" << RESET;
                       continue; // Repeat the loop for a new input attempt
                 }
                 switch (input) {
                       case 1:
                             client->ChatMsgStatus = true;
                             system("clear");
                             std::cout << BLUE << "Enter msg or -1 for back\n" << RESET;
                             printChat();
                             sendFxn(client);
                             client->ChatMsgStatus = false;
                             system("clear");
                             break;
                       case 2:
                             client->ChatMsgStatus = false;
                             client->createSetCase3to2 = false;
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
                             close(client->clientSocketfileDiscription);
                             shutdown(client->clientSocketfileDiscription, SHUT_RDWR);
                             std::cout<<"exiting\n";
                             exit(3);
                       default:
                             system("clear");
                             std::cout << RED << "INVALID INPUT\n" << RESET;
                             continue;
                 }
           }

     }

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
     void printChat() {
           Chat *temp = chatHead;
           while (temp) {
                 if (temp->msg[1] == 'c') {
                       for (int i = 2; temp->msg[i]; ++i) {
                             std::cout << temp->msg[i];
                       }
                 }
                 temp = temp->next;
           }
     }

// case 2 show set and answer

     void creatSet(int setNumber) {
           node *temp = new node(setNumber);
           if (!head) {
                 head = tail = temp;
           } else {
                 tail->next = temp;
                 tail = temp;
           }
     }

     void createSubSet(int setNumber, char data[SIZE]) {
           node *horizontal = head;

           while (horizontal and (horizontal->setNo != setNumber)) {
                 horizontal = horizontal->next;
           } // horizontal triversal to that node

           subNode *vertical = new subNode(data);
           if (horizontal->down == nullptr) { // pushing element to first;
                 horizontal->down = vertical;

           } else {
                 vertical->down = horizontal->down;
                 horizontal->down = vertical;
           }
     }

// case 2
     void printSubSet(int setNumber) {
           node *temp = head;
           while (temp and temp->setNo != setNumber) {
                 temp = temp->next;
           }
           system("clear");
           std::cout << "list of answer is setNumber"<<RED << setNumber
                     <<RESET << " :- \n";
           subNode *run = temp->down;
           while (run) {
                 std::cout << run->answer;
                 run = run->down;
           }
           return;
     }

     bool printSuperSet(bool check, int setnumber) {
           node *temp = head;
           while (temp) {
                 if (check) {
                       if (temp->setNo == setnumber) {
                             return true;
                       }
                 } else {
                       std::cout << "Set Number: " << temp->setNo << "\n";
                 }
                 temp = temp->next;
           }
           return false;
     }
/* ----- ================================------*/

/// case 3

     void case3_CreateSet(ClientDetails client) {
           system("clear");
           int setNumber;
           while (true) {
                 system("clear");
                 printSuperSet(false, 0);
                 std::cout << "enter set number: or -1 for exit: ";
                 std::cin >> setNumber;
                 if (std::cin.fail()) {
                       std::cin.clear();
                       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                       system("clear");
                       std::cout << RED << "Invalid input. Please enter an integer: \n" << RESET;
                       return;
                 }
                 if (setNumber == -1) {
                       return;
                 }
                 if (setNumber > 9 or setNumber < 0) {
                       system("clear");
                       std::cout << RED << " WARNING set number only allowed between 0 to 9  \n"
                                 << RESET;
                       return;
                 }
                 if (printSuperSet(true, setNumber)) {
                       std::cout << RED << "Set Already there\n" << RESET;
                       std::this_thread::sleep_for(std::chrono::seconds(2));
                       continue;
                 } else {
                       creatSet(setNumber);
                       char arr[20];
                       arr[0] = setNumber + 48;
                       arr[1] = 'a';
                       arr[2] = '\0';
                       send(client->clientSocketfileDiscription, arr, strlen(arr), 0);
                       memset(arr, '\0', strlen(arr));
                 }
           }
     }
// creating subSet for putting answer in set;

     void case5_CreateSubset(ClientDetails client) {
           system("clear");
           printSuperSet(false, 0);
           std::cout << "which set: or -1 for exit ";
           int setNumbet;
           std::cin >> setNumbet;
           if (std::cin.fail()) {
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 std::cout << RED << "invalid input. plz enter a intger \n" << RESET;
                 return;
           }
           if (setNumbet == -1) {
                 return;
           }
           if (printSuperSet(true, setNumbet)) {
                 char Data[SIZE];
                 char line[250];
                 int i = 0;
                 std::cout << BLUE << "Copy Paste you Answer ' " << setNumbet
                           << " ' -1 for exit \n"
                           << RESET;
                 while (fgets(line, 8192, stdin)) {
                       if (strcmp(line, "-1\n") == 0) {
                             break;
                       } else {
                             strcpy(Data + i, line);
                             i += strlen(line);
                             if (i == SIZE) {
                                   std::cout << RED << "size limit reached\n" << RESET;
                                   break;
                             }
                       }
                       // sending data to server
                 }

                 // sending sent number and data associated with it
                 char mesg[SIZE];
                 mesg[0] = 48 + setNumbet;
                 mesg[1] = 'b';
                 for (int j = 0; j < strlen(Data) and j < SIZE - 2; ++j) {
                       mesg[j + 2] = Data[j];
                 }
                 send(client->clientSocketfileDiscription, mesg, strlen(mesg), 0);
                 createSubSet(setNumbet, Data);
                 memset(mesg, '\0', strlen(mesg));
           }
     }

// show answer to user
     void case2_ShowAnswer(ClientDetails client) {
           system("clear");
           int setNumber;
           printSuperSet(false, 0); // call set print function
           std::cout << "Enter set number or -1 for exit: ";
           std::cin >> setNumber;
           if (std::cin.fail()) {
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 std::cout << RED << "Invalid input. Please enter an integer: \n" << RESET;
                 return;
           }
           if (setNumber == -1) {
                 return;
           }

           if (setNumber > 9 or setNumber < 0) {
                 system("clear");
                 std::cout << RED << "WARNING set number only allowed between 0 to 9\n"
                           << RESET;
                 return;
           }

           if (!printSuperSet(true, setNumber)) {
                 std::cout << RED << "Set not exist\n" << RESET;
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
                       client->createSetCase3to2 = setNumber;
                       char toSend[20];
                       memset(toSend, '\0', sizeof toSend);
                       toSend[1] = 'a';
                       toSend[0] = (char)(48 + setNumber);
                       toSend[2] = '\0';

                       send(client->clientSocketfileDiscription, toSend, strlen(toSend), 0);
                       creatSet(setNumber);
                 }
                 if (ch == 'n' or ch == 'N')
                       return;
           } else {
                 printSubSet(setNumber);
           }
           return;
     }

// linked list sync function for answer
/*
 *
 */
/* for set and answer*/

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

// chat delete
     void ChatMemoryManagement() {
           while (chatHead) {
                 Chat *ChatNext = chatHead->next;
                 /* deleting chat node*/
                 delete chatHead;
                 chatHead = ChatNext;
           }
     }


#endif /* client_h */
