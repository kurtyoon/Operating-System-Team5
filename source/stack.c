#include "../include/main.h"

//Stack IsEmpty
boolean isEmpty(Stack* stack) {
    if(stack->topNode == NULL){
        return TRUE;
    }
    return FALSE;
}

//Stack 초기화.
Stack *initStack() {
	Stack *newStack = (Stack *)malloc(sizeof(Stack));

	if (!newStack) {
        return NULL;
	}
    newStack->topNode = NULL;
	return newStack;
}

//Stack에 dirName 추가.
int pushStack(Stack *dirStack, char *dirName) {
	StackNode *dirNode = (StackNode *)malloc(sizeof(StackNode));

	if (!dirStack || !dirNode) {
        return FALSE;
	}
	strncpy(dirNode->name, dirName, MAX_NAME);
	dirNode->nextNode = dirStack->topNode;
    dirStack->topNode = dirNode;
    return TRUE;
}

//Stack에서 dirName pop
char *popStack(Stack *dirStack) {
	StackNode *returnNode = NULL;
	if (!dirStack || (isEmpty(dirStack) == TRUE)) {
        return NULL;
	}
	returnNode = dirStack->topNode;
	dirStack->topNode = returnNode->nextNode;
	return returnNode->name;
}
