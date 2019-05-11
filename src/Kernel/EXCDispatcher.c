#include <stdint.h>
#include "include/EXCDispatcher.h"
#include "include/lib.h"
#include "include/videoDriver.h"

#define ZERO 0
#define OPCODE 1

// Displays the status of the registers at the time of a 'Zero Division'
// exception
static void zero_division(uint64_t *sp);

// Displays the status of the registers at the time of an 'Invialid OPcode'
// exception
static void op_code(uint64_t *sp);

// Prints registers and their contents
static void printInfo(uint64_t *stackPointer);

/////////////////////////////////////////////////////////
void exceptionDispatcher(int exception, uint64_t *sp) {
  switch (exception) {
    case ZERO:
      zero_division(sp);
      break;
    case OPCODE:
      op_code(sp);
      break;
  }
  putStr("\n~~~ REBOOTING SYSTEM...\n");
  return;
}

static void zero_division(uint64_t *sp) {
  putStr("\nDivision by zero exception\n");
  printInfo(sp);
}

static void op_code(uint64_t *sp) {
  putStr("\nInvalid OPcode \n");
  printInfo(sp);
}

static void printInfo(uint64_t *sp) {
  char *reg[] = {"RAX ", "RBX ", "RCX ", "RDX ", "RBP ", "RDI ", "RSI ", "R8 ",
                 "R9 ",  "R10 ", "R11 ", "R12 ", "R13 ", "R14 ", "R15 "};
  int n, i, j;
  char buffer[11] = {0};
  
  for (i = 14, j = 0; i >= 0; i--, j++) {
    putStr(reg[i]);
    n = sp[j];
    char *b = decToStr(n, buffer);
    putStr(b);
    putStr("\n");
  }
  putStr("RSP ");
  putStr(decToStr((uint64_t)sp, buffer));
  putStr("\n");
}
