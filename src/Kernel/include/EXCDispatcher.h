#ifndef EXCEPTDISP_H
#define EXCEPTDISP_H

// Handles 'Zero Division' and 'Invialid OPcode' exceptions
void exceptionDispatcher(int exception, uint64_t *sp);

// Displays the status of the registers at the time of a 'Zero Division' exception
void zero_division(uint64_t *sp);

// Displays the status of the registers at the time of an 'Invialid OPcode' exception
void op_code(uint64_t *sp);

// Prints registers and their contents
void printInfo(uint64_t* stackPointer);

// Transformes a decimal to string. used for displaying the registers
char * decToStr(int num, char * buffer);

// VideoDriver function used for displaying exceptions
void putStr(char * str);

#endif