#include "../include/linked_tree.h"

TreeNode *IsExistDir(Tree *dirTree, char *dirName, char type) {
    TreeNode *returnNode = NULL;

    returnNode = dirTree->current->firstChild;

    while (returnNode) {
        if (strcmp(returnNode->dir->name, dirName) == 0 && returnNode->dir->type == type) break;
        returnNode = returnNode->nextSibling;
    }
    return returnNode;
}

int ModeToPermission(TreeNode *dirNode) {
    char buf[4];
    int tmp;
    int j;

    for (int i = 0; i < 9; i++) dirNode->dir->permission[i] = 0;
    sprintf(buf, "%d", dirNode->dir->mode);
    for (int i = 0; i < 3; i++) {
        tmp = buf[i] - '0';
        j = 2;
        while (tmp) {
            dirNode->dir->permission[3 * i + j] = tmp % 2;
            tmp /= 2;
            j--;
        }
    }
    return 0;
}

int ReadNode(Tree *dirTree, char *tmp) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    TreeNode *tmpNode = NULL;
    char *str;

    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->parent = NULLL;

    str = strtok(tmp, " ");
    strncpy(newNode->dir->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    newNode->dir->type = str[0];
    newNode->dir->mode = atoi(str);
    ModeToPermission(newNode);
    str = strtok(NULL, " ");
    newNode->dir->SIZE = atoi(str);
    str = strtok(NULL, " ");
    newNode->id->UID = atoi(str);
    str = strtok(NULL, " ");
    newNode->id->GID = atoi(str);
    str = strtok(NULL, " ");
    newNode->date->month = atoi(str);
    str = strtok(NULL, " ");
    newNode->date->day = atoi(str);
    str = strtok(NULL, " ");
    newNode->date->hour = atoi(str);
    str = strtok(NULL, " ");
    newNode->date->minute = atoi(str);

    str = strtok(NULL, " ");
    if (str) {
        str[strlen(str) - 1] = '\0';
        MovePath(dirTree, str);
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

Tree *LoadDir() {
    Tree *dirTree = (Tree *)malloc(sizeof(Tree));
    char tmp[MAX_LENGTH];

    dirFile = fopen("Directory.txt", "r");

    while (fgets(tmp, MAX_LENGTH, dirFile)) {
        ReadNode(dirTree, tmp);
    }
    fclose(dirFile);
    dirTree->current = dirTree->root;
    return dirTree;
}

int MoveCurrent(Tree *dirTree, char *dirPath) {
    TreeNode *tmpNode = NULL;

    if (strcmp(dirPath, ".") == 0) {
    } else if (strcmp(dirPath, "..") == 0) {
        if (dirTree->current != dirTree->root) {
            dirTree->current = dirTree->current->parent;
        }
    } else {
        tmpNode = IsExistDir(dirTree, dirPath, 'd');
        if (tmpNode) {
            dirTree->current = tmpNode;
        } else return -1;
    }
    return 0;
}

int MovePath(Tree *dirTree, char *dirPath) {

    TreeNode *tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char *str = NULL;
    int value = 0;

    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;

    if (strcmp(dirPath, "/") == 0) {
        dirTree->current = dirTree->root;
    } else {
        if (strncmp(dirPath, "/", 1) == 0) {
            if (strtok(dirPath, "/") == NULL) return -1;
        }
        dirTree->current = dirTree->root;

        str = strtok(tmpPath, "/");
        while (str) {
            value = MoveCurrent(dirTree, str);
            if (value) {
                dirTree->current = tmpNode;
                return -1;
            }
            str = strtok(NULL, "/");
        }
    }
    return 0;
}