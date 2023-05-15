#include "../include/main.h"

boolean IsEmpty(Stack* stack) {
    if(stack->TopNode == NULL){
        return -1;
    }
    return 0;
}

Stack *InitializeStack() {
	Stack *returnStack = (Stack *)malloc(sizeof(Stack));

	if (!returnStack) {
        printf("error occurred, returnStack.\n");
        return NULL;
	}
    returnStack->TopNode = NULL;
	return returnStack;
}

int Push(Stack *dirStack, char *dirName) {
	StackNode *dirNode = (StackNode *)malloc(sizeof(StackNode));

	if (!dirStack || !dirNode) {
        printf("error occurred, dirStack.\n");
        return -1;
	}
	strncpy(dirNode->name, dirName, MAX_NAME);
	dirNode->LinkNode = dirStack->TopNode;
    dirStack->TopNode = dirNode;
    return 0;
}

char *Pop(Stack *dirStack) {
	StackNode *returnNode = NULL;
	if (!dirStack) {
        printf("error occurred, dirStack.\n");
        return NULL;
	}
	if (IsEmpty(dirStack) == -1) {
        printf("Stack Empty.\n");
        return NULL;
	}
	returnNode = dirStack->TopNode;
	dirStack->TopNode = returnNode->LinkNode;
	return returnNode->name;
}
