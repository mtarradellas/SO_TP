#ifndef SYSCDispatcher_H_
#define SYSCDispatcher_H_

#define READ 0
#define WRITE 1
#define WAIT 2
#define BALL 3
#define RECTANGLE 4
#define BEEP 5
#define CURSOR 6

#define GET 0
#define SET 1

#define KEY 0
#define TIME 1
#define SCREENSIZE 2
#define DO 3
#define UNDO 4

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
void read(uint64_t mode, uint64_t p1, uint64_t p2);

// Handles Write syscalls
void write(uint64_t mode, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4);

// Calls Time Driver
void getTime(unsigned int * dest, uint64_t time);

// Call beep functions
char beepon();
void beepoff();

#endif
