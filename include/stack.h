#ifndef STACK_H
#define STACK_H

typedef struct LinkedStack {
    char name[20];
    LinkedStack *next;
} LinkedStack;

typedef struct Stack {
    LinkedStack *top;
    int cnt;
} Stack;

bool isEmpty(Stack *stack);
Stack *createStack();
int push(Stack *stack, char *data);
char *pop(Stack *stack);

#endif