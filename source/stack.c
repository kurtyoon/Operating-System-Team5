#include "../include/linked_tree.h"

bool isEmpty(Stack *stack) {
    if (stack->top == NULL) return -1;
    return 0;
}

Stack *createStack() {
    Stack *newStack = (Stack *)malloc(sizeof(Stack));
    if (!newStack) return NULL;
    newStack->top = NULL;
    return newStack;
}

int push(Stack *stack, char *data) {
    LinkedStack *newNode = (LinkedStack *)malloc(sizeof(LinkedStack));

    if (!stack || !newNode) return -1;
    strncpy(newNode->name, data, MAX_NAME);
    stack->top = newNode;

    return 0;
}

char *pop(Stack *stack) {
    LinkedStack *tmpNode = NULL;
    if (!stack) return NULL;
    if (isEmpty(stack) == -1) return NULL;

    tmpNode = stack->top;
    stack->top = tmpNode->next;
    return tmpNode->name;
}
