/**
 * COSC 3250 - Project 1
 * Explain briefly the functionality of the program.
 * DECODER
 * @author Davina Lettau
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO davina.lettau@marquette.edu
 */

#include <stdio.h>
#include <stdlib.h>
char getVal(char);
int main(int argc, char *argv[]) {
  char key = argc > 1 ? atoi(argv[1]) : 0;
  char previous = 0;
  char val = getVal(getchar());
  while (val != EOF) {
    if (val == -1)
      printf("\n");
    else {
      char encodedVal = (val - key - previous);
      while (encodedVal < 0) {
        encodedVal += 27;
      }
      printf("%c", encodedVal ? encodedVal + 'A' - 1 : ' ');
      previous = val;
    }
    val = getVal(getchar());
  }
  printf("\n");
	return 0;
}

char getVal(char c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 1;
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;
	else if (c == '\n')
    return -1;
  else
		return 0;
}
