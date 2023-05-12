#ifndef MAIN_H
#define MAIN_H

#define DEFAULT printf("%c[%dm", 0x1B, 0)
#define BOLD printf("%c[%dm", 0x1B, 1)
#define WHITE printf("\x1b[37m")
#define BLUE printf("\x1b[34m")
#define GREEN printf("\x1b[32m")

#include "linked_tree.h"
#include "stack.h"

UserList *usrList;
Tree *Linux;
Stack *stack;
FILE *dirFile;
FILE *userFile;

#endif