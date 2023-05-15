#include "../include/main.h"

int Mode2Permission(DirectoryNode *dirNode) {
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
    return 0;
}

void PrintPermission(DirectoryNode *dirNode) {
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
