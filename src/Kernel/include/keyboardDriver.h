#ifndef KEYBOARD_H
#define KEYBOARD_H

// Manages calls from the keyboard interruption handler
void keyboardHandler();

// Returns a char from buffer
unsigned char getKey();

#endif
