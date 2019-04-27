/* 
	****** 	Module for Pong 	******
*/


#ifndef PONGMODULE_H
#define PONGMODULE_H

#define LEFT 0
#define RIGHT 1

#define P1UP 119
#define P1DOWN 115

#define P2UP 11
#define P2DOWN 9

#define UP -45
#define DOWN 45

#include "videoModule.h"

typedef struct player{
	int points;
	int pos;
	int side;
} PlayerStruct;

typedef struct ball{
	int posX;
	int posY;
	int dirX;
	int dirY;
} BallStruct;

typedef PlayerStruct * Player;
typedef BallStruct * Ball;


// Prepares everything to start game
void startPong(); 

// Starts game
int play(Ball ball, Player p1, Player p2);

// Displays initial screen of game
void printInitScreen(Ball ball, Player p1, Player p2);

// Displays final screen in the case that the game ended on its own
void printWinScreen(int player);

// Prints player's racket
void printPlayer(Color color, Player player);

// Prints ball
void printBall(Color color, Ball ball);

// Prints screen in the post-goal mode
void printGoalScreen(int goal, Ball ball, Player p1, Player p2);

// Prints players' points
void printPoints(Player p1, Player p2);

// Prints game's limits
void printFrame();

// Moves player's racket
void movePlayer(Player p, int step);

// Moves ball
int moveBall(Ball ball, Player p1, Player p2);

// Used for when the ball is on the edge of the screen
int onEdge(Ball ball);

// Used for when a goal was made
int onGoal(Ball ball);

// Used for when the ball touches a racket
int onPlayer(Ball ball, Player p1, Player p2);

// Manages a goal
void scoreGoal(int goal, Ball ball, Player p1, Player p2);

// Recives an action from the player to move and executes it
void act(char com, Player p1, Player p2);
#endif