#include "include/shell.h"
#include <stdint.h>
#include "include/pongModule.h"
#include "include/soundModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"
#include "include/memoryModule.h"
#include "include/processModule.h"

#define INVCOM 0
#define HELP 1
#define CLEAR 2
#define TIME 3
#define PONG 4
#define ZERODIV 5
#define INVOPCODE 6
#define LENIA 7
#define EXIT 8
//////////////////////////
#define NEWPROC 9
//////////////////////////

#define MAXLEN 256

void opCode();

typedef void (*cmd)();

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

////// T E S T S
static void newProc();
static void test1();
static void test2();

cmd command_array[] = {
  (cmd)invCom,     (cmd)help,
  (cmd)clear,      (cmd)time,
  (cmd)pong,       (cmd)zeroDiv,
  (cmd)invOpCode,  (cmd)lenia,
  (cmd)exit,       (cmd)newProc
};

int on = 1;
void initShell() {
  printf(
      "\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about "
      "our commands\n\n\n");
  char command[MAXLEN];
  while (on) {
    printf("\n$> ");
    clearBuffer(command);
    scanAndPrint(command);
    int com = getCommand(command);

    command_array[com]();
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
  if (!strCmp("np", command)) return NEWPROC;
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

  printf("\n  Any other command will be taken as invalid\n");
}

static void clear() {
  clearScreen();
  printf("\n~~Welcome to Lenia's Shell~~\n\n");
}

static void time() {
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("\nLocal Time: %d:%d:%d", h, m, s);
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
}

static void exit() { on = 0; }

static void invCom() {
  printf("\nInvalid command");
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
typedef int (*mainf)();
static void newProc() {
  printf("\n");
  unsigned long int pid1 = createProcess("test1", (mainf)test1, 0, NULL, HIGHP);  
  unsigned long int pid2 = createProcess("test2", (mainf)test2, 0, NULL, HIGHP);  
  wait(100);
  printf("KILLING %d\n", pid1);
  kill(pid1);
}

static void test1() {
  while(1) {
    printf("\n1\n");
    wait(30);
  }
  return;
}

static void test2() {
  while(1) {
    printf("\n2\n");
    wait(30);
  }
}