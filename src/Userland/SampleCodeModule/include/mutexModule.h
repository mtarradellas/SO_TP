#ifndef MUTEX_MODULE_H
#define MUTEX_MODULE_H

#include <stddef.h>
#define MAX_MUTEX_ID 15

void mutexOpen(char id[MAX_MUTEX_ID]);
void mutexClose(char id[MAX_MUTEX_ID]);
void mutexLock(char id[MAX_MUTEX_ID]);
void mutexUnlock(char id[MAX_MUTEX_ID]);

#endif
