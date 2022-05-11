/**
* COSC 3250 - Project 2
* Explain briefly the functionality of the program.
* Codebreaker program
* @author Davina Lettau
* Instructor Dr.Brylow
* TA-BOT:MAILTO davina.lettau@marquette.edu
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1025

int countNumE(char inputtext[MAX_LINE]) {
  int countE = 0;
  int i = 0;
  for (i = 0; inputtext[i] != '\0'; i++) 
    if (inputtext[i] == 'E')
      countE++;
  return countE;
}

void decodeText(char inputtext[MAX_LINE], char outputtext[MAX_LINE], int key) {
  int previous = 0;
  int val = 0;
  int decodedVal = 0;
  int i = 0;
  for (i = 0; inputtext[i] != '\0'; i++) {
    if (inputtext[i] == '\n') {
        outputtext[i] = '\n';
    }
    else {
        if (inputtext[i] <= 'Z' && inputtext[i] >= 'A') {
          val = inputtext[i] - 'A' + 1;
        } else if (inputtext[i] <= 'z' && inputtext[i] >= 'a') {
          val = inputtext[i] - 'a' + 1;
        } else {
          val = 0;
        }
        decodedVal = val - key - previous;
        while (decodedVal < 0) {
          decodedVal += 27;
        }
        outputtext[i] = decodedVal ? decodedVal + 'A' - 1 : ' ';
        previous = val;
        }
  }
  outputtext[i] = '\0';
}


int main() {

  int storage[4] = {0,0,0,0};
  char inputtext[MAX_LINE];
  kgetc("%[^\001]", inputtext);
  
  int key = 0;
  int numE = 0;
  char decodedtext[MAX_LINE];
  for (key = 0; key < 27; key++) {
    
    decodeText(inputtext, decodedtext, key);
    numE = countNumE(decodedtext);
    
    if (numE > storage[1]) {
      storage[2] = storage[0];
      storage[3] = storage[1];
      storage[0] = key;
      storage[1] = numE;
      
    } else if (numE > storage[3]) {
      
      storage[2] = key;
      storage[3] = numE;
    }
  }
  decodeText(inputtext, decodedtext, storage[2]);
  
  printf("Most probable key is %d\n%s\n", storage[2], decodedtext);
  
}