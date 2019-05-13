#include "stdint.h"
#include "include/processModule.h"
#include "include/SYSCall.h"

unsigned long int createProcess(char* name, int (*entry)(int, char**), int argc, char** argv, int priority) {
	unsigned long int pid = systemCall((uint64_t)CREATEPROC, (uint64_t)name, 
                                      (uint64_t)entry, (uint64_t)argc, (uint64_t)argv, (uint64_t)priority);
	return pid;
}

void kill(unsigned long int pid) {
  systemCall((uint64_t)KILL, pid, 0, 0, 0, 0);
}

void getPS(tProcessData*** psVec, int *size) {
  systemCall((uint64_t)PS, psVec, size, 0, 0, 0);
}


void waitpid(unsigned long int pid) {
  systemCall((uint64_t)WAITPID, pid, 0, 0, 0, 0);
}