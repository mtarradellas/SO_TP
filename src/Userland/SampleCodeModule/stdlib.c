#include <stdarg.h>
#include <stdint.h>
#include "include/SYSCall.h"
#include "include/stdlib.h"

char buffer[BUFFER_SIZE] = {0};

void printf(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int aux;
  char* str;
  char aux2;
  while (*fmt) {
    if (*fmt != '%') {
      putChar(*fmt);
    } else {
      switch (*(fmt + 1)) {
        case 'd':
          aux = va_arg(args, int);
          putDec(aux);
          break;
        case 's':
          str = va_arg(args, char*);
          while (*str) {
            putChar(*str);
            str++;
          }
          break;
        case 'c':
          aux2 = va_arg(args, int);
          putChar(aux2);
          break;
      }
      fmt++;
    }
    fmt++;
  }
  va_end(args);
}

void putChar(char c) {
  Color white = {255, 255, 255};
  systemCall((uint64_t)WRITE, (uint64_t)CHARACTER, (uint64_t)&c,
             (uint64_t)&white, 0, 0);
}

void putDec(int i) {
  char buffer[11] = {0};
  char* b = decToStr(i, buffer);
  printf("%s", b);
}

void putStr(char* str) {
  systemCall((uint64_t)WRITE, (uint64_t)STRING, (uint64_t)str, 0, 0, 0);
}

char* decToStr(int num, char* buffer) {
  char const digit[] = "0123456789";
  char* p = buffer;
  if (num < 0) {
    *p++ = '-';
    num *= -1;
  }
  int shifter = num;
  do {  // Move to where representation ends
    ++p;
    shifter = shifter / 10;
  } while (shifter);
  *p = '\0';
  do {  // Move back, inserting digits as u go
    *--p = digit[num % 10];
    num = num / 10;
  } while (num);
  return buffer;
}

char getChar() {
  char c;
  systemCall((uint64_t)READ, (uint64_t)&c, 0, 0, 0, 0);
  return c;
}

void scanAndPrint(char* buffer) {
  char c;
  char* p = buffer;
  int idx = 0;
  while ((c = getChar()) != '\n') {
    if ((c > 31 && c < 127) || c == '\b' || c == '\n') {
      if (c == '\b' && idx > 0) {
        deleteChar();
        p--;
        idx--;
      } else if (c != '\b') {
        putChar(c);
        *p = c;
        p++;
        idx++;
      }
    }
  }
  *p = 0;
}

void clearBuffer(char* buffer) {
  char* b = buffer;
  while (*b) {
    *b = 0;
    b++;
  }
}

int strCmp(char* a, char* b) {
  while (*a && *b) {
    if (*a > *b) return 1;
    if (*a < *b) return -1;
    a++;
    b++;
  }
  if (*a) return 1;
  if (*b) return -1;
  return 0;
}

int abs(int n) {
  if (n > 0) return n;
  return -n;
}
