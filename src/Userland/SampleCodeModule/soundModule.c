#include <stdint.h>
#include "include/soundModule.h"
#include "include/SYSCall.h"

void doBeep() { systemCall((uint64_t)BEEPON, 0, 0, 0, 0, 0); }

void noBeep() { systemCall((uint64_t)BEEPOFF, 0, 0, 0, 0, 0); }
