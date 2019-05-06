#include <stdint.h>
#include "include/SYSCDispatcher.h"
#include "include/keyboardDriver.h"
#include "include/timeDriver.h"
#include "include/videoDriver.h"
#include "include/memoryManager.h"

// SYSTEM CALLS
#define READ 0
#define WRITE 1
#define WAIT 2
#define GETTIME 3
#define GETSCREENSIZE 4
#define DRAWCIRCLE 5
#define DRAWRECTANGLE 6
#define BEEPON 7
#define BEEPOFF 8
#define GETCURSOR 9
#define SETCURSOR 10
#define MALLOC 11
#define REALLOC 12
#define FREE 13


// WRITE
#define CHARACTER 0
#define DRAWCHAR 1
#define CLEAR 2
#define STRING 3

// TIME
#define HOUR 0
#define MINUTE 1
#define SECOND 2

void beepon();
void beepoff();

static void _read(char *c);
static void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3,
                   uint64_t p4);
static void _getTime(unsigned int *dest, uint64_t time);
static void _wait(int *sec);
static void _getScreenSize(int *x, int *y);
static void _drawCircle(Color *color, int *radius, int *x, int *y);
static void _drawRectangle(Color *color, int *b, int *h, int *x, int *y);
static void _getCursor(int *x, int *y);
static void _setCursor(int *x, int *y);
static void _beepon();
static void _beepoff();
static void _malloc(void** dest, size_t size);
static void _realloc(void* src, size_t size, void** dest);
static void _free(void* src);

typedef uint64_t (*SystemCall)();
SystemCall syscall_array[] = {
    (SystemCall)_read,          (SystemCall)_write,
    (SystemCall)_wait,          (SystemCall)_getTime,
    (SystemCall)_getScreenSize, (SystemCall)_drawCircle,
    (SystemCall)_drawRectangle, (SystemCall)_beepon,
    (SystemCall)_beepoff,       (SystemCall)_getCursor,
    (SystemCall)_setCursor,     (SystemCall)_malloc,
    (SystemCall)_realloc,       (SystemCall)_free
};
void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3,
                       uint64_t p4, uint64_t p5) {
  syscall_array[syscall](p1, p2, p3, p4, p5);
}

static void _read(char *c) { *c = getKey(); }

static void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3,
                   uint64_t p4) {
  switch (mode) {
    case CHARACTER:
      printChar(*((char *)p1), *((Color *)p2));
      break;
    case DRAWCHAR:
      drawChar(*((char *)p1), *((int *)p3), *((int *)p4), *((Color *)p2));
    case CLEAR:
      clear();
      break;
    case STRING:
      putStr((char *)p1);
      break;
  }
}

static void _wait(int *sec) { wait(*sec); }

static void _getTime(unsigned int *t, uint64_t time) {
  switch (time) {
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

static void _getScreenSize(int *x, int *y) { getScreenSize(x, y); }

static void _drawCircle(Color *color, int *radius, int *x, int *y) {
  drawCircle(*color, *radius, *x, *y);
}

static void _drawRectangle(Color *color, int *b, int *h, int *x, int *y) {
  drawRectangle(*color, *b, *h, *x, *y);
}

static void _beepon() { beepon(); }

static void _beepoff() { beepoff(); }

static void _getCursor(int *x, int *y) { getCursor(x, y); }

static void _setCursor(int *x, int *y) { setCursor(*x, *y); }

static void _malloc(void** dest, size_t size) { *dest = malloc(size); }

static void _realloc(void* src, size_t size, void**dest) { *dest = realloc(src, size); }

static void _free(void* src) { free(src); }