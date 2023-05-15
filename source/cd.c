#include "../include/main.h"

int Movecurrent(DirectoryTree* dirTree, char* dirPath) {
    DirectoryNode* tmpNode = NULL;

    if (!strcmp(dirPath,".")) {
    } else if (!strcmp(dirPath, "..")) {
        if (dirTree->current != dirTree->root) dirTree->current = dirTree->current->parent;
    } else {
        tmpNode = IsExistDir(dirTree, dirPath, 'd');
        if (tmpNode) dirTree->current = tmpNode;
        else return -1;
    }
    return 0;
}

int MovePath(DirectoryTree* dirTree, char* dirPath) {
    DirectoryNode* tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char* str = NULL;
    int val = 0;

    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;

    if (!strcmp(dirPath, "/")) dirTree->current = dirTree->root;
    else {
        if (!strncmp(dirPath, "/", 1)) {
            if (strtok(dirPath, "/")) return -1;
            dirTree->current = dirTree->root;
        }
        str = strtok(tmpPath, "/");
        while (str) {
            val = Movecurrent(dirTree, str);
            if (val) {
                dirTree->current = tmpNode;
                return -1;
            }
            str = strtok( NULL, "/");
        }
    }
    return 0;
}

int cd(DirectoryTree* dirTree, char* cmd) {
    DirectoryNode* tmpNode = NULL;
    char* str = NULL;
    char tmp[MAX_DIR];
    int val;

    if (!cmd) {
        strcpy(tmp, usrList->current->dir);
        MovePath(dirTree, tmp);
    } else if(cmd[0] == '-') {
        if (!strcmp(cmd, "--help")) {
            printf("사용법: cd 디렉터리...\n");
            printf("  Change the shell working directory.\n\n");
            printf("  Options:\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("cd: 잘못된 연산자\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            } else {
            printf("cd: 부적절한 옵션 -- '%s'\n", str);
            printf("Try 'cd --help' for more information.\n");
            return -1;
            }
        }
    } else {
        tmpNode = IsExistDir(dirTree, cmd, 'd');
        if (tmpNode) {
            if (HasPermission(tmpNode, 'r')) {
                printf("-bash: cd: '%s': 허가거부\n", cmd);
                return -1;
            }
        }
        tmpNode = IsExistDir(dirTree, cmd,  'f');
        if (tmpNode) {
            printf("-bash: cd: '%s': 디렉터리가 아닙니다\n", cmd);
            return -1;
        }
        val = MovePath(dirTree, cmd);
        if (val) printf("-bash: cd: '%s': 그런 파일이나 디렉터리가 없습니다\n", cmd);
    }
    return 0;
}