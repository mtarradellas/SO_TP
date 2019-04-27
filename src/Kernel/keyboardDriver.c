#include <keyboardDriver.h>
char SHIFT_ON = 0;
char CAPSLOCK_ON = 0;
int size = 0;
char buffer[BUFFER_SIZE] = {0};
int index = 0;
int getIndex = 0;


static unsigned char keys[] = { 0, ESC, '1', '2', '3', '4', '5', 
'6', '7', '8', '9', '0', '-', '=', BACKSPACE,TAB, 'q', 'w','e', 'r', 't',
'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0,'a', 's', 'd', 'f', 'g', 'h',
'j', 'k', 'l', ';', '\'', 0, LEFT_SHIFT,'\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
'.', '/', RIGHT_SHIFT, '*', LEFT_ALT,SPACE, CAPSLOCK,F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, 
NUM_LOCK, SCROLL_LOCK,HOME, UP, REPAG,'-', LEFT, 0, RIGHT, '+', END, DOWN,AVPAG, INSERT, 
SUPR, 0, 0, 0, F11, F12 };


static unsigned char keysWithShift[] = { 0, ESC, '!', '@', '#', '$', '%','^', '&', '*',
 '(', ')', '_', '+', BACKSPACE,TAB,'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
 'O', 'P', '{', '}', ENTER, 0,'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','"',
 '|', LEFT_SHIFT, '>','Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0,LEFT_ALT,SPACE, 
 CAPSLOCK,F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, NUM_LOCK,SCROLL_LOCK,HOME, UP, REPAG, '-', LEFT,
 0, RIGHT, '+', END, DOWN,AVPAG, INSERT, SUPR, 0, 0, '>', F11, F12 };

void keyboardHandler()
{
    unsigned char scanCode = _getKeyPress();
    unsigned char ascii;
    switch(scanCode)
    {
        case LEFT_SHIFT_SC:
                SHIFT_ON = 1;
                break;
        case RIGHT_SHIFT_SC:
                SHIFT_ON = 1;
                break;
        case CAPSLOCK_SC:
                CAPSLOCK_ON = !CAPSLOCK_ON;
    }
    if (isNotPressed(scanCode))
        { //shift is released
        if (scanCode == LEFT_SHIFT_RELEASE || scanCode == RIGHT_SHIFT_RELEASE) 
            {
                SHIFT_ON = 0;
            }
        return;
        }

    if((SHIFT_ON && CAPSLOCK_ON) || (!SHIFT_ON && !CAPSLOCK_ON))
        ascii = keys[scanCode]; 
    else {
        ascii = keysWithShift[scanCode];
    }
    addToBuffer(ascii);
   }

void addToBuffer(unsigned char ascii)
    {
        buffer[index % BUFFER_SIZE] = ascii;
        index++;
        if (size < BUFFER_SIZE) size++;
    }

int isNotPressed(unsigned char c)
    {
        return ( c & 0x80 );//return 1 if first bit is 1
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
