#include "../include/main.h"

int ls(DirectoryTree *dirTree, char *cmd) {
    DirectoryNode *tmpNode = NULL;
    char *str;
    int val;

    if (!cmd) {
        ListDir(dirTree, 0, 0);
        return 0;
    }
    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-al") || !strcmp(cmd, "-la")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 1, 1);
        } else if (!strcmp(cmd, "-l")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 0, 1);
        } else if (!strcmp(cmd, "-a")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 1, 0);
        } else if(!strcmp(cmd, "--help")) {
            printf("사용법: ls [<옵션>]... [<파일>]...\n");
            printf("  List information about the FILEs (the current directory by default).\n\n");
            printf("  Options:\n");
            printf("    -a, --all\t do not ignore entries starting with .\n");
            printf("    -l       \t use a long listing format\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("ls: 잘못된 연산자\n");
                printf("Try 'ls --help' for more information.\n");
                return -1;
            } else {
                printf("ls: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'ls --help' for more information.\n");
                return -1;
            }
        }
    } else {
        tmpNode = dirTree->current;
        val = MovePath(dirTree, cmd);
        if (val) return -1;
        ListDir(dirTree, 0, 0);
        dirTree->current = tmpNode;
    }
    if (str) dirTree->current = tmpNode;
    return 0;
}