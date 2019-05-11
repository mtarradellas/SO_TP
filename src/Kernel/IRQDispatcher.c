#include <stdint.h>
#include "keyboardDriver.h"
#include "scheduler.h"
#include "timeDriver.h"

static void int20(uint64_t rsp);
static void int21(void);

void irqDispatcher(uint64_t irq, uint64_t rsp) {
  switch (irq) {
    case 0:
      int20(rsp);  // timer tick interruption
      break;
    case 1:
      int21();  // keyboard interruption
      break;
  }
  return;
}

static void int20(uint64_t rsp) {
  timeHandler();
  lottery(rsp);
}

static void int21() { 
  //if(keyboardHandler() == 1) {
  //  signalAddedKey();
  //}
  keyboardHandler();
}
