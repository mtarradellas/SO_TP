#include "include/philosophers.h"
#include "include/memoryModule.h"
#include "include/mutexModule.h"
#include "include/processModule.h"
#include "include/queue.h"
#include "include/semModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"

typedef struct philosopher {
  int id;
  int pid;
} philosopher_t;

static void printMenu();
static void philosopherCreate();
static int cmp(void* a, void* b);
static int philosopher(int argc, char** argv);
static void philosopherKill();
static void philosopherSelfdestruct();
static void philosophersKillAll();
static void printStatus();

static int philosophersQty;
static int killing;
static int id, indexToDie;
static philosopher_t philosophers[MAX_PHILOSOPHERS] = {{0}};
static queue_t thinkingList, eatingList;
void philosophersRun() {
  printMenu();
  mutexOpen("philosophers");
  mutexOpen("eating");
  philosophersQty = 0;
  semOpen("forks", 0);
  semOpen("inTable", 0);
  killing = 0;
  id = 0;
  indexToDie = 0;
  thinkingList = queueCreate(sizeof(int));
  eatingList = queueCreate(sizeof(int));

  int c;
  while ((c = getChar()) != QUIT) {
    switch (c) {
      case INC:
        philosopherCreate();
        break;
      case DEC:
        philosopherKill();
        break;
      default:
        break;
    }
  }
  philosophersKillAll();
  mutexClose("philosophers");
  mutexClose("eating");
  semClose("forks");
  semClose("inTable");
  printf("End of program\n");
}

static void printMenu() {
  printf("\nThe table can have a maximum of %d philosophers\n",
         MAX_PHILOSOPHERS);
  printf("Insert:\n");
  printf("    %c to create a philosopher\n", INC);
  printf("    %c to kill a philosopher\n", DEC);
  printf("    %c to quit\n\n", QUIT);
}

static void philosopherCreate() {
  mutexLock("philosophers");
  if (philosophersQty >= MAX_PHILOSOPHERS) {
    printf("\nThe max capacity for philosophers has been reached.\n");
    mutexUnlock("philosophers");
    return;
  }
  if (killing == 1) {
    printf(
        "\nPlease wait for the philosopher to die before creating a new "
        "one\n\n");
        mutexUnlock("philosophers");
        return;
  }
  semPost("forks");
  int index = id++;
  philosopher_t phi = {};
  phi.pid =
      createProcess("philosopher", (mainf)philosopher, index, NULL, HIGHP);
  phi.id = index;
  indexToDie = index;
  philosophers[philosophersQty++] = phi;
  if (philosophersQty > 1) semPost("inTable");
  mutexUnlock("philosophers");

  printf("\nphilosopher %d has been created :)\n\n", index);

  mutexLock("eating");
  queueOffer(thinkingList, &index);
  mutexUnlock("eating");
}
static int cmp(void* a, void* b) {
  int left = *((int*)a);
  int right = *((int*)b);
  return right - left;
}
static int philosopher(int argc, char** argv) {
  int i = argc;

  while (1) {
    wait(1);
    mutexLock("philosophers");
    if (killing && i == indexToDie) {
      mutexUnlock("philosophers");
      philosopherSelfdestruct();
    }
    mutexUnlock("philosophers");
    semWait("inTable");
    semWait("forks");
    semWait("forks");

    mutexLock("eating");
    queueRemove(thinkingList, &cmp, &i);
    queueOffer(eatingList, &i);
    printStatus();
    mutexUnlock("eating");

    wait(30);
    semPost("forks");
    semPost("forks");
    semPost("inTable");

    mutexLock("eating");
    queueRemove(eatingList, &cmp, &i);
    queueOffer(thinkingList, &i);
    mutexUnlock("eating");
  }
  return 0;
}

static void philosopherKill() {
  mutexLock("philosophers");
  if (philosophersQty == 0) {
    printf("\nAll philosophers are already dead...\n");
    mutexUnlock("philosophers");
    return;
  }
  if (philosophersQty == 1) {
    printf(
        "\nplease create at least 2 philosophers before starting "
        "to kill them\n");
    mutexUnlock("philosophers");
    return;
  }

  if (killing == 0) {
    printf("\nasked a philosopher to kill himself >:)\n\n");
    killing = 1;
  } else {
    printf("\nwait for the philosopher to die before killing another one\n\n");
  }

  mutexUnlock("philosophers");
}

static void philosopherSelfdestruct() {
  mutexLock("philosophers");
  philosophersQty--;
  indexToDie = (philosophers[philosophersQty - 1]).id;
  philosopher_t philosopherToDie = philosophers[philosophersQty];
  mutexUnlock("philosophers");
  semWait("inTable");
  semWait("forks");
  mutexLock("eating");
  queueRemove(thinkingList, &cmp, &(philosopherToDie.id));
  queueRemove(eatingList, &cmp, &(philosopherToDie.id));
  mutexUnlock("eating");
  printf("\nphilosopher %d has died :(\n\n", philosopherToDie.id);
  killing = 0;
  kill(philosopherToDie.pid);
}

static void philosophersKillAll() {
  mutexLock("philosophers");
  for (int i = 0; i < philosophersQty; i++) {
    kill((philosophers[i]).pid);
  }

  queueFree(eatingList);
  queueFree(thinkingList);

  mutexUnlock("philosophers");
}
static int printList(queue_t queue) {
  int usedSpaces = 0;
  queueResetIter(queue);
  int next;
  while (queueGetNext(queue, &next) == 0) {
    if (next < 10) {
      printf("%d  ", next);
    } else {
      printf("%d ", next);
    }
    usedSpaces += 3;
  }
  return usedSpaces;
}
static void printStatus() {
  printf("Eating: ");
  int maxSpaces = (3 * MAX_PHILOSOPHERS / 2) + 1;
  int usedSpaces = printList(eatingList);

  for (int i = 0; i < maxSpaces - usedSpaces; i++) {
    printf(" ");
  }

  printf("Thinking: ");
  usedSpaces = printList(thinkingList);
  maxSpaces = 3 * (MAX_PHILOSOPHERS - 3);

  for (int i = 0; i < maxSpaces - usedSpaces; i++) {
    printf(" ");
  }

  printf("Total: %d", philosophersQty);
  printf("\n");
  wait(1);
}
