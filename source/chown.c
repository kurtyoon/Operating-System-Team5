#include "../include/main.h"

int chown_(DirectoryTree* dirTree, char* cmd) {
    DirectoryNode* tmpNode = NULL;
    UserNode* tmpUser = NULL;
    char* str;
    char tmp[MAX_NAME];

    if (!cmd) {
        printf("chown: 잘못된 연산자\n");
        printf("Try 'chown --help' for more information.\n");
        return -1;
    }
    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-R")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            tmpUser = IsExistUser(usrList, str);
            if (tmpUser) strncpy(tmp, str, MAX_NAME);
            else {
                printf("chown: 잘못된 사용자: '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            str = strtok(NULL, " ");
            if (!str) {
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if (tmpNode) {
                if (!tmpNode->firstChild) ChangeOwner(dirTree, tmp, str);
                else {
                    ChangeOwner(dirTree, tmp, str);
                    ChangeOwnerAll(tmpNode->firstChild, tmp);
                }
            } else {
                printf("chown: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                return -1;
            }
        } else if(!strcmp(cmd, "--help")) {
            printf("사용법: chown [옵션]... [소유자]... 파일...\n");
            printf("  Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\n");
            printf("  Options:\n");
            printf("    -R, --recursive\t change files and directories recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("chown: 잘못된 연산자\n");
                printf("Try 'chown --help' for more information.\n");
                return -1;
            } else {
                printf("chown: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return -1;
            }
        }
    } else {
        strncpy(tmp, cmd, MAX_NAME);
        str = strtok(NULL, " ");
        if (!str) {
            printf("chown: 잘못된 연산자\n");
            printf("Try 'chown --help' for more information.\n");
            return -1;
        } else ChangeOwner(dirTree, tmp, str);
    }
    return 0;
}