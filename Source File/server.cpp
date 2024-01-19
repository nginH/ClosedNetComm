/* MIT License

Copyright (c) 2024-2029 Harsh Anand

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

#include <chrono>
#include <cstring>
#include <iostream>
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

const uint32_t port = 6969;
const uint32_t backlog = 10;
const uint32_t SIZE = 1024;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
//
//#define auth_createSet "__creteSet__69k"
//#define auth_createSetComplete "__createSetComplete__69k"
//#define auth_putAnswer "__putAnswer__69k"
//#define auth_putAnswerComplete "__putAnswerComplete__69k"

// client details structure

/*******--------- Container Construction----------****/
//Encapsulates information about a connected client within a server application.
/*
 * Members:
        * ClientSocketFileDiscription: (int32_t) File descriptor for the client's socket.
        * serverSocketFileDiscription: (int32_t) File descriptor for the server's socket (for server-side functions).
        * whichClient[30]: (int32_t array) Stores file descriptors of up to 30 connected clients, used for broadcasting messages.
        * NumberOfClient: (uint64_t) Number of currently connected clients.
        * finalExit: (bool) Flag indicating whether the server should shut down.
        * broadCastingPrevMsgToClient: (bool) Flag controlling whether to broadcast previous chat messages to a newly connected client.
        * ChatMsgStatus: (bool) Indicates whether chat mode is active.
        * msgSend[1024]: (char array) Buffer for storing outgoing messages.
        * MsgResv[2048]: (char array) Buffer for storing received messages.
        * tempfd: (int32_t) Temporary file descriptor used for broadcasting operations.
*/
struct ClientDetails {
       int32_t ClientSocketFileDiscription;
       int32_t serverSocketFileDiscription;
       int32_t whichClient[30]{};
       uint64_t NumberOfClient;
       bool finalExit;
       bool broadCastingPrevMsgToClient;
       bool ChatMsgStatus;
       char msgSend[1024]{};
       char MsgResv[2048]{};
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

struct Chat {
       char *msg;
       Chat *next;
       explicit Chat(char msg[SIZE]) {
              this->msg = (char *) calloc(SIZE, sizeof(char));
              strcpy(this->msg, msg);
              this->next = nullptr;
       }
} *chatHead = nullptr, *chatTail = nullptr;
//____-------_______------__------__----_--------_--_-----_______------__---_---_----_----_-_-_----_-_-_--//

/*-----------------------function prototype Declaration-------------------------------*/
// chat save function

// for saving all kind of message recv from chat and client
void saveChat(char msg[SIZE]);

/*
 * Purpose:
 * *******Prints chat messages to the console and handles broadcasting to newly connected clients.
 * Parameters:
 * *******CliDetails client: A structure containing client-related information, including:
 * ------------broadCastingPrevMsgToClient: Flag indicating whether to broadcast previous messages to a new client.
 * ------------NumberOfClient: Number of connected clients.
 * ------------whichClient: Array storing file descriptors for connected clients.
 * Functionality:
 * ****** Iterates through the chat history:
 * ------------Uses a temp pointer to traverse a linked list of Chat structures, presumably containing chat messages.
 * ****** Handles broadcasting for new clients:
 * ------------If client->broadCastingPrevMsgToClient is true:
 * ------------------- Pauses briefly using std::this_thread::sleep_for.
 * ------------------- Sends the current message to the most recently connected client using the send function.
 * ------------------- Moves to the next message in the list.
 * ****** Prints chat messages:
 * ------------If the current message is a chat message (identified by "1c" at the beginning):
 * --------------------Prints the message to the console starting from the third character (skipping the identifier).
 * ****** Advances to the next message:
 * ------------Moves the temp pointer to continue iterating through the chat history.
 *
 */
void printChat(CliDetails);

/***=============== save option ====================***/
/*
 * this fxn present menu of option to user and process based on the selected optins. it operates in a loop ,
 * it operate on loop allowing user to intrect with different feature of chat applicaton. the option include like chatting etc
 * PARAMETER
 * 'client' type( cliDetaild ) a pointer to structure that hold various detail about the client , including socket information
 * memory - related information;
 *
 * !MENU PRESENTED
 * displays a menu with numberes options to the user to choose from,
 * option including
 *   1: Enter chat mode
 *   2: Show sets/answers.
 *   3: Create a new set.
 *   4: Put an answer in a set.
 *   5: Exit the application.
 *Option Processing:
 *Uses a switch-case statement to execute different actions based on the user's choice.
 * Case 1: Enters chat mode, prints the chat history, and allows the user to send messages.
 * Case 2: Shows answers and sets based on user input.
 * Case 3: Creates a new set.
 * Case 4: Puts an answer in a set.
 * Case 5: Shuts down the application, closing sockets, freeing memory, and send exit message to all
 * the connected client for clearing the terminal;
 *The function operates in an infinite loop (while(true)) until the user chooses to exit the application (Case 5).
 */

void option(CliDetails);

/*The msgSend function is responsible for handling user input and broadcasting messages
 * Purpose:
 * Facilitates message sending to connected clients within a chat application.
 * Handles chat functionality, exit commands, and message broadcasting.
 *
 * Parameters:
 *      CliDetails client: A structure containing client-related information, including:
 *      ChatMsgStatus: Indicates whether chat mode is active.
 *      finalExit: Flag for server shutdown.
 *      serverSocketFileDiscription: File descriptor for the server socket.
 *      NumberOfClient: Number of connected clients.
 *      whichClient: Array storing file descriptors for connected clients.
 *      msgSend: Buffer for storing the outgoing message.
 *
 * Functionality:
 * Enters a loop while chat mode is active:
 *      *Reads a message from standard input using fgets.
 *      *Processes the message based on its content:
 *         *"-1": Clears the screen and returns.
 *         *"exit" and finalExit is true:
 *                *Initiates server shutdown:
 *                *Closes the server socket and connected client sockets.
 *                *Clears the screen and exits the program.
 *         *Other messages:
 *            *Constructs a message to send to clients:
 *                 *Prefixes the message with "1c" (chat identifier).
 *                 *Copies the message to the toSend buffer.
 *                 *Saves the chat using the saveChat function (presumably for logging).
 *                 *Sends the message to all connected clients using the send function.
 *                 *Clears the toSend buffer for the next message.
 */
void msgSend(CliDetails);

/*Purpose:
 *      Continuously receives messages from connected clients.
 *```` Handles chat messages, set creation, answer addition, client disconnections, and message broadcasting.
*Parameters:
*     * CliDetails client: A structure containing client-related information, including:
*            * NumberOfClient: Number of connected clients.
*            * whichClient: Array storing file descriptors for connected clients.
*            * MsgResv: Buffer for storing received messages.
*            * tempfd: Temporary file descriptor for broadcasting.
*            * ChatMsgStatus: Indicates whether chat mode is active.
*Functionality:
*
*Enters an infinite loop:
*      Initializes a file descriptor set for monitoring client sockets.
*      Uses select to wait for activity on any of the client sockets.
*      If select fails, exits with an error.
*Iterates through connected clients:
*     +- If a client socket has activity:
*             +-Reads data from the socket using read.
*             +-If the client has disconnected:
*                   +- Prints a disconnection message.
*                   +- Closes the socket and marks it as disconnected.
*            +- If a message has been received:
*                    +-Saves the chat using the saveChat function.
*                   +- Stores the message and file descriptor for broadcasting.
*                    +- Broadcasts the message to all other clients using the messageBroadcast function (presumably defined elsewhere).
*                    +- Processes the message based on its content:
*                    +-Chat message ("1c"):
*                           +-Prints the message to the console if chat mode is active.
*                    +-Set creation message ("1a"):
*                         +-  Prints a notification message.
*                          +- Calls the creatSet function (presumably for set creation).
*                  +-  Answer addition message ("1b"):
*                          +-  Prints a notification message.
*                           +- Extracts the answer text from the message.
*                            +- Calls the createSubSet function (presumably for adding answers to a set).

 *
 */
void msgResv(CliDetails);

/*
 * Purpose:
 *         Distributes a received message to all connected clients except the sender.
 *  Parameters:
 *      -CliDetails client: A structure containing client-related information, including:
 *           *NumberOfClient: Number of connected clients.
 *           *whichClient: Array storing file descriptors for connected clients.
 *           *MsgResv: Buffer containing the message to be broadcasted.
 *           *tempfd: Temporary file descriptor representing the sender's socket.
 * Functionality:
 *        * Iterates through connected clients:
 *        * For each client file descriptor:
 *              * Checks if the client is not the sender and is still connected.
 *              * If so, sends the message to the client using the send function.
 *        * Clears the message buffer:
 *              * Resets the MsgResv buffer to empty for the next message.
 */
void messageBroadcast(CliDetails);

/*
 * Purpose:
 *     Guides a user through creating a new set with a specific set number within a larger system of sets.
 *     Manages communication with other connected clients regarding set creation.
 *
 *  1. Steps:
 *     Clears the screen and displays existing sets:
 *         Calls system("clear") to clear the terminal.
 *         Calls printSuperSet(false, 0) to display a list of existing sets.
 *  2. Prompts the user for a set number:
 *         * Continuously prompts the user to enter a set number or -1 to exit.
 *         * Validates input to ensure it's an integer within the allowed range (1 to 9).
 *  3. Creates the set if valid and not already existing:
 *         * If the entered set number is valid and not already in use:
 *               *Calls creatSet(setNumber) to create the new set.
 *               *Constructs a message string arr with the set number and a flag (e.g., "1a").
 *               *Calls saveChat(arr) to presumably save the chat history for the set.
 *               *Iterates through connected clients and sends the message string to each using send.
 * 4. Handles invalid input or existing set:
 *         * If the input is invalid or the set already exists, displays appropriate error messages and prompts the user to try again.
 *
 *  5. Exits on request:
 *         *If the user enters -1, the function returns without creating a set.
 */
void case3_CreateSet(CliDetails);

/*
 * Purpose:
 *         * Guides a user through viewing answers within a specific set in a larger system of sets.
 *         * Offers the option to create a set if it doesn't exist.
 *         * Manages communication with other connected clients regarding set creation and answer viewing.
 *  Steps:
 *    1. Clears the screen and displays existing sets:
 *         * Clears the terminal.
 *         * Shows a list of existing sets using printSuperSet(false, 0).
 *
 *    2.Prompts the user for a set number:
 *         * Asks the user to enter a set number to view or -1 to go back.
 *         * Validates input to ensure it's an integer within the allowed range (0 to 9).
 *
 *    3.Handles non-existent set:
 *        * If the set doesn't exist:
 *             * Informs the user.
 *             * Asks if they want to create the set.
 *             * If yes:
 *                   *Constructs a message string toSend with the set number and a flag (e.g., "1a").
 *                   *Calls saveChat(toSend) to presumably save the chat history for the set.
 *                   *Iterates through connected clients and sends the message string to each using send.
 *                   *Calls creatSet(setNumber) to create the new set.
 *
 *    4. Displays answers if the set exists:
 *           * If the set exists, calls printSubSet(setNumber) to display the answers within that set.
 *
 *    5. Exits on request:
 *           * If the user enters -1 or chooses not to create a non-existent set, the function returns without displaying answers.
 */
void case2_ShowAnswer(CliDetails);

/*
 * Purpose:
 *      * Guides a user through creating a new subset (answer) within an existing set.
 *      * Manages communication with other connected clients regarding subset creation.
 *
 *  Steps:
 *  1. Clears the screen and displays existing sets:
 *        * Clears the terminal.
 *        * Shows a list of existing sets using printSuperSet(false, 0).
 *  2. Prompts the user for a set number:
 *        * Asks the user to enter the set number where they want to create a subset or -1 to exit.
 *        * Validates input to ensure it's an integer within the allowed range.
 *
 *  3. Handles non-existent set:
 *       * If the entered set doesn't exist, returns without creating a subset.
 *
 *  4. Collects subset data:
 *       * Prompts the user to copy and paste the answer content for the new subset.
 *       * Reads the input line by line using fgets until the user enters "-1" or the maximum size is reached.
 *       * Stores the collected data in the Data array.
 *  5. Creates the subset:
 *       *Calls createSubSet(setNumbet, Data) to create the new subset within the specified set, using the collected data.
 *
 *  6. Notifies other clients:
 *       * zConstructs a message string mesg with the set number, a flag (e.g., "1b"), and the subset data.
 *       * Calls saveChat(mesg) to presumably save the chat history for the set.
 *       * Iterates through connected clients and sends the message string to each using send.
 *  7. Clears memory:
 *       * Zeroes out the Data and mesg arrays to clear any sensitive information.
 */
void case5_CreateSubset(CliDetails);

//saved related function

/*
 *  Purpose:
 *     * Creates a new node representing a set in a linked list structure.
 *
 *  Steps:
 *     * Allocates memory for a new node:
 *     * Creates a new node object using new node(setNumber), initializing it with the provided set number.
 *
 *  Handles an empty list:
 *     * If the list is empty (meaning head is null):
 *     * Assigns both head and tail to point to the newly created node, indicating it's the only node in the list.
 *
 *  Appends to an existing list:
 *    * If the list already has nodes:
 *    * Connects the existing tail node to the new node using tail->next = temp.
 *    * Updates the tail pointer to point to the newly added node, making it the new tail.
 */
void creatSet(int32_t setNumber);

/*
 *   Purpose:
 *       * Prints the answers (subsets) within a specific set in the linked list structure.
 *   Steps:
 *       * Finds the target set:
 *            *Iterates through the linked list using the temp pointer.
 *            *Continues until it finds the node with the matching setNo or reaches the end of the list.
 *
 *       * Clears the screen and prints a header:
 *            * Clears the terminal using system("clear").
 *            * Prints a header indicating the set number being printed.
 *
 *      *  Prints each answer within the set:
 *            * Accesses the first subset (answer) within the found node using temp->down.
 *            * Iterates through the linked list of subsets using the run pointer.
 *            * For each subset, prints its answer content to the console.
 *
 */
void printSubSet(int32_t setNumber);

/*
 *    Purpose:
 *          Manages the printing of sets within the linked list structure, either for displaying all sets or checking for a specific set's existence.
 *    Parameters:
 *         * check (bool): Determines the function's behavior:
 *                 false: Prints all sets in the list.
 *                 true: Checks if a specific set exists and returns a boolean indicating its presence.
 *         * setnumber (int): Used only when check is true, indicating the set number to search for.
 *
 * If the function reaches the end of the list without finding a match for setnumber (when check is true), returns false to indicate the set doesn't exist.
 *
 */
bool printSuperSet(bool check, int32_t setNumber);

/*
 *   Purpose:
 *       Creates a new subset (answer) within an existing set in the linked list structure.
 *
 *  Steps:
 *      * Finds the target set:
 *            * Iterates through the linked list horizontally using the horizontal pointer.
 *            * Continues until it finds the node with the matching setNo or reaches the end of the list.
 *      * Creates a new subset node:
 *            *Allocates memory for a new subNode object using new subNode(data), initializing it with the provided data.
 *  Appends the subset to the set:
 *      * If the set doesn't have any subsets yet (meaning horizontal->down is null):
 *            * Directly assigns the new subset node to the set's down pointer, making it the first subset.
 *      * If the set already has subsets:
 *            * Connects the new subset node to the existing first subset using vertical->down = horizontal->down.
 *            * Updates the set's down pointer to point to the newly added subset, making it the new first subset.
 */

void createSubSet(int32_t setNumber, char data[SIZE]);

/*
 * Purpose:
 *----   Deallocates a linked list of nodes and their associated sub-nodes, freeing up memory.
 *
 *  Steps:
 *   1. Iterates through the main list:
 *            Stores the next node in Setnext to maintain list traversal.
 *            Begins iterating through the sub-nodes of the current main node.
 *   2. Deletes sub-nodes:
 *          *Iterates through the sub-nodes of the current main node:
 *               *Stores the next sub-node in SubNodedown.
 *               *Deletes the current sub-node using delete.
 *               *Moves to the next sub-node.
 *   3. Deletes the main node:
 *            * After all sub-nodes are deleted, deletes the current main node using delete.
 *   4. Moves to the next main node:
 *            * Sets head to Setnext to continue iterating through the main list.
 */
void MemoryManagement();

/*Purpose:
*    * Deallocates a linked list of chat objects, freeing up memory.
* Steps:
*    * Iterates through the chat list:
*         *Stores the next chat object in ChatNext to maintain list traversal.
*    * Deletes the current chat object:
*         *Deletes the current chat object using delete.
*    * Moves to the next chat object:
*         *Sets chatHead to ChatNext to continue iterating through the chat list.
*/
void ChatMemoryManagement();

/*==========+++++++++++++==========+++++++++++========++++++++++++++++++============*/

/*=======================+++++====-- Main fxn-- =+++===+++++++++++=====+++++++====++=+======*/
int32_t main() {
       system("clear");
       int32_t serverSocketFileDiscription = socket(AF_INET, SOCK_STREAM, 0);
       if (serverSocketFileDiscription < 0) {
              perror("socket creation error\n");
              exit(1);
       }

       struct sockaddr_in address {};
       address.sin_family = AF_INET;
       address.sin_port = htons(port);
       inet_pton(AF_INET, "172.16.56.159", &address.sin_addr.s_addr);
       int32_t BindResult = bind(serverSocketFileDiscription, (struct sockaddr *) &address, sizeof address);

       //  BindResult?exit(2):printf("binding successful\n");
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

       auto clientDetails = new ClientDetails(0);

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
              // MemoryManagement();
              // ChatMemoryManagement();
              clientDetails->finalExit = false;
       }

       system("clear");
       //       system("exit");
       return 0;
}
/*=====++++++++++++++++++++++==========+++++=======+==+===++=+=+======+======+=+====+====*/

void msgSend(CliDetails client) {
       char msg[1024];
       while (client->ChatMsgStatus) {
              fgets(msg, 1024, stdin);
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
       char revMsgBuffer[1024];
       memset(revMsgBuffer, '\0', 1024);
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
                     exit(EXIT_SUCCESS);
              }

              for (int32_t i = 0; i < client->NumberOfClient and client->finalExit; ++i) {
                     if (__DARWIN_FD_ISSET(client->whichClient[i], &readfd)) {
                            char buffer[1024];
                            int64_t byteResv = read(client->whichClient[i], buffer, 1024);
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
                                          memset(buffer, '\0', 1024);
                                          continue;
                                   }

                                   // for creating the set on server
                                   if (buffer[1] == 'a') {
                                          // create set;
                                          std::cout << RED << "\nATTENTION! " << RESET << "some one Added Set : " << buffer[0] - 48 << "\n";
                                          // as it recive char 1 so we have to convert it intiger one by subtracting the 48
                                          creatSet(buffer[0] - 48);
                                          memset(buffer, '\0', 1024);
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

                                   memset(buffer, '\0', 1024);
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
       memset(client->MsgResv, '\0', 1024);
}

/***=============== save option ====================***/

void option(CliDetails client) {
       while (client->finalExit) {
              int32_t input;
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
                            printf("4. shutting down server and cient \n");
                            char exitNote[5];
                            memset(exitNote, '\0', sizeof exitNote);
                            exitNote[0] = '1';
                            exitNote[1] = 'e';
                            exitNote[2] = '\0';
                            for (int i = 0; i < client->NumberOfClient; ++i) {
                                   if (client->whichClient[i] != -1) {
                                          send(client->whichClient[i], exitNote, strlen(exitNote), 0);
                                   }
                            }
                            client->finalExit = false;
                            //                            MemoryManagement();
                            //                            ChatMemoryManagement();
                            close(client->serverSocketFileDiscription);
                            shutdown(client->serverSocketFileDiscription, SHUT_RDWR);
                            for (int i = 0; i < client->NumberOfClient; ++i) {
                                   close(client->whichClient[i]);
                                   shutdown(client->whichClient[i], SHUT_RDWR);
                            }
                            //                            free(chatHead);   // there are  much more memory alloted from heap but not freeing
                            //                            free(chatTail);   // there are  much more memory alloted from heap but not freeing
                            //                            free(head);       // there are  much more memory alloted from heap but not freeing
                            //                            free(tail);       // there are  much more memory alloted from heap but not freeing
                            exit(EXIT_SUCCESS);
                     default:
                            system("clear");
                            std::cout << RED << "INVALID INPUT\n"
                                      << RESET;
                            continue;
              }
       }
}
/* memeory management after usning calloc and malloc fxn for array

 function missing // coz it have lot of mess to do

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
              if (client->broadCastingPrevMsgToClient) {
                     std::this_thread::sleep_for(std::chrono::seconds(1));
                     send(client->whichClient[client->NumberOfClient - 1], temp->msg, strlen(temp->msg), 0);
                     temp = temp->next;
                     continue;
              }
              if (temp->msg[1] == 'c') {
                     for (int i = 2; temp->msg[i]; ++i) {
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
       }          // horinzontal triversal to that node

       auto *vertical = new subNode(data);
       if (horizontal->down == nullptr) {          // pushing element to first;
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
       std::cout << "-------printing setNumber-------  " << setNumber << " :- \n";
       subNode *run = temp->down;
       int x = 0;
       while (run) {
              x++;
              std::cout << run->answer;
              run = run->down;
       }
}

bool printSuperSet(bool check, int setnumber) {
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

                     std::cout << "Set Number: " << temp->setNo << "\n";
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
       int setNumber;
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
                     for (int i = 0; i < client->NumberOfClient; ++i) {
                            send(client->whichClient[i], arr, strlen(arr), 0);
                     }
              }
       }
}

void case5_CreateSubset(CliDetails client) {
       system("clear");
       printSuperSet(false, 0);
       std::cout << "which set: or -1 for exit: ";
       int setNumbet;
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
              while (fgets(line, 1024, stdin)) {
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
              for (int j = 0; j < client->NumberOfClient; ++j) {
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
       int setNumber;
       printSuperSet(false, 0);          // call set print function
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
 * Memory management related fxn
 */

void MemoryManagement() {
       auto *temp=head;
       while (temp) {
              node *Setnext = temp->next;
              subNode *currentSubnode = temp->down;
              while (temp->down) {
                     subNode *SubNodedown = currentSubnode->down;
                     delete (SubNodedown);
                     currentSubnode = SubNodedown;
              }
              delete temp;
              temp = Setnext;
       }
       head = nullptr;
       return;
}

// chat delete

void ChatMemoryManagement() {
       auto *temp= chatHead;
       while (temp) {
              Chat *ChatNext = temp->next;
              delete temp;
              temp= ChatNext;
       }
       chatHead= nullptr;
}
