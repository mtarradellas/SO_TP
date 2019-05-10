#ifndef KEYBOARD_H
#define KEYBOARD_H

// Manages calls from the keyboard interruption handler
// Returns 0 if no key was added to buffer, 1 if a key was added
int keyboardHandler();

// Returns a char from buffer
unsigned char getKey();

#endif
