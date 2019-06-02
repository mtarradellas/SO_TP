#ifndef PROCESSMODULE_H
#define PROCESSMODULE_H

#define HIGHP 250
#define MIDP 150
#define LOWP 50

#define STD_IN 0
#define STD_OUT 1

typedef struct tProcessData {
  unsigned long int pid;
  char* name;
  char* status;
  int memory;
  char* priority;
} tProcessData;

typedef int (*mainf)();

unsigned long int createProcess(char* name, int (*entry)(int, char**), int argc,
                                char** argv, int priority);
void kill(unsigned long int);
void getPS(tProcessData*** psVec, int* size);
void waitpid(unsigned long int pid);
void runProcess(unsigned long int pid);
unsigned long int setProcess(char* name, int (*entry)(int, char**), int argc,
                             char** argv, int priority);
void pipe(int fd[2]);
void dup(int pid, int fd, int pos);
void closeFD(int fd);

#endif
