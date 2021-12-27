// BINGO game in C
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include<math.h>
#include<stdbool.h>
#include "list.h"

int *make_card (int *number_array){
    int val = 0;
    int* card = (int*)malloc(50 * sizeof(int));
    for(int x = 0; x<50; x+=2)
    {
        int temp = *(number_array + val);
        *(card + x) = temp;
        val++;
        *(card + x + 1) = 0;
    }
    return card;
}

int mark_card(int *card, int val)
{
    for(int x=0; x<50; x+=2){
        if(*(card + x) == val){
            *(card + x + 1) = 1;
            return val;
        }
    }
    return 0;
}

void print_card (int *card){
    printf("L   I   N   U   X\n");
    int temp = 0;
    for(int x = 0; x < 50; x+=2)
    {
        if(*(card + x) < 10){
            if(*(card + x + 1) == 1){
                printf("0%dm ", *(card + x));
            }
            else{
                printf("0%d  ", *(card + x));
            }
        }
        else{
            if(*(card + x + 1) == 1){
                printf("%dm ", *(card + x));
            }
            else{
                printf("%d  ", *(card + x));
            }
        }
        temp+=1;
        if(temp == 5){
            temp = 0;
            printf("\n");
        }
    }
}

int check_win (int *card){
    int x;
    int y;
    bool flag=false;
    //check if all 4 corners are marked
    if(*(card + 1) == 1 && *(card + 41) == 1 && *(card + 9) == 1 && *(card + 49) == 1){
        flag = true;//set flag to true
        return 0;//end function
    }
    //check for marked rows
    if(!flag){
        x=0;
        y=0;
        int row_number = 1;
        for(int x=0; x<50; x+=2){
            if(*(card + x + 1) == 0){
                x = x + ((5*row_number) - x);
                y = 0;
                row_number +=1;
            }
            if(*(card + x + 1) == 1){
                y+=1;
            }
            if(y==5)
            {
                flag = true;
                return 0;
                break;
            }
        }
    }
    if(!flag)
    {
        x = 0;
        y = 0;
        int col_number = 0;
        int fails = 0;
        while(y<50)
        {
            if(*(card + y + 1) == 1)
            {
                y = y + (10);
                x+=1; 
            }
            if(*(card + y + 1) == 0){
                col_number+=1;
                y = col_number*2;
                x = 0;
                fails += 1;
            }
            if(x==4){
                flag = true;
                return 0;
                break;
            }
            if(fails == 5)
            {
                break;
            }
        }
    }
 }

//Checks number of arguments
int checkNumArg(int argc, char *argv[]) {
    if (argc != 3) {
    fprintf(stderr, "Usage: A2 seed cardFile\n");
    exit(1);
 }
}

/*Helper function for checkSeed
  This helper function checks for a positive integer*/
int isInt ( char *numSize ) {
  int i, length;
  length=strlen(numSize);
  if (length>0 && !isdigit(numSize[0]) ) return 0;
  for (i=1;i<length;i++) {
     if ( ! isdigit (numSize[i]) ) return 0;
  }
  return 1;
}

//Checks if seed parameter is correct
int checkSeed(int argc, char *argv[]) {
    if (!isInt(argv[1])) {
    fprintf(stderr, "Expected integer seed, but got %s\n",argv[1]);
    exit(2);
 }
}

//Check if file is readable and exists
int checkParameters(int argc, char *argv[]) 
{
    const char *card = argv[2];
    if( access( card, F_OK|R_OK ) != -1)
    {
        return 0;
    }
    else
    {
    fprintf(stderr, "%s is nonexistent or unreadable\n",card);
    }
    exit(3);
}

/*Checks if cardformat is correct and store the converted elements (str to int) into an array
  checkCardFormat returns a pointer to an array*/
void checkCardFormat(char file[], int size)
{
    regex_t regex1;
    regex_t regex2;
    const char* filename = file;

    FILE *in_file = fopen(filename, "r");

    struct stat cardStat; //Makes a struct containing status of filename
    stat(filename, &cardStat);

    int card[size];
    int count = 1;
    int x;
    int i=0;
    int j = 0;
    char * ele;
    char *file_contents = malloc(cardStat.st_size); //make an array of filename's size (see struct cardStat)
    int return_value;
    int return_value2;

    //initialize card to unused value
    for(int a = 0; a<size; a++ ){
        card[a] = 99;
    }
    /*While the file has 5 or less lines check against the regex and continue
      Note: the regex does not check for duplicates, so it will be checked when adding elements to the array*/
    return_value = regcomp(&regex1,"^([0][1-9]|[1][0-5])\\s([1][6-9]|[2][0-9]|[3][0])\\s([3][1-9]|[4][0-5])\\s([4][6-9]|[5][0-9]|[6][0])\\s([6][1-9]|[7][0-5])$",REG_EXTENDED);
    return_value2 = regcomp(&regex2,"^([0][1-9]|[1][0-5])\\s([1][6-9]|[2][0-9]|[3][0])\\s00\\s([4][6-9]|[5][0-9]|[6][0])\\s([6][1-9]|[7][0-5])$",REG_EXTENDED);
    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
        if (count > 5) {
          fprintf(stderr,"%s has bad format\n", filename);
          exit(4);
        }
        else if (count == 3) {
            return_value2 = regexec(&regex2, file_contents, 0, NULL, 0);
            if (return_value2 != 0) {
              fprintf(stderr,"%s has bad format\n", filename);
              exit(4);
            }
        }
        else {
            return_value = regexec(&regex1, file_contents, 0, NULL, 0);
            if (return_value != 0) {
              fprintf(stderr,"%s has bad format\n", filename);
              exit(4);
            }
        }
        /*Converts elements from string to integer, adds elements to array, and checks for duplicates*/
        ele = strtok (file_contents," "); //deliminate lines by space to get elements
        while (ele != NULL)
        {
            if(strcmp("00",ele) == 0){ //if ele==00 is true
                x=0; //get integer
            } else {
                x=atoi(ele); //get integer
            }

            for(j=0; j<size; j++) { //check duplicate
                if(card[j] == x) {
                fprintf(stderr,"%s has bad format\n", filename);
                exit(4);
                }
            }
            card[i]=x;
            i++;
            ele = strtok (NULL, " "); //next element in card
        }
        count++;
    }
    fclose(in_file);
}

void printCallList(int list[], int listSize) {
    printf("CallList: ");
    
    for (int c = 0; c <= listSize; c++) { 
        if(list[c] == 0) {

        } else if(list[c] > 0 && list[c] < 10 && list[c] != 99) {
            printf("L");
            printf("0%d ", list[c]);
        } else if(list[c] >= 10 && list[c] < 16 && list[c] != 99) {
            printf("L");
            printf("%d ", list[c]);
        } else if (list[c] >= 16 && list[c] < 31 && list[c] != 99) {
            printf("I");
            printf("%d ", list[c]);
        } else if (list[c] >= 31 && list[c] < 46 && list[c] != 99) {
            printf("N");
            printf("%d ", list[c]);
        } else if (list[c] >= 46 && list[c] < 61 && list[c] != 99) {
            printf("U");
            printf("%d ", list[c]);
        } else if (list[c] >= 61 && list[c] < 76 && list[c] != 99) {
            printf("X");
            printf("%d ", list[c]);
        } 
        
   }
   printf("\n\n");
}


int main(int argc, char *argv[])
{
    char numSize[strlen(argv[1])];
    static int size = 25;
    int goodCard[size];
    checkNumArg(argc, argv);
    checkSeed(argc, argv);
    checkParameters(argc, argv);
    checkCardFormat(argv[2], size);


    //fill goodCard Array 
    FILE *in_file = fopen(argv[2], "r");
    struct stat cardStat; //Makes a struct containing status of filename
    stat(argv[2], &cardStat);
    int x=0, i=0; 
    char * ele;
    char *file_contents = malloc(cardStat.st_size); //make an array of filename's size (see struct cardStat)
    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
        ele = strtok (file_contents," ");
        while (ele != NULL)
        {
            if(strcmp("00",ele) == 0){ //if ele==00 is true
                x=0; //get integer
            } else {
                x=atoi(ele); //get integer
            }
            goodCard[i]=x;
            i++;
            ele = strtok (NULL, " "); //next element in card
        }
    }    
    fclose(in_file);



    int seedArg = atoi(argv[1]);
    int markedVal; 
    int dummyNum = 99; 
    int listSize = 80; 
    int markedList[listSize];
    for(int a = 0; a<listSize; a++ ){
        markedList[a] = dummyNum;
    }
    int loopCount=0;

    int num;
    char c; 

    int *card = make_card(goodCard); 
    markedVal = mark_card(card, 0); //no need to print; don't set callList
    srand(seedArg);
    do {
     num = (rand() % (75 + 1 - 1)) + 1;
     system("clear");
     printCallList(markedList, listSize);
     print_card(card);
     printf("enter any non-enter key for Call (q to quit):" ); 
     c=getchar();
     printf("\n"); 

     if(c!=10 && c!=113){ //check for q and enter
      markedVal = mark_card(card, num);
      if (num != 0) { //all called numbers 
        int exists=0;
        for(int j=0; j<size; j++) { //check duplicate
            if(markedList[j] == num) {
                exists=1;
            }
        }
        if (exists ==0) {
            markedList[loopCount] = num;
            loopCount++;
        }
      }
      int win = check_win(card);
      if(win == 0){
          system("clear");
          printCallList(markedList, listSize);
          print_card(card);
          printf("WINNER\n");
          exit(0);
      }
     }
    } while (c != 'q' && c != EOF);
}
