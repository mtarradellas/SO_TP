#ifndef PROCESSMODULE_H
#define PROCESSMODULE_H

#define HIGHP 100
#define MIDP 60
#define LOWP 40

unsigned long int createProcess(char* name, int (*entry)(int, char**), int argc, char** argv, int priority);

#endif