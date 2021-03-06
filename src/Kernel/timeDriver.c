#include "include/timeDriver.h"
#include "include/timeDriverASM.h"
#include "include/videoDriver.h"
#include "include/lib.h"

void _sti();

static unsigned long ticks = 0;

void timeHandler() { ticks++; }

int ticksElapsed() { return ticks; }
void wait(int n) {
  _sti();
  unsigned long t = ticks + n;
  while (ticks < t) {}
}

unsigned int getHour() {
  unsigned int t = _getHour();
  return t;
}

unsigned int getMinute() {
  unsigned int t = _getMinute();
  return t;
}

unsigned int getSecond() {
  unsigned int t = _getSecond();
  return t;
}
