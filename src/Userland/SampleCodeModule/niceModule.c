#include <stdint.h>
#include "stdlib.h"
#include "videoModule.h"
#include "SYSCall.h"
#include "processModule.h"


void _cli();
void _sti();

void nice(char* priority, int PID){
    if(strCom(priority,HIGHP))   
      systemCall((uint64_t)NICE, (uint64_t)HIGHP, (uint64_t)PID, 0, 0, 0);
    else if(strCom(priority,MIDP))
      systemCall((uint64_t)NICE, (uint64_t)MIDP, (uint64_t)PID, 0, 0, 0);
    else if(strCom(priority,LOWP))
      systemCall((uint64_t)NICE, (uint64_t)LOWP, (uint64_t)PID, 0, 0, 0);

    return;

}

