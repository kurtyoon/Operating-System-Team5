#include "../include/main.h"

int modeToPermission(DirectoryNode *dirNode) {
    char buf[4];
    int tmp;
    int j;

    for (int i = 0; i < 9; i++) {
        dirNode->permission.permission[i] = 0;
    }
    sprintf(buf, "%d", dirNode->permission.mode);

    for (int i = 0; i < 3; i++) {
        tmp = buf[i] - '0';
        j = 2;
        while (tmp) {
        dirNode->permission.permission[3 * i + j] = tmp%2;
        tmp /= 2;
        j--;
        }
    }
    return SUCCESS;
}

void printPermission(DirectoryNode *dirNode) {
    char rwx[4] = "rwx";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (dirNode->permission.permission[3 * i + j] == 1) {
                printf("%c", rwx[j]);
            }
            else {
                printf("-");
            }
        }
    }
}

int checkPermission(DirectoryNode *dirNode, char mode) {
    if (usrList->current->id.UID == 0) return SUCCESS;
    if (usrList->current->id.UID == dirNode->id.UID) {
        if (mode == 'r') {
            if (dirNode->permission.permission[0] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'w') {
            if (dirNode->permission.permission[1] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'x') {
            if (dirNode->permission.permission[2] == 0) return FAIL;
            else return SUCCESS;
        }
    } else if (usrList->current->id.GID == dirNode->id.GID) {
        if (mode == 'r') {
            if(dirNode->permission.permission[3] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'w') {
            if(dirNode->permission.permission[4] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'x') {
            if(dirNode->permission.permission[5] == 0) return FAIL;
            else return SUCCESS;
        }
    }
    else {
        if(mode == 'r') {
            if (dirNode->permission.permission[6] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'w') {
            if (dirNode->permission.permission[7] == 0) return FAIL;
            else return SUCCESS;
        }
        if (mode == 'x'){
            if(dirNode->permission.permission[8] == 0) return FAIL;
            else return SUCCESS;
        }
    }
    return FAIL;
}