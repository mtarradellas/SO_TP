#include "videoModule.h"
#include "SYSCall.h"
#include "stdint.h"

void clearScreen() {
	systemCall((uint64_t)WRITE, (uint64_t)CLEAR, 0, 0, 0, 0);
}

void deleteChar() {
	char d = '\b';
	systemCall((uint64_t)WRITE, (uint64_t)CHARACTER,(uint64_t) &d, 0, 0, 0);
}

void drawBall(Color color, int radio, int x, int y){
	systemCall((uint64_t)BALL, (uint64_t) &color, (uint64_t)&radio, (uint64_t)&x, (uint64_t)&y, 0);
}

void drawRectangle(Color color, int x, int y, int b, int h){
	systemCall((uint64_t)RECTANGLE, (uint64_t)&color, (uint64_t) &b, (uint64_t)&h, (uint64_t) &x, (uint64_t)&y);
}

void getSize(int * x, int * y){
	systemCall((uint64_t)READ, (uint64_t)SCREENSIZE, (uint64_t)x, (uint64_t)y, 0, 0);
}

void getCursor(int * x, int * y) {
	systemCall((uint64_t) CURSOR, (uint64_t) GET, (uint64_t) x, (uint64_t) y, 0, 0);
}

void setCursor(int x, int y) {
	systemCall((uint64_t) CURSOR, (uint64_t) SET, (uint64_t) &x, (uint64_t) &y, 0, 0);
}