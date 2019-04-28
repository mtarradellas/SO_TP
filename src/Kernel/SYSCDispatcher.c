#include <stdint.h>
#include "keyboardDriver.h"
#include "videoDriver.h"
#include "timeDriver.h"
#include "SYSCDispatcher.h"

typedef uint64_t(*SystemCall)();
SystemCall syscall_array[] = {
    (SystemCall) _read,
    (SystemCall) _write,
    (SystemCall) _wait,
    (SystemCall) _getTime,
    (SystemCall) _getScreenSize,
    (SystemCall) _drawCircle,
    (SystemCall) _drawRectangle,
    (SystemCall) _beepon,
    (SystemCall) _beepoff,
    (SystemCall) _getCursor,
    (SystemCall) _setCursor

};

void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5){
  syscall_array[syscall](p1, p2, p3, p4, p5);
}

void _read(char *c) {
	*c = getKey();
}

void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4) {
	switch(mode) {
		case CHARACTER:
			printChar(*((char*) p1), *((Color*) p2));
			break;
		case DRAWCHAR:
			drawChar(*((char*) p1), *((int*) p3), *((int*) p4), *((Color*) p2));
		case CLEAR:
			clear();
			break;
		case STRING:
			putStr((char*)p1);
			break;
	}
}

void _wait(int * sec) {
	wait(*sec);
}

void _getTime(unsigned int * t, uint64_t time) {
	switch(time) {
		case HOUR:
			*t = getHour();
			break;
		case MINUTE:
			*t = getMinute();
			break;
		case SECOND:
			*t = getSecond();
			break;
	}
}

void _getScreenSize(int *x, int *y) {
	getScreenSize(x, y);
}

void _drawCircle(Color *color, int *radius, int *x, int *y) {
	drawCircle(*color, *radius, *x, *y);
}

void _drawRectangle(Color *color, int *b, int *h, int *x, int *y) {
	drawRectangle(*color, *b, *h, *x, *y);
}

void _beepon() {
	beepon();
}

void _beepoff() {
	beepoff();
}

void _getCursor(int *x, int *y) {
	getCursor(x, y);
}

void _setCursor(int *x, int *y) {
	setCursor(*x, *y);
}