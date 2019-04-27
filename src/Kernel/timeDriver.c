#include "timeDriver.h"
#include "timeDriverASM.h"
#include "videoDriver.h"

void _sti();

static unsigned long ticks = 0;

void timeHandler() {
	ticks++;
}

int ticksElapsed() {
	return ticks;	
}

void wait(int n) {
	_sti();
	unsigned long t = ticksElapsed() + n;
	while(ticksElapsed() < t);
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