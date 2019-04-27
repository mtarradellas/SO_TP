#ifndef TIMEDriver_h
#define TIMEDriver_h


// Increments ticks for each timer tick interruption
void timeHandler();

// Returns current ticks
int ticksElapsed();

//Get time
unsigned int getHour();
unsigned int getMinute();
unsigned int getSecond();

// Waits n ticks
void wait(int n);
#endif