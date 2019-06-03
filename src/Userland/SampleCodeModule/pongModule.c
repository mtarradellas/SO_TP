#include "include/pongModule.h"
#include "include/soundModule.h"
#include "include/stdlib.h"
#include "include/timeModule.h"
#include "include/videoModule.h"

#define LEFT 0
#define RIGHT 1

#define P1UP 119
#define P1DOWN 115

#define P2UP 11
#define P2DOWN 9

#define UP -45
#define DOWN 45

#define PAUSE '\n'
#define RAINBOW 'r'

typedef struct player {
  int points;
  int pos;
  int side;
} PlayerStruct;

typedef struct ball {
  int posX;
  int posY;
  int dirX;
  int dirY;
} BallStruct;

typedef PlayerStruct* Player;
typedef BallStruct* Ball;

// Starts game
static int play(Ball ball, Player p1, Player p2);
// Displays initial screen of game
static void printInitScreen(Ball ball, Player p1, Player p2);
// Displays final screen in the case that the game ended on its own
static void printWinScreen(int player);
// Prints player's racket
static void printPlayer(Color color, Player player);
// Prints ball
static void printBall(Color color, Ball ball);
// Prints screen in the post-goal mode
static void printGoalScreen(int goal, Ball ball, Player p1, Player p2);
// Prints players' points
static void printPoints(Player p1, Player p2);
// Prints game's limits
static void printFrame();
// Moves player's racket
static void movePlayer(Player p, int step);
// Moves ball
static int moveBall(Ball ball, Player p1, Player p2);
// Used for when the ball is on the edge of the screen
static int onEdge(Ball ball);
// Used for when a goal was made
static int onGoal(Ball ball);
// Used for when the ball touches a racket
static int onPlayer(Ball ball, Player p1, Player p2);
// Manages a goal
static void scoreGoal(int goal, Ball ball, Player p1, Player p2);
// Recives an action from the player to move and executes it
static void act(char com, Player p1, Player p2);
// Prints pause symbol when paused
static void printPause();
// Erases pause symbol
static void delPause();

static int xResolution;
static int yResolution;

static Color white = {255, 255, 255};
static Color black = {0, 0, 0};
static Color grey = {128, 128, 128};
static Color red = {255, 0, 0};
static Color green = {0, 255, 0};
static Color blue = {0, 0, 255};

Color* rainbowColors;
int rainbow;

void startPong() {
  getSize(&xResolution, &yResolution);
  Color aux[] = {red, green, blue};
  rainbowColors = aux;

  PlayerStruct p1S = {0, yResolution / 2, 0};
  PlayerStruct p2S = {0, yResolution / 2, 1};
  BallStruct ballS = {xResolution / 2, yResolution / 2, 15, 17};

  Player p1 = &p1S;
  Player p2 = &p2S;
  Ball ball = &ballS;

  printInitScreen(ball, p1, p2);

  char* str =
      "\n       ~~WELCOME TO LENIAS PONG, PRESS ENTER OR R TO PLAY OR PRESS "
      "BACKSPACE TO QUIT. YOU MAY QUIT ANYTIME DURING GAME~~";
  putStr(str);

  char c;
  while ((c = getChar()) != '\b' && c != '\n' && c != 'r') {
  }
  if (c == '\b') {
    return;
  }
  rainbow = (c == 'r' ? 1 : 0);
  drawRectangle(black, xResolution / 2, 20, (xResolution / 2) - 60, 10);

  int exitStatus = play(ball, p1, p2);

  if (exitStatus == 0) {
    return;
  }
  printWinScreen(exitStatus);
  wait(25);
  return;
}

static int play(Ball ball, Player p1, Player p2) {
  int playing = 1;
  int exitStatus = 0;
  while (playing) {
    wait(1);
    char command = getChar();
    if (command == '\b') {
      playing = 0;
    }
    act(command, p1, p2);
    int goal = moveBall(ball, p1, p2);
    if (goal) {
      scoreGoal(goal, ball, p1, p2);
    }
    if (p1->points == 3) {
      exitStatus = 1;
      playing = 0;
    } else if (p2->points == 3) {
      exitStatus = 2;
      playing = 0;
    }
    printPoints(p1, p2);
  }
  return exitStatus;
}

static void act(char command, Player p1, Player p2) {
  switch (command) {
    case P1UP:
      movePlayer(p1, UP);
      break;
    case P1DOWN:
      movePlayer(p1, DOWN);
      break;
    case P2UP:
      movePlayer(p2, UP);
      break;
    case P2DOWN:
      movePlayer(p2, DOWN);
      break;
    case PAUSE:
      printPause();
      while (getChar() != '\n') {
      }
      delPause();
    case RAINBOW:
      rainbow = !rainbow;
    default:
      if (rainbow == 1) {
        printPlayer(rainbowColors[rand() % 3], p1);
        printPlayer(rainbowColors[rand() % 3], p2);
      }
      break;
  }
}

static int moveBall(Ball ball, Player p1, Player p2) {
  int goal = 0;
  printBall(black, ball);
  if (onEdge(ball)) {
    ball->dirY = -ball->dirY;
  }
  goal = onGoal(ball);
  if (onPlayer(ball, p1, p2)) {
    ball->dirX = -ball->dirX;
    if (ball->dirX > 0) {
      ball->dirX += 2;
    } else {
      ball->dirX -= 2;
    }
  }
  ball->posX += ball->dirX;
  ball->posY += ball->dirY;
  if (rainbow == 1) {
    printBall(rainbowColors[rand() % 3], ball);
    return goal;
  }
  printBall(white, ball);
  return goal;
}

static int onPlayer(Ball ball, Player p1, Player p2) {
  int touchRightX = 0;
  int touchLeftX = 0;
  if (ball->posX + ball->dirX + 10 >= xResolution - 30 - 2) {
    touchRightX = 1;
  } else if (ball->posX + ball->dirX - 10 <= 30 + 2) {
    touchLeftX = 1;
  }
  if (touchRightX) {
    if (ball->posY + ball->dirY <= p2->pos + 70 &&
        ball->posY + ball->dirY >= p2->pos - 70) {
      return 1;
    }
    return 0;
  } else if (touchLeftX) {
    if (ball->posY + ball->dirY <= p1->pos + 70 &&
        ball->posY + ball->dirY >= p1->pos - 70) {
      return 1;
    }
    return 0;
  }
  return 0;
}

static int onGoal(Ball ball) {
  if (ball->posX + ball->dirX + 10 >= xResolution - 2) {
    return 1;
  }
  if (ball->posX + ball->dirX - 10 <= 2) {
    return 2;
  }
  return 0;
}

static void scoreGoal(int goal, Ball ball, Player p1, Player p2) {
  if (goal == 1) {
    p1->points = p1->points + 1;
  } else {
    p2->points = p2->points + 1;
  }
  p1->pos = yResolution / 2;
  p2->pos = yResolution / 2;
  ball->posX = xResolution / 2;
  ball->posY = yResolution / 2;
  ball->dirX = 10;
  ball->dirY = 17;
  printGoalScreen(goal, ball, p1, p2);
  return;
}

static void printGoalScreen(int goal, Ball ball, Player p1, Player p2) {
  char* str = "       G O O O O O O O O O O O A A A A A L ! ! !";
  setCursor((xResolution / 2) - 300, yResolution / 2);
  putStr(str);
  doBeep();
  wait(15);
  noBeep();
  printInitScreen(ball, p1, p2);
}

static int onEdge(Ball ball) {
  if (ball->posY + ball->dirY + 10 >= yResolution - 2 ||
      ball->posY + ball->dirY - 10 <= 2) {
    return 1;
  }
  return 0;
}

static void movePlayer(Player p, int step) {
  int xPos = p->side == 0 ? 30 : xResolution - 30;
  int yPos =
      step < 0 ? (p->pos + 70) - abs(step / 2) : (p->pos - 70) + abs(step / 2);
  if (step > 0) {
    if (p->pos + step + 71 >= yResolution - 2) return;
  } else if (step < 0) {
    if (p->pos + step - 71 <= 2) return;
  }

  drawRectangle(black, xPos, yPos, 4, abs(step / 2));
  yPos = step > 0 ? (p->pos + 70) - abs(step / 2) + step
                  : (p->pos - 70) + abs(step / 2) + step;
  if (rainbow == 1) {
    drawRectangle(rainbowColors[rand() % 3], xPos, yPos, 4, abs(step / 2));
    p->pos = p->pos + step;
    return;
  }
  drawRectangle(white, xPos, yPos, 4, abs(step / 2));
  p->pos = p->pos + step;
}

static void printInitScreen(Ball ball, Player p1, Player p2) {
  clearScreen();
  printFrame();
  printPlayer(white, p1);
  printPlayer(white, p2);
  printBall(white, ball);
}

static void printWinScreen(int player) {
  setCursor((xResolution / 2) + 50, 300);
  char p[2];
  decToStr(player, p);
  putStr("PLAYER ");
  putStr(p);
  putStr(" WINS ! !");
}

static void printPlayer(Color color, Player p) {
  int xPos = p->side == 0 ? 30 : xResolution - 30;
  int yPos = p->pos;
  drawRectangle(color, xPos, yPos, 4, 70);
}

static void printBall(Color color, Ball b) {
  drawCircle(color, 10, b->posX, b->posY);
}

static void printFrame() {
  drawRectangle(white, xResolution / 2, 2, (xResolution / 2) - 2, 0);
  drawRectangle(white, xResolution / 2, yResolution - 2, (xResolution / 2) - 2,
                0);
  drawRectangle(white, 2, yResolution / 2, 1, (yResolution / 2) - 2);
  drawRectangle(white, xResolution - 2, yResolution / 2, 1,
                (yResolution / 2) - 2);
}

static void printPoints(Player p1, Player p2) {
  setCursor(50, 30);
  char points[2];
  decToStr(p1->points, points);
  putStr(points);
  setCursor(xResolution - 100, 30);
  decToStr(p2->points, points);
  putStr(points);
}

static void printPause() {
  drawRectangle(grey, (xResolution / 2) - 20, yResolution / 2, 6, 50);
  drawRectangle(grey, (xResolution / 2) + 20, yResolution / 2, 6, 50);
}

static void delPause() {
  drawRectangle(black, (xResolution / 2) - 20, yResolution / 2, 6, 50);
  drawRectangle(black, (xResolution / 2) + 20, yResolution / 2, 6, 50);
}
