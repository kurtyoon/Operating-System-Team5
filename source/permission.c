#include "../include/main.h"

//file, directory permission
//permission[9] => [0~2] : User's permission, [3~5] : Group's permission, [6~8] : Other's permission


//modeToPermssion
//dirNode's mode 값을 사용하여, dirNode의 권한을 permission[]에 설정.
//이 때, 각 자릿수를 binary로 변환하여 permission[]에 저장.
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

//permission[]을 사용하여 해당 file directory permission print.
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

//dirNode의 권한을 확인하고 주어진 mode에 따라 r,w,x를 각각 확인함.
//이 때, 'userList->currnet'와 'dirNode'의 UID(user_id), GID(group_id)을 비교하여, 권한을 확인한다.
//권한 O, SUCCESS ; 권한 X, FAIL
int checkPermission(DirectoryNode *dirNode, char mode) {
    //usrList에 
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