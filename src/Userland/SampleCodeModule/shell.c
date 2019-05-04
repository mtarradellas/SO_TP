#include "include/shell.h"
#include <stdint.h>
#include "include/pongModule.h"
#include "include/soundModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"

#define INVCOM 0
#define HELP 1
#define CLEAR 2
#define TIME 3
#define PONG 4
#define ZERODIV 5
#define INVOPCODE 6
#define ALEPUTO 7
#define LENIA 8
#define SHELLSHOCK 9
#define EXIT 10

#define MAXLEN 256

void opCode();

// Gets command ready to use in a switch function
static int getCommand(char* command);
// Executes the 'help' command. Displays help menu
static void help();
// Executes the 'clear' command. Clears screen
static void clear();
// Executes the 'time' command. Displays local time
static void time();
// Executes the 'pong' command. Initializes the pong game
static void pong();
// Executes the 'zerodiv' command. Triggers a Zero Division Exception
static void zeroDiv();
// Executes the 'invopcode' command. Triggers an Invalid OP Code Exception
static void invOpCode();
// Executes the 'lenia' command. Makes beep sound
static void lenia();
// Executes the 'exit' command. Exits the kernel
static void exit();
// Displays the message for when a command was not recognized
static void invCom();

int on = 1;
void initShell() {
  printf(
      "\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about "
      "our commands\n\n\n");

  char command[MAXLEN];
  while (on) {
    printf("$> ");
    clearBuffer(command);
    scanAndPrint(command);
    int com = getCommand(command);

    switch (com) {
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

static int getCommand(char* command) {
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

static void help() {
  printf("\n\n********  Help Menu  ********\n\n");
  printf("  * clear     :       Clears screen\n");
  printf("  * invopcode :       Executes Invalid OP Code Interruption\n");
  printf("  * zerodiv   :       Executes Zero Division Interruption\n");
  printf("  * exit      :       Exits shell\n");
  printf("  * lenia     :       Beep\n");
  printf("  * time      :       Displays current time\n");
  printf(
      "  * pong      :       Iniciates pong when user presses 'enter' which "
      "will run until\n");
  printf(
      "                      end of game or until user presses 'backspace' to "
      "leave\n");

  printf("\n  Any other command will be taken as invalid\n\n");
}

static void clear() {
  clearScreen();
  printf("\n~~Welcome to Lenia's Shell~~\n\n\n");
}

static void time() {
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("\nLocal Time: %d:%d:%d\n", h, m, s);
}

static void pong() {
  startPong();
  clear();
}

static void zeroDiv() {
  int a = 0;
  a = 2 / a;
}

static void invOpCode() { opCode(); }

static void lenia() {
  doBeep();
  wait(20);
  noBeep();
  printf("\n");
}

static void exit() { on = 0; }

static void invCom() {
  printf("\nInvalid command\n");
}
