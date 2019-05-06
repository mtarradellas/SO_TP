#include "timeDriver.h"
#include "timeDriverASM.h"

#include "videoDriver.h"
#include "lib.h"

void _sti();

static unsigned long ticks = 0;

void timeHandler() { ticks++; }

int ticksElapsed() { return ticks; }
//  N O   A N D A
void wait(int n) {
  _sti();
  //char buff[50];
  unsigned long t = ticks + n;
  //putStr("");
  //putStr(decToStr(t))
  while (ticks < t) {
  	//putStr("o");
  }
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
