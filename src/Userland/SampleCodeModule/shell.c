#include "include/shell.h"
#include <stdint.h>
#include "include/memoryModule.h"
#include "include/mutexModule.h"
#include "include/niceModule.h"
#include "include/philosophers.h"
#include "include/pongModule.h"
#include "include/processModule.h"
#include "include/prodCon.h"
#include "include/soundModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"

typedef enum {
  INVCOM,
  HELP,
  CLEAR,
  TIME,
  PONG,
  ZERODIV,
  INVOPCODE,
  LENIA,
  EXIT,
  PTEST,
  MEMTEST,
  PS,
  KILLTEST,
  STACKOV,
  MUTEX,
  PRODCON,
  PIPETEST,
  PHILOSOPHERS,
  NICE,
  DUMMY,
  PRODUCER,
  CONSUMER
} Command;

void _opCode();

typedef unsigned long int (*cmd)();

// Gets command ready to use in a switch function
static int getCommand(char* command);
// Checks if command is set to background or foreground
static void checkForeground(char* command);
// Executes the 'help' command. Displays help menu
static unsigned long int help();
// Executes the 'clear' command. Clears screen
static unsigned long int clear();
// Executes the 'time' command. Displays local time
static unsigned long int time();
// Executes the 'pong' command. Initializes the pong game
static unsigned long int pong();
// Executes the 'zerodiv' command. Triggers a Zero Division Exception
static unsigned long int zeroDiv();
// Executes the 'invopcode' command. Triggers an Invalid OP Code Exception
static unsigned long int invOpCode();
// Triggers a stack overflow by calling an endless recursive function
static unsigned long int stackOv();
// Executes the 'lenia' command. Makes beep sound
static unsigned long int lenia();
// Executes the 'exit' command. Exits the kernel
static unsigned long int exit();
// Displays the message for when a command was not recognized
static unsigned long int invCom();
// Displays table with processes information
static unsigned long int ps();
// Launches ProdCon application
static unsigned long int prodCon();
// Memory allocation test
static unsigned long int memTest();
// Multiple processes test wrapper
static unsigned long int pTestWrapper();
// Kills all processes from test
static unsigned long int killTest();
// Tests pipes
static unsigned long int pipeTest();
// Launches eating philosophers application
static unsigned long int philosophers();
// Changes a process's priority
static unsigned long int nice();
// Spaws a dummy process with the given name
// and runs for a certain time with given priority
static unsigned long int dummy();
// Spawns a producer process that writes numbers to stdout 
static unsigned long int producer();
static void producerProc();
// Spawns a consumer process that reads lines from stdout
static unsigned long int consumer();
static void consumerProc();

static unsigned long int mutex();
static void pTest();
static void test1();
static void test2();

static cmd command_array[] = {
    (cmd)invCom,   (cmd)help,         (cmd)clear,     (cmd)time,
    (cmd)pong,     (cmd)zeroDiv,      (cmd)invOpCode, (cmd)lenia,
    (cmd)exit,     (cmd)pTestWrapper, (cmd)memTest,   (cmd)ps,
    (cmd)killTest, (cmd)stackOv,      (cmd)mutex,     (cmd)prodCon,
    (cmd)pipeTest, (cmd)philosophers, (cmd)nice,      (cmd)dummy,
    (cmd)producer, (cmd)consumer};

static int sonsVec[50];
static int sonsSize = 0;
static int on;
static int foreground;
static int toPipe;

static char argv[MAX_ARGUMENTS][MAXLEN];

void initShell() {
  on = 1;
  printf(
      "\n~~WELCOME TO LENIA'S SHELL~~\n\nPlease type 'help' to find out about "
      "our commands\n\n\n");
  char command[MAXLEN];
  while (on) {
    foreground = 1;
    toPipe = 0;
    printf("\n$> ");
    clearBuffer(command);
    scanAndPrint(command);
    printf("\n");
    int com = getCommand(command);

    int pid = command_array[com]();
    int pid2;
    if (toPipe) {
      memcpy(command, argv[2], strLen(argv[2])+1);
      pid2 = command_array[getCommand(command)]();
      int fd[2];
      pipe(fd);
      dup(pid, fd[1], STD_OUT);
      dup(pid2, fd[0], STD_IN);
      closeFD(fd[0]);
      closeFD(fd[1]);
      runProcess(pid2);
    }
    if (pid != 0) runProcess(pid);
    if (pid == 0) foreground = 0;
    if (foreground == 1) {
      waitpid(pid);
      if (toPipe) waitpid(pid2);
    }
  }
  printf("\n\n End of program");
}

static int getCommand(char* command) {
  checkForeground(command);
  // ugly implementation, fix later
  for (int i = 0; i < MAX_ARGUMENTS; i++) {
    argv[i][0] = 0;
  }
  splitString(command, argv, MAX_ARGUMENTS);
  if (strCmp(argv[1], "\\") == 0) toPipe = 1;
  // printf("\n");
  // for (int i = 0; i < MAX_ARGUMENTS; i++) {
  //   printf("arg%d: %s, ", i + 1, argv[i]);
  // }
  // printf("\n");
  if (!strCmp("help", argv[0])) return HELP;
  if (!strCmp("clear", argv[0])) return CLEAR;
  if (!strCmp("time", argv[0])) return TIME;
  if (!strCmp("pong", argv[0])) return PONG;
  if (!strCmp("zerodiv", argv[0])) return ZERODIV;
  if (!strCmp("invopcode", argv[0])) return INVOPCODE;
  if (!strCmp("stackov", argv[0])) return STACKOV;
  if (!strCmp("lenia", argv[0])) return LENIA;
  if (!strCmp("exit", argv[0])) return EXIT;
  if (!strCmp("ptest", argv[0])) return PTEST;
  if (!strCmp("killtest", argv[0])) return KILLTEST;
  if (!strCmp("memtest", argv[0])) return MEMTEST;
  if (!strCmp("ps", argv[0])) return PS;
  if (!strCmp("mutex", argv[0])) return MUTEX;
  if (!strCmp("prodcon", argv[0])) return PRODCON;
  if (!strCmp("philosophers", argv[0])) return PHILOSOPHERS;
  if (!strCmp("nice", argv[0])) return NICE;
  if (!strCmp("dummy", argv[0])) return DUMMY;
  if (!strCmp("pipetest", argv[0])) return PIPETEST;
  if (!strCmp("producer", argv[0])) return PRODUCER;
  if (!strCmp("consumer", argv[0])) return CONSUMER;
  return INVCOM;
}

static void checkForeground(char* command) {
  int len = strLen(command);
  if (len < 3) return;
  if (strCmp(" &", command + len - 2) == 0) {
    foreground = 0;
    command[len - 2] = 0;
    command[len - 1] = 0;
  }
}

static unsigned long int help() {
  printf("\n********  Help Menu  ********\n\n");
  printf("  * clear        :       Clears screen\n");
  printf("  * invopcode    :       Executes Invalid OP Code Interruption\n");
  printf("  * zerodiv      :       Executes Zero Division Interruption\n");
  printf(
      "  * stackov      :       Executes Stack Overflow Exception via "
      "recursive function\n");
  printf("  * exit         :       Exits shell\n");
  printf("  * lenia        :       Beep\n");
  printf("  * time         :       Displays current time\n");
  printf("  * prodcon      :       Launches ProdCon application\n");
  printf("  * memtest      :       Shows functioning Memory Management\n");
  printf(
      "  * ptest        :       Runs multiple processes to show "
      "functionality\n");
  printf(
      "  * killtest     :       Kills all processes created from ptest "
      "command\n");
  printf("  * mutex        :       Runs the mutex test\n");
  printf("  * philosophers :       Runs the eating philosophers application\n");
  printf(
      "  * nice         :       Recieves pid and priority and changes that "
      "process's priority\n");
  printf(
      "  * ps           :       Displays process table with, name, pid, "
      "status, foreground, memory, priority\n");
  printf(
      "  * dummy        :       Recieves a priority and a time and "
      "creates a dumy process that runs for that time\n");
  printf(
      "  * pong         :       Iniciates pong when user presses 'enter' which "
      "will run until\n");
  printf(
      "                         end of game or until user presses 'backspace' "
      "to leave\n");
  printf(
      "  * pipetest     :       Shows pipe functionality with processes Father and Son communicating\n");
  printf("\n  Any other command will be taken as invalid\n");
  printf("Commands may be executed on background by typing ' &' at the end\n");
  return 0;
}

static unsigned long int clear() {
  clearScreen();
  printf("~~Welcome to Lenia's Shell~~\n\n");
  return 0;
}

static unsigned long int time() {
  unsigned int h = getHour();
  unsigned int m = getMinute();
  unsigned int s = getSecond();
  printf("Local Time: %d:%d:%d", h, m, s);
  return 0;
}

static unsigned long int pong() {
  startPong();
  clear();
  return 0;
}

static unsigned long int zeroDiv() {
  int a = 0;
  a = 2 / a;
  return 0;
}

static unsigned long int invOpCode() {
  _opCode();
  return 0;
}

static unsigned long int stackOv() {
  printf(
      "        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  printf(
      "        //////////////////////////////////////////  ///////  "
      "///////////////////////////////////////////////////////\n");
  printf(
      "        //////////////////////////////////////////  ///////  "
      "///////////////////////////////////////////////////////\n");
  printf(
      "        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  printf(
      "        /////////////////////////////////////////              "
      "//////////////////////////////////////////////////////\n");
  printf(
      "        ///////////////////////////////////////  /////////////  "
      "////////////////////////////////////////////////////\n");
  printf(
      "        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  stackOv();
  return 0;
}

static unsigned long int lenia() {
  doBeep();
  wait(20);
  noBeep();
  return 0;
}

static unsigned long int exit() {
  on = 0;
  return 0;
}

static unsigned long int invCom() {
  printf("Invalid command");
  return 0;
}

static unsigned long int ps() {
  tProcessData** psVec;
  int size;
  getPS(&psVec, &size);

  printf("PID     Status     Memory    Priority     Name\n");
  for (int i = 0; i < size; i++) {
    printf("%d        %s    %d      %s      %s\n", psVec[i]->pid,
           psVec[i]->status, psVec[i]->memory, psVec[i]->priority,
           psVec[i]->name);
    free(psVec[i]->name);
    free(psVec[i]);
  }
  free(psVec);
  return 0;
}

static unsigned long int prodCon() {
  startProdCon();
  return 0;
}

static unsigned long int philosophers() {
  philosophersRun();
  return 0;
}
static unsigned long int memTest() {
  char* mem = malloc(25);
  printf(
      "Memory has been allocated correctly (and string has been inserted). "
      "Showing memory block:");

  char copy[25] = "Penguins have knees";
  memcpy(mem, copy, sizeof(copy));

  printNode(mem);

  free(mem);
  printf("Memory has been freed. Showing memory block:\n");

  printNode(mem);

  char* mem2 = malloc(16);
  printf(
      "\n New memory has been allocated correctly in the same block. Showing "
      "memory block:");

  printNode(mem2);
  char copy2[16] = "it works, relax";
  memcpy(mem, copy2, sizeof(copy2));

  printf("\n Showing memory block with new inserted string:");
  printNode(mem2);

  free(mem2);
  printf("Memory has been freed.\n");

  printf(
      "\n        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  printf(
      "        //////////////////////////////////////////  ///////  "
      "///////////////////////////////////////////////////////\n");
  printf(
      "        //////////////////////////////////////////  ///////  "
      "///////////////////////////////////////////////////////\n");
  printf(
      "        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  printf(
      "        ///////////////////////////////////////  ////////////  "
      "/////////////////////////////////////////////////////\n");
  printf(
      "        ////////////////////////////////////////             "
      "///////////////////////////////////////////////////////\n");
  printf(
      "        "
      "////////////////////////////////////////////////////////////////////////"
      "////////////////////////////////////\n");
  return 0;
}

int global = 0;

static void doSomething() {
  mutexLock("pepe");
  global += 1;
  printf("\n Job %d started\n", global);
  wait(15);
  ps();
  printf("\n Job %d finished\n", global);
  mutexUnlock("pepe");
}

static unsigned long int mutex() {
  int kAmount = 3;
  mutexOpen("pepe");
  global = 0;
  char** argv = NULL;
  int procs[kAmount];
  for (int i = 0; i < kAmount; i++) {
    procs[i] = createProcess("mutexTest", (mainf)doSomething, 0, argv, HIGHP);
  }

  for (int i = 0; i < kAmount; i++) {
    waitpid(procs[i]);
  }

  mutexClose("pepe");
  return 0;
}

static unsigned long int pTestWrapper() {
  return setProcess("procWrapp", (mainf)pTest, 0, NULL, MIDP);
}

static void pTest() {
  if (sonsSize > 48) {
    printf(
        "Maximun processes created, please wait for some to end or kill all "
        "with 'killtest' command\n");
    return;
  }
  printf("Creating two processes that will end after a couple of seconds\n");
  printf("Process 1 has high priority, process 2 has low priority\n");

  printf(
      "If on background, you may kill all test processes at anytime with "
      "'killtest' command\n");
  int pid1 = createProcess("test1", (mainf)test1, 0, NULL, HIGHP);
  int pid2 = createProcess("test2", (mainf)test2, 0, NULL, LOWP);
  sonsVec[sonsSize++] = pid1;
  sonsVec[sonsSize++] = pid2;
  waitpid(pid1);
  waitpid(pid2);
}

static unsigned long int killTest() {
  for (int i = 0; i < sonsSize; i++) {
    kill(sonsVec[i]);
  }
  sonsSize = 0;
  return 0;
}

static void test1() {
  int i = 0;
  while (i < 7) {
    printf("  1  ");
    wait(30);
    i++;
  }
  printf(" test 1 done ");
  return;
}

static void test2() {
  int i = 0;
  while (i < 7) {
    printf("  2  ");
    wait(30);
    i++;
  }
  printf(" test 2 done ");
}

void sonTest();
static unsigned long int pipeTest() {
  int fd[2];
  pipe(fd);
  unsigned long int sonPid =
      setProcess("sonTest", (mainf)sonTest, 0, NULL, HIGHP);
  dup(sonPid, fd[1], STD_IN);
  dup(sonPid, fd[0], STD_OUT);
  runProcess(sonPid);
  wait(10);

  printf("(F) writing 'Lenia' to pipe\n");
  write(fd[1], "Lenia", 6);
  wait(10);

  printf("(F) reading from pipe\n");
  char buff[50] = {0};
  read(fd[0], buff, 49);
  printf("(F) string read: %s.\n", buff);
  waitpid(sonPid);
  closeFD(fd[0]);
  closeFD(fd[1]);
  return 0;
}

void sonTest() {
  char buff[50] = {0};
  read(0, buff, 10);
  printf("(Son from stdin To stdout) string read: %s", buff);
  return;
}

static int getPriority(char* val) {
  int priority;
  if (strCmp("HIGHP", val) == 0) {
    priority = HIGHP;
  } else if (strCmp("MIDP", val) == 0) {
    priority = MIDP;
  } else {
    priority = LOWP;
  }
  return priority;
}

static void doNothing(int argc) { wait(argc); }

static unsigned long int dummy() {
  int maxTime = 1000;
  int priority = getPriority(argv[1]);
  int time = atoi(argv[2]);
  if (time > maxTime) time = maxTime;
  createProcess("dummy", (mainf)doNothing, time, NULL, priority);
  return 0;
}

static unsigned long int nice() {
  int pid = atoi(argv[1]);
  int priority = getPriority(argv[2]);
  niceCall(pid, priority);
  return 0;
}

static unsigned long int producer() {
  unsigned long int pid = setProcess("Producer", (mainf)producerProc, 0, NULL, MIDP);
  sonsVec[sonsSize++] = pid;
  return pid;
}

static unsigned long int consumer() {
  unsigned long int pid = setProcess("Consumer", (mainf)consumerProc, 0, NULL, MIDP);
  sonsVec[sonsSize++] = pid;
  return pid;
}

static void producerProc() {
  int times = 0;
  while (times < 10) {
    wait(15);
    printf("%d\n", times++);
  }
}

static void consumerProc() {
  int times = 0;
  char buff[50] = {0};
  while (times < 10) {
    read(STD_IN, buff, 49);
    printf("(%d) string read: %s\n", times++, buff);
  }
}