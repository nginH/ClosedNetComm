

#include <iostream>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"


int SIZE =1024;

void option           ();
void creatSet         (int setNumber);
void printSubSet      (int setNumber);
bool printSuperSet    (bool check , int setNumber);
void createSubSet     (int setNumber, char data[SIZE]);


//bool printSuperSet (bool ,int);

/* structure for node*/

struct subNode {
  char *answer;
  subNode * down;
  explicit subNode(char ans[SIZE]){
       this->answer= (char*)calloc(SIZE, sizeof(char));
       strcpy(this->answer, ans);
       this->down= nullptr;
//      memset(this->answer, '\0', SIZE);
  }
};

struct node {
  int     setNo;
  node    *next;
  subNode *down;
  explicit node(int  set){
       this->setNo=set;
       this->next=nullptr;
       this->down=nullptr;
  }
}*head=nullptr, *tail= nullptr;



int main(){

     option();

     return 0;
}

void option(){

     int j;
     bool check;
     while (true) {
          check=true;
          std::cout<<"----select option----\n";
          std::cout<<"-------------------\n";
          printSuperSet(false , 0);
          std::cout<<"-------------------\n";
          std::cout<<YELLOW;
          printf("Enter Set number for answer\n");
          printf("69   :Create\n");
          printf("6969 : Exit\n");
          printf("0    : for entering value in set\n");
          printf("enter value ( example type 6969 for exit) : ");
          std::cout<<RESET;
          std::cin>>j;
          if (std::cin.fail()) {
               std::cin.clear();
               std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               std::cout<<RED << "Invalid input. Please enter an integer\n"<<RESET;
               continue; // Repeat the loop for a new input attempt
          }
          switch (j) {
               case 69: // create super set
                    printf("enter set number: ");
                    int k;
                    std::cin>>k;
                    if (std::cin.fail()) {
                         std::cin.clear();
                         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                         std::cout << "Invalid input. Please enter an integer: ";
                         continue; // Repeat the loop for a new input attempt
                    }
                    if (printSuperSet(true, k)) {
                         printf("Set Already there\n");
                         continue;
                    }else{
                         creatSet(k);
                    }
                    break;
               case 6969: // exit
                    printf("success\n");
                    return;
               case 0: // create sub set;
                    int set;
                    system("clear");
                    std::cout<<"----select option----\n";
                    std::cout<<"-------------------\n";
                    printSuperSet(false, 0);
                    std::cout<<"-------------------\n";
                    std::cout<<"which set : ";
                    std::cin>>set;
                         if (std::cin.fail()) {
                              std::cin.clear();
                              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                              std::cout << "Invalid input. Please enter an integer: ";
                              continue; // Repeat the loop for a new input attempt
                         }
                    if(printSuperSet(true, set)){
                         char Data[SIZE];
                         std::cout<<BLUE<<"Copy Paste you Answer ' " <<set<<" ' -1 for exit \n" <<RESET;
                         while (true) {
                              fgets(Data, 1024, stdin);
                              if (strcmp(Data, "-1\n")==0) {
                                   break;
                              }else{
                                   createSubSet(set, Data);
                              }
                         }
                    }
                    break;
               default:
                    check=false;
                    if (printSuperSet(true, j)) {
                         printf("\n\n");
                         printSubSet(j);
                         printf("\n\n");
                    }else{
                         printf("\n\n");
                         std::cout<<RED<< "invalid input "<<RESET;
                         printf("\n\n");
                    }
                    break;
          }
          if(check){
               system("clear");
          }
     }
}
