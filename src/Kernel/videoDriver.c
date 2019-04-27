/*  
    ********* VIDEO CONTROL *********
*/

#include "videoDriver.h"
#include "charFont.h"
#include <naiveConsole.h>
#include <lib.h>


#define CHAR_WIDTH 8
#define CHAR_HEIGHT 11

#define LINE_SPACE 2// SPACE BETWEEN LINES
#define CHAR_SPACE 0// SPACE BETWEEN CHARACTERS
#define MARGIN 4
/*
    get the contents for the struct 
    location obtained from Pure64 System Variables 
    VBEModeInfoBlock:   equ 0x0000000000005C00  
*/
vesaModeStruct videoStruct = (vesaModeStruct)0x5C00;

//cursor position(coordenates within the screen)
static int cursor_x = MARGIN;
static int cursor_y = MARGIN;
/*
    common colors
*/
static Color black = {0, 0, 0}; //used for background
static Color white = {255, 255, 255}; // default font color


void getCursor(int * x, int * y) {
    *x = cursor_x;
    *y = cursor_y;
}

void setCursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

/*
    plots pixel in the desired coordinates (x, y) and with the color provided
*/
void plotPixel(int x, int y, Color color){

    int where = y * videoStruct->pitch + x * (videoStruct->BitsPerPixel/8);       
    char* screen = (char* ) (videoStruct->PhysBasePtr);
    screen[where] = color.blue;
    screen[where + 1] = color.green;  
    screen[where + 2] = color.red;

}


// draws char by pixel acording to  font array located in font.h (not for user use)
void drawChar(char c, int x, int y, Color color) {
    
    if (c > 31 && c < 127){//our font map has characters starting at 31 on the ascii                      
        char * charDesign = charMap((char)c - 1);
        for (int j=0; j < CHAR_HEIGHT; j++){
            for (int i = 0; i < CHAR_WIDTH; i++) {
                if ((1<<i) & charDesign[j]){   
                    plotPixel((CHAR_WIDTH - i) + x, j + y, color);
                }
                else{
                    plotPixel((CHAR_WIDTH - i) + x, j + y, black);
                }
            }
        }
    }
}

/* 
    for user use
    checks if it is a special character (\n or \b)
    calls draw char
    reaccomodates coordenates of cursor
*/
void printChar(char c, Color color) {   
    if (c == '\n') {
       newLine();
    }
    else if (c == '\b') {
        delChar();
    
    }
    else{
        drawChar(c, cursor_x, cursor_y, color);
        cursor_x += CHAR_WIDTH + CHAR_SPACE;
    }
    accomodateScreen();
}

void putStr(char * str) {
    int i = 0;
    char c;
    while ( (c = str[i++])) {
        printChar(c, white);
    }
}

/*
    erases char in current line, if there is none it goes to the previous line
*/
void delChar() {

    cursor_x -= (CHAR_WIDTH + CHAR_SPACE);
    accomodateScreen(); //corrects cursor if it ended in an invalid position after erasing character
    drawChar(' ', cursor_x, cursor_y, black);
}

void newLine() {
    cursor_y += CHAR_HEIGHT + LINE_SPACE;
    cursor_x = MARGIN;
}

/*
    checks if the cursor is out of boundaries and acts accordingly

    scrolls screen up if it has run out of space
*/
void accomodateScreen() {
    if (cursor_x + CHAR_WIDTH + CHAR_SPACE + MARGIN > videoStruct->XResolution) {  
            newLine();
    } 
    else if (cursor_x < MARGIN) {
        cursor_x = videoStruct->XResolution - MARGIN;
        cursor_y -= (LINE_SPACE + CHAR_HEIGHT);
    }
    if (cursor_y + CHAR_HEIGHT + LINE_SPACE + MARGIN > videoStruct->YResolution) { 
            scrollUp();
    }
    else if (cursor_y < MARGIN) {
        cursor_y = MARGIN;
    }
}

void scrollUp() {
    int whereFrom = (MARGIN + 3*(CHAR_HEIGHT + LINE_SPACE)) * videoStruct->pitch + MARGIN * (videoStruct->BitsPerPixel/8);       
    char * source = (char *) (videoStruct->PhysBasePtr + whereFrom);
    int whereTo = MARGIN * videoStruct->pitch + MARGIN * (videoStruct->BitsPerPixel/8);       
    char * dest = (char *) (videoStruct->PhysBasePtr + whereTo);
    int size = (videoStruct->YResolution)*(videoStruct->XResolution)*3;
    memcpy(dest, source, size);
    cursor_x = MARGIN;
    cursor_y = videoStruct->YResolution - MARGIN - 3*(CHAR_HEIGHT + LINE_SPACE);
}

void clear() {
    for (int y = 0; y < videoStruct->YResolution; y++) {
        for (int x = 0; x < videoStruct->XResolution; x++) {
            plotPixel(x, y, black);
        }
    }
    cursor_x = MARGIN;
    cursor_y = MARGIN;
}

void getSize(int * x, int * y){
    *x = videoStruct->XResolution;
    *y = videoStruct->YResolution;

}

void drawBall(Color color, int radius, int x, int y){


    for(int i = -radius; i <= radius; i++){
        for(int j = -radius; j <= radius; j++){
            if(i*i+j*j <= radius*radius){
                plotPixel(x+i, y+j, color);
            }
        }
    }
}

void drawRectangle(Color color, int b, int h, int x, int y){
    for(int i = -b; i <= b; i++){
        for(int j = -h; j <= h; j++){
            plotPixel(x+i, y+j, color);
        }
    }
}