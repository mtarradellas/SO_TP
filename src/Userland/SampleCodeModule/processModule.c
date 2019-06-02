#include "stdint.h"
#include "include/processModule.h"
#include "include/SYSCall.h"

unsigned long int createProcess(char* name, int (*entry)(int, char**), int argc, char** argv, int priority) {
	unsigned long int pid = systemCall((uint64_t)CREATEPROC, (uint64_t)name, 
                                      (uint64_t)entry, (uint64_t)argc, (uint64_t)argv, (uint64_t)priority);
	return pid;
}

void kill(unsigned long int pid) {
  systemCall((uint64_t)KILL, (uint64_t)pid, 0, 0, 0, 0);
}

void getPS(tProcessData*** psVec, int *size) {
  systemCall((uint64_t)PS, (uint64_t)psVec, (uint64_t)size, 0, 0, 0);
}


void waitpid(unsigned long int pid) {
  systemCall((uint64_t)WAITPID, (uint64_t)pid, 0, 0, 0, 0);
}

void runProcess(unsigned long int pid) {
  systemCall((uint64_t)RUNPROCESS, (uint64_t)pid, 0, 0, 0, 0);
}

unsigned long int setProcess(char* name, int (*entry)(int, char**), int argc, char** argv, int priority) {
  unsigned long int pid = systemCall((uint64_t)SETPROCESS, (uint64_t)name, 
                                      (uint64_t)entry, (uint64_t)argc, (uint64_t)argv, (uint64_t)priority);
  return pid;
}

void pipe(int fd[2]) {
  systemCall((uint64_t)PIPE, (uint64_t)fd, 0, 0, 0, 0);
}

void dup(int pid, int fd, int pos) {
  systemCall((uint64_t)DUP, (uint64_t)pid, (uint64_t)fd, (uint64_t)pos, 0, 0);
}