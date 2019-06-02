#include <stdint.h>
#include "include/videoModule.h"
#include "include/SYSCall.h"
#include "include/processModule.h"

void clearScreen() {
  int x, y;
  getSize(&x, &y);
  systemCall((uint64_t)ERASESCREEN, 0, (uint64_t)y, 0, 0, 0);
  resetCursor();
}

void resetCursor() {
  systemCall((uint64_t)RESETCURSOR, 0, 0, 0, 0, 0);
}

void deleteChar() {
  char d = '\b';
  systemCall((uint64_t)WRITE, (uint64_t)STD_OUT, (uint64_t)&d, 1, 0, 0);
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

void eraseScreen(int y1, int y2) {
  systemCall((uint64_t)ERASESCREEN, (uint64_t)y1, (uint64_t)y2, 0, 0, 0);
}
