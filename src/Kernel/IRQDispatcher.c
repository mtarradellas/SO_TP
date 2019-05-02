#include <stdint.h>
#include "keyboardDriver.h"
#include "timeDriver.h"
#include "scheduler.h"

static void int20(void);
static void int21(void);

void irqDispatcher(uint64_t irq, uint64_t rsp) {

	switch (irq) {
		case 0:
			int20();	//timer tick interruption
			break;
        case 1:
        	int21();	//keyboard interruption
            break;
	}
	return;
}

static void int20() {
	timeHandler();
	lottery(rsp);
}

static void int21() {
	keyboardHandler();
}