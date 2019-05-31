#include "include/prodCon.h"
#include "include/videoModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/processModule.h"
#include "include/memoryModule.h"
#include "include/semModule.h"
#include "include/mutexModule.h"

#define INITPROD 2
#define INITCONS 2

typedef enum {
  EXIT,
  ADDPROD,
  ADDCONS,
  DELPROD,
  DELCONS,
  INVCOM
}prodconcom;

void _cli();
void _sti();

void createShowProc();
void showProc();
void createInitProd(int n);
void createInitCons(int n);
void createProd();
void createCons();
void deleteProd();
void deleteCons();
void producer();
void consumer();
void printInitScreen();
int getcmd(char* cmd);
static unsigned long int ps();

static int products[10] = {0};
static int prod[5] = {0};
static int cons[5] = {0};
static int products_size;
static int prod_size;
static int cons_size;
static int showProcPid;

static int on = 1;

char action[50];
void startProdCon() {
  on = 1;
  for (int i = 0; i < 10; i++) {
    products[i] = 0;
  }
  products_size = 0;
  prod_size = 0;
  cons_size = 0;
  printInitScreen();
  setCursor(250, 100);
  mutexOpen("prodConBuff");
  semOpen("prodConItems", 0);
  createShowProc();
  createInitProd(INITPROD);
  createInitCons(INITCONS);
  while(on) {
    clearBuffer(action);
    scan(action);
    int cmd = getcmd(action);
    switch(cmd) {
      case EXIT:
          on = 0;
          break;
      case ADDPROD:
          if (prod_size < 5) {
            createInitProd(1);
          }
          break;
      case ADDCONS:
          if (cons_size < 5) {
            createInitCons(1);
          }
          break;
      case DELPROD:
          if (prod_size > 0) {
            deleteProd();
          }
          break;
      case DELCONS:
          if (cons_size > 0) {
            deleteCons();
          }
          break;
      case INVCOM:
          break;
    }
  }
  kill(showProcPid);
  for(int i = 0; i < prod_size; i++) {
    kill(prod[i]);
  }
  for(int i = 0; i < cons_size; i++) {
    kill(cons[i]);
  }
  mutexClose("prodConBuff");
  semClose("prodConItems");
  clearScreen();
}

int getcmd(char* command) {
  if (!strCmp("q", command)) return EXIT;
  if (!strCmp("addp", command)) return ADDPROD;
  if (!strCmp("addc", command)) return ADDCONS;
  if (!strCmp("delc", command)) return DELCONS;
  if (!strCmp("delp", command)) return DELPROD;
  return INVCOM;
}

void printInitScreen() {
  clearScreen();
  printf("Type addp to add producer\n");
  printf("Type delp to delete producer\n");
  printf("Type addc to add consumer\n");
  printf("Type delc to delete consumer\n");
  printf("Type q to exit\n");
}

void createInitProd(int n) {
  for(int i = 0; i < n; i++) {
    createProd();
  }
}

void createInitCons(int n) {
  for(int i = 0; i < n; i++) {
    createCons();
  }
}

void createShowProc() {
  showProcPid = createProcess("showProc", (mainf)showProc, 0, NULL, HIGHP);
}

void showProc() {
  printf("PRODUCTS ARRAY:\n");
  int x, y;
  int x1, y1;
  int x2, y2;
  getCursor(&x, &y);
  x += 200;
  y += 50;
  while(1) {
    setCursor(x, y);
    //mutexLock("prodConBuff");
    for (int i = 0; i < 10; i++) {
      printf(" %d ", products[i]);
    }
    //mutexUnlock("prodConBuff");
    printf("\n\nProducers: %d | Consumers: %d\n",prod_size, cons_size);
    getCursor(&x1, &y1);
    //ps();
    getCursor(&x2, &y2);
    wait(10);
    eraseScreen(y1, y2);
  }
}

static unsigned long int ps() {
  tProcessData** psVec;
  int size;
  getPS(&psVec, &size);

  printf("\nPID     Status     Memory    Priority     Name\n");
  for (int i = 0; i < size; i++) {
    printf("%d       %s    %d      %s       %s\n", psVec[i]->pid,
           psVec[i]->status, psVec[i]->memory, psVec[i]->priority,
           psVec[i]->name);
    free(psVec[i]->name);
    free(psVec[i]);
  }
  free(psVec);
  return 0;
}

void createProd() {
  prod[prod_size] = createProcess("prod", (mainf)producer, 0, NULL, MIDP);
  prod_size++;
}

void createCons() {
  cons[cons_size] = createProcess("cons", (mainf)consumer, 0, NULL, MIDP);
  cons_size++;
}


void deleteProd() {
  kill(prod[prod_size-1]);
  prod_size--;
}

void deleteCons() {
  kill(cons[cons_size-1]);
  cons_size--;
}

void producer() {
  while(1) {
    mutexLock("prodConBuff");
    if (products_size < 10) {
      products[products_size]++;
      products_size++;
      semPost("prodConItems");
    }
    mutexUnlock("prodConBuff");
    wait(30);
  }
}

void consumer() {
  while(1) {
    semWait("prodConItems");
    mutexLock("prodConBuff");
    if (products_size > 0) {
      products[products_size-1]--;
      products_size--;
    }
    mutexUnlock("prodConBuff");
    wait(30);
  }
}