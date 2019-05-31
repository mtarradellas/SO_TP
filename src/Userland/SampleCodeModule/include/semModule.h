#ifndef SEM_MODULE_H
#define SEM_MODULE_H

#include <stddef.h>
#define MAX_SEM_ID 30

void semOpen(char id[MAX_SEM_ID], int start);
void semClose(char id[MAX_SEM_ID]);
void semWait(char id[MAX_SEM_ID]);
void semPost(char id[MAX_SEM_ID]);

#endif