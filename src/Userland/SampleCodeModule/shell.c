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
#define PTEST 9
#define MEMTEST 10
#define PS 11
#define KILLTEST 12

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
// Displays table with processes information
static void ps();
// Memory allocation test
static void memTest();
// Multiple processes test
static void pTest();
static void test1();
static void test2();
static void killTest();

cmd command_array[] = {
  (cmd)invCom,     (cmd)help,
  (cmd)clear,      (cmd)time,
  (cmd)pong,       (cmd)zeroDiv,
  (cmd)invOpCode,  (cmd)lenia,
  (cmd)exit,       (cmd)pTest,
  (cmd)memTest,    (cmd)ps,
  (cmd)killTest
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
  if (!strCmp("ptest", command)) return PTEST;
  if (!strCmp("killtest", command)) return KILLTEST;
  if (!strCmp("memtest", command)) return MEMTEST;
  if (!strCmp("ps", command)) return PS; 
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
  printf("  * memtest   :       Shows functioning Memory Management\n");
  printf("  * ptest     :       Runs multiple processes to show functionality\n");
  printf("  * killtest  :       Kills all processes created from ptest command\n");
  printf("  * ps        :       Displays process table with, name, pid, status, foreground, memory, priority\n");
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

static void ps() {
  tProcessData** psVec;
  int size;
  getPS(&psVec, &size);
  printf("\nPID     Name      Status     Memory    Priority\n"); // 8 10 12 13 10
  for (int i = 0; i < size; i++) {
    printf("%d       %s     %s      %d      %s\n", psVec[i]->pid, psVec[i]->name, 
                                          psVec[i]->status, psVec[i]->memory, psVec[i]->priority);
  }
}

static void memTest(){

  char * mem = malloc(25);
  printf("\n Memory has been allocated correctly (and string has been inserted). Showing memory block:");

  char copy[25] = "Penguins have knees";
  memcpy(mem, copy, sizeof(copy));

  printNode(mem);

  free(mem);
  printf("Memory has been freed. Showing memory block:\n");

  printNode(mem);

  char * mem2 = malloc(16);
  printf("\n New memory has been allocated correctly in the same block. Showing memory block:");

  printNode(mem2);
  char copy2[16] = "it works, relax";
  memcpy(mem, copy2, sizeof(copy2));

  printf("\n Showing memory block with new inserted string:");
  printNode(mem2);

  free(mem2);
  printf("Memory has been freed.\n");

  printf("\n        ////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
  printf("        //////////////////////////////////////////  ///////  ///////////////////////////////////////////////////////\n");
  printf("        //////////////////////////////////////////  ///////  ///////////////////////////////////////////////////////\n");
  printf("        ////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
  printf("        ///////////////////////////////////////  ////////////  /////////////////////////////////////////////////////\n");
  printf("        ////////////////////////////////////////             ///////////////////////////////////////////////////////\n");
  printf("        ////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
}  

unsigned long int testp[2];
typedef int (*mainf)();
static void pTest() {
  printf("\nCreating two processes that will end after a couple of seconds\n");
  printf("Process 1 has high priority, process 2 has low priority\n");
  printf("You may kill all test processes at anytime with 'killtest' command\n");
  testp[0] = createProcess("test1", (mainf)test1, 0, NULL, HIGHP);  
  testp[1] = createProcess("test2", (mainf)test2, 0, NULL, LOWP);  
}

static void killTest() {
  kill(testp[0]);
  kill(testp[1]);
}

static void test1() {
  int i = 0;
  while(i < 5) {
    printf("  ~1\n");
    wait(50);
    i++;
  }
  printf("test 1 done\n");
  return;
}

static void test2() {
  int i = 0;
  while(i < 5) {
    printf("  ~2\n");
    wait(50);
    i++;
  }
  printf("test 2 done\n");
}