#include "../include/main.h"

int pwd(DirectoryTree *dirTree, Stack *dirStack, char *cmd) {
    char *str = NULL;
    if (!cmd) {
        PrintPath(dirTree, dirStack);
    } else if(cmd[0] == '-') {
        if (!strcmp(cmd, "--help")) {
            printf("사용법: pwd\n");
            printf("  Print the name of the current working directory.\n\n");
            printf("  Options:\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("pwd: 잘못된 연산자\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            } else {
            printf("pwd: 부적절한 옵션 -- '%s'\n", str);
            printf("Try 'pwd --help' for more information.\n");
            return -1;
            }
        }
    }
    return 0;
}