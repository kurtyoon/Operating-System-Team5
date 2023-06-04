#include "../include/main.h"

DirectoryNode *dirExistence(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *returnNode = NULL;

    returnNode = dirTree->current->firstChild;
    while (returnNode) {
        if (!strcmp(returnNode->name, dirName) && returnNode->type == type) break;
        returnNode = returnNode->nextSibling;
    }
    return returnNode;
}

char *getDir(char *dirPath) {
    char *tmpPath = (char *)malloc(MAX_DIR);
    char *str = NULL;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];

    strncpy(tmp, dirPath, MAX_DIR);
    str = strtok(dirPath, "/");
    while (str) {
        strncpy(tmp2, str, MAX_DIR);
        str = strtok(NULL, "/");
    }
    strncpy(tmpPath, tmp, strlen(tmp) - strlen(tmp2) - 1);
    tmpPath[strlen(tmp) - strlen(tmp2) - 1] = '\0';
    return tmpPath;
}

void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";

    tmpNode = dirNode->parent;
    if (tmpNode == dirTree->root) strcpy(tmp, "/");
    else {
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (isEmpty(dirStack) == FALSE) {
            strcat(tmp, "/");
            strcat(tmp, popStack(dirStack));
        }
    }
    fprintf(Dir, " %s\n", tmp);
}

void writeDirNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    fprintf(Dir, "%s %c %d ", dirNode->name, dirNode->type, dirNode->permission.mode);
    fprintf(Dir, "%d %d %d %d %d %d %d", dirNode->SIZE, dirNode->id.UID, dirNode->id.GID, dirNode->date.month, dirNode->date.day, dirNode->date.hour, dirNode->date.minute);

    if (dirNode == dirTree->root) fprintf(Dir, "\n");
    else getPath(dirTree, dirNode, dirStack);
    if (dirNode->nextSibling) writeDirNode(dirTree, dirNode->nextSibling, dirStack);
    if (dirNode->firstChild) writeDirNode(dirTree, dirNode->firstChild, dirStack);
}

int readDirNode(DirectoryTree *dirTree, char *tmp) {
    DirectoryNode *newNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;
    char *str;

    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->parent = NULL;

    str = strtok(tmp, " ");
    strncpy(newNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    newNode->type = str[0];
    str = strtok(NULL, " ");
    newNode->permission.mode = atoi(str);
    modeToPermission(newNode);
    str = strtok(NULL, " ");
    newNode->SIZE = atoi(str);
    str = strtok(NULL, " ");
    newNode->id.UID = atoi(str);
    str = strtok(NULL, " ");
    newNode->id.GID = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.month = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.day = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.hour = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.minute = atoi(str);

    str = strtok(NULL, " ");
    if (str) {
        str[strlen(str) - 1] = '\0';
        changePath(dirTree, str);
        newNode->parent = dirTree->current;
        if (!dirTree->current->firstChild) dirTree->current->firstChild = newNode;
        else {
            tmpNode = dirTree->current->firstChild;
            while (tmpNode->nextSibling) tmpNode = tmpNode->nextSibling;
            tmpNode->nextSibling = newNode;
        }
    } else {
        dirTree->root = newNode;
        dirTree->current = dirTree->root;
    }
    return 0;
}
