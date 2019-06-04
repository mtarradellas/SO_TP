#include "include/stdlib.h"
#include <stdarg.h>
#include <stdint.h>
#include "include/SYSCall.h"
#include "include/processModule.h"
#include "include/memoryModule.h"

#define A 25214903917
#define C 11
#define M 281474976710656
#define MEM_BLOCK 50
#define MOD 50

char buffer[BUFFER_SIZE] = {0};

void printf(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char* parsedStr = malloc(strLen(fmt)+1);
  int idx = 0;
  int size = strLen(fmt)+1;

  int num;
  char* str;
  char aux2;
  char buff[20];
  while (*fmt) {
    if (*fmt != '%') {
      if (idx == size) {
        parsedStr = realloc(parsedStr, size + MEM_BLOCK);
        size += MEM_BLOCK;
      }
      parsedStr[idx++] = *fmt;
    } else {
      switch (*(fmt + 1)) {
        case 'd':
          num = va_arg(args, int);
          decToStr(num, buff);
          if (idx + strLen(buff) >= size) {
            parsedStr = realloc(parsedStr, size + MEM_BLOCK);
            size += MEM_BLOCK;
          }
          strCpy(parsedStr + idx, buff);
          idx += strLen(buff);
          break;
        case 's':
          str = va_arg(args, char*);
          if (idx + strLen(str) >= size) {
            parsedStr = realloc(parsedStr, size + MEM_BLOCK);
            size += MEM_BLOCK;
          }
          strCpy(parsedStr + idx, str);
          idx += strLen(str);
          break;
        case 'c':
          aux2 = va_arg(args, int);
          if (idx + 1 >= size) {
            parsedStr = realloc(parsedStr, size + MEM_BLOCK);
            size += MEM_BLOCK;
          }
          parsedStr[idx++] = aux2;
          break;
      }
      fmt++;
    }
    fmt++;
  }
  va_end(args);
  parsedStr[idx] = 0;
  putStr(parsedStr);
  free(parsedStr);
}

void putChar(char c) {
  systemCall((uint64_t)WRITE, (uint64_t)STD_OUT, (uint64_t)&c, 1, 0, 0);
}

void putDec(int i) {
  char buffer[11] = {0};
  decToStr(i, buffer);
}

void putStr(char* str) {
  systemCall((uint64_t)WRITE, (uint64_t)STD_OUT, (uint64_t)str,
             (uint64_t)strLen(str) + 1, 0, 0);
}

void read(int fd, char* buff, int bytes) {
  systemCall((uint64_t)READ, (uint64_t)fd, (uint64_t)buff, bytes, 0, 0);
}

void write(int fd, char* buff, int bytes) {
  systemCall((uint64_t)WRITE, (uint64_t)fd, (uint64_t)buff, bytes, 0, 0);
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
  systemCall((uint64_t)READ, (uint64_t)STD_IN, (uint64_t)&c, 1, 0, 0);
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

void scan(char* buffer) {
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

void strCpy(char* dest, char* source) {
  while (*source != 0) {
    *dest = *source;
    dest++;
    source++;
  }
}

int abs(int n) {
  if (n > 0) return n;
  return -n;
}

int strLen(char* str) {
  int len = 0;
  while (*str != 0) {
    len++;
    str++;
  }
  return len;
}

int atoi(char* str) {
  int res = 0;  // Initialize result

  // Iterate through all characters of input string and
  // update result
  for (int i = 0; str[i] != '\0'; ++i) res = res * 10 + str[i] - '0';

  // return result.
  return res;
}

void* memcpy(void* destination, const void* source, uint64_t length) {
  /*
   * memcpy does not support overlapping buffers, so always do it
   * forwards. (Don't change this without adjusting memmove.)
   *
   * For speedy copying, optimize the common case where both pointers
   * and the length are word-aligned, and copy word-at-a-time instead
   * of byte-at-a-time. Otherwise, copy by bytes.
   *
   * The alignment logic below should be portable. We rely on
   * the compiler to be reasonably intelligent about optimizing
   * the divides and modulos out. Fortunately, it is.
   */
  uint64_t i;

  if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
      (uint64_t)source % sizeof(uint32_t) == 0 &&
      length % sizeof(uint32_t) == 0) {
    uint32_t* d = (uint32_t*)destination;
    const uint32_t* s = (const uint32_t*)source;

    for (i = 0; i < length / sizeof(uint32_t); i++) d[i] = s[i];
  } else {
    uint8_t* d = (uint8_t*)destination;
    const uint8_t* s = (const uint8_t*)source;

    for (i = 0; i < length; i++) d[i] = s[i];
  }

  return destination;
}

unsigned long int var = 1;

void srand(unsigned long int seed);
void lcg(unsigned long int* x, unsigned long int a, int c, unsigned long int m);

void srand(unsigned long int seed) { var = seed; }

void lcg(unsigned long int* x, unsigned long int a, int c,
         unsigned long int m) {
  *x = (a * (*x) + c) % m;
}

unsigned long int rand() {
  lcg(&var, A, C, M);
  return var;
}

void splitString(char* buffer, char output[MAX_ARGUMENTS][MAXLEN], int argc) {
  int currentWord = 0;
  int j = 0;
  for (int i = 0; i < strLen(buffer) && currentWord < argc; i++) {
    output[currentWord][j++] = buffer[i];
    if (buffer[i] == ' ') {
      output[currentWord][j - 1] = 0;
      currentWord++;
      j = 0;
    }
  }
  output[currentWord][j] = 0;
}
