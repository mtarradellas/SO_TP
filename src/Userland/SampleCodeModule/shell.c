#include "shell.h"
#include "stdlib.h"
#include <stdint.h>
#include "timeModule.h"
#include "videoModule.h"
#include "pongModule.h"
#include "soundModule.h"

int on = 1;
void initShell(){

  printf("\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about our commands\n\n\n");

  char command[MAXLEN];
  while (on){

    printf("$> ");
    clearBuffer(command);
    scanAndPrint(command);
    int com = getCommand(command);

    switch(com) {
      case HELP:
          help();
          break;

      case CLEAR:
          clear();
          break;

      case TIME:
          time();
          break;

      case PONG:
          pong();
          break;

      case ZERODIV:
          zeroDiv();
          break;

      case INVOPCODE:
          invOpCode();
          break;

      case LENIA:
          lenia();
          break;

      case EXIT:
          exit();
          break;

      case INVCOM:
          invCom();
    }
  }
   printf("\n\n End of program");
}

int getCommand(char * command) {
  if (!strCmp("help", command)) return HELP;
  if (!strCmp("clear", command)) return CLEAR;
  if (!strCmp("time", command)) return TIME;
  if (!strCmp("pong", command)) return PONG;
  if (!strCmp("zerodiv", command)) return ZERODIV;
  if (!strCmp("invopcode", command)) return INVOPCODE;
  if (!strCmp("lenia", command)) return LENIA;
  if (!strCmp("exit", command)) return EXIT;
  return INVCOM;
}

void help() {
  printf("\n\n********  Help Menu  ********\n\n");
  printf("  * clear     :       Clears screen\n");
  printf("  * invopcode :       Executes Invalid OP Code Interruption\n");
  printf("  * zerodiv   :       Executes Zero Division Interruption\n");
  printf("  * exit      :       Exits shell\n");
  printf("  * lenia     :       Beep\n");
  printf("  * time      :       Displays current time\n");
  printf("  * pong      :       Iniciates pong when user presses 'enter' which will run until\n"); 
  printf("                      end of game or until user presses 'backspace' to leave\n");

  printf("\n  Any other command will be taken as invalid\n\n");

}

void clear() {
  clearScreen();
  printf("\n~~Welcome to Lenia's Shell~~\n\n\n");
}

void time(){
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("\nLocal Time: %d:%d:%d\n", h, m, s);
}

void pong() {
  startPong();
  clear();
}

void zeroDiv() {
  int a = 0;
  a = 2/a;
}

void invOpCode(){
  opCode();
}

void lenia() {

  doBeep();
  wait(20);
  noBeep();
  printf("\n");
}


void exit() {
  on = 0;
}

void invCom() {
  printf("\nInvalid command\n");
}