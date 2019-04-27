#ifndef SHELL_H
#define SHELL_H

#define INVCOM 0
#define HELP   1
#define CLEAR   2
#define TIME   3
#define PONG   4
#define ZERODIV   5
#define INVOPCODE   6
#define ALEPUTO   7
#define LENIA   8
#define SHELLSHOCK   9
#define EXIT   10

#define MAXLEN 256


// Initializes shell
void initShell();

//Gets command ready to use in a switch function
int getCommand(char * command);

// Executes the 'help' command. Displays help menu
void help();

// Executes the 'clear' command. Clears screen
void clear();

// Executes the 'time' command. Displays local time
void time();

// Executes the 'pong' command. Initializes the pong game
void pong();

// Executes the 'zerodiv' command. Triggers a Zero Division Exception
void zeroDiv();

// Executes the 'invopcode' command. Triggers an Invalid OP Code Exception
void invOpCode();

// Executes the 'lenia' command. Makes beep sound
void lenia();

// Executes the 'exit' command. Exits the kernel
void exit();

// Displays the message for when a command was not recognized
void invCom();

// Declaration of external function
void opCode();

#endif