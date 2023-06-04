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
            dirNode->permission.permission[3 * i + j] = tmp % 2;
            tmp /= 2;
            j--;
        }
    }
    return SUCCESS;
}

void printPermission(DirectoryNode *dirNode) {
    const char *rwx[2] = {"-", "rwx"};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int index = dirNode->permission.permission[3 * i + j];
            printf("%s", rwx[index]);
        }
    }
}

int checkPermission(DirectoryNode *dirNode, char mode) {
    if (usrList->current->id.UID == 0) return SUCCESS;
    int offset = (usrList->current->id.UID == dirNode->id.UID) ? 0 : ((usrList->current->id.GID == dirNode->id.GID) ? 3 : 6);

    if (mode == 'r') {
        if (dirNode->permission.permission[offset] == 0) return FAIL;
    } else if (mode == 'w') {
        if (dirNode->permission.permission[offset + 1] == 0) return FAIL;
    } else if (mode == 'x') {
        if (dirNode->permission.permission[offset + 2] == 0) return FAIL;
    }

    return SUCCESS;
}
