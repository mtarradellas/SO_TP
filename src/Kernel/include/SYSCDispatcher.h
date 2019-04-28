#ifndef SYSCDispatcher_H_
#define SYSCDispatcher_H_

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

#define CHARACTER 0
#define DRAWCHAR 1
#define CLEAR 2
#define STRING 3

#define HOUR 0
#define MINUTE 1
#define SECOND 2

// Handles General Systemcalls
void syscallDispatcher(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5);

// Handles Read syscalls
void _read(char *c);

// Handles Write syscalls
void _write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4);

// Calls Time Driver
void _getTime(unsigned int * dest, uint64_t time);

void _wait(int * sec);
void _getScreenSize(int *x, int *y);
void _drawCircle(Color *color, int *radius, int *x, int *y);
void _drawRectangle(Color *color, int *b, int *h, int *x, int *y);
void _getCursor(int *x, int *y);
void _setCursor(int *x, int *y);

// Call beep functions
void _beepon();
void _beepoff();

char beepon();
void beepoff();

#endif
