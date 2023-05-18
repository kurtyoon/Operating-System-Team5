#include "../include/main.h"

DirectoryNode *IsExistDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *returnNode = NULL;

    returnNode = dirTree->current->firstChild;
    while (returnNode) {
        if(!strcmp(returnNode->name, dirName) && returnNode->type == type) break;
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
        str  = strtok(NULL, "/");
    }
    strncpy(tmpPath, tmp, strlen(tmp) - strlen(tmp2) - 1);
    tmpPath[strlen(tmp) - strlen(tmp2) - 1] = '\0';
    return tmpPath;
}

void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";

    tmpNode = dirNode->parent;
    if (tmpNode == dirTree->root) {
        strcpy(tmp, "/");
    } else {
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

    if(dirNode == dirTree->root) {
        fprintf(Dir, "\n");
    } else {
        getPath(dirTree, dirNode, dirStack);
    }
    if (dirNode->nextSibling) {
        writeDirNode(dirTree, dirNode->nextSibling, dirStack);
    }
    if (dirNode->firstChild) {
        writeDirNode(dirTree, dirNode->firstChild, dirStack);
    }
}

int readDirNode(DirectoryTree *dirTree, char *tmp) {
    DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;
    char *str;

    NewNode->firstChild = NULL;
    NewNode->nextSibling = NULL;
    NewNode->parent = NULL;

    str = strtok(tmp, " ");
    strncpy(NewNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    NewNode->type = str[0];
    str = strtok(NULL, " ");
    NewNode->permission.mode = atoi(str);
    modeToPermission(NewNode);
    str = strtok(NULL, " ");
    NewNode->SIZE = atoi(str);
    str = strtok(NULL, " ");
    NewNode->id.UID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->id.GID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.month = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.day = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.hour = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.minute = atoi(str);

    str = strtok(NULL, " ");
    if (str) {
        str[strlen(str) - 1] = '\0';
        movePath(dirTree, str);
        NewNode->parent = dirTree->current;
        if (!dirTree->current->firstChild) {
            dirTree->current->firstChild = NewNode;
        } else {
            tmpNode = dirTree->current->firstChild;
            while (tmpNode->nextSibling) {
                tmpNode = tmpNode->nextSibling;
            }
            tmpNode->nextSibling = NewNode;
        }
    } else {
        dirTree->root = NewNode;
        dirTree->current = dirTree->root;
    }
    return 0;
}
