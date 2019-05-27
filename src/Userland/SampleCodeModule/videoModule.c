#include <stdint.h>
#include "include/videoModule.h"
#include "include/SYSCall.h"

void clearScreen() { systemCall((uint64_t)WRITE, (uint64_t)CLEAR, 0, 0, 0, 0); }

void deleteChar() {
  char d = '\b';
  systemCall((uint64_t)WRITE, (uint64_t)CHARACTER, (uint64_t)&d, 0, 0, 0);
}

void drawCircle(Color color, int radio, int x, int y) {
  systemCall((uint64_t)DRAWCIRCLE, (uint64_t)&color, (uint64_t)&radio,
             (uint64_t)&x, (uint64_t)&y, 0);
}

void drawRectangle(Color color, int x, int y, int b, int h) {
  systemCall((uint64_t)DRAWRECTANGLE, (uint64_t)&color, (uint64_t)&b,
             (uint64_t)&h, (uint64_t)&x, (uint64_t)&y);
}

void getSize(int* x, int* y) {
  systemCall((uint64_t)GETSCREENSIZE, (uint64_t)x, (uint64_t)y, 0, 0, 0);
}

void getCursor(int* x, int* y) {
  systemCall((uint64_t)GETCURSOR, (uint64_t)x, (uint64_t)y, 0, 0, 0);
}

void setCursor(int x, int y) {
  systemCall((uint64_t)SETCURSOR, (uint64_t)&x, (uint64_t)&y, 0, 0, 0);
}

void drawChar(char c, int x, int y, Color color) {
  systemCall((uint64_t)WRITE, (uint64_t)DRAWCHAR, (uint64_t)&c, (uint64_t)&x, (uint64_t)&y, (uint64_t)&color); 
}

void eraseScreen(int x1, int y1, int x2, int y2) {
  systemCall((uint64_t)ERASESCREEN, (uint64_t)x1, (uint64_t)y1, (uint64_t)x2, (uint64_t)y2, 0);
}