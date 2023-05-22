#include "../include/main.h"

//current_directory의 Path 출력.
//current_directory ~ parent_directory 까지 Stack에 저장한 후,
//Pop()을 통해, 순차적으로 출력 
int printPath(DirectoryTree *dirTree, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;

    tmpNode = dirTree->current;
    if (tmpNode == dirTree->root) printf("/");
    else {
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (isEmpty(dirStack) == FALSE) {
            printf("/");
            printf("%s", popStack(dirStack));
        }
    }
    printf("\n");
    return SUCCESS;
}

int ft_pwd(DirectoryTree *dirTree, Stack *dirStack, char *command) {
    char *str = NULL;
    if (!command) {
        printPath(dirTree, dirStack);
    } else if(command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("pwd: pwd\n");
            printf("    Print the name of the current working directory.\n\n");
            printf("Options:\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("pwd: Invalid option\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            } else {
            printf("pwd: Unrecognized option -- '%s'\n", str);
            printf("Try 'pwd --help' for more information.\n");
            return FAIL;
            }
        }
    }
    return SUCCESS;
}