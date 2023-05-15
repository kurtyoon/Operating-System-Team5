#include "../include/main.h"

int find_(DirectoryTree *dirTree, char *cmd) {
    char *str;
    if (!cmd) {
        FindDir(dirTree, dirTree->current->name, 1);
        return 0;
    } else if(cmd[0] == '-'){
        if (!strcmp(cmd, "-name")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("find: 잘못된 연산자\n");
                printf("Try 'find --help' for more information.\n");
                return -1;
            }
            FindDir(dirTree, str, 0);
        } else if (!strcmp(cmd, "--help")) {
            printf("사용법: find [<옵션>]... [<파일>]...\n");
            printf("\n");
            printf("  Options:\n");
            printf("    -name\t finds file by name\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("find: 잘못된 연산자\n");
                printf("Try 'find --help' for more information.\n");
                return -1;
            } else {
                printf("find: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'find --help' for more information.\n");
                return -1;
            }
        }
    }
    else FindDir(dirTree, cmd, 1);
    return 0;
}