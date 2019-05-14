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

void createShowProc();
void shProc();
void createInitProd();
void createInitCons();
void createProd();
void createCons();
void deleteProd();
void deleteCons();
void producer();
void consumer();
void printInitScreen();
int getcmd(char* cmd);


static int products[10] = {0};
static int prod[2] = {0};
static int cons[2] = {0};
static int products_size;
static int prod_size;
static int cons_size;
int shp;

static int on = 1;

char action[50];
void startProdCon() {
  printInitScreen();
  printf("Type addp to add producer\n");
  printf("Type delp to delete producer\n");
  printf("Type addc to add consumer\n");
  printf("Type delp to delete consumer\n");
  printf("Press backspapce to exit\n");
  wait(20);
  setCursor(250, 100);
  mutexOpen("producer");
  semOpen("consumer", 0);
  createShowProc();
  createInitProd();
  createInitCons();
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
            createProd();
          }
          break;
      case ADDCONS:
          if (cons_size < 5) {
            createCons();
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
  kill(shp);
  for(int i = 0; i < prod_size; i++) {
    kill(prod[i]);
  }
  for(int i = 0; i < cons_size; i++) {
    kill(cons[i]);
  }
  mutexClose("producer");
  semClose("consumer");
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
}

void createInitProd() {
  for(int i = 0; i < INITPROD; i++) {
    createProd();
  }
}

void createInitCons() {
  for(int i = 0; i < INITPROD; i++) {
    createCons();
  }
}

void createShowProc() {
  shp = createProcess("shp", (mainf)shProc, 0, NULL, HIGHP);
}

void shProc() {
  printf("PRODUCTS ARRAY:\n");
  int x;
  int y;
  getCursor(&x, &y);
  x += 200;
  y += 50;
  while(1) {
    setCursor(x, y);
    for (int i = 0; i < 10; i++) {
      printf(" %d ", products[i]);
    }
    printf("\n\nProducers: %d | Consumers: %d\n",prod_size, cons_size);
  }
}

void createProd() {
  prod[prod_size] = createProcess("prod", (mainf)producer, prod_size, NULL, MIDP);
  prod_size++;
}

void createCons() {
  cons[cons_size] = createProcess("cons", (mainf)consumer, cons_size, NULL, MIDP);
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

void producer(int id) {
  while(1) {
    mutexLock("producer");
    if (products_size < 10) {
      products[products_size]++;
      products_size++;
      semPost("consumer");
    }
    mutexUnlock("producer");
    wait(30);
  }
}

void consumer(int id) {
  while(1) {
    mutexLock("producer");
    semWait("consumer");
    if (products_size > 0) {
      products[products_size-1]--;
      products_size--;
    }
    mutexUnlock("producer");
    wait(30);
  }
}