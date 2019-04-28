#include <stdint.h>
#include <keyboardDriver.h>
#include <timeDriver.h>

static void int20(void);
static void int21(void);

void irqDispatcher(uint64_t irq) {

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
}

static void int21() {
	keyboardHandler();
}