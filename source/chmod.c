#include "../include/main.h"

int chmod(DirectoryTree* dirTree, char* cmd) {
    DirectoryNode* tmpNode = NULL;
    char* str;
    int tmp;

    if (!cmd) {
        printf("chmod: 잘못된 연산자\n");
        printf("Try 'chmod --help' for more information.\n");
        return -1;
    }
    if(cmd[0] == '-') {
        if (!strcmp(cmd, "-R")) {
            str = strtok(NULL, " ");
            if(!str) {
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
            if (str[0] - '0' < 8 && str[1] - '0' < 8 && str[2] - '0' < 8 && strlen(str) == 3) {
                tmp = atoi(str);
                str = strtok(NULL, " ");
                if (!str) {
                    printf("chmod: 잘못된 연산자\n");
                    printf("Try 'chmod --help' for more information.\n");
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                if (tmpNode) {
                    if (!tmpNode->firstChild) ChangeMode(dirTree, tmp, str);
                    else{
                        ChangeMode(dirTree, tmp, str);
                        ChangeModeAll(tmpNode->firstChild, tmp);
                    }
                } else {
                    printf("chmod: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
            } else {
                printf("chmod: 잘못된 모드: '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
        } else if (!strcmp(cmd, "--help")) {
            printf("사용법: chmod [옵션]... 8진수-MODE... 디렉터리...\n");
            printf("  Change the mode of each FILE to MODE.\n\n");
            printf("  Options:\n");
            printf("    -R, --recursive\t change files and directories recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            } else {
                printf("chmod: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
        }
    } else {
        if (cmd[0] - '0' < 8 && cmd[1] - '0' < 8 && cmd[2] - '0' <8  && strlen(cmd) == 3) {
            tmp = atoi(cmd);
            str = strtok(NULL, " ");
            if (!str) {
                printf("chmod: 잘못된 연산자\n");
                printf("Try 'chmod --help' for more information.\n");
                return -1;
            }
            ChangeMode(dirTree, tmp, str);
        } else {
            printf("chmod: 잘못된 모드: '%s'\n", cmd);
            printf("Try 'chmod --help' for more information.\n");
            return -1;
        }
    }
    return 0;
}