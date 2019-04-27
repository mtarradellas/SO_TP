#include "SYSCall.h"
#include "stdint.h"
#include "soundModule.h"

void doBeep(){
	systemCall((uint64_t)BEEP, (uint64_t)DO, 0, 0, 0, 0);
}

void noBeep(){
	systemCall((uint64_t)BEEP, (uint64_t)UNDO, 0, 0, 0, 0);
}