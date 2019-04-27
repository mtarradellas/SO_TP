#ifndef INTERRUPTIONS_H_
#define INTERRUPTIONS_H_

#include <IDTLoader.h>

// Timer tick
void _irq00Handler(void);

// Keyboard
void _irq01Handler(void);


// Exceptions
void _exception0Handler(void);
void _exceptionInvalidOpcodeHandler(void);

void _syscall_handler(void);

void _cli(void);
void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

//Finishes CPU process.
void haltcpu(void);

#endif /* INTERRUPS_H_ */
