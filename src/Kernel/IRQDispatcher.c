//#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <naiveConsole.h>
#include <timeDriver.h>

void int20(void);
void int21(void);



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

void int20() {
	timeHandler();
}

void int21() {
	keyboardHandler();
}