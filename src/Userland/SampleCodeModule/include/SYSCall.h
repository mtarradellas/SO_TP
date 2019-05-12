#ifndef systemCall_h
#define systemCall_h
#include <stdint.h>
#include "videoModule.h"

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
#define CREATEPROC 14
#define PRINTNODE 15
#define KILL 16
#define PS 17


// WRITE
#define CHARACTER 0
#define DRAWCHAR 1
#define CLEAR 2
#define STRING 3

// TIME
#define HOUR 0
#define MINUTE 1
#define SECOND 2

#define BUFFER_SIZE 256

// Triggers systemcall
unsigned int systemCall(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3,
                        uint64_t p4, uint64_t p5);

#endif
