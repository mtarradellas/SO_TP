#include "include/niceModule.h"
#include "include/SYSCall.h"
#include "include/processModule.h"

void niceCall(unsigned long int pid, int priority) {
  // if (pid >= 1) return;
  if (priority == HIGHP || priority == MIDP || priority == LOWP) {
    systemCall((uint64_t)NICE, (uint64_t)pid, (uint64_t)priority, 0, 0, 0);
  }
}
