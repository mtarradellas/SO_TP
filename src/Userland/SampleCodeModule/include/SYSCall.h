#ifndef systemCall_h
#define systemCall_h
#include <stdint.h>
#include "videoModule.h"

// SYSTEM CALLS
typedef enum {
  READ,
  WRITE,
  WAIT,
  GETTIME,
  GETSCREENSIZE,
  DRAWCIRCLE,
  DRAWRECTANGLE,
  BEEPON,
  BEEPOFF,
  GETCURSOR,
  SETCURSOR,
  MALLOC,
  REALLOC,
  FREE,
  CREATEPROC,
  PRINTNODE,
  KILL,
  PS,
  WAITPID
} Syscall;

// WRITE
typedef enum { CHARACTER, DRAWCHAR, CLEAR, STRING } Write;

// TIME
typedef enum {HOUR, MINUTE, SECOND} Time;

#define BUFFER_SIZE 256

// Triggers systemcall
unsigned int systemCall(uint64_t syscall, uint64_t p1, uint64_t p2, uint64_t p3,
                        uint64_t p4, uint64_t p5);

#endif
