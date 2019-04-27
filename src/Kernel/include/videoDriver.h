#ifndef videoDriver_h
#define videoDriver_h
#include <stdint.h>

/*
	Credit: Osdev.org
		user: Omarrx024
		** VESA tutorial **
*/

typedef struct __attribute__((packed)) ModeInfoBlock {
        uint16_t ModeAttributes;
        uint8_t WinAAttributes;
        uint8_t WinBAttributes;
        uint16_t WinGranularity;
        uint16_t WinSize;
        uint16_t WinSegmentA;
        uint16_t WinSegmentB;
        uint32_t WinRealFctPtr;
        uint16_t pitch; // Bytes per ScanLine.

        uint16_t XResolution;
        uint16_t YResolution;
        uint8_t XcharSize;
        uint8_t YcharSize;
        uint8_t NumberOfPlanes;
        uint8_t BitsPerPixel;
        uint8_t NumberOfBanks;
        uint8_t MemoryModel;
        uint8_t BankSize;
        uint8_t NumberOfImagePages;
        uint8_t ReservedPage;

        uint8_t RedMaskSize;
        uint8_t RedMaskPosition;
        uint8_t GreenMaskSize;
        uint8_t GreenMaskPosition;
        uint8_t BlueMaskSize;
        uint8_t BlueMaskPosition;
        uint8_t ReservedMaskSize;
        uint8_t ReservedMaskPosition;
        uint8_t DirectColorAttributes;

        uint32_t PhysBasePtr;  // Your LFB (Linear Framebuffer) address.
        uint32_t OffScreenMemOffset;
        uint16_t OffScreenMemSize;
} Vesa;

typedef struct ModeInfoBlock  * vesaModeStruct;

/*
	Data type color based on RGB composition
*/
typedef struct Color{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
} Color;




// Getter for cursor
void getCursor(int * x, int * y);

// Setter for cursor
void setCursor(int x, int y);

// Prints pixel to screen in the given coordinates
void plotPixel(int x, int y, Color color);

// Prints char to screen in the given coordinates
void drawChar(char c, int x, int y, Color color);

// Prints char to screen where the cursor is set
void printChar(char c, Color color);

// Prints string to screen where the cursor is set
void putStr(char * str);

// Deletes char where cursor is set
void delChar();

// Moves the cursor to the begining of the next line
void newLine();

// Corrects invalid cursor movements
void accomodateScreen();

// Clears screen, moves cursor to the begining
void clear();

// Getter of the screen measures
void getSize(int * x, int * y);

// Draws ball for pong in the given coordinates
void drawBall(Color color, int radius, int x, int y);

// Draws rectangle in the given coordinates
void drawRectangle(Color color, int b, int h, int x, int y);

// Deletes the first three lines in the screen and moves the rest up, creating more space
void scrollUp();

#endif