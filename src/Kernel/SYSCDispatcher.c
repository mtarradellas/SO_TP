#include <stdint.h>
#include "keyboardDriver.h"
#include "videoDriver.h"
#include "timeDriver.h"
#include "SYSCDispatcher.h"


void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5) {
	switch(syscall) {
		case READ:
			read(p1, p2, p3);
			break;
		case WRITE:
			write(p1, p2, p3, p4, p5);
			break;
		case WAIT:
			wait( *( (int*) p1) );
			break;
		case BALL:
			drawBall( *((Color*) p1), *((int*)p2), *((int*) p3), *((int*) p4));
			break;
		case RECTANGLE:
			drawRectangle(*((Color*) p1), *((int*) p2), *((int*) p3), *((int*) p4), *((int*) p5));
			break;
		case BEEP:
			switch(p1){
				case DO:
					beepon();
					break;
				case UNDO:
					beepoff();
					break;
			}
			break;
		case CURSOR:
			switch(p1){
				case GET:
					getCursor((int*)p2, (int*)p3);
					break;
				case SET:
					setCursor(*(int*)p2, *(int*)p3);
					break;
			}	
	}
}

void read(uint64_t mode, uint64_t p1, uint64_t p2) {
	unsigned int * t = (unsigned int *) p1;
	char * c = (char *) p1;
	switch(mode) {
		case TIME:
			getTime(t, p2);
			break;
		case KEY:
			*c = getKey();
			break;
		case SCREENSIZE:
			getSize((int*) p1, (int*) p2);
			break;
	}
}

void write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4) {
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


void getTime(unsigned int * t, uint64_t time) {
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