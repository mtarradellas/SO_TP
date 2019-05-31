#include "include/keyboardDriver.h"
#include "lib.h"
#include "include/semaphore.h"

#define BUFFER_SIZE 256
#define LEFT_SHIFT_SC 42
#define RIGHT_SHIFT_SC 54
#define CAPSLOCK_SC 58
#define LEFT_SHIFT_RELEASE 170
#define RIGHT_SHIFT_RELEASE 182
#define CTRL 0
#define ALT 0
#define UP 11
#define RIGHT 0
#define LEFT 0
#define DOWN 9
#define ESC 0
#define BACKSPACE '\b'
#define TAB 0
#define ENTER '\n'
#define LEFT_SHIFT 0
#define RIGHT_SHIFT 0
#define LEFT_ALT 0
#define SPACE ' '
#define CAPSLOCK 0
#define F1 0
#define F2 0
#define F3 0
#define F4 0
#define F5 0
#define F6 0
#define F7 0
#define F8 0
#define F9 0
#define F10 0
#define F11 0
#define F12 0
#define NUM_LOCK 0
#define SCROLL_LOCK 0
#define INSERT 0
#define SUPR 0
#define HOME 0
#define END 0
#define REPAG 0
#define AVPAG 0

extern sem_t readSem;

// Returns code for pressed key
unsigned int _getKeyPress();

// Adds character to the buffer as an ascii value
static void addToBuffer(unsigned char c);

// Decides if the given code is on key pressed or key released
static int isNotPressed(unsigned char c);

char SHIFT_ON = 0;
char CAPSLOCK_ON = 0;
int size = 0;
char buffer[BUFFER_SIZE] = {0};
int index = 0;
int getIndex = 0;

static unsigned char keys[] = {
    0,        ESC,   '1',        '2',  '3',   '4',      '5',         '6',
    '7',      '8',   '9',        '0',  '-',   '=',      BACKSPACE,   TAB,
    'q',      'w',   'e',        'r',  't',   'y',      'u',         'i',
    'o',      'p',   '[',        ']',  ENTER, 0,        'a',         's',
    'd',      'f',   'g',        'h',  'j',   'k',      'l',         ';',
    '\'',     0,     LEFT_SHIFT, '\\', 'z',   'x',      'c',         'v',
    'b',      'n',   'm',        ',',  '.',   '/',      RIGHT_SHIFT, '*',
    LEFT_ALT, SPACE, CAPSLOCK,   F1,   F2,    F3,       F4,          F5,
    F6,       F7,    F8,         F9,   F10,   NUM_LOCK, SCROLL_LOCK, HOME,
    UP,       REPAG, '-',        LEFT, 0,     RIGHT,    '+',         END,
    DOWN,     AVPAG, INSERT,     SUPR, 0,     0,        0,           F11,
    F12};

static unsigned char keysWithShift[] = {
    0,        ESC,   '!',        '@',  '#',   '$',      '%',         '^',
    '&',      '*',   '(',        ')',  '_',   '+',      BACKSPACE,   TAB,
    'Q',      'W',   'E',        'R',  'T',   'Y',      'U',         'I',
    'O',      'P',   '{',        '}',  ENTER, 0,        'A',         'S',
    'D',      'F',   'G',        'H',  'J',   'K',      'L',         ':',
    '"',      '|',   LEFT_SHIFT, '>',  'Z',   'X',      'C',         'V',
    'B',      'N',   'M',        '<',  '>',   '?',      0,           0,
    LEFT_ALT, SPACE, CAPSLOCK,   F1,   F2,    F3,       F4,          F5,
    F6,       F7,    F8,         F9,   F10,   NUM_LOCK, SCROLL_LOCK, HOME,
    UP,       REPAG, '-',        LEFT, 0,     RIGHT,    '+',         END,
    DOWN,     AVPAG, INSERT,     SUPR, 0,     0,        '>',         F11,
    F12};

int keyboardHandler() {
  unsigned char scanCode = _getKeyPress();
  unsigned char ascii;
  switch (scanCode) {
    case LEFT_SHIFT_SC:
      SHIFT_ON = 1;
      break;
    case RIGHT_SHIFT_SC:
      SHIFT_ON = 1;
      break;
    case CAPSLOCK_SC:
      CAPSLOCK_ON = !CAPSLOCK_ON;
  }
  if (isNotPressed(scanCode)) {  // shift is released
    if (scanCode == LEFT_SHIFT_RELEASE || scanCode == RIGHT_SHIFT_RELEASE) {
      SHIFT_ON = 0;
    }
    return 0;
  }

  if ((SHIFT_ON && CAPSLOCK_ON) || (!SHIFT_ON && !CAPSLOCK_ON)) {
    ascii = keys[scanCode];
  } else {
    ascii = keysWithShift[scanCode];
  }
  addToBuffer(ascii);
  return 1;
}

static void addToBuffer(unsigned char ascii) {
  buffer[index % BUFFER_SIZE] = ascii;
  index++;
  if (size < BUFFER_SIZE) size++;
  semPost(readSem);
}

static int isNotPressed(unsigned char c) {
  return (c & 0x80);  // return 1 if first bit is 1
}

unsigned char getKey() {
  if (size <= 0) {
    return 0;
  }
  unsigned char key = buffer[getIndex%BUFFER_SIZE];
  getIndex++;
  size--;
  return key;
}